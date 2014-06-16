
#include <stddef.h>
#include <stdint.h>
#include "hpack_errors.h"

uint32_t HuffmanDecodes[] = {
	UINT32_C(65538),
	UINT32_C(196612),
	UINT32_C(983056),
	UINT32_C(327686),
	UINT32_C(524297),
	UINT32_C(2150662193),
	UINT32_C(2150760455),
	UINT32_C(2149613615),
	UINT32_C(655371),
	UINT32_C(786445),
	UINT32_C(2150858849),
	UINT32_C(2154004581),
	UINT32_C(2154397807),
	UINT32_C(2155085838),
	UINT32_C(2149941294),
	UINT32_C(1114130),
	UINT32_C(2031648),
	UINT32_C(1245204),
	UINT32_C(1638426),
	UINT32_C(1376278),
	UINT32_C(1507352),
	UINT32_C(2150924341),
	UINT32_C(2151055415),
	UINT32_C(2151186489),
	UINT32_C(2151317565),
	UINT32_C(1769500),
	UINT32_C(1900574),
	UINT32_C(2153021540),
	UINT32_C(2154266728),
	UINT32_C(2154594413),
	UINT32_C(2154725488),
	UINT32_C(2162722),
	UINT32_C(2949166),
	UINT32_C(2293796),
	UINT32_C(2555944),
	UINT32_C(2154987635),
	UINT32_C(2424870),
	UINT32_C(2150006828),
	UINT32_C(2150465601),
	UINT32_C(2687018),
	UINT32_C(2818092),
	UINT32_C(2151972934),
	UINT32_C(2152169549),
	UINT32_C(2152628307),
	UINT32_C(2153742434),
	UINT32_C(3080240),
	UINT32_C(3604536),
	UINT32_C(3211314),
	UINT32_C(3342388),
	UINT32_C(2154201205),
	UINT32_C(2155249783),
	UINT32_C(2155380857),
	UINT32_C(3473462),
	UINT32_C(2151383106),
	UINT32_C(2151907397),
	UINT32_C(3735610),
	UINT32_C(4128832),
	UINT32_C(3866684),
	UINT32_C(3997758),
	UINT32_C(2152300623),
	UINT32_C(2152759381),
	UINT32_C(2153283690),
	UINT32_C(2154528890),
	UINT32_C(4259906),
	UINT32_C(4653128),
	UINT32_C(4390980),
	UINT32_C(4522054),
	UINT32_C(2149744681),
	UINT32_C(2152235082),
	UINT32_C(2152431692),
	UINT32_C(2152824914),
	UINT32_C(4784202),
	UINT32_C(4915276),
	UINT32_C(2153152599),
	UINT32_C(2153349210),
	UINT32_C(2154889293),
	UINT32_C(5111887),
	UINT32_C(2150137898),
	UINT32_C(2150334527),
	UINT32_C(5242961),
	UINT32_C(2153480285),
	UINT32_C(5374035),
	UINT32_C(2155643006),
	UINT32_C(5505109),
	UINT32_C(2149679143),
	UINT32_C(5636183),
	UINT32_C(2149810270),
	UINT32_C(5767257),
	UINT32_C(2149875774),
	UINT32_C(2151678042),
	UINT32_C(5963868),
	UINT32_C(2151448699),
	UINT32_C(2155675741),
	UINT32_C(2153775198),
	UINT32_C(6226016),
	UINT32_C(6357090),
	UINT32_C(10420384),
	UINT32_C(6488164),
	UINT32_C(8454274),
	UINT32_C(6619238),
	UINT32_C(7536756),
	UINT32_C(6750312),
	UINT32_C(7143534),
	UINT32_C(6881386),
	UINT32_C(7012460),
	UINT32_C(2158264485),
	UINT32_C(2158395559),
	UINT32_C(2158526633),
	UINT32_C(2158657707),
	UINT32_C(7274608),
	UINT32_C(7405682),
	UINT32_C(2158788781),
	UINT32_C(2158919855),
	UINT32_C(2159050929),
	UINT32_C(2159182003),
	UINT32_C(7667830),
	UINT32_C(8061052),
	UINT32_C(7798904),
	UINT32_C(7929978),
	UINT32_C(2159313077),
	UINT32_C(2159444151),
	UINT32_C(2159575225),
	UINT32_C(2159706299),
	UINT32_C(8192126),
	UINT32_C(8323200),
	UINT32_C(2159837373),
	UINT32_C(2159968447),
	UINT32_C(2160099521),
	UINT32_C(2160230595),
	UINT32_C(8585348),
	UINT32_C(9502866),
	UINT32_C(8716422),
	UINT32_C(9109644),
	UINT32_C(8847496),
	UINT32_C(8978570),
	UINT32_C(2160361669),
	UINT32_C(2160492743),
	UINT32_C(2160623817),
	UINT32_C(2160754891),
	UINT32_C(9240718),
	UINT32_C(9371792),
	UINT32_C(2160885965),
	UINT32_C(2161017039),
	UINT32_C(2161148113),
	UINT32_C(2161279187),
	UINT32_C(9633940),
	UINT32_C(10027162),
	UINT32_C(9765014),
	UINT32_C(9896088),
	UINT32_C(2161410261),
	UINT32_C(2161541335),
	UINT32_C(2161672409),
	UINT32_C(2161803483),
	UINT32_C(10158236),
	UINT32_C(10289310),
	UINT32_C(2161934557),
	UINT32_C(2162065631),
	UINT32_C(2162196705),
	UINT32_C(2162327779),
	UINT32_C(10551458),
	UINT32_C(12714179),
	UINT32_C(10682532),
	UINT32_C(11600050),
	UINT32_C(10813606),
	UINT32_C(11206828),
	UINT32_C(10944680),
	UINT32_C(11075754),
	UINT32_C(2162458853),
	UINT32_C(2162589927),
	UINT32_C(2162721001),
	UINT32_C(2162852075),
	UINT32_C(11337902),
	UINT32_C(11468976),
	UINT32_C(2162983149),
	UINT32_C(2163114223),
	UINT32_C(2163245297),
	UINT32_C(2163376371),
	UINT32_C(11731124),
	UINT32_C(12124346),
	UINT32_C(11862198),
	UINT32_C(11993272),
	UINT32_C(2163507445),
	UINT32_C(2163638519),
	UINT32_C(2163769593),
	UINT32_C(2163900667),
	UINT32_C(12255420),
	UINT32_C(12386494),
	UINT32_C(2164031741),
	UINT32_C(2164162815),
	UINT32_C(2164261055),
	UINT32_C(12583105),
	UINT32_C(2147516417),
	UINT32_C(2147647491),
	UINT32_C(2147778565),
	UINT32_C(12845253),
	UINT32_C(14811363),
	UINT32_C(12976327),
	UINT32_C(13893845),
	UINT32_C(13107401),
	UINT32_C(13500623),
	UINT32_C(13238475),
	UINT32_C(13369549),
	UINT32_C(2147909639),
	UINT32_C(2148040713),
	UINT32_C(2148171787),
	UINT32_C(2148302861),
	UINT32_C(13631697),
	UINT32_C(13762771),
	UINT32_C(2148433935),
	UINT32_C(2148565009),
	UINT32_C(2148696083),
	UINT32_C(2148827157),
	UINT32_C(14024919),
	UINT32_C(14418141),
	UINT32_C(14155993),
	UINT32_C(14287067),
	UINT32_C(2148958231),
	UINT32_C(2149089305),
	UINT32_C(2149220379),
	UINT32_C(2149351453),
	UINT32_C(14549215),
	UINT32_C(14680289),
	UINT32_C(2149482527),
	UINT32_C(2153545855),
	UINT32_C(2155905153),
	UINT32_C(2156036227),
	UINT32_C(14942437),
	UINT32_C(15859955),
	UINT32_C(15073511),
	UINT32_C(15466733),
	UINT32_C(15204585),
	UINT32_C(15335659),
	UINT32_C(2156167301),
	UINT32_C(2156298375),
	UINT32_C(2156429449),
	UINT32_C(2156560523),
	UINT32_C(15597807),
	UINT32_C(15728881),
	UINT32_C(2156691597),
	UINT32_C(2156822671),
	UINT32_C(2156953745),
	UINT32_C(2157084819),
	UINT32_C(15991029),
	UINT32_C(16384251),
	UINT32_C(16122103),
	UINT32_C(16253177),
	UINT32_C(2157215893),
	UINT32_C(2157346967),
	UINT32_C(2157478041),
	UINT32_C(2157609115),
	UINT32_C(16515325),
	UINT32_C(16646399),
	UINT32_C(2157740189),
	UINT32_C(2157871263),
	UINT32_C(2158002337),
	UINT32_C(2158133411)
};

#define ZERO(tc) (uint16_t)((tc)>>16)
#define ONE(tc) (uint16_t)((tc)&0xFFFF)

#define IS_INT(x) (((x)&0x8000)==0x8000)
#define VALUE_OF(x) ((x)&0x7FFF)

size_t huffman_decode(
		uint8_t *huff, size_t bytesize, size_t *consumed,
		uint8_t *buff, size_t n, size_t *produced
) {
	uint32_t tc = *HuffmanDecodes;
	uint16_t tmp;
	uint8_t byte, bc, mask;

	size_t _p, _c;
	if (!produced) { produced = &_p; }
	if (!consumed) { consumed = &_c; }
	*produced = *consumed = 0;

	if (bytesize < 1) {
		return ERROR_NONE;
	}

	if (buff && n < 1) {
		return ERROR_OVERFLOW;
	}

	while (bytesize > 0) {
		byte = *huff; huff++; (*consumed)++; bytesize--;
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
					return ERROR_EOS;
				} else {
					if (buff) {
						*buff = (uint8_t)(tmp); buff++;
						n--;
					}
					(*produced)++;
					if (bytesize < 1 && (byte & mask) == mask) {
						tc = 0;
						goto done;
					} else if (buff && n < 1) {
						return ERROR_OVERFLOW;
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
		return ERROR_TRUNCATED;
	}
	return ERROR_NONE;
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

size_t huffman_encode(
		uint8_t *str, size_t bytesize, size_t *consumed,
		uint8_t *buff, size_t n, size_t *produced
) {
	uint8_t shift;
	uint64_t mask;
	uint64_t val;
	hnode_t hnode;

	uint64_t bitq = 0; /* a queue */
	uint8_t  bitn = 0; /* depth of the queue */

	size_t _p, _c;
	if (!produced) { produced = &_p; }
	if (!consumed) { consumed = &_c; }
	*produced = *consumed = 0;

	while (bytesize > 0) {
		if (buff && n < 1) {
			return ERROR_OVERFLOW;
		}
		hnode = HuffmanCodes[*str]; str++; (*consumed)++; bytesize--;
		bitq = (bitq << hnode.size) | hnode.bits; /* (max 33 bits wide) */
		bitn += hnode.size;
		/* canibalise the top bytes */
		while (bitn >= 8) {
			if (buff) {
				shift = bitn - 8;
				mask = UINT64_C(0xFF) << shift;
				val = (bitq & mask);
				*buff = (uint8_t)(val >> shift); buff++; n--;
				bitq ^= val;
			}
			(*produced)++;
			bitn -= 8;
		}
	}
	/* pad with EOS (incidentally all 1s) */
	if (bitn > 0) {
		if (buff) {
			shift = 8 - bitn;
			mask = (1 << shift) - 1;
			*buff = ((bitq << shift) | mask); buff++; n--;
		}
		(*produced)++;
	}
	return ERROR_NONE;
}

size_t huffman_length(
		uint8_t *str, size_t bytesize
) {
	size_t result = 7;
	while (bytesize > 0) {
		result += HuffmanCodes[*str].size; str++; bytesize--;
	}
	return (result / 8);
}
