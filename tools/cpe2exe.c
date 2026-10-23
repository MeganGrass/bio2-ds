/*
 * cpe2exe.c - build a PS-X EXE from the PSYLINK CPE of the main executable.
 * C port of tools\cpe2exe.pl; the output is byte-identical to it (checked against OBJ\PSX\BUILD\SLUS_007.48).
 *
 * usage: cpe2exe SLUS.CPE SLUS_007.48 [MAINEND_HEX]
 *   MAINEND_HEX (optional): first address after the main image. Chunks at or above it are overlay payloads and are skipped.
 *   Without it the main image is the run of chunks from the lowest address up to the first gap over 1 MB. SLUS has a 1.4 MB
 *   gap before the DIESPR overlay; the beta MAIN.CPE has no overlays and only a 152 KB gap before its last 4-byte buffer
 *   chunk, so a smaller threshold drops that chunk.
 *
 * The header is the DS layout: "PS-X EXE", pc0, gp0, t_addr, t_size, initial sp 0x801ffff0 at 0x30, region string at 0x4c.
 * The image is extended to the end of the .bss sections in SLUS.MAP (next to the CPE), because retail keeps its zero-filled
 * .bss inside the EXE. This is the same rule as the perl script: map rows with 6 columns and "bss" in column 5.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned int u32;

#define TADDR 0x80010000u
#define HDR_SIZE 0x800u

typedef struct {
	u32 addr;
	u32 size;
	const u8 *data;
} CHUNK;

static void die(const char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

static u32 rd32(const u8 *p)
{
	return (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24);
}

static u32 rd16(const u8 *p)
{
	return (u32)p[0] | ((u32)p[1] << 8);
}

static void put32(u8 *p, u32 v)
{
	p[0] = (u8)v;
	p[1] = (u8)(v >> 8);
	p[2] = (u8)(v >> 16);
	p[3] = (u8)(v >> 24);
}

static int cmp_chunk(const void *a, const void *b)
{
	const CHUNK *x = (const CHUNK *)a, *y = (const CHUNK *)b;
	return (x->addr > y->addr) - (x->addr < y->addr);
}

static u8 *read_file(const char *path, long *len)
{
	FILE *f = fopen(path, "rb");
	u8 *buf;
	if (!f) return NULL;
	fseek(f, 0, SEEK_END);
	*len = ftell(f);
	fseek(f, 0, SEEK_SET);
	buf = (u8 *)malloc((size_t)*len + 1);
	if (!buf || fread(buf, 1, (size_t)*len, f) != (size_t)*len) { fclose(f); return NULL; }
	fclose(f);
	return buf;
}

/* SLUS.MAP in the same directory as the CPE: bss end = highest (address + 1) over rows with 6 columns and "bss" in column 5 */
static u32 map_bss_end(const char *cpe_path)
{
	char map_path[1024];
	char line[512];
	u32 bssend = 0;
	FILE *f;
	const char *slash = strrchr(cpe_path, '\\');
	const char *slash2 = strrchr(cpe_path, '/');
	size_t dir_len = 0;
	if (slash2 && (!slash || slash2 > slash)) slash = slash2;
	if (slash) dir_len = (size_t)(slash - cpe_path) + 1;
	memcpy(map_path, cpe_path, dir_len);
	strcpy(map_path + dir_len, "SLUS.MAP");

	f = fopen(map_path, "r");
	if (!f) return 0;
	while (fgets(line, sizeof line, f)) {
		char c[7][64];
		int n = sscanf(line, "%63s %63s %63s %63s %63s %63s %63s", c[0], c[1], c[2], c[3], c[4], c[5], c[6]);
		unsigned long start;
		u32 e;
		if (n != 6 || strcmp(c[4], "bss") != 0) continue;
		start = strtoul(c[1], NULL, 16);
		e = (u32)start + 1;
		if (e > bssend) bssend = e;
	}
	fclose(f);
	return bssend;
}

int main(int argc, char **argv)
{
	long cpe_len;
	u8 *d;
	CHUNK *chunks = NULL;
	size_t nchunks = 0, cap = 0;
	size_t p = 4;
	u32 pc0 = 0, gp0 = 0;
	u32 mainend = 0;
	u32 min = 0xFFFFFFFFu, max = 0, bssend;
	size_t main_chunks = 0;
	u32 size;
	u8 *img;
	u8 hdr[HDR_SIZE];
	FILE *out;
	size_t i;

	if (argc < 3) die("usage: cpe2exe SLUS.CPE OUT [MAINEND_HEX]");
	if (argc >= 4) mainend = (u32)strtoul(argv[3], NULL, 16);

	d = read_file(argv[1], &cpe_len);
	if (!d) { fprintf(stderr, "%s: cannot read\n", argv[1]); return 1; }
	if (cpe_len < 4 || memcmp(d, "CPE", 3) != 0) die("not a CPE file");

	while (p < (size_t)cpe_len) {
		u8 t = d[p++];
		if (t == 1) {
			CHUNK ch;
			if (p + 8 > (size_t)cpe_len) die("truncated CPE");
			ch.addr = rd32(d + p);
			ch.size = rd32(d + p + 4);
			p += 8;
			if (p + ch.size > (size_t)cpe_len) die("truncated CPE chunk");
			ch.data = d + p;
			p += ch.size;
			if (nchunks == cap) {
				cap = cap ? cap * 2 : 256;
				chunks = (CHUNK *)realloc(chunks, cap * sizeof(CHUNK));
				if (!chunks) die("out of memory");
			}
			chunks[nchunks++] = ch;
		} else if (t == 3) {
			u32 reg = rd16(d + p);
			u32 val = rd32(d + p + 2);
			if (reg == 0x90) pc0 = val;
			if (reg == 0x8c) gp0 = val;
			p += 6;
		} else if (t == 8) {
			p += 1;
		} else if (t == 0) {
			break;
		} else {
			fprintf(stderr, "unknown CPE record %u\n", (unsigned)t);
			return 1;
		}
	}

	if (!mainend) {
		/* no end given: the main image is the run of chunks from the lowest address up to the first gap over 64 KB */
		u32 end;
		qsort(chunks, nchunks, sizeof(CHUNK), cmp_chunk);
		if (nchunks == 0) die("no chunks");
		end = chunks[0].addr + chunks[0].size;
		for (i = 1; i < nchunks; i++) {
			u32 e;
			if (chunks[i].addr > end && chunks[i].addr - end > 0x100000) break;
			e = chunks[i].addr + chunks[i].size;
			if (e > end) end = e;
		}
		mainend = end;
	}

	for (i = 0; i < nchunks; i++) {
		u32 e;
		if (chunks[i].addr >= mainend) continue;
		main_chunks++;
		if (chunks[i].addr < min) min = chunks[i].addr;
		e = chunks[i].addr + chunks[i].size;
		if (e > max) max = e;
	}

	bssend = map_bss_end(argv[1]);
	if (bssend > max) max = bssend;

	if (min < TADDR) {
		fprintf(stderr, "image starts at %08x, below %08x\n", min, TADDR);
		return 1;
	}

	size = (max - TADDR + 0x7FF) & ~0x7FFu;
	img = (u8 *)calloc(size, 1);
	if (!img) die("out of memory");
	for (i = 0; i < nchunks; i++) {
		if (chunks[i].addr >= mainend) continue;
		memcpy(img + (chunks[i].addr - TADDR), chunks[i].data, chunks[i].size);
	}

	/* header: "PS-X EXE", 8 zero bytes, then pc0, gp0, t_addr, t_size, d_addr, d_size, b_addr, b_size, sp, fp */
	memset(hdr, 0, sizeof hdr);
	memcpy(hdr, "PS-X EXE", 8);
	put32(hdr + 16, pc0);
	put32(hdr + 20, gp0);
	put32(hdr + 24, TADDR);
	put32(hdr + 28, size);
	put32(hdr + 48, 0x801ffff0u);
	memcpy(hdr + 0x4c, "Sony Computer Entertainment Inc. for North America area",
	       strlen("Sony Computer Entertainment Inc. for North America area"));

	out = fopen(argv[2], "wb");
	if (!out) { fprintf(stderr, "%s: cannot write\n", argv[2]); return 1; }
	fwrite(hdr, 1, sizeof hdr, out);
	fwrite(img, 1, size, out);
	fclose(out);

	printf("%s: pc0 %08x  t_addr %08x  t_size %06x  (%u of %u chunks, %u bytes)\n",
	       argv[2], pc0, TADDR, size, (unsigned)main_chunks, (unsigned)nchunks, (unsigned)(HDR_SIZE + size));
	return 0;
}
