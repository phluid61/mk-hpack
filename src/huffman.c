
#include <stddef.h>
#include <stdint.h>

uint32_t HuffmanDecodes[] = {
	65538,
	196612,
	983056,
	327686,
	524297,
	2150662193,
	2150760455,
	2149613615,
	655371,
	786445,
	2150858849,
	2154004581,
	2154397807,
	2155085838,
	2149941294,
	1114130,
	2031648,
	1245204,
	1638426,
	1376278,
	1507352,
	2150924341,
	2151055415,
	2151186489,
	2151317565,
	1769500,
	1900574,
	2153021540,
	2154266728,
	2154594413,
	2154725488,
	2162722,
	2949166,
	2293796,
	2555944,
	2154987635,
	2424870,
	2150006828,
	2150465601,
	2687018,
	2818092,
	2151972934,
	2152169549,
	2152628307,
	2153742434,
	3080240,
	3604536,
	3211314,
	3342388,
	2154201205,
	2155249783,
	2155380857,
	3473462,
	2151383106,
	2151907397,
	3735610,
	4128832,
	3866684,
	3997758,
	2152300623,
	2152759381,
	2153283690,
	2154528890,
	4259906,
	4653128,
	4390980,
	4522054,
	2149744681,
	2152235082,
	2152431692,
	2152824914,
	4784202,
	4915276,
	2153152599,
	2153349210,
	2154889293,
	5111887,
	2150137898,
	2150334527,
	5242961,
	2153480285,
	5374035,
	2155643006,
	5505109,
	2149679143,
	5636183,
	2149810270,
	5767257,
	2149875774,
	2151678042,
	5963868,
	2151448699,
	2155675741,
	2153775198,
	6226016,
	6357090,
	10420384,
	6488164,
	8454274,
	6619238,
	7536756,
	6750312,
	7143534,
	6881386,
	7012460,
	2158264485,
	2158395559,
	2158526633,
	2158657707,
	7274608,
	7405682,
	2158788781,
	2158919855,
	2159050929,
	2159182003,
	7667830,
	8061052,
	7798904,
	7929978,
	2159313077,
	2159444151,
	2159575225,
	2159706299,
	8192126,
	8323200,
	2159837373,
	2159968447,
	2160099521,
	2160230595,
	8585348,
	9502866,
	8716422,
	9109644,
	8847496,
	8978570,
	2160361669,
	2160492743,
	2160623817,
	2160754891,
	9240718,
	9371792,
	2160885965,
	2161017039,
	2161148113,
	2161279187,
	9633940,
	10027162,
	9765014,
	9896088,
	2161410261,
	2161541335,
	2161672409,
	2161803483,
	10158236,
	10289310,
	2161934557,
	2162065631,
	2162196705,
	2162327779,
	10551458,
	12714179,
	10682532,
	11600050,
	10813606,
	11206828,
	10944680,
	11075754,
	2162458853,
	2162589927,
	2162721001,
	2162852075,
	11337902,
	11468976,
	2162983149,
	2163114223,
	2163245297,
	2163376371,
	11731124,
	12124346,
	11862198,
	11993272,
	2163507445,
	2163638519,
	2163769593,
	2163900667,
	12255420,
	12386494,
	2164031741,
	2164162815,
	2164261055,
	12583105,
	2147516417,
	2147647491,
	2147778565,
	12845253,
	14811363,
	12976327,
	13893845,
	13107401,
	13500623,
	13238475,
	13369549,
	2147909639,
	2148040713,
	2148171787,
	2148302861,
	13631697,
	13762771,
	2148433935,
	2148565009,
	2148696083,
	2148827157,
	14024919,
	14418141,
	14155993,
	14287067,
	2148958231,
	2149089305,
	2149220379,
	2149351453,
	14549215,
	14680289,
	2149482527,
	2153545855,
	2155905153,
	2156036227,
	14942437,
	15859955,
	15073511,
	15466733,
	15204585,
	15335659,
	2156167301,
	2156298375,
	2156429449,
	2156560523,
	15597807,
	15728881,
	2156691597,
	2156822671,
	2156953745,
	2157084819,
	15991029,
	16384251,
	16122103,
	16253177,
	2157215893,
	2157346967,
	2157478041,
	2157609115,
	16515325,
	16646399,
	2157740189,
	2157871263,
	2158002337,
	2158133411
};

#define ZERO(tc) (uint16_t)((tc)>>16)
#define ONE(tc) (uint16_t)((tc)&0xFFFF)

#define IS_INT(x) (((x)&0x8000)==0x8000)
#define VALUE_OF(x) ((x)&0x7FFF)

int huffman_decoder_error;
#define ERROR_OVERFLOW  1
#define ERROR_TRUNCATED 2
#define ERROR_EOS       3

size_t huffman_decode(uint8_t *huff, size_t bytesize, uint8_t *buff, size_t n) {
	size_t count = 0;
	uint32_t tc = *HuffmanDecodes;
	uint16_t tmp;
	uint8_t byte, bc, mask;

	huffman_decoder_error = 0;

	if (bytesize < 1) {
		return 0;
	}

	if (n < 1) {
		huffman_decoder_error = ERROR_OVERFLOW;
		return 0;
	}

	while (bytesize > 0) {
		byte = *huff; huff++; bytesize--;
		bc = 0x80;   /* bit cursor */
		mask = 0x7F; /* padding mask */
		while (bc > 0) {
			if ((byte & bc) == bc) {
				tmp = ONE(tc);
			} else {
				tmp = ZERO(tc);
			}
			if (IS_INT(tmp)) {
				tmp = VALUE_OF(tmp);
				if (tmp > 0xFF) {
					huffman_decoder_error = ERROR_EOS;
					return 0;
				} else {
					*buff = (uint8_t)(tmp); buff++; count++; n--;
					if (bytesize < 1 && (byte & mask) == mask) {
						tc = 0;
						goto done;
					} else if (n < 1) {
						huffman_decoder_error = ERROR_OVERFLOW;
						return count;
					} else {
						tc = *HuffmanDecodes;
					}
				}
			} else {
				/*tmp = VALUE_OF(tmp);*/
				/* FIXME: assert(tmp < 256) */
				tc = HuffmanDecodes[tmp];
			}
			bc >>= 1;
			mask >>= 1;
		}
	}
done:
	if (tc) {
		huffman_decoder_error = ERROR_TRUNCATED;
		/*return count;*/
	}
	return count;
}

/* ^^^ Decoder | Encoder vvv */

typedef struct hnode_t {
	uint32_t bits;
	uint8_t  size;
} hnode_t;

hnode_t HuffmanCodes[] = {
	(hnode_t){0x3ffffba, 26},
	(hnode_t){0x3ffffbb, 26},
	(hnode_t){0x3ffffbc, 26},
	(hnode_t){0x3ffffbd, 26},
	(hnode_t){0x3ffffbe, 26},
	(hnode_t){0x3ffffbf, 26},
	(hnode_t){0x3ffffc0, 26},
	(hnode_t){0x3ffffc1, 26},
	(hnode_t){0x3ffffc2, 26},
	(hnode_t){0x3ffffc3, 26},
	(hnode_t){0x3ffffc4, 26},
	(hnode_t){0x3ffffc5, 26},
	(hnode_t){0x3ffffc6, 26},
	(hnode_t){0x3ffffc7, 26},
	(hnode_t){0x3ffffc8, 26},
	(hnode_t){0x3ffffc9, 26},
	(hnode_t){0x3ffffca, 26},
	(hnode_t){0x3ffffcb, 26},
	(hnode_t){0x3ffffcc, 26},
	(hnode_t){0x3ffffcd, 26},
	(hnode_t){0x3ffffce, 26},
	(hnode_t){0x3ffffcf, 26},
	(hnode_t){0x3ffffd0, 26},
	(hnode_t){0x3ffffd1, 26},
	(hnode_t){0x3ffffd2, 26},
	(hnode_t){0x3ffffd3, 26},
	(hnode_t){0x3ffffd4, 26},
	(hnode_t){0x3ffffd5, 26},
	(hnode_t){0x3ffffd6, 26},
	(hnode_t){0x3ffffd7, 26},
	(hnode_t){0x3ffffd8, 26},
	(hnode_t){0x3ffffd9, 26},
	(hnode_t){0x6, 5},
	(hnode_t){0x1ffc, 13},
	(hnode_t){0x1f0, 9},
	(hnode_t){0x3ffc, 14},
	(hnode_t){0x7ffc, 15},
	(hnode_t){0x1e, 6},
	(hnode_t){0x64, 7},
	(hnode_t){0x1ffd, 13},
	(hnode_t){0x3fa, 10},
	(hnode_t){0x1f1, 9},
	(hnode_t){0x3fb, 10},
	(hnode_t){0x3fc, 10},
	(hnode_t){0x65, 7},
	(hnode_t){0x66, 7},
	(hnode_t){0x1f, 6},
	(hnode_t){0x7, 5},
	(hnode_t){0x0, 4},
	(hnode_t){0x1, 4},
	(hnode_t){0x2, 4},
	(hnode_t){0x8, 5},
	(hnode_t){0x20, 6},
	(hnode_t){0x21, 6},
	(hnode_t){0x22, 6},
	(hnode_t){0x23, 6},
	(hnode_t){0x24, 6},
	(hnode_t){0x25, 6},
	(hnode_t){0x26, 6},
	(hnode_t){0xec, 8},
	(hnode_t){0x1fffc, 17},
	(hnode_t){0x27, 6},
	(hnode_t){0x7ffd, 15},
	(hnode_t){0x3fd, 10},
	(hnode_t){0x7ffe, 15},
	(hnode_t){0x67, 7},
	(hnode_t){0xed, 8},
	(hnode_t){0xee, 8},
	(hnode_t){0x68, 7},
	(hnode_t){0xef, 8},
	(hnode_t){0x69, 7},
	(hnode_t){0x6a, 7},
	(hnode_t){0x1f2, 9},
	(hnode_t){0xf0, 8},
	(hnode_t){0x1f3, 9},
	(hnode_t){0x1f4, 9},
	(hnode_t){0x1f5, 9},
	(hnode_t){0x6b, 7},
	(hnode_t){0x6c, 7},
	(hnode_t){0xf1, 8},
	(hnode_t){0xf2, 8},
	(hnode_t){0x1f6, 9},
	(hnode_t){0x1f7, 9},
	(hnode_t){0x6d, 7},
	(hnode_t){0x28, 6},
	(hnode_t){0xf3, 8},
	(hnode_t){0x1f8, 9},
	(hnode_t){0x1f9, 9},
	(hnode_t){0xf4, 8},
	(hnode_t){0x1fa, 9},
	(hnode_t){0x1fb, 9},
	(hnode_t){0x7fc, 11},
	(hnode_t){0x3ffffda, 26},
	(hnode_t){0x7fd, 11},
	(hnode_t){0x3ffd, 14},
	(hnode_t){0x6e, 7},
	(hnode_t){0x3fffe, 18},
	(hnode_t){0x9, 5},
	(hnode_t){0x6f, 7},
	(hnode_t){0xa, 5},
	(hnode_t){0x29, 6},
	(hnode_t){0xb, 5},
	(hnode_t){0x70, 7},
	(hnode_t){0x2a, 6},
	(hnode_t){0x2b, 6},
	(hnode_t){0xc, 5},
	(hnode_t){0xf5, 8},
	(hnode_t){0xf6, 8},
	(hnode_t){0x2c, 6},
	(hnode_t){0x2d, 6},
	(hnode_t){0x2e, 6},
	(hnode_t){0xd, 5},
	(hnode_t){0x2f, 6},
	(hnode_t){0x1fc, 9},
	(hnode_t){0x30, 6},
	(hnode_t){0x31, 6},
	(hnode_t){0xe, 5},
	(hnode_t){0x71, 7},
	(hnode_t){0x72, 7},
	(hnode_t){0x73, 7},
	(hnode_t){0x74, 7},
	(hnode_t){0x75, 7},
	(hnode_t){0xf7, 8},
	(hnode_t){0x1fffd, 17},
	(hnode_t){0xffc, 12},
	(hnode_t){0x1fffe, 17},
	(hnode_t){0xffd, 12},
	(hnode_t){0x3ffffdb, 26},
	(hnode_t){0x3ffffdc, 26},
	(hnode_t){0x3ffffdd, 26},
	(hnode_t){0x3ffffde, 26},
	(hnode_t){0x3ffffdf, 26},
	(hnode_t){0x3ffffe0, 26},
	(hnode_t){0x3ffffe1, 26},
	(hnode_t){0x3ffffe2, 26},
	(hnode_t){0x3ffffe3, 26},
	(hnode_t){0x3ffffe4, 26},
	(hnode_t){0x3ffffe5, 26},
	(hnode_t){0x3ffffe6, 26},
	(hnode_t){0x3ffffe7, 26},
	(hnode_t){0x3ffffe8, 26},
	(hnode_t){0x3ffffe9, 26},
	(hnode_t){0x3ffffea, 26},
	(hnode_t){0x3ffffeb, 26},
	(hnode_t){0x3ffffec, 26},
	(hnode_t){0x3ffffed, 26},
	(hnode_t){0x3ffffee, 26},
	(hnode_t){0x3ffffef, 26},
	(hnode_t){0x3fffff0, 26},
	(hnode_t){0x3fffff1, 26},
	(hnode_t){0x3fffff2, 26},
	(hnode_t){0x3fffff3, 26},
	(hnode_t){0x3fffff4, 26},
	(hnode_t){0x3fffff5, 26},
	(hnode_t){0x3fffff6, 26},
	(hnode_t){0x3fffff7, 26},
	(hnode_t){0x3fffff8, 26},
	(hnode_t){0x3fffff9, 26},
	(hnode_t){0x3fffffa, 26},
	(hnode_t){0x3fffffb, 26},
	(hnode_t){0x3fffffc, 26},
	(hnode_t){0x3fffffd, 26},
	(hnode_t){0x3fffffe, 26},
	(hnode_t){0x3ffffff, 26},
	(hnode_t){0x1ffff80, 25},
	(hnode_t){0x1ffff81, 25},
	(hnode_t){0x1ffff82, 25},
	(hnode_t){0x1ffff83, 25},
	(hnode_t){0x1ffff84, 25},
	(hnode_t){0x1ffff85, 25},
	(hnode_t){0x1ffff86, 25},
	(hnode_t){0x1ffff87, 25},
	(hnode_t){0x1ffff88, 25},
	(hnode_t){0x1ffff89, 25},
	(hnode_t){0x1ffff8a, 25},
	(hnode_t){0x1ffff8b, 25},
	(hnode_t){0x1ffff8c, 25},
	(hnode_t){0x1ffff8d, 25},
	(hnode_t){0x1ffff8e, 25},
	(hnode_t){0x1ffff8f, 25},
	(hnode_t){0x1ffff90, 25},
	(hnode_t){0x1ffff91, 25},
	(hnode_t){0x1ffff92, 25},
	(hnode_t){0x1ffff93, 25},
	(hnode_t){0x1ffff94, 25},
	(hnode_t){0x1ffff95, 25},
	(hnode_t){0x1ffff96, 25},
	(hnode_t){0x1ffff97, 25},
	(hnode_t){0x1ffff98, 25},
	(hnode_t){0x1ffff99, 25},
	(hnode_t){0x1ffff9a, 25},
	(hnode_t){0x1ffff9b, 25},
	(hnode_t){0x1ffff9c, 25},
	(hnode_t){0x1ffff9d, 25},
	(hnode_t){0x1ffff9e, 25},
	(hnode_t){0x1ffff9f, 25},
	(hnode_t){0x1ffffa0, 25},
	(hnode_t){0x1ffffa1, 25},
	(hnode_t){0x1ffffa2, 25},
	(hnode_t){0x1ffffa3, 25},
	(hnode_t){0x1ffffa4, 25},
	(hnode_t){0x1ffffa5, 25},
	(hnode_t){0x1ffffa6, 25},
	(hnode_t){0x1ffffa7, 25},
	(hnode_t){0x1ffffa8, 25},
	(hnode_t){0x1ffffa9, 25},
	(hnode_t){0x1ffffaa, 25},
	(hnode_t){0x1ffffab, 25},
	(hnode_t){0x1ffffac, 25},
	(hnode_t){0x1ffffad, 25},
	(hnode_t){0x1ffffae, 25},
	(hnode_t){0x1ffffaf, 25},
	(hnode_t){0x1ffffb0, 25},
	(hnode_t){0x1ffffb1, 25},
	(hnode_t){0x1ffffb2, 25},
	(hnode_t){0x1ffffb3, 25},
	(hnode_t){0x1ffffb4, 25},
	(hnode_t){0x1ffffb5, 25},
	(hnode_t){0x1ffffb6, 25},
	(hnode_t){0x1ffffb7, 25},
	(hnode_t){0x1ffffb8, 25},
	(hnode_t){0x1ffffb9, 25},
	(hnode_t){0x1ffffba, 25},
	(hnode_t){0x1ffffbb, 25},
	(hnode_t){0x1ffffbc, 25},
	(hnode_t){0x1ffffbd, 25},
	(hnode_t){0x1ffffbe, 25},
	(hnode_t){0x1ffffbf, 25},
	(hnode_t){0x1ffffc0, 25},
	(hnode_t){0x1ffffc1, 25},
	(hnode_t){0x1ffffc2, 25},
	(hnode_t){0x1ffffc3, 25},
	(hnode_t){0x1ffffc4, 25},
	(hnode_t){0x1ffffc5, 25},
	(hnode_t){0x1ffffc6, 25},
	(hnode_t){0x1ffffc7, 25},
	(hnode_t){0x1ffffc8, 25},
	(hnode_t){0x1ffffc9, 25},
	(hnode_t){0x1ffffca, 25},
	(hnode_t){0x1ffffcb, 25},
	(hnode_t){0x1ffffcc, 25},
	(hnode_t){0x1ffffcd, 25},
	(hnode_t){0x1ffffce, 25},
	(hnode_t){0x1ffffcf, 25},
	(hnode_t){0x1ffffd0, 25},
	(hnode_t){0x1ffffd1, 25},
	(hnode_t){0x1ffffd2, 25},
	(hnode_t){0x1ffffd3, 25},
	(hnode_t){0x1ffffd4, 25},
	(hnode_t){0x1ffffd5, 25},
	(hnode_t){0x1ffffd6, 25},
	(hnode_t){0x1ffffd7, 25},
	(hnode_t){0x1ffffd8, 25},
	(hnode_t){0x1ffffd9, 25},
	(hnode_t){0x1ffffda, 25},
	(hnode_t){0x1ffffdb, 25},
	(hnode_t){0x1ffffdc, 25},
};

int huffman_encoder_error;

size_t huffman_encode(uint8_t *str, size_t bytesize, uint8_t *buff, size_t n) {
	size_t count = 0;
	uint8_t shift;
	uint64_t mask;
	uint64_t val;
	hnode_t hnode;

	uint64_t bitq = 0; /* a queue */
	uint64_t bitn = 0; /* depth of the queue */
	while (bytesize > 0) {
		if (n < 1) {
			huffman_encoder_error = ERROR_OVERFLOW;
			return count;
		}
		hnode = HuffmanCodes[*str]; str++; bytesize--;
		bitq = (bitq << hnode.size) | hnode.bits; /* (max 33 bits wide) */
		bitn += hnode.size;
		/* canibalise the top bytes */
		while (bitn >= 8) {
			shift = bitn - 8;
			mask = 0xFF << shift;
			val = (bitq & mask);
			*buff = (uint8_t)(val >> shift); buff++; n--;
			count++;
			bitq ^= val;
			bitn -= 8;
		}
	}
	/* pad with EOS (incidentally all 1s) */
	if (bitn > 0) {
		shift = 8 - bitn;
		mask = (1 << shift) - 1;
		*buff = ((bitq << shift) | mask); buff++; n--;
		count++;
	}
	return count;
}

