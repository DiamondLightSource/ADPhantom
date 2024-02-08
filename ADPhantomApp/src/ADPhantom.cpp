#include "ADPhantom.h"

//lookup table required to convert non-linear pixel data to linear pixel data
const int ADPhantom::PHANTOM_LinLUT[1024] = {
  2, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 17, 18,
  19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 33,
  34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 48,
  49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 63,
  64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
  79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94,
  94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
  110, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124,
  125, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 137, 138,
  139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154,
  156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 167, 168, 169, 170, 171, 172,
  173, 175, 176, 177, 178, 179, 181, 182, 183, 184, 186, 187, 188, 189, 191, 192,
  193, 194, 196, 197, 198, 200, 201, 202, 204, 205, 206, 208, 209, 210, 212, 213,
  215, 216, 217, 219, 220, 222, 223, 225, 226, 227, 229, 230, 232, 233, 235, 236,
  238, 239, 241, 242, 244, 245, 247, 249, 250, 252, 253, 255, 257, 258, 260, 261,
  263, 265, 266, 268, 270, 271, 273, 275, 276, 278, 280, 281, 283, 285, 287, 288,
  290, 292, 294, 295, 297, 299, 301, 302, 304, 306, 308, 310, 312, 313, 315, 317,
  319, 321, 323, 325, 327, 328, 330, 332, 334, 336, 338, 340, 342, 344, 346, 348,
  350, 352, 354, 356, 358, 360, 362, 364, 366, 368, 370, 372, 374, 377, 379, 381,
  383, 385, 387, 389, 391, 394, 396, 398, 400, 402, 404, 407, 409, 411, 413, 416,
  418, 420, 422, 425, 427, 429, 431, 434, 436, 438, 441, 443, 445, 448, 450, 452,
  455, 457, 459, 462, 464, 467, 469, 472, 474, 476, 479, 481, 484, 486, 489, 491,
  494, 496, 499, 501, 504, 506, 509, 511, 514, 517, 519, 522, 524, 527, 529, 532,
  535, 537, 540, 543, 545, 548, 551, 553, 556, 559, 561, 564, 567, 570, 572, 575,
  578, 581, 583, 586, 589, 592, 594, 597, 600, 603, 606, 609, 611, 614, 617, 620,
  623, 626, 629, 632, 635, 637, 640, 643, 646, 649, 652, 655, 658, 661, 664, 667,
  670, 673, 676, 679, 682, 685, 688, 691, 694, 698, 701, 704, 707, 710, 713, 716,
  719, 722, 726, 729, 732, 735, 738, 742, 745, 748, 751, 754, 758, 761, 764, 767,
  771, 774, 777, 781, 784, 787, 790, 794, 797, 800, 804, 807, 811, 814, 817, 821,
  824, 828, 831, 834, 838, 841, 845, 848, 852, 855, 859, 862, 866, 869, 873, 876,
  880, 883, 887, 890, 894, 898, 901, 905, 908, 912, 916, 919, 923, 927, 930, 934,
  938, 941, 945, 949, 952, 956, 960, 964, 967, 971, 975, 979, 982, 986, 990, 994,
  998,1001,1005,1009,1013,1017,1021,1025,1028,1032,1036,1040,1044,1048,1052,1056,
  1060,1064,1068,1072,1076,1080,1084,1088,1092,1096,1100,1104,1108,1112,1116,1120,
  1124,1128,1132,1137,1141,1145,1149,1153,1157,1162,1166,1170,1174,1178,1183,1187,
  1191,1195,1200,1204,1208,1212,1217,1221,1225,1230,1234,1238,1243,1247,1251,1256,
  1260,1264,1269,1273,1278,1282,1287,1291,1295,1300,1304,1309,1313,1318,1322,1327,
  1331,1336,1340,1345,1350,1354,1359,1363,1368,1372,1377,1382,1386,1391,1396,1400,
  1405,1410,1414,1419,1424,1428,1433,1438,1443,1447,1452,1457,1462,1466,1471,1476,
  1481,1486,1490,1495,1500,1505,1510,1515,1520,1524,1529,1534,1539,1544,1549,1554,
  1559,1564,1569,1574,1579,1584,1589,1594,1599,1604,1609,1614,1619,1624,1629,1634,
  1639,1644,1649,1655,1660,1665,1670,1675,1680,1685,1691,1696,1701,1706,1711,1717,
  1722,1727,1732,1738,1743,1748,1753,1759,1764,1769,1775,1780,1785,1791,1796,1801,
  1807,1812,1818,1823,1828,1834,1839,1845,1850,1856,1861,1867,1872,1878,1883,1889,
  1894,1900,1905,1911,1916,1922,1927,1933,1939,1944,1950,1956,1961,1967,1972,1978,
  1984,1989,1995,2001,2007,2012,2018,2024,2030,2035,2041,2047,2053,2058,2064,2070,
  2076,2082,2087,2093,2099,2105,2111,2117,2123,2129,2135,2140,2146,2152,2158,2164,
  2170,2176,2182,2188,2194,2200,2206,2212,2218,2224,2231,2237,2243,2249,2255,2261,
  2267,2273,2279,2286,2292,2298,2304,2310,2317,2323,2329,2335,2341,2348,2354,2360,
  2366,2373,2379,2385,2392,2398,2404,2411,2417,2423,2430,2436,2443,2449,2455,2462,
  2468,2475,2481,2488,2494,2501,2507,2514,2520,2527,2533,2540,2546,2553,2559,2566,
  2572,2579,2586,2592,2599,2605,2612,2619,2625,2632,2639,2645,2652,2659,2666,2672,
  2679,2686,2693,2699,2706,2713,2720,2726,2733,2740,2747,2754,2761,2767,2774,2781,
  2788,2795,2802,2809,2816,2823,2830,2837,2844,2850,2857,2864,2871,2878,2885,2893,
  2900,2907,2914,2921,2928,2935,2942,2949,2956,2963,2970,2978,2985,2992,2999,3006,
  3013,3021,3028,3035,3042,3049,3057,3064,3071,3078,3086,3093,3100,3108,3115,3122,
  3130,3137,3144,3152,3159,3166,3174,3181,3189,3196,3204,3211,3218,3226,3233,3241,
  3248,3256,3263,3271,3278,3286,3294,3301,3309,3316,3324,3331,3339,3347,3354,3362,
  3370,3377,3385,3393,3400,3408,3416,3423,3431,3439,3447,3454,3462,3470,3478,3486,
  3493,3501,3509,3517,3525,3533,3540,3548,3556,3564,3572,3580,3588,3596,3604,3612,
  3620,3628,3636,3644,3652,3660,3668,3676,3684,3692,3700,3708,3716,3724,3732,3740,
  3749,3757,3765,3773,3781,3789,3798,3806,3814,3822,3830,3839,3847,3855,3863,3872,
  3880,3888,3897,3905,3913,3922,3930,3938,3947,3955,3963,3972,3980,3989,3997,4006,
  4014,4022,4031,4039,4048,4056,4064,4095,4095,4095,4095,4095,4095,4095,4095,4095
};

const char *ADPhantom::PHANTOM_CnNameString[] = {
    "PHANTOM_C0_NAME",
    "PHANTOM_C1_NAME",
    "PHANTOM_C2_NAME",
    "PHANTOM_C3_NAME",
    "PHANTOM_C4_NAME",
    "PHANTOM_C5_NAME",
    "PHANTOM_C6_NAME",
    "PHANTOM_C7_NAME",
    "PHANTOM_C8_NAME",
    "PHANTOM_C9_NAME",
    "PHANTOM_C10_NAME",
    "PHANTOM_C11_NAME",
    "PHANTOM_C12_NAME",
    "PHANTOM_C13_NAME",
    "PHANTOM_C14_NAME",
    "PHANTOM_C15_NAME",
    "PHANTOM_C16_NAME",
    "PHANTOM_C17_NAME",
    "PHANTOM_C18_NAME",
    "PHANTOM_C19_NAME",
    "PHANTOM_C20_NAME",
    "PHANTOM_C21_NAME",
    "PHANTOM_C22_NAME",
    "PHANTOM_C23_NAME",
    "PHANTOM_C24_NAME",
    "PHANTOM_C25_NAME",
    "PHANTOM_C26_NAME",
    "PHANTOM_C27_NAME",
    "PHANTOM_C28_NAME",
    "PHANTOM_C29_NAME",
    "PHANTOM_C30_NAME",
    "PHANTOM_C31_NAME",
    "PHANTOM_C32_NAME",
    "PHANTOM_C33_NAME",
    "PHANTOM_C34_NAME",
    "PHANTOM_C35_NAME",
    "PHANTOM_C36_NAME",
    "PHANTOM_C37_NAME",
    "PHANTOM_C38_NAME",
    "PHANTOM_C39_NAME",
    "PHANTOM_C40_NAME",
    "PHANTOM_C41_NAME",
    "PHANTOM_C42_NAME",
    "PHANTOM_C43_NAME",
    "PHANTOM_C44_NAME",
    "PHANTOM_C45_NAME",
    "PHANTOM_C46_NAME",
    "PHANTOM_C47_NAME",
    "PHANTOM_C48_NAME",
    "PHANTOM_C49_NAME",
    "PHANTOM_C50_NAME",
    "PHANTOM_C51_NAME",
    "PHANTOM_C52_NAME",
    "PHANTOM_C53_NAME",
    "PHANTOM_C54_NAME",
    "PHANTOM_C55_NAME",
    "PHANTOM_C56_NAME",
    "PHANTOM_C57_NAME",
    "PHANTOM_C58_NAME",
    "PHANTOM_C59_NAME",
    "PHANTOM_C60_NAME",
    "PHANTOM_C61_NAME",
    "PHANTOM_C62_NAME",
    "PHANTOM_C63_NAME"
};

const char *ADPhantom::PHANTOM_CnWidthString[] = {
    "PHANTOM_C0_WIDTH",
    "PHANTOM_C1_WIDTH",
    "PHANTOM_C2_WIDTH",
    "PHANTOM_C3_WIDTH",
    "PHANTOM_C4_WIDTH",
    "PHANTOM_C5_WIDTH",
    "PHANTOM_C6_WIDTH",
    "PHANTOM_C7_WIDTH",
    "PHANTOM_C8_WIDTH",
    "PHANTOM_C9_WIDTH",
    "PHANTOM_C10_WIDTH",
    "PHANTOM_C11_WIDTH",
    "PHANTOM_C12_WIDTH",
    "PHANTOM_C13_WIDTH",
    "PHANTOM_C14_WIDTH",
    "PHANTOM_C15_WIDTH",
    "PHANTOM_C16_WIDTH",
    "PHANTOM_C17_WIDTH",
    "PHANTOM_C18_WIDTH",
    "PHANTOM_C19_WIDTH",
    "PHANTOM_C20_WIDTH",
    "PHANTOM_C21_WIDTH",
    "PHANTOM_C22_WIDTH",
    "PHANTOM_C23_WIDTH",
    "PHANTOM_C24_WIDTH",
    "PHANTOM_C25_WIDTH",
    "PHANTOM_C26_WIDTH",
    "PHANTOM_C27_WIDTH",
    "PHANTOM_C28_WIDTH",
    "PHANTOM_C29_WIDTH",
    "PHANTOM_C30_WIDTH",
    "PHANTOM_C31_WIDTH",
    "PHANTOM_C32_WIDTH",
    "PHANTOM_C33_WIDTH",
    "PHANTOM_C34_WIDTH",
    "PHANTOM_C35_WIDTH",
    "PHANTOM_C36_WIDTH",
    "PHANTOM_C37_WIDTH",
    "PHANTOM_C38_WIDTH",
    "PHANTOM_C39_WIDTH",
    "PHANTOM_C40_WIDTH",
    "PHANTOM_C41_WIDTH",
    "PHANTOM_C42_WIDTH",
    "PHANTOM_C43_WIDTH",
    "PHANTOM_C44_WIDTH",
    "PHANTOM_C45_WIDTH",
    "PHANTOM_C46_WIDTH",
    "PHANTOM_C47_WIDTH",
    "PHANTOM_C48_WIDTH",
    "PHANTOM_C49_WIDTH",
    "PHANTOM_C50_WIDTH",
    "PHANTOM_C51_WIDTH",
    "PHANTOM_C52_WIDTH",
    "PHANTOM_C53_WIDTH",
    "PHANTOM_C54_WIDTH",
    "PHANTOM_C55_WIDTH",
    "PHANTOM_C56_WIDTH",
    "PHANTOM_C57_WIDTH",
    "PHANTOM_C58_WIDTH",
    "PHANTOM_C59_WIDTH",
    "PHANTOM_C60_WIDTH",
    "PHANTOM_C61_WIDTH",
    "PHANTOM_C62_WIDTH",
    "PHANTOM_C63_WIDTH"
};

const char *ADPhantom::PHANTOM_CnHeightString[] = {
    "PHANTOM_C0_HEIGHT",
    "PHANTOM_C1_HEIGHT",
    "PHANTOM_C2_HEIGHT",
    "PHANTOM_C3_HEIGHT",
    "PHANTOM_C4_HEIGHT",
    "PHANTOM_C5_HEIGHT",
    "PHANTOM_C6_HEIGHT",
    "PHANTOM_C7_HEIGHT",
    "PHANTOM_C8_HEIGHT",
    "PHANTOM_C9_HEIGHT",
    "PHANTOM_C10_HEIGHT",
    "PHANTOM_C11_HEIGHT",
    "PHANTOM_C12_HEIGHT",
    "PHANTOM_C13_HEIGHT",
    "PHANTOM_C14_HEIGHT",
    "PHANTOM_C15_HEIGHT",
    "PHANTOM_C16_HEIGHT",
    "PHANTOM_C17_HEIGHT",
    "PHANTOM_C18_HEIGHT",
    "PHANTOM_C19_HEIGHT",
    "PHANTOM_C20_HEIGHT",
    "PHANTOM_C21_HEIGHT",
    "PHANTOM_C22_HEIGHT",
    "PHANTOM_C23_HEIGHT",
    "PHANTOM_C24_HEIGHT",
    "PHANTOM_C25_HEIGHT",
    "PHANTOM_C26_HEIGHT",
    "PHANTOM_C27_HEIGHT",
    "PHANTOM_C28_HEIGHT",
    "PHANTOM_C29_HEIGHT",
    "PHANTOM_C30_HEIGHT",
    "PHANTOM_C31_HEIGHT",
    "PHANTOM_C32_HEIGHT",
    "PHANTOM_C33_HEIGHT",
    "PHANTOM_C34_HEIGHT",
    "PHANTOM_C35_HEIGHT",
    "PHANTOM_C36_HEIGHT",
    "PHANTOM_C37_HEIGHT",
    "PHANTOM_C38_HEIGHT",
    "PHANTOM_C39_HEIGHT",
    "PHANTOM_C40_HEIGHT",
    "PHANTOM_C41_HEIGHT",
    "PHANTOM_C42_HEIGHT",
    "PHANTOM_C43_HEIGHT",
    "PHANTOM_C44_HEIGHT",
    "PHANTOM_C45_HEIGHT",
    "PHANTOM_C46_HEIGHT",
    "PHANTOM_C47_HEIGHT",
    "PHANTOM_C48_HEIGHT",
    "PHANTOM_C49_HEIGHT",
    "PHANTOM_C50_HEIGHT",
    "PHANTOM_C51_HEIGHT",
    "PHANTOM_C52_HEIGHT",
    "PHANTOM_C53_HEIGHT",
    "PHANTOM_C54_HEIGHT",
    "PHANTOM_C55_HEIGHT",
    "PHANTOM_C56_HEIGHT",
    "PHANTOM_C57_HEIGHT",
    "PHANTOM_C58_HEIGHT",
    "PHANTOM_C59_HEIGHT",
    "PHANTOM_C60_HEIGHT",
    "PHANTOM_C61_HEIGHT",
    "PHANTOM_C62_HEIGHT",
    "PHANTOM_C63_HEIGHT"
};

const char *ADPhantom::PHANTOM_CnStatusString[] = {
    "PHANTOM_C0_STATUS",
    "PHANTOM_C1_STATUS",
    "PHANTOM_C2_STATUS",
    "PHANTOM_C3_STATUS",
    "PHANTOM_C4_STATUS",
    "PHANTOM_C5_STATUS",
    "PHANTOM_C6_STATUS",
    "PHANTOM_C7_STATUS",
    "PHANTOM_C8_STATUS",
    "PHANTOM_C9_STATUS",
    "PHANTOM_C10_STATUS",
    "PHANTOM_C11_STATUS",
    "PHANTOM_C12_STATUS",
    "PHANTOM_C13_STATUS",
    "PHANTOM_C14_STATUS",
    "PHANTOM_C15_STATUS",
    "PHANTOM_C16_STATUS",
    "PHANTOM_C17_STATUS",
    "PHANTOM_C18_STATUS",
    "PHANTOM_C19_STATUS",
    "PHANTOM_C20_STATUS",
    "PHANTOM_C21_STATUS",
    "PHANTOM_C22_STATUS",
    "PHANTOM_C23_STATUS",
    "PHANTOM_C24_STATUS",
    "PHANTOM_C25_STATUS",
    "PHANTOM_C26_STATUS",
    "PHANTOM_C27_STATUS",
    "PHANTOM_C28_STATUS",
    "PHANTOM_C29_STATUS",
    "PHANTOM_C30_STATUS",
    "PHANTOM_C31_STATUS",
    "PHANTOM_C32_STATUS",
    "PHANTOM_C33_STATUS",
    "PHANTOM_C34_STATUS",
    "PHANTOM_C35_STATUS",
    "PHANTOM_C36_STATUS",
    "PHANTOM_C37_STATUS",
    "PHANTOM_C38_STATUS",
    "PHANTOM_C39_STATUS",
    "PHANTOM_C40_STATUS",
    "PHANTOM_C41_STATUS",
    "PHANTOM_C42_STATUS",
    "PHANTOM_C43_STATUS",
    "PHANTOM_C44_STATUS",
    "PHANTOM_C45_STATUS",
    "PHANTOM_C46_STATUS",
    "PHANTOM_C47_STATUS",
    "PHANTOM_C48_STATUS",
    "PHANTOM_C49_STATUS",
    "PHANTOM_C50_STATUS",
    "PHANTOM_C51_STATUS",
    "PHANTOM_C52_STATUS",
    "PHANTOM_C53_STATUS",
    "PHANTOM_C54_STATUS",
    "PHANTOM_C55_STATUS",
    "PHANTOM_C56_STATUS",
    "PHANTOM_C57_STATUS",
    "PHANTOM_C58_STATUS",
    "PHANTOM_C59_STATUS",
    "PHANTOM_C60_STATUS",
    "PHANTOM_C61_STATUS",
    "PHANTOM_C62_STATUS",
    "PHANTOM_C63_STATUS"
};

const char *ADPhantom::PHANTOM_CnFrameCountString[] = {
    "PHANTOM_C0_FR_COUNT",
    "PHANTOM_C1_FR_COUNT",
    "PHANTOM_C2_FR_COUNT",
    "PHANTOM_C3_FR_COUNT",
    "PHANTOM_C4_FR_COUNT",
    "PHANTOM_C5_FR_COUNT",
    "PHANTOM_C6_FR_COUNT",
    "PHANTOM_C7_FR_COUNT",
    "PHANTOM_C8_FR_COUNT",
    "PHANTOM_C9_FR_COUNT",
    "PHANTOM_C10_FR_COUNT",
    "PHANTOM_C11_FR_COUNT",
    "PHANTOM_C12_FR_COUNT",
    "PHANTOM_C13_FR_COUNT",
    "PHANTOM_C14_FR_COUNT",
    "PHANTOM_C15_FR_COUNT",
    "PHANTOM_C16_FR_COUNT",
    "PHANTOM_C17_FR_COUNT",
    "PHANTOM_C18_FR_COUNT",
    "PHANTOM_C19_FR_COUNT",
    "PHANTOM_C20_FR_COUNT",
    "PHANTOM_C21_FR_COUNT",
    "PHANTOM_C22_FR_COUNT",
    "PHANTOM_C23_FR_COUNT",
    "PHANTOM_C24_FR_COUNT",
    "PHANTOM_C25_FR_COUNT",
    "PHANTOM_C26_FR_COUNT",
    "PHANTOM_C27_FR_COUNT",
    "PHANTOM_C28_FR_COUNT",
    "PHANTOM_C29_FR_COUNT",
    "PHANTOM_C30_FR_COUNT",
    "PHANTOM_C31_FR_COUNT",
    "PHANTOM_C32_FR_COUNT",
    "PHANTOM_C33_FR_COUNT",
    "PHANTOM_C34_FR_COUNT",
    "PHANTOM_C35_FR_COUNT",
    "PHANTOM_C36_FR_COUNT",
    "PHANTOM_C37_FR_COUNT",
    "PHANTOM_C38_FR_COUNT",
    "PHANTOM_C39_FR_COUNT",
    "PHANTOM_C40_FR_COUNT",
    "PHANTOM_C41_FR_COUNT",
    "PHANTOM_C42_FR_COUNT",
    "PHANTOM_C43_FR_COUNT",
    "PHANTOM_C44_FR_COUNT",
    "PHANTOM_C45_FR_COUNT",
    "PHANTOM_C46_FR_COUNT",
    "PHANTOM_C47_FR_COUNT",
    "PHANTOM_C48_FR_COUNT",
    "PHANTOM_C49_FR_COUNT",
    "PHANTOM_C50_FR_COUNT",
    "PHANTOM_C51_FR_COUNT",
    "PHANTOM_C52_FR_COUNT",
    "PHANTOM_C53_FR_COUNT",
    "PHANTOM_C54_FR_COUNT",
    "PHANTOM_C55_FR_COUNT",
    "PHANTOM_C56_FR_COUNT",
    "PHANTOM_C57_FR_COUNT",
    "PHANTOM_C58_FR_COUNT",
    "PHANTOM_C59_FR_COUNT",
    "PHANTOM_C60_FR_COUNT",
    "PHANTOM_C61_FR_COUNT",
    "PHANTOM_C62_FR_COUNT",
    "PHANTOM_C63_FR_COUNT"
};

const char *ADPhantom::PHANTOM_CnFirstFrameString[] = {
    "PHANTOM_C0_FIRST_FR",
    "PHANTOM_C1_FIRST_FR",
    "PHANTOM_C2_FIRST_FR",
    "PHANTOM_C3_FIRST_FR",
    "PHANTOM_C4_FIRST_FR",
    "PHANTOM_C5_FIRST_FR",
    "PHANTOM_C6_FIRST_FR",
    "PHANTOM_C7_FIRST_FR",
    "PHANTOM_C8_FIRST_FR",
    "PHANTOM_C9_FIRST_FR",
    "PHANTOM_C10_FIRST_FR",
    "PHANTOM_C11_FIRST_FR",
    "PHANTOM_C12_FIRST_FR",
    "PHANTOM_C13_FIRST_FR",
    "PHANTOM_C14_FIRST_FR",
    "PHANTOM_C15_FIRST_FR",
    "PHANTOM_C16_FIRST_FR",
    "PHANTOM_C17_FIRST_FR",
    "PHANTOM_C18_FIRST_FR",
    "PHANTOM_C19_FIRST_FR",
    "PHANTOM_C20_FIRST_FR",
    "PHANTOM_C21_FIRST_FR",
    "PHANTOM_C22_FIRST_FR",
    "PHANTOM_C23_FIRST_FR",
    "PHANTOM_C24_FIRST_FR",
    "PHANTOM_C25_FIRST_FR",
    "PHANTOM_C26_FIRST_FR",
    "PHANTOM_C27_FIRST_FR",
    "PHANTOM_C28_FIRST_FR",
    "PHANTOM_C29_FIRST_FR",
    "PHANTOM_C30_FIRST_FR",
    "PHANTOM_C31_FIRST_FR",
    "PHANTOM_C32_FIRST_FR",
    "PHANTOM_C33_FIRST_FR",
    "PHANTOM_C34_FIRST_FR",
    "PHANTOM_C35_FIRST_FR",
    "PHANTOM_C36_FIRST_FR",
    "PHANTOM_C37_FIRST_FR",
    "PHANTOM_C38_FIRST_FR",
    "PHANTOM_C39_FIRST_FR",
    "PHANTOM_C40_FIRST_FR",
    "PHANTOM_C41_FIRST_FR",
    "PHANTOM_C42_FIRST_FR",
    "PHANTOM_C43_FIRST_FR",
    "PHANTOM_C44_FIRST_FR",
    "PHANTOM_C45_FIRST_FR",
    "PHANTOM_C46_FIRST_FR",
    "PHANTOM_C47_FIRST_FR",
    "PHANTOM_C48_FIRST_FR",
    "PHANTOM_C49_FIRST_FR",
    "PHANTOM_C50_FIRST_FR",
    "PHANTOM_C51_FIRST_FR",
    "PHANTOM_C52_FIRST_FR",
    "PHANTOM_C53_FIRST_FR",
    "PHANTOM_C54_FIRST_FR",
    "PHANTOM_C55_FIRST_FR",
    "PHANTOM_C56_FIRST_FR",
    "PHANTOM_C57_FIRST_FR",
    "PHANTOM_C58_FIRST_FR",
    "PHANTOM_C59_FIRST_FR",
    "PHANTOM_C60_FIRST_FR",
    "PHANTOM_C61_FIRST_FR",
    "PHANTOM_C62_FIRST_FR",
    "PHANTOM_C63_FIRST_FR"
};

const char *ADPhantom::PHANTOM_CnLastFrameString[] = {
    "PHANTOM_C0_LAST_FR",
    "PHANTOM_C1_LAST_FR",
    "PHANTOM_C2_LAST_FR",
    "PHANTOM_C3_LAST_FR",
    "PHANTOM_C4_LAST_FR",
    "PHANTOM_C5_LAST_FR",
    "PHANTOM_C6_LAST_FR",
    "PHANTOM_C7_LAST_FR",
    "PHANTOM_C8_LAST_FR",
    "PHANTOM_C9_LAST_FR",
    "PHANTOM_C10_LAST_FR",
    "PHANTOM_C11_LAST_FR",
    "PHANTOM_C12_LAST_FR",
    "PHANTOM_C13_LAST_FR",
    "PHANTOM_C14_LAST_FR",
    "PHANTOM_C15_LAST_FR",
    "PHANTOM_C16_LAST_FR",
    "PHANTOM_C17_LAST_FR",
    "PHANTOM_C18_LAST_FR",
    "PHANTOM_C19_LAST_FR",
    "PHANTOM_C20_LAST_FR",
    "PHANTOM_C21_LAST_FR",
    "PHANTOM_C22_LAST_FR",
    "PHANTOM_C23_LAST_FR",
    "PHANTOM_C24_LAST_FR",
    "PHANTOM_C25_LAST_FR",
    "PHANTOM_C26_LAST_FR",
    "PHANTOM_C27_LAST_FR",
    "PHANTOM_C28_LAST_FR",
    "PHANTOM_C29_LAST_FR",
    "PHANTOM_C30_LAST_FR",
    "PHANTOM_C31_LAST_FR",
    "PHANTOM_C32_LAST_FR",
    "PHANTOM_C33_LAST_FR",
    "PHANTOM_C34_LAST_FR",
    "PHANTOM_C35_LAST_FR",
    "PHANTOM_C36_LAST_FR",
    "PHANTOM_C37_LAST_FR",
    "PHANTOM_C38_LAST_FR",
    "PHANTOM_C39_LAST_FR",
    "PHANTOM_C40_LAST_FR",
    "PHANTOM_C41_LAST_FR",
    "PHANTOM_C42_LAST_FR",
    "PHANTOM_C43_LAST_FR",
    "PHANTOM_C44_LAST_FR",
    "PHANTOM_C45_LAST_FR",
    "PHANTOM_C46_LAST_FR",
    "PHANTOM_C47_LAST_FR",
    "PHANTOM_C48_LAST_FR",
    "PHANTOM_C49_LAST_FR",
    "PHANTOM_C50_LAST_FR",
    "PHANTOM_C51_LAST_FR",
    "PHANTOM_C52_LAST_FR",
    "PHANTOM_C53_LAST_FR",
    "PHANTOM_C54_LAST_FR",
    "PHANTOM_C55_LAST_FR",
    "PHANTOM_C56_LAST_FR",
    "PHANTOM_C57_LAST_FR",
    "PHANTOM_C58_LAST_FR",
    "PHANTOM_C59_LAST_FR",
    "PHANTOM_C60_LAST_FR",
    "PHANTOM_C61_LAST_FR",
    "PHANTOM_C62_LAST_FR",
    "PHANTOM_C63_LAST_FR"
};

const char *ADPhantom::PHANTOM_CnRecordStartString[] = {
    "PHANTOM_C0_REC_START",
    "PHANTOM_C1_REC_START",
    "PHANTOM_C2_REC_START",
    "PHANTOM_C3_REC_START",
    "PHANTOM_C4_REC_START",
    "PHANTOM_C5_REC_START",
    "PHANTOM_C6_REC_START",
    "PHANTOM_C7_REC_START",
    "PHANTOM_C8_REC_START",
    "PHANTOM_C9_REC_START",
    "PHANTOM_C10_REC_START",
    "PHANTOM_C11_REC_START",
    "PHANTOM_C12_REC_START",
    "PHANTOM_C13_REC_START",
    "PHANTOM_C14_REC_START",
    "PHANTOM_C15_REC_START",
    "PHANTOM_C16_REC_START",
    "PHANTOM_C17_REC_START",
    "PHANTOM_C18_REC_START",
    "PHANTOM_C19_REC_START",
    "PHANTOM_C20_REC_START",
    "PHANTOM_C21_REC_START",
    "PHANTOM_C22_REC_START",
    "PHANTOM_C23_REC_START",
    "PHANTOM_C24_REC_START",
    "PHANTOM_C25_REC_START",
    "PHANTOM_C26_REC_START",
    "PHANTOM_C27_REC_START",
    "PHANTOM_C28_REC_START",
    "PHANTOM_C29_REC_START",
    "PHANTOM_C30_REC_START",
    "PHANTOM_C31_REC_START",
    "PHANTOM_C32_REC_START",
    "PHANTOM_C33_REC_START",
    "PHANTOM_C34_REC_START",
    "PHANTOM_C35_REC_START",
    "PHANTOM_C36_REC_START",
    "PHANTOM_C37_REC_START",
    "PHANTOM_C38_REC_START",
    "PHANTOM_C39_REC_START",
    "PHANTOM_C40_REC_START",
    "PHANTOM_C41_REC_START",
    "PHANTOM_C42_REC_START",
    "PHANTOM_C43_REC_START",
    "PHANTOM_C44_REC_START",
    "PHANTOM_C45_REC_START",
    "PHANTOM_C46_REC_START",
    "PHANTOM_C47_REC_START",
    "PHANTOM_C48_REC_START",
    "PHANTOM_C49_REC_START",
    "PHANTOM_C50_REC_START",
    "PHANTOM_C51_REC_START",
    "PHANTOM_C52_REC_START",
    "PHANTOM_C53_REC_START",
    "PHANTOM_C54_REC_START",
    "PHANTOM_C55_REC_START",
    "PHANTOM_C56_REC_START",
    "PHANTOM_C57_REC_START",
    "PHANTOM_C58_REC_START",
    "PHANTOM_C59_REC_START",
    "PHANTOM_C60_REC_START",
    "PHANTOM_C61_REC_START",
    "PHANTOM_C62_REC_START",
    "PHANTOM_C63_REC_START"
};

const char *ADPhantom::PHANTOM_CnRecordEndString[] = {
    "PHANTOM_C0_REC_END",
    "PHANTOM_C1_REC_END",
    "PHANTOM_C2_REC_END",
    "PHANTOM_C3_REC_END",
    "PHANTOM_C4_REC_END",
    "PHANTOM_C5_REC_END",
    "PHANTOM_C6_REC_END",
    "PHANTOM_C7_REC_END",
    "PHANTOM_C8_REC_END",
    "PHANTOM_C9_REC_END",
    "PHANTOM_C10_REC_END",
    "PHANTOM_C11_REC_END",
    "PHANTOM_C12_REC_END",
    "PHANTOM_C13_REC_END",
    "PHANTOM_C14_REC_END",
    "PHANTOM_C15_REC_END",
    "PHANTOM_C16_REC_END",
    "PHANTOM_C17_REC_END",
    "PHANTOM_C18_REC_END",
    "PHANTOM_C19_REC_END",
    "PHANTOM_C20_REC_END",
    "PHANTOM_C21_REC_END",
    "PHANTOM_C22_REC_END",
    "PHANTOM_C23_REC_END",
    "PHANTOM_C24_REC_END",
    "PHANTOM_C25_REC_END",
    "PHANTOM_C26_REC_END",
    "PHANTOM_C27_REC_END",
    "PHANTOM_C28_REC_END",
    "PHANTOM_C29_REC_END",
    "PHANTOM_C30_REC_END",
    "PHANTOM_C31_REC_END",
    "PHANTOM_C32_REC_END",
    "PHANTOM_C33_REC_END",
    "PHANTOM_C34_REC_END",
    "PHANTOM_C35_REC_END",
    "PHANTOM_C36_REC_END",
    "PHANTOM_C37_REC_END",
    "PHANTOM_C38_REC_END",
    "PHANTOM_C39_REC_END",
    "PHANTOM_C40_REC_END",
    "PHANTOM_C41_REC_END",
    "PHANTOM_C42_REC_END",
    "PHANTOM_C43_REC_END",
    "PHANTOM_C44_REC_END",
    "PHANTOM_C45_REC_END",
    "PHANTOM_C46_REC_END",
    "PHANTOM_C47_REC_END",
    "PHANTOM_C48_REC_END",
    "PHANTOM_C49_REC_END",
    "PHANTOM_C50_REC_END",
    "PHANTOM_C51_REC_END",
    "PHANTOM_C52_REC_END",
    "PHANTOM_C53_REC_END",
    "PHANTOM_C54_REC_END",
    "PHANTOM_C55_REC_END",
    "PHANTOM_C56_REC_END",
    "PHANTOM_C57_REC_END",
    "PHANTOM_C58_REC_END",
    "PHANTOM_C59_REC_END",
    "PHANTOM_C60_REC_END",
    "PHANTOM_C61_REC_END",
    "PHANTOM_C62_REC_END",
    "PHANTOM_C63_REC_END"
};

const char *ADPhantom::PHANTOM_CnRecordString[] = {
    "PHANTOM_C0_RECORD",
    "PHANTOM_C1_RECORD",
    "PHANTOM_C2_RECORD",
    "PHANTOM_C3_RECORD",
    "PHANTOM_C4_RECORD",
    "PHANTOM_C5_RECORD",
    "PHANTOM_C6_RECORD",
    "PHANTOM_C7_RECORD",
    "PHANTOM_C8_RECORD",
    "PHANTOM_C9_RECORD",
    "PHANTOM_C10_RECORD",
    "PHANTOM_C11_RECORD",
    "PHANTOM_C12_RECORD",
    "PHANTOM_C13_RECORD",
    "PHANTOM_C14_RECORD",
    "PHANTOM_C15_RECORD",
    "PHANTOM_C16_RECORD",
    "PHANTOM_C17_RECORD",
    "PHANTOM_C18_RECORD",
    "PHANTOM_C19_RECORD",
    "PHANTOM_C20_RECORD",
    "PHANTOM_C21_RECORD",
    "PHANTOM_C22_RECORD",
    "PHANTOM_C23_RECORD",
    "PHANTOM_C24_RECORD",
    "PHANTOM_C25_RECORD",
    "PHANTOM_C26_RECORD",
    "PHANTOM_C27_RECORD",
    "PHANTOM_C28_RECORD",
    "PHANTOM_C29_RECORD",
    "PHANTOM_C30_RECORD",
    "PHANTOM_C31_RECORD",
    "PHANTOM_C32_RECORD",
    "PHANTOM_C33_RECORD",
    "PHANTOM_C34_RECORD",
    "PHANTOM_C35_RECORD",
    "PHANTOM_C36_RECORD",
    "PHANTOM_C37_RECORD",
    "PHANTOM_C38_RECORD",
    "PHANTOM_C39_RECORD",
    "PHANTOM_C40_RECORD",
    "PHANTOM_C41_RECORD",
    "PHANTOM_C42_RECORD",
    "PHANTOM_C43_RECORD",
    "PHANTOM_C44_RECORD",
    "PHANTOM_C45_RECORD",
    "PHANTOM_C46_RECORD",
    "PHANTOM_C47_RECORD",
    "PHANTOM_C48_RECORD",
    "PHANTOM_C49_RECORD",
    "PHANTOM_C50_RECORD",
    "PHANTOM_C51_RECORD",
    "PHANTOM_C52_RECORD",
    "PHANTOM_C53_RECORD",
    "PHANTOM_C54_RECORD",
    "PHANTOM_C55_RECORD",
    "PHANTOM_C56_RECORD",
    "PHANTOM_C57_RECORD",
    "PHANTOM_C58_RECORD",
    "PHANTOM_C59_RECORD",
    "PHANTOM_C60_RECORD",
    "PHANTOM_C61_RECORD",
    "PHANTOM_C62_RECORD",
    "PHANTOM_C63_RECORD"
};

const char *ADPhantom::PHANTOM_CnRecordCountString[] = {
    "PHANTOM_C0_REC_COUNT",
    "PHANTOM_C1_REC_COUNT",
    "PHANTOM_C2_REC_COUNT",
    "PHANTOM_C3_REC_COUNT",
    "PHANTOM_C4_REC_COUNT",
    "PHANTOM_C5_REC_COUNT",
    "PHANTOM_C6_REC_COUNT",
    "PHANTOM_C7_REC_COUNT",
    "PHANTOM_C8_REC_COUNT",
    "PHANTOM_C9_REC_COUNT",
    "PHANTOM_C10_REC_COUNT",
    "PHANTOM_C11_REC_COUNT",
    "PHANTOM_C12_REC_COUNT",
    "PHANTOM_C13_REC_COUNT",
    "PHANTOM_C14_REC_COUNT",
    "PHANTOM_C15_REC_COUNT",
    "PHANTOM_C16_REC_COUNT",
    "PHANTOM_C17_REC_COUNT",
    "PHANTOM_C18_REC_COUNT",
    "PHANTOM_C19_REC_COUNT",
    "PHANTOM_C20_REC_COUNT",
    "PHANTOM_C21_REC_COUNT",
    "PHANTOM_C22_REC_COUNT",
    "PHANTOM_C23_REC_COUNT",
    "PHANTOM_C24_REC_COUNT",
    "PHANTOM_C25_REC_COUNT",
    "PHANTOM_C26_REC_COUNT",
    "PHANTOM_C27_REC_COUNT",
    "PHANTOM_C28_REC_COUNT",
    "PHANTOM_C29_REC_COUNT",
    "PHANTOM_C30_REC_COUNT",
    "PHANTOM_C31_REC_COUNT",
    "PHANTOM_C32_REC_COUNT",
    "PHANTOM_C33_REC_COUNT",
    "PHANTOM_C34_REC_COUNT",
    "PHANTOM_C35_REC_COUNT",
    "PHANTOM_C36_REC_COUNT",
    "PHANTOM_C37_REC_COUNT",
    "PHANTOM_C38_REC_COUNT",
    "PHANTOM_C39_REC_COUNT",
    "PHANTOM_C40_REC_COUNT",
    "PHANTOM_C41_REC_COUNT",
    "PHANTOM_C42_REC_COUNT",
    "PHANTOM_C43_REC_COUNT",
    "PHANTOM_C44_REC_COUNT",
    "PHANTOM_C45_REC_COUNT",
    "PHANTOM_C46_REC_COUNT",
    "PHANTOM_C47_REC_COUNT",
    "PHANTOM_C48_REC_COUNT",
    "PHANTOM_C49_REC_COUNT",
    "PHANTOM_C50_REC_COUNT",
    "PHANTOM_C51_REC_COUNT",
    "PHANTOM_C52_REC_COUNT",
    "PHANTOM_C53_REC_COUNT",
    "PHANTOM_C54_REC_COUNT",
    "PHANTOM_C55_REC_COUNT",
    "PHANTOM_C56_REC_COUNT",
    "PHANTOM_C57_REC_COUNT",
    "PHANTOM_C58_REC_COUNT",
    "PHANTOM_C59_REC_COUNT",
    "PHANTOM_C60_REC_COUNT",
    "PHANTOM_C61_REC_COUNT",
    "PHANTOM_C62_REC_COUNT",
    "PHANTOM_C63_REC_COUNT"
};

const char *ADPhantom::PHANTOM_CfFileNameString[] = {
    "PHANTOM_CF1_FILE_NAME",
    "PHANTOM_CF2_FILE_NAME",
    "PHANTOM_CF3_FILE_NAME",
    "PHANTOM_CF4_FILE_NAME",
    "PHANTOM_CF5_FILE_NAME",
    "PHANTOM_CF6_FILE_NAME",
    "PHANTOM_CF7_FILE_NAME",
    "PHANTOM_CF8_FILE_NAME"
};

const char *ADPhantom::PHANTOM_CfFileSizeString[] = {
    "PHANTOM_CF1_FILE_SIZE",
    "PHANTOM_CF2_FILE_SIZE",
    "PHANTOM_CF3_FILE_SIZE",
    "PHANTOM_CF4_FILE_SIZE",
    "PHANTOM_CF5_FILE_SIZE",
    "PHANTOM_CF6_FILE_SIZE",
    "PHANTOM_CF7_FILE_SIZE",
    "PHANTOM_CF8_FILE_SIZE"
};

const char *ADPhantom::PHANTOM_CfFileDateString[] = {
    "PHANTOM_CF1_FILE_DATE",
    "PHANTOM_CF2_FILE_DATE",
    "PHANTOM_CF3_FILE_DATE",
    "PHANTOM_CF4_FILE_DATE",
    "PHANTOM_CF5_FILE_DATE",
    "PHANTOM_CF6_FILE_DATE",
    "PHANTOM_CF7_FILE_DATE",
    "PHANTOM_CF8_FILE_DATE"
};

/**
 * A bit of C glue to make the config function available in the startup script (ioc shell) 
 */
extern "C"
{
  int ADPhantomConfig(const char *portName, const char *ctrlPort, const char *dataPort, int maxBuffers, size_t maxMemory, int priority,	int stackSize)
  {
    new ADPhantom(portName, ctrlPort, dataPort, maxBuffers, maxMemory, priority, stackSize);
    return asynSuccess;
  }

  int phantomSetDebugLevel(const char *driver, const char *method, int debug)
  {
    ADPhantom *sA;
    static const char *functionName = "phantomSetDebugLevel";

    sA = (ADPhantom*)findAsynPortDriver(driver);
    if (!sA){
      printf("%s: Error: port %s not found.", functionName, driver);
      return -1;
    }
    return sA->debugLevel(method, debug);
  }
}

/**
 * Function to convert pixel data in parallel
 */
static void phantomPixelConversionTaskC(void *drvPvt)
{
  ADPhantom *pPvt = (ADPhantom *)drvPvt;
  pPvt->phantomConversionTask();
}

/**
 * Function to run the camera task within a separate thread in C++
 */
static void phantomCameraTaskC(void *drvPvt)
{
  ADPhantom *pPvt = (ADPhantom *)drvPvt;
  pPvt->phantomCameraTask();
}

/**
 * Function to run the status task within a separate thread in C++
 */
static void phantomStatusTaskC(void *drvPvt)
{
  ADPhantom *pPvt = (ADPhantom *)drvPvt;
  pPvt->phantomStatusTask();
}

/**
 * Function to run the camera task within a separate thread in C++
 */
static void phantomPreviewTaskC(void *drvPvt)
{
  ADPhantom *pPvt = (ADPhantom *)drvPvt;
  pPvt->phantomPreviewTask();
}

/**
 * Function to run the flash delete and format task within a separate thread in C++
 */
static void phantomFlashTaskC(void *drvPvt)
{
  ADPhantom *pPvt = (ADPhantom *)drvPvt;
  pPvt->phantomFlashTask();
}

/**
 * Function to run the download task within a separate thread in C++
 */
static void phantomDownloadTaskC(void *drvPvt)
{
  ADPhantom *pPvt = (ADPhantom *)drvPvt;
  pPvt->phantomDownloadTask();
}

/**
 * ADPhantom destructor
 */
ADPhantom::~ADPhantom()
{
}

ADPhantom::ADPhantom(const char *portName, const char *ctrlPort, const char *dataPort, int maxBuffers, size_t maxMemory, int priority, int stackSize) :
/** 
    Constructor for the ADDriver class. 
[in]	portNameIn	The name of the asyn port.
[in]	maxAddr 	The maximum number of asyn addr addresses this driver supports. Often 
                        it is 1 (which is the minimum). This controls the number of parameter tables that are 
                        created.
[in]	numParams	The number of parameters that this driver supports.
[in]	maxBuffers	The maximum number of NDArray buffers that the NDArrayPool for this driver is allowed to 
                        allocate. Set this to 0 to allow an unlimited number of buffers.
[in]	maxMemory	The maximum amount of memory that the NDArrayPool for this driver is allowed to allocate. 
                        Set this to 0 to allow an unlimited amount of memory. 
[in]	interfaceMask	Bit mask defining the asyn interfaces that this driver supports. The bit mask values are 
                        defined in asynPortDriver.h, e.g. asynInt32Mask.
[in]	interruptMask	Bit mask definining the asyn interfaces that can generate interrupts (callbacks). The bit 
                        mask values are defined in asynPortDriver.h, e.g. asynInt8ArrayMask.
[in]	asynFlags	Flags when creating the asyn port driver; includes ASYN_CANBLOCK and ASYN_MULTIDEVICE.
[in]	autoConnect	The autoConnect flag for the asyn port driver. 1 if the driver should autoconnect.
[in]	priority	The thread priority for the asyn port driver thread if ASYN_CANBLOCK is set in asynFlags. 
                        If it is 0 then the default value of epicsThreadPriorityMedium will be assigned by 
                        asynManager.
[in]	stackSize	The stack size for the asyn port driver thread if ASYN_CANBLOCK is set in asynFlags. If it 
                        is 0 then the default value of epicsThreadGetStackSize(epicsThreadStackMedium) will be  
                        assigned by asynManager. 
*/
  ADDriver(portName,
           1,
           NUM_PHANTOM_PARAMS,
           maxBuffers,
           maxMemory,
           asynEnumMask | asynFloat64ArrayMask,
           asynEnumMask | asynFloat64ArrayMask, /* No interfaces beyond those set in ADDriver.cpp */
           ASYN_CANBLOCK,
           1, //asynflags (CANBLOCK means separate thread for this driver)
           priority,
           stackSize) // thread priority and stack size (0=default)
{
  static const char *functionName = "ADPhantom::ADPhantom";
  int status = asynSuccess;
  int index = 0;
  
  //Time profiling 
  clock_gettime(CLOCK_MONOTONIC_RAW, &frameStart_);
  clock_gettime(CLOCK_MONOTONIC_RAW, &readStart_);

  // Setup flag to state this is our first connection
  firstConnect_ = true;

  // Initialise the debugger
  initDebugger(0);

  //Initialize non static data members
  portUser_  = NULL;
  dataChannel_ = NULL;
  strcpy(ctrlPort_, ctrlPort);
  strcpy(dataPort_, dataPort);
  
  // Create the epicsEvents for signalling to the PHANTOM task when acquisition starts
  this->startEventId_ = epicsEventCreate(epicsEventEmpty);
  if (!this->startEventId_){
    debug(functionName, "epicsEventCreate failure for start event");
    status = asynError;
  }

  // Create the epicsEvents for signalling to the PHANTOM Analyser task when acquisition stops
  this->stopEventId_ = epicsEventCreate(epicsEventEmpty);
  if (!this->stopEventId_){
    debug(functionName, "epicsEventCreate failure for stop event");
    status = asynError;
  }

  // Create the epicsEvents for signalling to the PHANTOM task when acquisition starts
  this->startPreviewEventId_ = epicsEventCreate(epicsEventEmpty);
  if (!this->startPreviewEventId_){
    debug(functionName, "epicsEventCreate failure for start preview event");
    status = asynError;
  }

  // Create the epicsEvents for signalling to the PHANTOM Analyser task when acquisition stops
  this->stopPreviewEventId_ = epicsEventCreate(epicsEventEmpty);
  if (!this->stopPreviewEventId_){
    debug(functionName, "epicsEventCreate failure for stop preview event");
    status = asynError;
  }

  // Create the epicsEvents for signalling to the PHANTOM flash task when format or file delete occurs
  this->flashEventId_ = epicsEventCreate(epicsEventEmpty);
  if (!this->flashEventId_){
    debug(functionName, "epicsEventCreate failure to create flash event");
    status = asynError;
  }

  // Create the epicsEvents for signalling to the PHANTOM task when download starts
  this->startDownloadEventId_ = epicsEventCreate(epicsEventEmpty);
  if (!this->startDownloadEventId_){
    debug(functionName, "epicsEventCreate failure for start download event");
    status = asynError;
  }

  // Create all PHANTOM parameters
  createParam(PHANTOMConnectString,                   asynParamInt32,         &PHANTOMConnect_);
  createParam(PHANTOMConnectedString,                 asynParamInt32,         &PHANTOMConnected_);
  createParam(PHANTOM_AcquireStateString,             asynParamInt32,         &PHANTOM_AcquireState_);
  createParam(PHANTOM_SoftwareTriggerString,          asynParamInt32,         &PHANTOM_SoftwareTrigger_);
  createParam(PHANTOM_PerformCSRString,               asynParamInt32,         &PHANTOM_PerformCSR_);
  createParam(PHANTOM_CSRCountString,                 asynParamInt32,         &PHANTOM_CSRCount_);
  createParam(PHANTOM_SettingsSlotString,             asynParamInt32,         &PHANTOM_SettingsSlot_);
  createParam(PHANTOM_SettingsSaveString,             asynParamInt32,         &PHANTOM_SettingsSave_);
  createParam(PHANTOM_SettingsLoadString,             asynParamInt32,         &PHANTOM_SettingsLoad_);
  createParam(PHANTOM_AutoAdvanceString,              asynParamInt32,         &PHANTOM_AutoAdvance_);
  createParam(PHANTOM_AutoSaveString,                 asynParamInt32,         &PHANTOM_AutoSave_);
  createParam(PHANTOM_AutoRestartString,              asynParamInt32,         &PHANTOM_AutoRestart_);
  createParam(PHANTOM_AutoCSRString,                  asynParamInt32,         &PHANTOM_AutoCSR_);
  createParam(PHANTOM_PostTrigCountString,            asynParamInt32,         &PHANTOM_PostTrigCount_);
  createParam(PHANTOM_TotalFrameCountString,          asynParamInt32,         &PHANTOM_TotalFrameCount_);
  createParam(PHANTOM_MaxFrameCountString,            asynParamInt32,         &PHANTOM_MaxFrameCount_);
  createParam(PHANTOM_SelectedCineString,             asynParamInt32,         &PHANTOM_SelectedCine_);
  createParam(PHANTOM_CineNameString,                 asynParamOctet,         &PHANTOM_CineName_);
  createParam(PHANTOM_CineWidthString,                asynParamInt32,         &PHANTOM_CineWidth_);
  createParam(PHANTOM_CineHeightString,               asynParamInt32,         &PHANTOM_CineHeight_);
  createParam(PHANTOM_CineFrameCountString,           asynParamInt32,         &PHANTOM_CineFrameCount_);
  createParam(PHANTOM_CineFirstFrameString,           asynParamInt32,         &PHANTOM_CineFirstFrame_);
  createParam(PHANTOM_CineLastFrameString,            asynParamInt32,         &PHANTOM_CineLastFrame_);
  createParam(PHANTOM_DownloadStartFrameString,       asynParamInt32,         &PHANTOM_DownloadStartFrame_);
  createParam(PHANTOM_DownloadEndFrameString,         asynParamInt32,         &PHANTOM_DownloadEndFrame_);
  createParam(PHANTOM_DownloadStartCineString,        asynParamInt32,         &PHANTOM_DownloadStartCine_);
  createParam(PHANTOM_DownloadEndCineString,          asynParamInt32,         &PHANTOM_DownloadEndCine_);
  createParam(PHANTOM_DownloadString,                 asynParamInt32,         &PHANTOM_Download_);
  createParam(PHANTOM_DownloadAbortString,            asynParamInt32,         &PHANTOM_DownloadAbort_);
  createParam(PHANTOM_DownloadCountString,            asynParamInt32,         &PHANTOM_DownloadCount_);
  createParam(PHANTOM_DownloadFrameModeString,        asynParamInt32,         &PHANTOM_DownloadFrameMode_);
  createParam(PHANTOM_MarkCineSavedString,            asynParamInt32,         &PHANTOM_MarkCineSaved_);
  createParam(PHANTOM_CineSaveCFString,               asynParamInt32,         &PHANTOM_CineSaveCF_);
  createParam(PHANTOM_DeleteString,                   asynParamInt32,         &PHANTOM_Delete_);
  createParam(PHANTOM_DeleteStartCineString,          asynParamInt32,         &PHANTOM_DeleteStartCine_);
  createParam(PHANTOM_DeleteEndCineString,            asynParamInt32,         &PHANTOM_DeleteEndCine_);
  createParam(PHANTOM_LivePreviewString,              asynParamInt32,         &PHANTOM_LivePreview_);
  createParam(PHANTOM_SetPartitionString,             asynParamInt32,         &PHANTOM_SetPartition_);
  createParam(PHANTOM_GetCineCountString,             asynParamInt32,         &PHANTOM_GetCineCount_);
  createParam(PHANTOM_CFStateString,                  asynParamInt32,         &PHANTOM_CFState_);
  createParam(PHANTOM_CFActionString,                 asynParamInt32,         &PHANTOM_CFAction_);
  createParam(PHANTOM_CFSizeString,                   asynParamInt32,         &PHANTOM_CFSize_);
  createParam(PHANTOM_CFUsedString,                   asynParamInt32,         &PHANTOM_CFUsed_);
  createParam(PHANTOM_CFProgressString,               asynParamInt32,         &PHANTOM_CFProgress_);
  createParam(PHANTOM_CFErrorString,                  asynParamInt32,         &PHANTOM_CFError_);
  createParam(PHANTOM_CFNumFilesString,               asynParamInt32,         &PHANTOM_CFNumFiles_);
  createParam(PHANTOM_CFFormatString,                 asynParamInt32,         &PHANTOM_CFFormat_);
  createParam(PHANTOM_CFMaxPagesString,               asynParamInt32,         &PHANTOM_CFMaxPages_);
  createParam(PHANTOM_CFCurPageString,                asynParamInt32,         &PHANTOM_CFCurPage_);
  createParam(PHANTOM_CFFileNameString,               asynParamOctet,         &PHANTOM_CFFileName_);
  createParam(PHANTOM_CFSIndexString,                 asynParamInt32,         &PHANTOM_CFSIndex_);
  createParam(PHANTOM_CFSWidthString,                 asynParamInt32,         &PHANTOM_CFSWidth_);
  createParam(PHANTOM_CFSHeightString,                asynParamInt32,         &PHANTOM_CFSHeight_);
  createParam(PHANTOM_CFSFrameCountString,            asynParamInt32,         &PHANTOM_CFSFrameCount_);
  createParam(PHANTOM_CFSFirstFrameString,            asynParamInt32,         &PHANTOM_CFSFirstFrame_);
  createParam(PHANTOM_CFSLastFrameString,             asynParamInt32,         &PHANTOM_CFSLastFrame_);
  createParam(PHANTOM_CFSRecordStartString,           asynParamInt32,         &PHANTOM_CFSRecordStart_);
  createParam(PHANTOM_CFSRecordEndString,             asynParamInt32,         &PHANTOM_CFSRecordEnd_);
  createParam(PHANTOM_CFSRecordString,                asynParamInt32,         &PHANTOM_CFSRecord_);
  createParam(PHANTOM_CFSRecordCountString,           asynParamInt32,         &PHANTOM_CFSRecordCount_);
  createParam(PHANTOM_CFSFileDeleteString,            asynParamInt32,         &PHANTOM_CFSFileDelete_);
  createParam(PHANTOM_InfoSensorTempString,           asynParamInt32,         &PHANTOM_InfoSensorTemp_);
  createParam(PHANTOM_InfoThermoPowerString,          asynParamInt32,         &PHANTOM_InfoThermoPower_);
  createParam(PHANTOM_InfoCameraTempString,           asynParamInt32,         &PHANTOM_InfoCameraTemp_);
  createParam(PHANTOM_InfoFanPowerString,             asynParamInt32,         &PHANTOM_InfoFanPower_);
  createParam(PHANTOM_EDRString,                      asynParamInt32,         &PHANTOM_EDR_);
  createParam(PHANTOM_CamExtSyncString,               asynParamInt32,         &PHANTOM_CamExtSync_);
  createParam(PHANTOM_CamFrameDelayString,            asynParamInt32,         &PHANTOM_CamFrameDelay_);
  createParam(PHANTOM_CamTriggerEdgeString,           asynParamInt32,         &PHANTOM_CamTriggerEdge_);
  createParam(PHANTOM_CamTriggerFilterString,         asynParamInt32,         &PHANTOM_CamTriggerFilter_);
  createParam(PHANTOM_CamReadySignalString,           asynParamInt32,         &PHANTOM_CamReadySignal_);
  createParam(PHANTOM_CamAux1PinString,               asynParamInt32,         &PHANTOM_CamAux1Pin_);
  createParam(PHANTOM_CamAux2PinString,               asynParamInt32,         &PHANTOM_CamAux2Pin_);
  createParam(PHANTOM_CamAux4PinString,               asynParamInt32,         &PHANTOM_CamAux4Pin_);
  createParam(PHANTOM_CamQuietFanString,              asynParamInt32,         &PHANTOM_CamQuietFan_);
  createParam(PHANTOM_SyncClockString,                asynParamInt32,         &PHANTOM_SyncClock_);
  createParam(PHANTOM_AutoTriggerXString,             asynParamInt32,         &PHANTOM_AutoTriggerX_);
  createParam(PHANTOM_AutoTriggerYString,             asynParamInt32,         &PHANTOM_AutoTriggerY_);
  createParam(PHANTOM_AutoTriggerWString,             asynParamInt32,         &PHANTOM_AutoTriggerW_);
  createParam(PHANTOM_AutoTriggerHString,             asynParamInt32,         &PHANTOM_AutoTriggerH_);
  createParam(PHANTOM_AutoTriggerThresholdString,     asynParamInt32,         &PHANTOM_AutoTriggerThreshold_);
  createParam(PHANTOM_AutoTriggerAreaString,          asynParamInt32,         &PHANTOM_AutoTriggerArea_);
  createParam(PHANTOM_AutoTriggerIntervalString,      asynParamInt32,         &PHANTOM_AutoTriggerInterval_);
  createParam(PHANTOM_AutoTriggerModeString,          asynParamInt32,         &PHANTOM_AutoTriggerMode_);
  createParam(PHANTOM_DataFormatString,               asynParamInt32,         &PHANTOM_DataFormat_);
  createParam(PHANTOM_FramesPerSecondString,          asynParamInt32,         &PHANTOM_FramesPerSecond_);
  for (index = 0; index < PHANTOM_NUMBER_OF_CINES; index++){
    createParam(PHANTOM_CnNameString[index],            asynParamOctet,         &PHANTOM_CnName_[index]);
    createParam(PHANTOM_CnWidthString[index],           asynParamInt32,         &PHANTOM_CnWidth_[index]);
    createParam(PHANTOM_CnHeightString[index],          asynParamInt32,         &PHANTOM_CnHeight_[index]);
    createParam(PHANTOM_CnStatusString[index],          asynParamInt32,         &PHANTOM_CnStatus_[index]);
    createParam(PHANTOM_CnFrameCountString[index],      asynParamInt32,         &PHANTOM_CnFrameCount_[index]);
    createParam(PHANTOM_CnFirstFrameString[index],      asynParamInt32,         &PHANTOM_CnFirstFrame_[index]);
    createParam(PHANTOM_CnLastFrameString[index],       asynParamInt32,         &PHANTOM_CnLastFrame_[index]);
    createParam(PHANTOM_CnRecordStartString[index],     asynParamInt32,         &PHANTOM_CnRecordStart_[index]);
    createParam(PHANTOM_CnRecordEndString[index],       asynParamInt32,         &PHANTOM_CnRecordEnd_[index]);
    createParam(PHANTOM_CnRecordString[index],          asynParamInt32,         &PHANTOM_CnRecord_[index]);
    createParam(PHANTOM_CnRecordCountString[index],     asynParamInt32,         &PHANTOM_CnRecordCount_[index]);
  }
  for (index = 0; index < PHANTOM_NUMBER_OF_FLASH_FILES; index++){
    createParam(PHANTOM_CfFileNameString[index],        asynParamOctet,         &PHANTOM_CfFileName_[index]);
    createParam(PHANTOM_CfFileSizeString[index],        asynParamOctet,         &PHANTOM_CfFileSize_[index]);
    createParam(PHANTOM_CfFileDateString[index],        asynParamOctet,         &PHANTOM_CfFileDate_[index]);
  }
 
  // Initialise PHANTOM parameters
  setIntegerParam(PHANTOMConnected_, 0);
  setIntegerParam(PHANTOM_TotalFrameCount_, 0);
  setIntegerParam(PHANTOM_FramesPerSecond_, 0);
  setIntegerParam(PHANTOM_AcquireState_, 0);
  setIntegerParam(NDDataType, (NDDataType_t) 3); // 3 is equal to NDUInt16 (see NDAttribute.h)
  setIntegerParam(ADSizeX, 1280);
  setIntegerParam(ADSizeY, 800);
  setIntegerParam(PHANTOM_LivePreview_, 0);
  setIntegerParam(PHANTOM_SelectedCine_, 1);
  setStringParam(ADManufacturer,  "Vision Research");
  setStringParam(PHANTOM_CFFileName_,  "");
  setIntegerParam(PHANTOM_SettingsSlot_,  1);
  setIntegerParam(PHANTOM_DataFormat_,  4); //P10 by default
  bitDepth_=10;
  conversionBitDepth_=10; // These are only updated prior to conversion start to ensure no race condition if bitDepth_ is changed before all conversion threads start
  conversionBytes_=1280*800*1.25;
  phantomToken_ = "P10";
  downloadingFlag_=0;

  // Initialise meta data to save
  metaArray_.push_back(new PhantomMeta("exposure", "Camera exposure time", "c%d.exp", NDAttrInt32, 0x674, 4));
  metaArray_.push_back(new PhantomMeta("rate", "Camera frame rate", "c%d.rate", NDAttrInt32, 0x354, 4));
  metaArray_.push_back(new PhantomMeta("trigger_secs", "Trigger time (seconds since 1970)", "c%d.trigtime.secs", NDAttrInt32, 0x028, 4));
  metaArray_.push_back(new PhantomMeta("trigger_usecs", "Trigger time fraction (micro seconds)", "c%d.trigtime.frac", NDAttrInt32, 0x024, 4));
  metaArray_.push_back(new PhantomMeta("label", "Label of data capture", "c%d.meta.name", NDAttrString, 0x1B9C, 256));
  metaArray_.push_back(new PhantomMeta("trigger_polarity", "1 = Rising Edge, 0 = Falling Edge", "c%d.cam.trigpol", NDAttrInt32, 0x1690, 4));
  metaArray_.push_back(new PhantomMeta("trigger_filter", "Trigger filter time (us)", "c%d.cam.trigfilt", NDAttrInt32, 0x1694, 4));
  metaArray_.push_back(new PhantomMeta("fsync", "Frame sync mode", "c%d.cam.syncimg", NDAttrInt32, 0x065, 1));
  metaArray_.push_back(new PhantomMeta("roi_width", "Region of interest pixel width", "c%d.meta.vw", NDAttrInt32, 0x030, 4));
  metaArray_.push_back(new PhantomMeta("roi_height", "Region of interest pixel height", "c%d.meta.vh", NDAttrInt32, 0x034, 4));
  metaArray_.push_back(new PhantomMeta("aux1_mode", "Auxiliary mode 1", "c%d.cam.aux1mode", NDAttrInt32, 0x0, 0));
  metaArray_.push_back(new PhantomMeta("aux2_mode", "Auxiliary mode 2", "c%d.cam.aux2mode", NDAttrInt32, 0x0, 0));
  metaArray_.push_back(new PhantomMeta("aux4_mode", "Auxiliary mode 4", "c%d.cam.aux4mode", NDAttrInt32, 0x0, 0));
  metaArray_.push_back(new PhantomMeta("first_frame", "First frame number", "c%d.firstfr", NDAttrInt32, 0x010, 4));
  metaArray_.push_back(new PhantomMeta("frame_count", "Total frame count", "c%d.frcount", NDAttrInt32, 0x014, 4));
//  metaArray_.push_back(new PhantomMeta("post_trig_frames", "Post trigger frame count", "c%d.lastfr", NDAttrInt32, 0x0, 0));

  if (status == asynSuccess){
    debug(functionName, "Starting up polling task....");
    // Create the thread that runs the acquisition
    status = (epicsThreadCreate("PhantomCameraTask",
                                epicsThreadPriorityMedium,
                                epicsThreadGetStackSize(epicsThreadStackMedium),
                                (EPICSTHREADFUNC)phantomCameraTaskC,
                                this) == NULL);
    if (status){
      debug(functionName, "epicsThreadCreate failure for image task");
    }
  }

  if (status == asynSuccess){
    debug(functionName, "Starting up preview task....");
    // Create the thread that runs the acquisition
    status = (epicsThreadCreate("PhantomPreviewTask",
                                epicsThreadPriorityMedium,
                                epicsThreadGetStackSize(epicsThreadStackMedium),
                                (EPICSTHREADFUNC)phantomPreviewTaskC,
                                this) == NULL);
    if (status){
      debug(functionName, "epicsThreadCreate failure for preview task");
    }
  }

  if (status == asynSuccess){
    debug(functionName, "Starting up status task....");
    // Create the thread that runs the live preview
    status = (epicsThreadCreate("PhantomStatusTask",
                                epicsThreadPriorityMedium,
                                epicsThreadGetStackSize(epicsThreadStackMedium),
                                (EPICSTHREADFUNC)phantomStatusTaskC,
                                this) == NULL);
    if (status){
      debug(functionName, "epicsThreadCreate failure for status task");
    }
  }

  if (status == asynSuccess){
    debug(functionName, "Starting up flash task....");
    // Create the thread that runs the flash format and delete
    status = (epicsThreadCreate("PhantomFlashTask",
                                epicsThreadPriorityMedium,
                                epicsThreadGetStackSize(epicsThreadStackMedium),
                                (EPICSTHREADFUNC)phantomFlashTaskC,
                                this) == NULL);
    if (status){
      debug(functionName, "epicsThreadCreate failure for flash task");
    }
  }

  if (status == asynSuccess){
    debug(functionName, "Starting up download task....");
    // Create the thread that runs downloads
    status = (epicsThreadCreate("PhantomDownloadTask",
                                epicsThreadPriorityMedium,
                                epicsThreadGetStackSize(epicsThreadStackMedium),
                                (EPICSTHREADFUNC)phantomDownloadTaskC,
                                this) == NULL);
    if (status){
      debug(functionName, "epicsThreadCreate failure for download task");
    }
  }

  if (status == asynSuccess){
    debug(functionName, "Starting up conversion task....");
    for (int i =0; i<PHANTOM_CONV_THREADS; i++){
      //All threads share the same input and output data pointers
      convStartEvt_[i] = epicsEventCreate(epicsEventEmpty);
      convFinishEvt_[i] = epicsEventCreate(epicsEventEmpty);
      char threadName[30];
      sprintf(threadName, "conversionThread%d", i); 
      status = (epicsThreadCreate(threadName, 
                        epicsThreadPriorityMedium, 
                        epicsThreadGetStackSize(epicsThreadStackMedium), 
                        (EPICSTHREADFUNC)phantomPixelConversionTaskC, 
                        this)==NULL);
    }
  }

  if (status == asynSuccess){
    // Attempt connection
    status |= makeConnection();

    if (status == asynSuccess){
      int sens_width = 0;
      int sens_height = 0;
      getCameraDataStruc("info", paramMap_);
      setStringParam(ADModel, (paramMap_["info.model"].getValue()).c_str());
      stringToInteger(paramMap_["info.xmax"].getValue(), sens_width);
      setIntegerParam(ADMaxSizeX, sens_width);
      stringToInteger(paramMap_["info.ymax"].getValue(), sens_height);
      setIntegerParam(ADMaxSizeY, sens_height);
    }
    if (status == asynSuccess){
      int exp_nano = 0;
      double fps = 0;
      getCameraDataStruc("defc", paramMap_);
      stringToInteger(paramMap_["defc.exp"].getValue(), exp_nano);
      setDoubleParam(ADAcquireTime, (double)exp_nano/1000000000.0);
      stringToDouble(paramMap_["defc.rate"].getValue(), fps);
      setDoubleParam(ADAcquirePeriod, (double)1.0/fps);
    }
  // Check if the status is bad.  If it is do our best to set the status record and message
  else if (status != asynSuccess){
    setIntegerParam(ADStatus, ADStatusError);
    setStringParam(ADStatusMessage, "Failed to initialise - check connection");
    }
  }
  callParamCallbacks();

}

asynStatus ADPhantom::makeConnection()
{
  int status = asynSuccess;
  static const char *functionName = "ADPhantom::makeConnection";
  
  status = connect();
  std::cout << "makeConnection(). connection status = " << status << '\n';
  if (status == asynSuccess){
    debug(functionName, "Connected OK");
    if (firstConnect_ == true){
      // Publish any updates necessary
      callParamCallbacks();

      if (status == asynSuccess){
        // First connection established, set flag
        firstConnect_ = false;
      }
    }
  }

  return (asynStatus)status;
}

asynStatus ADPhantom::connect()
{
  asynStatus status = asynSuccess;
  const char *functionName = "ADPhantom::connect";

  // Connect our Asyn user to the low level port that is a parameter to this constructor
  status = asynPortConnect(ctrlPort_, 0, &portUser_, "\n", "\n");
  if (status != asynSuccess){
    debug(functionName, "Failed to connect to low level asynOctetSyncIO port", ctrlPort_);
    // Failure, set the connected status to 0
    setIntegerParam(PHANTOMConnected_,     0);
    setIntegerParam(ADStatus, ADStatusError);
    callParamCallbacks();
  } else {
    // Success, set the connected status to 1
    setIntegerParam(PHANTOMConnected_,     1);
    setStringParam(ADStatusMessage, "Connected to PHANTOM");
    setIntegerParam(ADStatus, ADStatusIdle);
    callParamCallbacks();
  }

  if (status == asynSuccess){
    debug(functionName, "Connected to low level asynOctetSyncIO port", ctrlPort_);
    // Connect to the data asyn port
    status = pasynOctetSyncIO->connect(dataPort_, 0, &dataChannel_, NULL);
  }
  if (status != asynSuccess){
    debug(functionName, "Failed to connect to low level asynOctetSyncIO port", dataPort_);
    // Failure, set the connected status to 0
    setIntegerParam(PHANTOMConnected_,     0);
    setIntegerParam(ADStatus, ADStatusError);
    callParamCallbacks();
  } else {
    debug(functionName, "Connected to low level asynOctetSyncIO port", "dataPort");
    attachToPort("dataPort");
  }

  return status;
}

asynStatus ADPhantom::disconnect()
{
  asynStatus status = asynSuccess;
  const char *functionName = "ADPhantom::disconnect";
  int connected = 0;

  getIntegerParam(PHANTOMConnected_, &connected);
  // We can only force a disconnect if we are connected
  if (connected == 1){
    // disconnect from the ctrl port
    status = asynPortDisconnect(portUser_);
    if (status != asynSuccess){
      debug(functionName, "Failed to disconnect to low level asynOctetSyncIO port", ctrlPort_);
    }
  }
  return status;
}

/** 
 *  This function runs the acquisition thread.
 *  It is started in the class constructor and must not return until the IOC stops.
 *
*/ 
void ADPhantom::phantomCameraTask()
{
  static const char *functionName = "ADPhantom::phantomCameraTask";
  int status = asynSuccess;
  int acquire = 0;
  int imageMode = 0;
  int numImages = 0;
  int imageCounter = 0;
  int numImagesCounter = 0;
  int cine = 0;
  int firstFrame = 0;
  int lastFrame = 0;
  int frameCount = 0;
  int cineState = 0;
  int autoAdvance = 0;
  char command[PHANTOM_MAX_STRING];
  std::string response;
  std::string cineStr;

  this->lock();

  while (1){
    getIntegerParam(ADAcquire, &acquire);
    // If we are not acquiring or encountered a problem then wait for a semaphore that is given when acquisition is started
    if (!acquire){
      // Only set the status message if we didn't encounter a problem last time, so we don't overwrite the error mesage
      if(!status){
        debug(functionName, "Waiting for the acquire command");
        setStringParam(ADStatusMessage, "Ready");
        setIntegerParam(ADStatus, ADStatusIdle);
      }
      // Reset the counters
      setIntegerParam(ADNumExposuresCounter, 0);
      setIntegerParam(ADNumImagesCounter, 0);
      callParamCallbacks();

      // Release the lock while we wait for an event that says acquire has started, then lock again
      this->unlock();
      debug(functionName, "Waiting for acquire to start");
      status = epicsEventWait(this->startEventId_);
      this->lock();
      getIntegerParam(ADAcquire, &acquire);

      // Read in the selected cine
      getIntegerParam(PHANTOM_SelectedCine_, &cine);
      // Acquisition has been requested, read in the cine
      // Issue the start recording for the cine
      sprintf(command, "rec %d", cine);
      status = sendSimpleCommand(command, &response);

      // Create the cine string
      sprintf(command, "c%d", cine);
      cineStr.assign(command);
    }

    setStringParam(ADStatusMessage, "Recording frames into Cine");
    setIntegerParam(ADStatus, ADStatusAcquire);

    // Read out the cine status and counter
    getCameraDataStruc(cineStr, paramMap_);
    // Decode the status
    debug(functionName, paramMap_[cineStr + ".state"].getValue().c_str());
    cineStates(paramMap_[cineStr + ".state"].getValue(), cineState);
    setIntegerParam(PHANTOM_AcquireState_, cineState);
    if (checkState(paramMap_[cineStr + ".state"].getValue(), "WTR")){
      debug(functionName, "WTR state");
    }
    if (checkState(paramMap_[cineStr + ".state"].getValue(), "STR")){
      debug(functionName, "STR state");
    }
    if (checkState(paramMap_[cineStr + ".state"].getValue(), "TRG")){
      debug(functionName, "TRG state");
    }
    if (checkState(paramMap_[cineStr + ".state"].getValue(), "ACT")){
      debug(functionName, "ACT state");
    }
    // Decode the frame count
    status = stringToInteger(paramMap_[cineStr + ".frcount"].getValue(), frameCount);
    status = stringToInteger(paramMap_[cineStr + ".firstfr"].getValue(), firstFrame);
    status = stringToInteger(paramMap_[cineStr + ".lastfr"].getValue(), lastFrame);
    if (lastFrame > 0){
      imageCounter = lastFrame+1;
    } else {
      imageCounter = 0;
    }
    numImagesCounter = imageCounter;

    // Set a bit of areadetector image/frame statistics...
    getIntegerParam(ADNumImages, &numImages);
    getIntegerParam(ADImageMode, &imageMode);
    setIntegerParam(NDArrayCounter, imageCounter);
    setIntegerParam(ADNumImagesCounter, numImagesCounter);
    setIntegerParam(PHANTOM_TotalFrameCount_, frameCount);

    getIntegerParam(PHANTOM_AutoAdvance_, &autoAdvance);

    debug(functionName, "cineState", cineState);
    debug(functionName, "cineState", (cineState & PHANTOM_CINE_STATE_STR));
    if ((cineState & PHANTOM_CINE_STATE_STR) == PHANTOM_CINE_STATE_STR){
      bool found = false;
      // Check if set to auto advance to next available cine
      if (autoAdvance){
        // Find the next active cine
        for (int index = 1; index < PHANTOM_NUMBER_OF_CINES; index++){
          updateCine(index);
          getIntegerParam(PHANTOM_CnStatus_[index], &cineState);
          if ((cineState & PHANTOM_CINE_STATE_ACT) == PHANTOM_CINE_STATE_ACT){
            cine = index;
            // Write out the selected cine
            setIntegerParam(PHANTOM_SelectedCine_, cine);
            // Update the cine string
            sprintf(command, "c%d", cine);
            cineStr.assign(command);
            found = true;
            break;
          }
        }
      }
      else{ //Auto advance not set so stop acquisition
        std::string response = "";
        sendSimpleCommand(PHANTOM_CMD_ABORT, &response);
      }
      if (!found){
        acquire = 0;
        setIntegerParam(ADAcquire, 0);
      }
    }

    if (status != asynError){
      // Check to see if acquisition is complete
      if ((imageMode == ADImageSingle) || ((imageMode == ADImageMultiple) && (numImagesCounter >= numImages))){
        setIntegerParam(ADAcquire, 0);
        debug(functionName, "Acquisition completed");
      }
      // Call the callbacks to update any changes
      callParamCallbacks();
      getIntegerParam(ADAcquire, &acquire);

      // If we are acquiring then check for a stop event
      if (acquire){
        callParamCallbacks();
        this->unlock();
        status = epicsEventWaitWithTimeout(this->stopEventId_, 0.1);
        this->lock();
      }
    }
  }
}

/** 
 *  This function runs the download thread.
 *  It is started in the class constructor and must not return until the IOC stops.
 *
*/ 
void ADPhantom::phantomDownloadTask()
{
  static const char *functionName = "ADPhantom::phantomDownloadTask";
  int status = asynSuccess;
  int preview = 0;
  int start_frame = 0;
  int end_frame = 0;
  int start_cine = 0;
  int end_cine = 0;
  int num_cines = 0;
  int uni_frame_lim = false; //Whether frame limits are applied to all cines
  bool rangeValid = true;
  std::string response;
  this->lock();

  while (1){
    debug(functionName, "Waiting for the download command");
    this->unlock();
    status = epicsEventWait(this->startDownloadEventId_);
    debug(functionName, "Download command recieved");
    this->lock();
    getIntegerParam(PHANTOM_LivePreview_, &preview);
    if (preview){
      setStringParam(ADStatusMessage, "Cannot download while live previewing");
      setIntegerParam(ADStatus, ADStatusError);
    }
    else{
      if (status != asynError){
        setStringParam(ADStatusMessage, "Downloading");
        rangeValid = true;

        //Read in total number of cines available
        getIntegerParam(PHANTOM_GetCineCount_, &num_cines);

        // Read in the number of frames to download
        getIntegerParam(PHANTOM_DownloadStartFrame_, &start_frame);
        getIntegerParam(PHANTOM_DownloadEndFrame_, &end_frame);
        getIntegerParam(PHANTOM_DownloadStartCine_, &start_cine);
        getIntegerParam(PHANTOM_DownloadEndCine_, &end_cine);
        getIntegerParam(PHANTOM_DownloadFrameMode_, &uni_frame_lim);

        debug(functionName, "Download start cine", start_cine);
        debug(functionName, "Download end cine", end_cine);
        debug(functionName, "Download start frame", start_frame);
        debug(functionName, "Download end frame", end_frame);

        if (start_cine < 1 || start_cine > num_cines){
          rangeValid=false;
          setStringParam(ADStatusMessage, "start_cine value invalid");
          debug(functionName, "start_cine value invalid");
        }  else if (end_cine < 1 || end_cine > num_cines){
          rangeValid=false;
          setStringParam(ADStatusMessage, "end_cine value invalid");
          debug(functionName, "end_cine value invalid");
        } else if(uni_frame_lim){ //Start frame and end frame are applied to every cine
          int first_frame = 0;
          int last_frame = 0;
          for(int cine{start_cine}; cine <= end_cine; cine++){
            debug(functionName, "Starting sanity checks on cine ", cine);
            getIntegerParam(PHANTOM_CnFirstFrame_[cine], &first_frame);
            getIntegerParam(PHANTOM_CnLastFrame_[cine], &last_frame);
            if(start_frame < first_frame || start_frame > last_frame){
              rangeValid=false;
              char message[256];
              sprintf(message, "start_frame value invalid in cine %d", cine);
              setStringParam(ADStatusMessage, message);
              debug(functionName, message);
              break;
            } else if(end_frame < first_frame || end_frame > last_frame ){
              rangeValid=false;
              char message[256];
              sprintf(message, "end_frame value invalid in cine %d", cine);
              setStringParam(ADStatusMessage, message);
              debug(functionName, message);
              break;
            } else if(end_frame < start_frame){
              rangeValid=false;
              setStringParam(ADStatusMessage, "start_frame cannot be after end_frame");
              debug(functionName, "start_frame cannot be after end_frame");
              break;
            }
          }
        } else{ //Start frame refers to start cine and end frame refers to end cine
          int start_cine_first_frame = 0;
          int start_cine_last_frame = 0;
          int end_cine_first_frame = 0;
          int end_cine_last_frame = 0;
          getIntegerParam(PHANTOM_CnFirstFrame_[start_cine], &start_cine_first_frame);
          getIntegerParam(PHANTOM_CnLastFrame_[start_cine], &start_cine_last_frame);
          getIntegerParam(PHANTOM_CnFirstFrame_[end_cine], &end_cine_first_frame);
          getIntegerParam(PHANTOM_CnLastFrame_[end_cine], &end_cine_last_frame);

          if (start_frame < start_cine_first_frame || start_frame > start_cine_last_frame) {
            rangeValid=false;
            setStringParam(ADStatusMessage, "start_frame value invalid");
          } else if(end_frame < end_cine_first_frame || end_frame > end_cine_last_frame) {
            rangeValid=false;
            setStringParam(ADStatusMessage, "end_frame value invalid");
          } else if (start_cine == end_cine && end_frame < start_frame) {
            rangeValid=false;
            setStringParam(ADStatusMessage, "end_frame cannot be less than start_frame within a cine");
          }
          downloadingFlag_ = 0;
        }

        if(rangeValid){
          // Attach to the correct port
          //status = attachToPort("dataPort");

          // Download the timestamp information
          status = readoutTimestamps(start_cine, end_cine, start_frame, end_frame, uni_frame_lim);

          // Download the data and process arrays
          status = readoutDataStream(start_cine, end_cine, start_frame, end_frame, uni_frame_lim);

          setStringParam(ADStatusMessage, "Ready");
        }
        else{
          setIntegerParam(ADStatus, ADStatusError);
        }
      }
      else {
        setStringParam(ADStatusMessage, "Error in preview task");
        setIntegerParam(ADStatus, status);
      }
    }  
   setIntegerParam(PHANTOM_Download_, 1);
  }
}


void ADPhantom::phantomStatusTask()
{
  static const char *functionName = "ADPhantom::phantomStatusTask";
  int status = asynSuccess;
  int cine = 0;
  char command[PHANTOM_MAX_STRING];
  std::string cineStr;
  int frameCount = 0;
  int acquire = 0;

  debug(functionName, "Starting thread...");
  while (1){
    epicsThreadSleep(0.25);
    if (!downloadingFlag_){
      // This thread has a large effect on performance so pause while downloading
      this->lock();

      // Read out the preview cine status
      status = getCameraDataStruc("cam", paramMap_);

      if (status == asynSuccess){
        int cines = 0;
        std::string scines = paramMap_["cam.cines"].getValue();
        cleanString(scines, " ");
        status = stringToInteger(scines, cines);
        setIntegerParam(PHANTOM_GetCineCount_, cines);
      }

      updateInfoStatus();
      updateCameraStatus();
      updateDefcStatus();
      updateMetaStatus();
      updateFlash();
      updateAutoStatus();

      for( int index{1}; index < PHANTOM_NUMBER_OF_CINES; index++){
        updateCine(index);
      }

      getIntegerParam(ADAcquire, &acquire);
      // If we are not acquiring update the cine frame count
      if (!acquire){
        // Read in the selected cine
        getIntegerParam(PHANTOM_SelectedCine_, &cine);
        // Create the cine string
        sprintf(command, "c%d", cine);
        cineStr.assign(command);
        // Read out the cine status and counter
        getCameraDataStruc(cineStr, paramMap_);
        status = stringToInteger(paramMap_[cineStr + ".frcount"].getValue(), frameCount);
        // Set a bit of areadetector image/frame statistics...
        setIntegerParam(PHANTOM_TotalFrameCount_, frameCount);
        callParamCallbacks();
      }

      this->unlock();
  }
  }
}

void ADPhantom::phantomPreviewTask()
{
  static const char *functionName = "ADPhantom::phantomPreviewTask";
  int status = asynSuccess;
  int preview = 0;
  std::string response;

  this->lock();
  while (1){
    getIntegerParam(PHANTOM_LivePreview_, &preview);
    // If we are not acquiring or encountered a problem then wait for a semaphore that is given when acquisition is started
    if (!preview){
      // Release the lock while we wait for an event that says acquire has started, then lock again
      this->unlock();
      debug(functionName, "Waiting for preview to start");
      status = epicsEventWait(this->startPreviewEventId_);
      this->lock();
      getIntegerParam(PHANTOM_LivePreview_, &preview);
      status = updatePreviewCine();
      //status = attachToPort("dataPort");

      // Issue the start recording for the cine
      status = sendSimpleCommand("rec 0", &response);
    }

    // Now perform a readout of the preview cine
    readoutPreviewData();

    if (preview){
      this->unlock();
      status = epicsEventWaitWithTimeout(this->stopPreviewEventId_, 0.5);
      this->lock();
    }

    if (status){
      setStringParam(ADStatusMessage, "Error in preview task");
      setIntegerParam(ADStatus, status);
    }
  }
}

void ADPhantom::phantomFlashTask()
{
  static const char *functionName = "ADPhantom::phantomFlashTask";
  int status = asynSuccess;
  int formatFlash = 0;
  int deleteFile = 0;

  this->lock();
  while (1){
    // Release the lock while we wait for an event that says acquire has started, then lock again
    this->unlock();
    debug(functionName, "Waiting for flash event");
    status = epicsEventWait(this->flashEventId_);
    this->lock();
    getIntegerParam(PHANTOM_CFFormat_, &formatFlash);
    if (formatFlash){
      this->unlock();
      this->formatFlash();
      this->lock();
      setIntegerParam(PHANTOM_CFFormat_, 0);
    }
    getIntegerParam(PHANTOM_CFSFileDelete_, &deleteFile);
    if (deleteFile){
      this->unlock();
      this->deleteFlashFile();
      this->lock();
      setIntegerParam(PHANTOM_CFSFileDelete_, 0);
    }
    callParamCallbacks();

    if (status){
      setStringParam(ADStatusMessage, "Error in flash task");
      setIntegerParam(ADStatus, status);
    }
  }
}

void ADPhantom::phantomConversionTask()
{
  // This task is run by 10 threads in parallel to convert data faster for P10 and P12L types
  static const char *functionName = "ADPhantom::phantomConversionTask";
  unsigned char *input = (unsigned char *)data_;
  unsigned char *output = (unsigned char *)flashData_;
  std::string threadName = epicsThreadGetNameSelf();
  std::string newString = threadName.substr(threadName.find("conversionThread") + 16);
  int i = std::stoi(newString);

  while (1){
    // Wait for the main download thread to send the signal to each thread to start
    epicsEventWait(this->convStartEvt_[i]);

    // Convert a slice of the new data
    int myBytes=conversionBytes_/PHANTOM_CONV_THREADS;
    int startByte = i*myBytes;
    if (bitDepth_==8){
      debug(functionName, "Starting 8 bit conversion");
      this->convert8BitPacketTo16Bit(input+startByte, output+startByte*2, myBytes);
    }
    else if (conversionBitDepth_ == 10){
      debug(functionName, "Starting 10 bit conversion");
      for (int bIndex = startByte/5; bIndex < (startByte+myBytes)/5; bIndex++){
        this->convert10BitPacketTo16Bit(input+(bIndex*5), output+(bIndex*8));
      }
    }
    else if (conversionBitDepth_ == 12){
      debug(functionName, "Starting 12 bit conversion");
      for (int bIndex = startByte/3; bIndex < (startByte+myBytes)/3; bIndex++){
        this->convert12BitPacketTo16Bit(input+(bIndex*3), output+(bIndex*4));
      }
    }

    // Send the signal to the main thread that processing is done
    epicsEventSignal(convFinishEvt_[i]);
  }
}

/**
 * Connect to the underlying low level Asyn port that is used for comms.
 * This uses the asynOctetSyncIO interface, and also sets the input and output terminators.
 *
 * @param port - Name of the port to connect to.
 * @param addr - Address to connect to.
 * @param ppasynUser - Pointer to the asyn user structure.
 * @param inputEos - String input EOS.
 * @param outputEos - String output EOS.
 */
asynStatus ADPhantom::asynPortConnect(const char *port, int addr, asynUser **ppasynUser, const char *inputEos, const char *outputEos)
{
  static const char *functionName = "ADPhantom::asynPortConnect";

  asynStatus status = asynSuccess;
  std::map<std::string, std::string> data;

  status = pasynOctetSyncIO->connect( port, addr, ppasynUser, NULL);
  if (status) {
    debug(functionName, "Unable to connect to port", port);
    return status;
  }

  status = pasynOctetSyncIO->setInputEos(*ppasynUser, inputEos, strlen(inputEos));
  status = pasynOctetSyncIO->setOutputEos(*ppasynUser, outputEos, strlen(outputEos));

  debug(functionName, "Connected to low level asynOctetSyncIO port", port);

  // Record that we are connected
  setIntegerParam(PHANTOMConnected_, 1);

/*  // Issue the connect command to complete connection
  status = sendSimpleCommand(PHANTOM_CMD_CONNECT, &data);
  if (status != asynSuccess){
    debug(functionName, "Failed to send command", PHANTOM_CMD_CONNECT);
  } else {
    debug(functionName, "Connect command return data", data);
  } */
  if (status == asynSuccess){
    // The connection was successful, record the server name and the protocol version
/*    std::string server = data["ServerName"];
    cleanString(server, "\"");
    setStringParam(PHANTOMServerName_, server.c_str());
    std::string protocol = data["ProtocolVersion"];
    cleanString(protocol, "\"");
    setStringParam(PHANTOMProtocolVersion_, protocol.c_str()); */
  }
  return status;
}

asynStatus ADPhantom::asynPortDisconnect(asynUser *pasynUser)
{
  static const char *functionName = "ADPhantom::asynPortDisconnect";

  asynStatus status = asynSuccess;
  std::map<std::string, std::string> data;

  status = pasynOctetSyncIO->disconnect(pasynUser);
  if (status) {
    debug(functionName, "Unable to disconnect from asyn port");
  }
  // Record that we are not connected
  setIntegerParam(PHANTOMConnected_, 0);

  return status;
}

/**
 * Method for sending a simple command (no parameters) that will check the
 * error status and do the right thing.
 */
asynStatus ADPhantom::sendSimpleCommand(const std::string& command, std::string *response, double timeout)
{
  static const char *functionName = "ADPhantom::sendSimpleCommand";
  asynStatus status = asynSuccess;

  debug(functionName, "Sending command", command);
// Send the command and get the reply
  status = commandResponse(command, response, timeout);
  
// Check the command sent OK
  if (status == asynSuccess){
    // Check the response for any errors
    std::string errstr(PHANTOM_ERROR_STRING);
    if (*response == errstr){
      debug(functionName, "Unable to successfully issue the command", command);
//      debug(functionName, "Returned error", response);
      setStringParam(ADStatusMessage, response->c_str());
      status = asynError;
    }
  }
  return status;
}

/**
 * This sends a command to the device and parses the response.  Data is returned
 * in a std::map that is indexed by the parameter name
 * @param command - String command to send.
 * @param response - String response back (OK or ERROR)
 * @param data - Map of data items indexed by name
 */
asynStatus ADPhantom::commandResponse(const std::string &command, std::string *response, double timeout)
{
  static const char *functionName = "ADPhantom::commandResponse";

  asynStatus status = asynSuccess;
  std::string errorCode = "";
  std::string replyString = "";
  std::string nameValueString = "";
  char replyArray[PHANTOM_MAX_STRING];

  debug(functionName, "Command to send", command);
  status = asynWriteRead(command.c_str(), replyArray, timeout);
  // Only continue if the status is good...
  if (status == asynSuccess){
    // OK we need to first find out if the command was accepted or not
    response->assign(replyArray);
    //std::cout << "commandResponse: reply received = " << *response << '\n';
  } 
  else {
    //std::cout << "Error in commandResponse: status = " << status << '\n';
  }
  return status;
}

/**
 * Wrapper for asynOctetSyncIO write/read functions.
 * @param command - String command to send.
 * @param response - String response back.
 */
asynStatus ADPhantom::asynWriteRead(const char *command, char *response, double timeout)
{
  static const char *functionName = "ADPhantom::asynWriteRead";

  asynStatus status = asynSuccess;
  int eomReason;
  size_t nwrite = 0;
  size_t nread = 0;
  size_t tot = 0;
  int connected = 0;
  char sendString[256];
  char *inpPtr;

  callParamCallbacks();

  debug(functionName, "Writing command", command);

  //Setup the string to send to the detector
  sprintf(sendString, "%s\n", command);

  // If there is no asyn port user then something higher up has failed
  // Make sure to set connected to 0 and then bail out of this call with an error
  if (!portUser_) {
    setIntegerParam(PHANTOMConnected_, 0);
    return asynError;
  }

  // Get the connected status
  getIntegerParam(PHANTOMConnected_, &connected);

  // Only proceed if we are connected
  if (connected == 1){
	  inpPtr = response;
	  status = pasynOctetSyncIO->writeRead(portUser_ ,
	  																		 sendString, strlen(sendString),
                                         inpPtr, PHANTOM_MAX_STRING,
                                         timeout,
                                         &nwrite, &nread, &eomReason );
	  tot += nread;
	  inpPtr += nread;
	  while (response[tot-2] == '\\'){
	  	*inpPtr = '\n';
	  	inpPtr++;
	  	tot++;
	  	status = pasynOctetSyncIO->read(portUser_ ,
	  																	inpPtr,
																			PHANTOM_MAX_STRING,
																			PHANTOM_TIMEOUT,
																			&nread,
																			&eomReason);
	  	tot += nread;
	  	inpPtr += nread;
	  }
  }

  if (status){
    setStringParam(ADStatusMessage, "Error in asynWriteRead");
    setIntegerParam(ADStatus, status);
  }

  // Return error if no data read
  return (tot == 0 ? asynError : asynSuccess);
}

int ADPhantom::getLocalPortNum(const std::string& driverPort)
{
// Get local TCP socket port number for asyn port
  FILE *fp;
  struct sockaddr_in sa;
  int status = 0;
  
  //tfname = tmpnam(NULL);
  fp = fopen("/tmp/phantom_port","w");
  pasynManager->report(fp, 2, driverPort.c_str());  // Output asynManager Report (level 2) into temp file
  fclose(fp);
  std::string stfname("/tmp/phantom_port");
  std::string asynRepString = fileToString(stfname);  // Read file into a string
  std::size_t fd_pos = asynRepString.find("fd:");  // Locate line starting "fd:"
  if (fd_pos!=std::string::npos)
  {
    std::string sfd = asynRepString.substr(fd_pos+3, -1);
    std::size_t cr_pos = sfd.find("\n");           // Find string following "fd:" to end of line
    sfd = asynRepString.substr(fd_pos+3, cr_pos);  // This is the socket fd as a string
    int sa_len = sizeof(sa);
// Call getsockname() to get the local TCP socket number in the structure sa
    status = getsockname(atoi(sfd.c_str()), (struct sockaddr *)&sa, (socklen_t *)&sa_len);
  }
  remove("/tmp/phantom_port");
  return (status == 0 ? (int)ntohs(sa.sin_port) : 0);
}

asynStatus ADPhantom::cleanString(std::string &str, const std::string &search, int where)
{
  if (str != ""){
    if (where == 0 || where == 1){
      // Clean up from the front of the string
      while (str.substr(0, 1).find_first_of(search) != std::string::npos){
        str = str.substr(1);
      }
    }
    if (where == 0 || where == 2){
      // Clean up from the back of the string
      while (str.substr(str.length()-1, 1).find_first_of(search) != std::string::npos){
        str = str.substr(0, str.length()-1);
      }
    }
  }
  return asynSuccess;
}

/** Called when asyn clients call pasynEnum->read().
  * The base class implementation simply prints an error message.
  * @param pasynUser - pasynUser structure that encodes the reason and address.
  * @param strings - Array of string pointers.
  * @param values - Array of values
  * @param severities - Array of severities
  * @param nElements - Size of value array
  * @param nIn - Number of elements actually returned 

asynStatus ADPhantom::readEnum(asynUser *pasynUser, char *strings[], int values[], int severities[], size_t nElements, size_t *nIn)
{

  const char *functionName = "ADPhantom::readEnum";
  asynStatus status = asynSuccess;
  int function = pasynUser->reason;
  size_t index;

  if (function == PHANTOMLensMode_){
    for (index = 0; ((index < (size_t)lensModes_.size()) && (index < nElements)); index++){
      if (strings[index]){
        free(strings[index]);
      }
      strings[index] = epicsStrDup(lensModes_[index].c_str());
      debug(functionName, "Reading lens mode", strings[index]);
      values[index] = index;
      severities[index] = 0;
    }
    *nIn = index;
  } else if (function == PHANTOMScanRange_){
    for (index = 0; ((index < (size_t)scanRanges_.size()) && (index < nElements)); index++){
      if (strings[index]){
        free(strings[index]);
      }
      strings[index] = epicsStrDup(scanRanges_[index].c_str());
      debug(functionName, "Reading scan range", strings[index]);
      values[index] = index;
      severities[index] = 0;
    }
    *nIn = index;
  } else if (function == PHANTOMRunMode_){
    for (index = 0; ((index < (size_t)runModes_.size()) && (index < nElements)); index++){
      if (strings[index]){
        free(strings[index]);
      }
      strings[index] = epicsStrDup(runModes_[index].c_str());
      debug(functionName, "Reading run mode", strings[index]);
      values[index] = index;
      severities[index] = 0;
    }
    *nIn = index;
  } else {
    *nIn = 0;
    status = asynError;
  }

  *nIn = 0;
  return status;
}
*/

/**
 * Called when asyn clients call pasynInt32->write().
 * Write integer value to the drivers parameter table.
 *
 * @param pasynUser - Pointer to the asyn user structure.
 * @param value - The new value to write
 */
asynStatus ADPhantom::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
  const char *functionName = "ADPhantom::writeInt32";
  int status = asynSuccess;
  int function = pasynUser->reason;
  int oldValue;

  // parameters for functions
  int adstatus;

  getIntegerParam(function, &oldValue);
  setIntegerParam(function, value);
  getIntegerParam(ADStatus, &adstatus);
  setStringParam(ADStatusMessage, "Ready");
  setIntegerParam(ADStatus, ADStatusIdle);

  if (function == ADAcquire){
    if (value && (adstatus == ADStatusIdle || adstatus == ADStatusError || adstatus == ADStatusAborted)){
      // Send an event to wake up the electronAnalyser task.
      epicsEventSignal(this->startEventId_);
    }
    if (!value && (adstatus != ADStatusIdle)){
      // Sent the message to the phantom to stop
      std::string response = "";
      sendSimpleCommand(PHANTOM_CMD_ABORT, &response);
      // Stop acquiring ( abort any hardware processing )
      epicsEventSignal(this->stopEventId_);
    }
  } else if (function == PHANTOM_LivePreview_){
    if (value){
      // Send an event to wake up the live preview
      int downloadCount = 0;
      getIntegerParam(PHANTOM_DownloadCount_, &downloadCount);
      if(downloadCount){
      setStringParam(ADStatusMessage, "Cannot live preview while downloading");  
      setIntegerParam(ADStatus, ADStatusError);
      status |= asynError;
      } else{
        epicsEventSignal(this->startPreviewEventId_);
      }
    }
    if (!value){
      // Stop live preview
      epicsEventSignal(this->stopPreviewEventId_);
    }
  } else if (function == PHANTOM_SoftwareTrigger_){
    sendSoftwareTrigger();
  } else if (function == PHANTOM_Download_){
    setIntegerParam(PHANTOM_DownloadAbort_, 0); 
    int preview = 0;
    getIntegerParam(PHANTOM_LivePreview_, &preview);
    if (preview){
      setStringParam(ADStatusMessage, "Cannot download while live previewing");
      setIntegerParam(ADStatus, ADStatusError);
      status |= asynError;
    } else{
      if(value > 0 && value < PHANTOM_NUMBER_OF_CINES){ 
        //Allows the easy download of single cine
        setIntegerParam(PHANTOM_DownloadStartCine_, value);
        setIntegerParam(PHANTOM_DownloadEndCine_, value);
      }
      //Send an event to start the download
      epicsEventSignal(this->startDownloadEventId_);
      downloadingFlag_ = 1;
    }
  } else if(function == PHANTOM_Delete_){
    if(value > 0 && value < PHANTOM_NUMBER_OF_CINES){ 
        //Allows the easy deletion of single cine
        setIntegerParam(PHANTOM_DeleteStartCine_, value);
        setIntegerParam(PHANTOM_DeleteEndCine_, value);
    }
    status = deleteCineFiles();
  } else if (function == PHANTOM_CineSaveCF_){
    status |= saveCineToFlash(value);
  } else if (function == PHANTOM_SettingsSave_){
    status |= saveSettings();
  } else if (function == PHANTOM_SettingsLoad_){
    status |= loadSettings();
  } else if (function == PHANTOM_SelectedCine_){
    // Set up the selected cine
    status |= selectCine(value);
  } else if (function == PHANTOM_SetPartition_){
    // Set up the partition
    status |= setPartition(value);
  } else if (function == PHANTOM_CFCurPage_){
    // Set the current page
    status |= updateFlashFileTable();
  } else if (function == PHANTOM_CFSIndex_){
    // Select the flash file from the table by index
    status |= selectFlashByIndex(value);
  } else if (function == PHANTOM_CFSRecord_){
    // Download the flash file
    int preview = 0;
    getIntegerParam(PHANTOM_LivePreview_, &preview);
    if (preview){
      setStringParam(ADStatusMessage, "Cannot download while live previewing");
      setIntegerParam(ADStatus, ADStatusError);
      status |= asynError;
    } else {
      int downloadCount = 0;
      getIntegerParam(PHANTOM_DownloadCount_, &downloadCount);
      if(downloadCount){
        setStringParam(ADStatusMessage, "Cannot download to flash while downloading to file");  
        setIntegerParam(ADStatus, ADStatusError);
        status |= asynError;
      } else{
        status |= downloadFlashFile();
      }
    }
  } else if (function == PHANTOM_CFSFileDelete_){
    // Delete the flash file
    epicsEventSignal(this->flashEventId_);
  } else if (function == PHANTOM_CFFormat_){
    // Format the flash disk
    epicsEventSignal(this->flashEventId_);
  } else if (function == PHANTOM_CamExtSync_){
    status |= setCameraParameter("cam.syncimg", value);
  } else if (function == PHANTOM_CamFrameDelay_){
    status |= setCameraParameter("cam.frdelay", value);
  } else if (function == PHANTOM_CamTriggerEdge_){
    status |= setCameraParameter("cam.trigpol", value);
  } else if (function == PHANTOM_CamTriggerFilter_){
    status |= setCameraParameter("cam.trigfilt", value);
  } else if (function == PHANTOM_CamReadySignal_){
    status |= setCameraParameter("cam.longready", value);
  } else if (function == PHANTOM_CamAux1Pin_){
    status |= setCameraParameter("cam.aux1mode", value);
  } else if (function == PHANTOM_CamAux2Pin_){
    status |= setCameraParameter("cam.aux2mode", value);
  } else if (function == PHANTOM_CamAux4Pin_){
    status |= setCameraParameter("cam.aux4mode", value);
  } else if (function == PHANTOM_CamQuietFan_){
    status |= setCameraParameter("cam.quiet", value);
  } else if (function == ADSizeX || function == ADSizeY){
    status |= setCameraResolution();
  } else if (function == PHANTOM_PostTrigCount_){
    int maxFrameCount = 1;
    getIntegerParam(PHANTOM_MaxFrameCount_, &maxFrameCount);
    if (value > maxFrameCount){
      value = maxFrameCount;
    }
    status |= setCameraParameter("defc.ptframes", value);
  } else if (function == PHANTOM_PerformCSR_){
    status |= performCSR();
  } else if (function == PHANTOM_AutoSave_){
    status |= setCameraParameter("auto.filesave", value);
  } else if (function == PHANTOM_AutoRestart_){
    status |= setCameraParameter("auto.acqrestart", value);
  } else if (function == PHANTOM_EDR_){
    status |= setCameraParameter("defc.edrexp", value);
  } else if (function == PHANTOM_SyncClock_){
    char command[PHANTOM_MAX_STRING];
    std::string response;
    sprintf(command, "setrtc %ld", std::time(NULL));
    sendSimpleCommand(command, &response);
  } else if (function == PHANTOM_AutoTriggerX_){
    //Apply corrections such that PV has coord origin in top left corner
    int res = 0;
    getIntegerParam(ADSizeX, &res);
    value -= res/2 ;
    status |= setCameraParameter("auto.trigger.x", value);
  } else if (function == PHANTOM_AutoTriggerY_){
    //Apply corrections such that PV has coord origin in top left corner
    int res = 0;
    getIntegerParam(ADSizeY, &res);
    value -= res/2 ;
    status |= setCameraParameter("auto.trigger.y", value);
  } else if (function == PHANTOM_AutoTriggerW_){
    status |= setCameraParameter("auto.trigger.w", value);
  } else if (function == PHANTOM_AutoTriggerH_){
    status |= setCameraParameter("auto.trigger.h", value);
  } else if (function == PHANTOM_AutoTriggerThreshold_){
    status |= setCameraParameter("auto.trigger.threshold", value);
  } else if (function == PHANTOM_AutoTriggerArea_){
    status |= setCameraParameter("auto.trigger.area", value);
  } else if (function == PHANTOM_AutoTriggerInterval_){
    status |= setCameraParameter("auto.trigger.speed", value);
  } else if (function == PHANTOM_AutoTriggerMode_){
    if(value !=0){
      //Disable auto black ref prior to enabling auto trigger
      status |= setCameraParameter("auto.bref", 0);
      if(status == asynSuccess) setIntegerParam(PHANTOM_AutoCSR_, 0);
    }
    if(status == asynSuccess){
      status |= setCameraParameter("auto.trigger.mode", value);
    }
  } else if (function == PHANTOM_AutoCSR_){
    if( value!=0){
      //Disable auto trigger prior to enabling auto black ref
      status |= setCameraParameter("auto.trigger.mode", 0);
      if (status == asynSuccess) setIntegerParam(PHANTOM_AutoTriggerMode_, 0);
    }
    if (status == asynSuccess){
      status |= setCameraParameter("auto.bref", value);
    } 
  }
  else if (function == PHANTOM_DataFormat_){
    //Update the selected token and associated bit depth
    int downloadCount = 0;
    getIntegerParam(PHANTOM_DownloadCount_, &downloadCount);
    if(downloadCount){
      setStringParam(ADStatusMessage, "Cannot change pixel type while downloading!");  
      setIntegerParam(ADStatus, ADStatusError);
      status |= asynError;
    }
    else{
      if (value==0){
        bitDepth_ = 8;
        phantomToken_ = "8";
      }
      else if (value==1){
        bitDepth_ = 8;
        phantomToken_ = "8R";
      }
      else if (value==2){
        bitDepth_ = 16;
        phantomToken_ = "P16";
      }
      else if (value==3){
        bitDepth_ = 16;
        phantomToken_ = "P16R";
      }
      else if (value==4){
        bitDepth_ = 10;
        phantomToken_ = "P10";
      }
      else if (value==5){
        bitDepth_ = 12;
        phantomToken_ = "P12L";
      }
      else{
        printf("Invalid Data Format selected!\n");
        status = asynError;
      }
    }
  }

  // If the status is bad reset the original value
  if (status){
    setIntegerParam(function, oldValue);
  }
  this->lock();
  // Do callbacks so higher layers see any changes
  callParamCallbacks();
  this->unlock();
  if (status){
    debug(functionName, "Error, status", status);
    debug(functionName, "Error, function", function);
    debug(functionName, "Error, value", value);
    setIntegerParam(function, oldValue);
  } else {
    debug(functionName, "Function", function);
    debug(functionName, "Value", value);
  }

  return (asynStatus)status;
}

/**
 * Called when asyn clients call pasynFloat64->write().
 * Write integer value to the drivers parameter table.
 *
 * @param pasynUser - Pointer to the asyn user structure.
 * @param value - The new value to write
*/
asynStatus ADPhantom::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
{
  const char *functionName = "ADPhantom::writeFloat64";
  asynStatus status = asynSuccess;
  int function = pasynUser->reason;
  double oldValue;
  char command[PHANTOM_MAX_STRING];
  int exposure = 0;
  double fps = 0;
  std::string response;

  // parameters for functions
  int adstatus;

  getDoubleParam(function, &oldValue);
  setDoubleParam(function, value);
  getIntegerParam(ADStatus, &adstatus);

  if (function == ADAcquireTime){
    // Exposure time in nanoseconds
    exposure = (int)(value * 1000000000.0);
    // Create the command
    sprintf(command, "set defc.exp %d", exposure);
    sendSimpleCommand(command, &response);
    getCameraDataStruc("defc", paramMap_);
    stringToInteger(paramMap_["defc.exp"].getValue(), exposure);
    value = (double)exposure/1000000000.0;
    setDoubleParam(function, (double)exposure/1000000000.0);
  } else if (function == ADAcquirePeriod){
    // Number of frames per second
    fps = (double)(1.0 / value);
    // Create the command
    sprintf(command, "set defc.rate %f", fps);
    sendSimpleCommand(command, &response);
    getCameraDataStruc("defc", paramMap_);
    stringToDouble(paramMap_["defc.rate"].getValue(), fps);
    value = (double)1.0/(double)(fps);
    setDoubleParam(function, value);
  }

  /*
  // Check if the function is one of our stored parameter index values
  if (paramIndexes_.count(function) == 1){
    // This means the parameter was read out from the PHANTOM hardware at startup
    debug(functionName, "Update request of parameter", paramIndexes_[function]);
    debug(functionName, "New Value", value);
    status = setCameraParameter(paramMap_[paramIndexes_[function]], value);
    double newValue = 0.0;
    if (status == asynSuccess){
      status = getCameraParameter(paramMap_[paramIndexes_[function]], newValue);
    }
    if (status == asynSuccess){
      setDoubleParam(function, newValue);
    } else {
      setDoubleParam(function, oldValue);
    }
  }
*/
  this->lock();
  // Do callbacks so higher layers see any changes
  callParamCallbacks();
  this->unlock();
  if (status){
    debug(functionName, "Error, status", status);
    debug(functionName, "Error, function", function);
    debug(functionName, "Error, value", value);
  } else {
    debug(functionName, "Function", function);
    debug(functionName, "Value", (double)value);
  }

  return status;
}

asynStatus ADPhantom::writeOctet(asynUser *pasynUser, const char *value, size_t nChars, size_t *nActual)
{
  int addr=0;
  int function = pasynUser->reason;
  asynStatus status = asynSuccess;
  const char *functionName = "writeOctet";
  status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);
  // Set the parameter in the parameter library.
  status = (asynStatus)setStringParam(addr, function, (char *)value);
  if (status != asynSuccess) return(status);

  if (function == PHANTOM_CFFileName_){
    status = this->downloadFlashHeader(value);
  } else if (function == PHANTOM_CineName_){
    status = this->setCameraParameter("meta.name", value);
  }

  // Do callbacks so higher layers see any changes
  callParamCallbacks(addr, addr);

  //Call base class method. This will handle callCallbacks even if the function was handled here.
  status = ADDriver::writeOctet(pasynUser, value, nChars, nActual);


  if (status != asynSuccess){
    epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                  "%s: status=%d, function=%d, value=%s",
                  functionName, status, function, value);
  } else {
    asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
              "%s: function=%d, value=%s\n",
              functionName, function, value);
  }
  *nActual = nChars;
  return status;
}

asynStatus ADPhantom::setCameraResolution()
{
  const char * functionName = "ADPhantom::setCameraResolution";
  char command[PHANTOM_MAX_STRING];
  std::string response;
  int x = 0;
  int y = 0;
  asynStatus status = asynSuccess;

  // Read both the desired x and y and set the resolution
  getIntegerParam(ADSizeX, &x);
  getIntegerParam(ADSizeY, &y);

  sprintf(command, "%s defc.res %dx%d", PHANTOM_CMD_SET_VALUE, x, y);

  debug(functionName, "Sending command", command);
  status = sendSimpleCommand(command, &response);

  return status;
}

asynStatus ADPhantom::performCSR()
{
  const char * functionName = "ADPhantom::performCSR";
  std::string response;
  asynStatus status = asynSuccess;

  //Disable auto trigger prior to CSR
  status = setCameraParameter("auto.trigger.mode", 0);
  if( status == asynSuccess){
    setIntegerParam(PHANTOM_AutoTriggerMode_, 0);
    debug(functionName, "Sending command", PHANTOM_CMD_BLACKREF);
    status = sendSimpleCommand(PHANTOM_CMD_BLACKREF, &response);
  }

  return status;
}

asynStatus ADPhantom::attachToPort(const std::string& portName)
{
  const char * functionName = "ADPhantom::attachToPort";
  char command[PHANTOM_MAX_STRING];
  std::string response;
  asynStatus status = asynSuccess;

  int port = this->getLocalPortNum(portName.c_str());

  debug(functionName, "Data port number", port);
  sprintf(command, "attach {port:%d}", port);
  status = sendSimpleCommand(command, &response);
  debug(functionName, "Response", response);
  printf("%s response: %s\n", functionName, response.c_str());
  return status;
}

asynStatus ADPhantom::readoutPreviewData()
{
  const char * functionName = "ADPhantom::readoutPreviewData";
  char command[PHANTOM_MAX_STRING];
  int nBytes = 0;
  std::string response;
  NDArray *pImage;
  size_t dims[2];
  NDDataType_t dataType;
  int acquire;
  int arrayCallbacks   = 0;
  int status = asynSuccess;

  // Calculate the number of bytes to read
  nBytes = (int)((double)previewWidth_ * (double)previewHeight_ * ((float)bitDepth_/8));
  debug(functionName, "nBytes", nBytes);

  // Flush the data connection
  pasynOctetSyncIO->flush(dataChannel_);

  // Read in the acquire state and selected cine
  getIntegerParam(ADAcquire, &acquire);
  if (acquire){
    sprintf(command, "img {cine:-1, start:0, cnt:1, fmt:%s}", phantomToken_.c_str());
    status = sendSimpleCommand(command, &response);
  } else {
    sprintf(command, "img {cine:0, start:0, cnt:1, fmt:%s}", phantomToken_.c_str());
    status = sendSimpleCommand(command, &response);
    debug(functionName, command);
  }
  debug(functionName, "Response", response);

  struct timespec endTime;
  clock_gettime(CLOCK_MONOTONIC_RAW, &endTime);
  uint64_t delta_ms = (endTime.tv_sec - frameStart_.tv_sec) * 1000 + (endTime.tv_nsec - frameStart_.tv_nsec) / 1000000; 
  debug(functionName, "Time taken for driver to process preview data", (int)delta_ms);

  this->readFrame(nBytes);

  // Lock the number of bytes to prevent race condition
  // Send event to conversion threads to start converting their slice of the new data
  // Wait for all conv threads to send the finished event
  // Unlock
  conversionBitDepth_ = bitDepth_;
  conversionBytes_ = nBytes;
  for (int i =0; i<PHANTOM_CONV_THREADS; i++){
    epicsEventSignal(convStartEvt_[i]);
  }
  this->unlock();
  for (int i =0; i<PHANTOM_CONV_THREADS; i++){
    status = epicsEventWaitWithTimeout(convFinishEvt_[i], 0.1);
    if (status == epicsEventWaitTimeout){
      printf("Asyn timeout on conversion! This shouldnt happen\n");
    }
  }
  this->lock();

  // Allocate NDArray memory
  dims[0] = previewWidth_;
  dims[1] = previewHeight_;
  nBytes = (dims[0] * dims[1]) * sizeof(int16_t);
  dataType= NDUInt16;
  pImage = this->pNDArrayPool->alloc(2, dims, dataType, nBytes, NULL);

  if (bitDepth_!=16){
    //must use converted data
    memcpy(pImage->pData, flashData_, nBytes);
  }
  else if (bitDepth_==16){
    //raw data can be used
    memcpy(pImage->pData, data_, nBytes);
  }
  pImage->dims[0].size = dims[0];
  pImage->dims[1].size = dims[1];

  getIntegerParam(NDArrayCallbacks, &arrayCallbacks);
  if (arrayCallbacks){
    // Must release the lock here, or we can get into a deadlock, because we can
    // block on the plugin lock, and the plugin can be calling us
    this->unlock();
    debug(functionName, "Calling NDArray callback");
    doCallbacksGenericPointer(pImage, NDArrayData, 0);
    this->lock();
  }

  // Free the image buffer
  pImage->release();

  return (asynStatus) status;
}

asynStatus ADPhantom::sendSoftwareTrigger()
{
  const char * functionName = "ADPhantom::sendSoftwareTrigger";
  std::string response;
  asynStatus status = asynSuccess;

  status = sendSimpleCommand(PHANTOM_CMD_TRIG, &response);
  debug(functionName, "Response", response);

  return status;
}

asynStatus ADPhantom::deleteCineFiles()
{
  const char * functionName = "ADPhantom::deleteCineFiles";

  int start_cine = 0;
  int end_cine = 0;
  int num_cines = 0;
  char command[PHANTOM_MAX_STRING];
  asynStatus status = asynSuccess;
  std::string response;

  // Read in the number of frames to download
  getIntegerParam(PHANTOM_DeleteStartCine_, &start_cine);
  getIntegerParam(PHANTOM_DeleteEndCine_, &end_cine);
  getIntegerParam(PHANTOM_GetCineCount_, &num_cines);

  debug(functionName, "Delete start cine", start_cine);
  debug(functionName, "Delete end cine", end_cine);

  if(start_cine > num_cines){
    setStringParam(ADStatusMessage, "Delete start cine greater than number of cines");  
    setIntegerParam(ADStatus, ADStatusError);
    return asynError;
  }
  else if(end_cine > num_cines){
    setStringParam(ADStatusMessage, "Delete end cine greater than number of cines");  
    setIntegerParam(ADStatus, ADStatusError);
    return asynError;
  }
  
  int cine{start_cine};
  do {
    if(status != asynSuccess){
       break;
    }
    //Allows range to loop around
    if(cine > num_cines){
      cine = 1;
    }
    sprintf(command, "del %d", cine);
    status = sendSimpleCommand(command, &response);
    debug(functionName, "Command", command);
    debug(functionName, "Response", response);
  } while(cine ++ != end_cine);

  return status;
}

asynStatus ADPhantom::saveCineToFlash(int cine)
{
  const char * functionName = "ADPhantom::saveCineToFlash";
  char command[PHANTOM_MAX_STRING];
  std::string response = "";
  asynStatus status = asynSuccess;

  // Verify the cine file is valid
  if (cine < 1 || cine >= PHANTOM_NUMBER_OF_CINES ){
    printf("%s: Error - Invalid cine index %d\n", functionName, cine);
    status = asynError;
  }

  if (status == asynSuccess){
    // Execute the save cine command
    sprintf(command, "%s {cine: %d}", PHANTOM_CMD_CFSAVE, cine);
    status = sendSimpleCommand(command, &response);
  }

  return status;
}

asynStatus ADPhantom::saveSettings()
{
  const char * functionName = "ADPhantom::saveSettings";
  char command[PHANTOM_MAX_STRING];
  std::string response = "";
  int slot = 1;
  asynStatus status = asynSuccess;

  getIntegerParam(PHANTOM_SettingsSlot_, &slot);
  sprintf(command, "%s { slot: %d }", PHANTOM_CMD_USERSAVE, slot);
  status = sendSimpleCommand(command, &response);
  debug(functionName, response.c_str());
  return status;
}

asynStatus ADPhantom::loadSettings()
{
  const char * functionName = "ADPhantom::loadSettings";
  char command[PHANTOM_MAX_STRING];
  std::string response = "";
  int slot = 1;
  asynStatus status = asynSuccess;

  getIntegerParam(PHANTOM_SettingsSlot_, &slot);
  sprintf(command, "%s { slot: %d }", PHANTOM_CMD_USERLOAD, slot);
  status = sendSimpleCommand(command, &response);
  debug(functionName, response.c_str());
  return status;
}

asynStatus ADPhantom::formatFlash()
{
  const char * functionName = "ADPhantom::formatFlash";
  std::string response = "";
  asynStatus status = asynSuccess;

  status = sendSimpleCommand(PHANTOM_CMD_CFFORMAT, &response, PHANTOM_EXTENDED_TIMEOUT);
  debug(functionName, response.c_str());
  return status;
}

asynStatus ADPhantom::deleteFlashFile()
{
  const char * functionName = "ADPhantom::deleteFlashFile";
  char command[PHANTOM_MAX_STRING];
  char filename[PHANTOM_MAX_STRING];
  std::string response = "";
  asynStatus status = asynSuccess;

  // Read in the selected file
  getStringParam(PHANTOM_CFFileName_, PHANTOM_MAX_STRING, filename);

  // Verify the cine file is valid
  if (strcmp(filename, "") == 0){
    printf("%s: Error - Invalid empty filename\n", functionName);
    status = asynError;
  }

  if (status == asynSuccess){
    // Execute the save cine command
    sprintf(command, "%s {filename: \"%s\"}", PHANTOM_CMD_CFRM, filename);
    status = sendSimpleCommand(command, &response);
  }

  return status;
}

asynStatus ADPhantom::selectFlashByIndex(int index)
{
  const char * functionName = "ADPhantom::selectFlashByIndex";
  char filename[PHANTOM_MAX_STRING];
  asynStatus status = asynSuccess;

  // Check the index is between 1 and 8
  if (index < 1 || index > 8){
    debug(functionName, "Select flash index out of range (1-8)");
    status = asynError;
  }
  if (status == asynSuccess){
    getStringParam(PHANTOM_CfFileName_[index-1], PHANTOM_MAX_STRING, filename);
    if (strcmp(filename, "")){
      setStringParam(PHANTOM_CFFileName_, filename);
      status = downloadFlashHeader(filename);
    }
  }
  return status;
}

asynStatus ADPhantom::downloadFlashFile()
{
  const char * functionName = "ADPhantom::downloadFlashFile";
  int start = 0;
  int end = 0;
  char filename[PHANTOM_MAX_STRING];
  asynStatus status = asynSuccess;

  // Read in the number of frames to download
  getIntegerParam(PHANTOM_CFSRecordStart_, &start);
  getIntegerParam(PHANTOM_CFSRecordEnd_, &end);

  // Attach to the correct port
  //status = attachToPort("dataPort");

  // Read in the selected file
  getStringParam(PHANTOM_CFFileName_, PHANTOM_MAX_STRING, filename);

  // Verify the file is valid
  if (strcmp(filename, "") == 0){
    printf("%s: Error - Invalid empty filename\n", functionName);
    status = asynError;
  }

  if (start < (int)cineHeader_.FirstImageNo || start > (int)(cineHeader_.FirstImageNo + cineHeader_.ImageCount)){
    printf("Flash Download failed: Invalid start index\n");
    setStringParam(ADStatusMessage, "Flash Download failed: Index error");
    setIntegerParam(ADStatus, ADStatusError);
    callParamCallbacks();
    status = asynError;
  }
  if (end < (int)cineHeader_.FirstImageNo || end > (int)(cineHeader_.FirstImageNo + cineHeader_.ImageCount)){
    printf("Flash Download failed: Invalid end index\n");
    setStringParam(ADStatusMessage, "Flash Download failed: Index error");
    setIntegerParam(ADStatus, ADStatusError);
    callParamCallbacks();
    status = asynError;
  }
  if (start > end){
    printf("Flash Download failed: End index must be greater than start index\n");
    setStringParam(ADStatusMessage, "Flash Download failed: Index error");
    setIntegerParam(ADStatus, ADStatusError);
    callParamCallbacks();
    status = asynError;
  }

  // we need to calculate the index values of the frames (starting at index 0)
  start = start - cineHeader_.FirstImageNo;
  end = end - cineHeader_.FirstImageNo;

  //printf("*** Flash: first image index: %d\n", start);
  //printf("*** Flash: last image index: %d\n", end);

  if (status == asynSuccess){
    // Download the data and process arrays
    status = downloadFlashImages(filename, start, end);
  }
  return status;
}

asynStatus ADPhantom::downloadFlashHeader(const std::string& filename)
{
  const char * functionName = "ADPhantom::downloadFlashHeader";
  char command[PHANTOM_MAX_STRING];
  std::string response;
  short setupLength = 0;
  int noOfTimes = 0;
  int ofsBlock = 0;
  int defaultExposure = 0;
  INFOBLOCK block;
  asynStatus status = asynSuccess;

  // Attach to the correct port
  //status = attachToPort("dataPort");
  // Clear out timestamp data
  flashTsData_.clear();
  flashExpData_.clear();

  if (status == asynSuccess){
    // Flush the data connection
    pasynOctetSyncIO->flush(dataChannel_);

    sprintf(command, "cfread {filename: \"%s\", offset: 0, count:44 }", filename.c_str());
    status = sendSimpleCommand(command, &response);
    debug(functionName, "Response", response);
  }

  if (status == asynSuccess){
    status = this->readFrame(44);
  }

  if (status == asynSuccess){
    memcpy(&cineHeader_, data_, 44);
    char stype[3];
    strncpy(stype, (char *)&(cineHeader_.Type), 2);
    stype[2] = 0;
    debug(functionName, "OffImageOffsets", (int)cineHeader_.OffImageOffsets);
  }

  if (status == asynSuccess){
    sprintf(command, "cfread {filename: \"%s\", offset: 44, count:40 }", filename.c_str());
    status = sendSimpleCommand(command, &response);
    debug(functionName, "Response", response);
  }

  if (status == asynSuccess){
    status = this->readFrame(40);
    memcpy(&cineBitmapHeader_, data_, 40);
    setIntegerParam(PHANTOM_CFSWidth_, cineBitmapHeader_.biWidth);
    setIntegerParam(PHANTOM_CFSHeight_, cineBitmapHeader_.biHeight);
    setIntegerParam(PHANTOM_CFSFrameCount_, cineHeader_.TotalImageCount);
    setIntegerParam(PHANTOM_CFSFirstFrame_, cineHeader_.FirstImageNo);
    std::cout<<"cineHeader_.FirstImageNo: "<<cineHeader_.FirstImageNo<<std::endl;
    setIntegerParam(PHANTOM_CFSLastFrame_, cineHeader_.FirstImageNo + cineHeader_.ImageCount - 1);
  }

  // Read in the default exposure time ns
  if (status == asynSuccess){
    sprintf(command, "cfread {filename: \"%s\", offset: 1652, count:4 }", filename.c_str());
    status = sendSimpleCommand(command, &response);
    debug(functionName, "Response", response);
  }
  if (status == asynSuccess){
    status = this->readFrame(4);
    memcpy(&defaultExposure, data_, 4);
  }

  // Read in the setup length
  if (status == asynSuccess){
    sprintf(command, "cfread {filename: \"%s\", offset: 226, count:2 }", filename.c_str());
    status = sendSimpleCommand(command, &response);
    debug(functionName, "Response", response);
  }
  if (status == asynSuccess){
    status = this->readFrame(2);
    memcpy(&setupLength, data_, 2);
    // Now read in the first tagged information block header
    ofsBlock = 84 + setupLength;
    sprintf(command, "cfread {filename: \"%s\", offset: %d, count:8 }", filename.c_str(), ofsBlock);
    status = sendSimpleCommand(command, &response);
    debug(functionName, "Response", response);
  }
  if (status == asynSuccess){
    status = this->readFrame(8);
    memcpy(&block, data_, 8);
    noOfTimes = (block.size - 8) / 8;
    // Read out the data block and record the time values
    ofsBlock += 8;
    sprintf(command, "cfread {filename: \"%s\", offset: %d, count:%d }", filename.c_str(), ofsBlock, (block.size-8));
    status = sendSimpleCommand(command, &response);
    debug(functionName, "Response", response);
  }
  if (status == asynSuccess){
    tagTIME64 ts;
    status = this->readFrame(block.size-8);
    char *ptr = data_;
    for (int count = 0; count < noOfTimes; count++){
      memcpy(&ts, ptr, 8);
      char locked = ts.frac & 0x01;
      flashIrigData_.push_back(locked);
      char event_active = (ts.frac & 0x02) >> 1;
      flashEventData_.push_back(event_active);
      //printf("tsec: %d\n", ts.secs);
      //printf("tusec: %d\n", (int32_t)round((double)(ts.frac) / 4294.967296));
      ts.frac = (uint32_t)round((double)(ts.frac) / 4294.967296);
      flashTsData_.push_back(ts);
      ptr+= 8;
    }
  }
  if (status == asynSuccess){
    // Now read in the second tagged information block header
    ofsBlock += (block.size - 16);
    sprintf(command, "cfread {filename: \"%s\", offset: %d, count:8 }", filename.c_str(), ofsBlock);
    status = sendSimpleCommand(command, &response);
    debug(functionName, "Response", response);
  }
  if (status == asynSuccess){
    status = this->readFrame(8);
    memcpy(&block, data_, 8);
    noOfTimes = (block.size - 8) / 4;
    // Read out the data block and record the time values
    ofsBlock += 8;
    sprintf(command, "cfread {filename: \"%s\", offset: %d, count:%d }", filename.c_str(), ofsBlock, (block.size-8));
    status = sendSimpleCommand(command, &response);
    debug(functionName, "Response", response);
  }
  if (status == asynSuccess){
    status = this->readFrame(block.size-8);
    uint32_t *exp = (uint32_t *)data_;
    for (int count = 0; count < noOfTimes; count++){
      if (*exp == 0){
        flashExpData_.push_back((uint32_t)defaultExposure);
      } else {
        flashExpData_.push_back((uint32_t)round((double)(*exp) / 4294.967296));
      }
      exp++;
    }
  }

  // Loop over meta array to read values
  for (int mc = 0; mc < (int)metaArray_.size(); mc++){
    if (metaArray_[mc]->offset_ > 0){
      sprintf(command, "cfread {filename: \"%s\", offset: %d, count:%d }", filename.c_str(), metaArray_[mc]->offset_, metaArray_[mc]->size_);
      status = sendSimpleCommand(command, &response);
      debug(functionName, "Response", response);
      status = this->readFrame(metaArray_[mc]->size_);
      memcpy(metaArray_[mc]->vPtr_, data_, metaArray_[mc]->size_);
      if (metaArray_[mc]->type_ == NDAttrInt32){
        int32_t *ival = (int32_t *)metaArray_[mc]->vPtr_;
        if (metaArray_[mc]->name_ == "trigger_usecs"){
          uint32_t uival = (uint32_t)*ival;
          double timeVal = (double)(uival) / 4294.967296;
          int32_t tival = (int32_t)round(timeVal);
          flashTrigUsecs_ = tival;
          memcpy(metaArray_[mc]->vPtr_, &tival, 4);
          ival = (int32_t *)metaArray_[mc]->vPtr_;
        }
        if (metaArray_[mc]->name_ == "trigger_secs"){
          uint32_t uival = (uint32_t)*ival;
          flashTrigSecs_ = uival;
        }
      }
    }
  }

  return status;
}

asynStatus ADPhantom::downloadFlashImages(const std::string& filename, int start, int end)
{
  const char * functionName = "ADPhantom::downloadFlashFile";
  char command[PHANTOM_MAX_STRING];
  std::string response;
  NDArray *pImage;
  size_t dims[2];
  NDDataType_t dataType;
  int width = 0;
  int height = 0;
  int nBytes = 0;
  int nbytes = 0;
  int arrayCallbacks   = 0;
  int recordCount = 0;
  int metaFrame = 0;
  int frames = end - start + 1;
  int firstfr = 0;
  int postTrig = 0;
  int first_tv_sec = 0;
  int first_tv_usec = 0;
  int status = asynSuccess;

  // Attach to the correct port
  //status = attachToPort("dataPort");

  // Flush the data connection
  pasynOctetSyncIO->flush(dataChannel_);

  // Obtain the post trig count
  postTrig = cineHeader_.FirstImageNo + cineHeader_.TotalImageCount;
  // Obtain the first frame number
  firstfr = start + cineHeader_.FirstImageNo;

  sprintf(command, "cfread {filename: \"%s\", offset:%d, count:%d }", filename.c_str(), cineHeader_.OffImageOffsets, (8*cineHeader_.TotalImageCount) );
  status = sendSimpleCommand(command, &response);
  debug(functionName, "Response", response);
  status = this->readFrame(8*cineHeader_.TotalImageCount);
  unsigned long imagePtr[8*cineHeader_.TotalImageCount];
  memcpy(imagePtr, data_, 8*cineHeader_.TotalImageCount);

  // Read the frame size for the selected cine file
  width = cineBitmapHeader_.biWidth;
  height = cineBitmapHeader_.biHeight;
  // Calculate the number of bytes to read
  nBytes = (int)((double)width * (double)height * ((float)bitDepth_/8));
  debug(functionName, "Width", width);
  debug(functionName, "Height", height);
  debug(functionName, "nBytes", nBytes);

  first_tv_sec = flashTsData_[start].secs;
  first_tv_usec = flashTsData_[start].frac;
  recordCount = 0;
  for (int count = start; count <= end; count++){
    metaFrame = count + cineHeader_.FirstImageNo;
    //printf("Image %d offset value: %lX\n", count, imagePtr[count]);
    sprintf(command, "cfread {filename: \"%s\", offset:%lu, count:4 }", filename.c_str(), imagePtr[count]);
    status = sendSimpleCommand(command, &response);
    debug(functionName, "Response", response);
    status = this->readFrame(4);
    unsigned int *iPtr = (unsigned int *)data_;
    //printf("Annotation size %d\n", *iPtr);
    unsigned long pixelCountAddress = imagePtr[count] + *iPtr - 4;
    //printf("Pixel count address: %lX\n", pixelCountAddress);
    sprintf(command, "cfread {filename: \"%s\", offset:%lu, count:4 }", filename.c_str(), pixelCountAddress );
    status = sendSimpleCommand(command, &response);
    debug(functionName, "Response", response);
    status = this->readFrame(4);
    iPtr = (unsigned int *)data_;
    debug(functionName, "Pixel count", (int)(*iPtr));
    pixelCountAddress += 4;
    sprintf(command, "cfread {filename: \"%s\", offset:%lu, count:%d }", filename.c_str(), pixelCountAddress, nBytes/2);
    status = sendSimpleCommand(command, &response);
    //printf("%s Response: %s\n", functionName, response.c_str());
    pixelCountAddress += nBytes/2;
    sprintf(command, "cfread {filename: \"%s\", offset:%lu, count:%d }", filename.c_str(), pixelCountAddress, nBytes/2);
    status = sendSimpleCommand(command, &response);
    //printf("%s Response: %s\n", functionName, response.c_str());
    debug(functionName, "Response", response);
    status = this->readFrame(nBytes);

    // Lock the number of bytes to prevent race condition
    // Send event to conversion threads to start converting their slice of the new data
    // Wait for all conv threads to send the finished event
    // Unlock
    conversionBitDepth_ = bitDepth_;
    conversionBytes_ = nBytes;
    for (int i =0; i<PHANTOM_CONV_THREADS; i++){
      epicsEventSignal(convStartEvt_[i]);
    }
    this->unlock();
    for (int i =0; i<PHANTOM_CONV_THREADS; i++){
      status = epicsEventWaitWithTimeout(convFinishEvt_[i], 0.1);
      if (status == epicsEventWaitTimeout){
        printf("Asyn timeout on conversion! This shouldnt happen\n");
      }
    }
    this->lock();

    if (status == asynSuccess){
      // Allocate NDArray memory
      dims[0] = cineBitmapHeader_.biWidth;
      dims[1] = cineBitmapHeader_.biHeight;
      setIntegerParam(NDArraySizeX, cineBitmapHeader_.biWidth);
      setIntegerParam(NDArraySizeY, cineBitmapHeader_.biHeight);
      nbytes = (dims[0] * dims[1]) * sizeof(int16_t);
      dataType= NDUInt16;
      pImage = this->pNDArrayPool->alloc(2, dims, dataType, nbytes, NULL);

      memcpy(pImage->pData, flashData_, nbytes);
      pImage->dims[0].size = dims[0];
      pImage->dims[1].size = dims[1];
      // Add the frame number attribute
      pImage->pAttributeList->add("number", "Frame number", NDAttrInt32, (void *)(&metaFrame));
      // Add the download start frame
      pImage->pAttributeList->add("rec_first_frame", "First frame of recording", NDAttrInt32, (void *)(&firstfr));
      // Add the download frame count
      pImage->pAttributeList->add("rec_frame_count", "Frame count of recording", NDAttrInt32, (void *)(&frames));
      // Add the flash filename
      char sval[256];
      strncpy(sval, (char *)filename.c_str(), 256);
      pImage->pAttributeList->add("filename", "Flash file name", NDAttrString, (void *)(sval));
      // Add the post trigger frame count
      pImage->pAttributeList->add("post_trig_frames", "Post trigger frame count", NDAttrInt32, (void *)(&postTrig));

      int tv_sec = flashTsData_[count].secs;
      pImage->pAttributeList->add("ts_sec", "Timestamp of frames (seconds since 1970)", NDAttrUInt32, (void *)(&tv_sec));
      int tv_usec = flashTsData_[count].frac;
      pImage->pAttributeList->add("ts_usec", "Timestamp of frames (microseconds)", NDAttrUInt32, (void *)(&tv_usec));

      pImage->pAttributeList->add("exp_time", "Exposure time (nanoseconds)", NDAttrUInt32, (void *)(&(flashExpData_[count])));
      pImage->pAttributeList->add("irig_sync", "IRIG synchronized", NDAttrInt8, (void *)(&(flashIrigData_[count])));
      pImage->pAttributeList->add("event_input", "Event Input (1 = open)", NDAttrInt8, (void *)(&(flashEventData_[count])));
      //pixel token
      pImage->pAttributeList->add("pixel_token", "Phantom pixel type token", NDAttrString, (void *)(phantomToken_.c_str()));
      // Loop over meta array to create attributes
      for (int mc = 0; mc < (int)metaArray_.size(); mc++){
        if (metaArray_[mc]->type_ == NDAttrInt8){
          pImage->pAttributeList->add(metaArray_[mc]->name_.c_str(),
              metaArray_[mc]->desc_.c_str(),
              NDAttrInt8,
              metaArray_[mc]->vPtr_);
        } else if (metaArray_[mc]->type_ == NDAttrInt32){
          pImage->pAttributeList->add(metaArray_[mc]->name_.c_str(),
              metaArray_[mc]->desc_.c_str(),
              NDAttrInt32,
              metaArray_[mc]->vPtr_);
        } else if (metaArray_[mc]->type_ == NDAttrFloat64){
          pImage->pAttributeList->add(metaArray_[mc]->name_.c_str(),
              metaArray_[mc]->desc_.c_str(),
              NDAttrFloat64,
              metaArray_[mc]->vPtr_);
        } else if (metaArray_[mc]->type_ == NDAttrString){
          char sval[256];
          strncpy(sval, (char *)metaArray_[mc]->vPtr_, metaArray_[mc]->size_);
          pImage->pAttributeList->add(metaArray_[mc]->name_.c_str(),
              metaArray_[mc]->desc_.c_str(),
              NDAttrString,
              (void *)(sval));
        }
      }
      int tfts = tv_sec - flashTrigSecs_;
      int tftus = tv_usec - flashTrigUsecs_;
      if (tftus < 0){
        tfts--;
        tftus += 1000000;
      }
      int tft = (tfts * 1000000) + tftus;
      pImage->pAttributeList->add("tft", "Time from trigger (microseconds)", NDAttrInt32, (void *)(&tft));

      int ifts = tv_sec - first_tv_sec;
      int iftus = tv_usec - first_tv_usec;
      if (iftus < 0){
        ifts--;
        iftus += 1000000;
      }
      int ift = (ifts * 1000000) + iftus;
      pImage->pAttributeList->add("ift", "Inter frame time (microseconds)", NDAttrInt32, (void *)(&ift));
      first_tv_sec = tv_sec;
      first_tv_usec = tv_usec;

      getIntegerParam(NDArrayCallbacks, &arrayCallbacks);
      if (arrayCallbacks){
        // Must release the lock here, or we can get into a deadlock, because we can
        // block on the plugin lock, and the plugin can be calling us
        this->unlock();
        debug(functionName, "Calling NDArray callback");
        doCallbacksGenericPointer(pImage, NDArrayData, 0);
        this->lock();
      }

      // Free the image buffer
      pImage->release();
    }
    recordCount++;
    setIntegerParam(PHANTOM_CFSRecordCount_, recordCount);
    callParamCallbacks();
  }

  return (asynStatus) status;
}

asynStatus ADPhantom::convert12BitPacketTo16Bit(unsigned char *inBytes, unsigned char *outBytes)
{
  asynStatus status = asynSuccess;

  int pIndex = 0;
  int rawValue = (inBytes[0]<<4) + ((inBytes[1]&0xF0)>>4);
  outBytes[pIndex] = (rawValue&0x00FF);
  pIndex++;
  outBytes[pIndex] = (rawValue&0xFF00)>>8;
  pIndex++;

  rawValue = ((inBytes[1]&0x0F)<<8) + ((inBytes[2]&0xFF));
  outBytes[pIndex] = (rawValue&0x00FF);
  pIndex++;
  outBytes[pIndex] = (rawValue&0xFF00)>>8;
  pIndex++;

  return status;
}

asynStatus ADPhantom::convert10BitPacketTo16Bit(unsigned char *inBytes, unsigned char *outBytes)
{
  asynStatus status = asynSuccess;

  int pIndex = 0;
  int rawValue = PHANTOM_LinLUT[(inBytes[0]<<2) + ((inBytes[1]&0xC0)>>6)];
  outBytes[pIndex] = (rawValue&0x00FF);
  pIndex++;
  outBytes[pIndex] = (rawValue&0xFF00)>>8;
  pIndex++;

  rawValue = PHANTOM_LinLUT[((inBytes[1]&0x3F)<<4) + ((inBytes[2]&0xF0)>>4)];
  outBytes[pIndex] = (rawValue&0x00FF);
  pIndex++;
  outBytes[pIndex] = (rawValue&0xFF00)>>8;
  pIndex++;

  rawValue = PHANTOM_LinLUT[((inBytes[2]&0x0F)<<6) + ((inBytes[3]&0xFC)>>2)];
  outBytes[pIndex] = (rawValue&0x00FF);
  pIndex++;
  outBytes[pIndex] = (rawValue&0xFF00)>>8;
  pIndex++;

  rawValue = PHANTOM_LinLUT[((inBytes[3]&0x03)<<8) + inBytes[4]];
  outBytes[pIndex] = (rawValue&0x00FF);
  pIndex++;
  outBytes[pIndex] = (rawValue&0xFF00)>>8;
  pIndex++;
  return status;
}


asynStatus ADPhantom::convert8BitPacketTo16Bit(unsigned char *inBytes, unsigned char *outBytes, int nBytes)
{
  asynStatus status = asynSuccess;
  
  for (int i=0;i<nBytes;i++)
  {
    outBytes[(i*2)+1]=inBytes[i];
    outBytes[(i*2)]=0;
  }
  //uint8_t byte1 = inBytes[0];
  //uint16_t byte2 = (outBytes[0] << 8) + outBytes[1];
  //printf("8bit val = %d  16bit val= %d\n",byte1,byte2);
  return status;
}

asynStatus ADPhantom::readoutTimestamps(int start_cine, int end_cine, int start_frame, int end_frame, bool uni_frame_lim)
{
  const char * functionName = "ADPhantom::readoutTimestamps";
  char command[PHANTOM_MAX_STRING];
  int nBytes = 0;
  std::string response;
  int first_frame = 0;
  int last_frame = 0;
  int frames = 0;
  int num_cines = 0;
  short_time_stamp32 ts;
  asynStatus status = asynSuccess;

  getIntegerParam(PHANTOM_GetCineCount_, &num_cines);

  // Flush the data connection
  pasynOctetSyncIO->flush(dataChannel_);
  // Clear any old timestamp data out
  timestampData_.clear();

  int cine{start_cine};
  do{ //do/while loop ending when cine == end_cine

    //Previous cine timestaps have failed to load
    if(status != asynSuccess){
      return status;
    }
    //Allows range to loop back around
    if(cine > num_cines){ 
      cine = 1;
    }

    //Determine first and last frame to read
    if(uni_frame_lim){ //All cines have same frame limits
      first_frame = start_frame;
      last_frame = end_frame;
    } else { //Frame limits only applied to first and last cine
      if (cine == start_cine){
        first_frame = start_frame;
      } else{
        getIntegerParam(PHANTOM_CnFirstFrame_[cine], &first_frame);
      } 
      if(cine == end_cine){
        last_frame = end_frame;
      } else{
        getIntegerParam(PHANTOM_CnLastFrame_[cine], &last_frame);
      }
    }
    frames = last_frame - first_frame + 1;

    sprintf(command, "time {cine:%d, start:%d, cnt:%d}", cine, first_frame, frames);
    status = sendSimpleCommand(command, &response);
    debug(functionName, "Command", command);
    debug(functionName, "Response", response);

    // Read back all of the timestamps in one go
    nBytes = frames * 12;
    status = this->readFrame(nBytes);
    char *dPtr = data_;
    for (int frame = 0; frame < frames; frame++){
      memcpy(&ts, dPtr, 12);
      // time from beginning of the year in 1/100 sec units
      //printf("Time sec 1/100: %u\n", ntohl(ts.csecs));
      // exposure time in us
      //printf("Exp time us: %d\n", ts.exptime);
      // bits[15..2]: fractions (us to 10000); b[1]:event; b[0]:lock
      //printf("Fractions us to 10000: %u\n", ntohs(ts.frac)>>2);
      // exposure extension up to 32 bits
      //printf("Exposure extension: %d\n", ts.exptime32);
      //uint32_t ns_exp = (ntohs(ts.exptime)*1000) + (int)(floor((double)ntohs(ts.exptime32)/65535.0*1000.0 + 0.5));
      //printf("Total Exposure ns: %d\n", ns_exp);
      // fractions extension up to 32 bits
      //printf("Fractions extension: %d\n", ts.frac32);
      dPtr+=12;
      timestampData_.push_back(ts);
    }
  }  while (cine++ != end_cine);
  return status;
}

asynStatus ADPhantom::readoutDataStream(int start_cine, int end_cine, int start_frame, int end_frame, bool uni_frame_lim)
{
  const char * functionName = "ADPhantom::readoutDataStream";
  // Time profiling
  struct timespec endTime;
  clock_gettime(CLOCK_MONOTONIC_RAW, &endTime);
  uint64_t delta_ms = (endTime.tv_sec - frameStart_.tv_sec) * 1000 + (endTime.tv_nsec - frameStart_.tv_nsec) / 1000000; 
  debug(functionName, "Time since last download/init (msec)", (int)delta_ms);
  //

  char command[PHANTOM_MAX_STRING];
  int width = 0;
  int height = 0;
  int nBytes = 0;
  int frame = 0; //Frame within cine
  int total_frame = 0; //Frame within all cines being downloaded
  std::string response;
  NDArray *pImage;
  size_t dims[2];
  NDDataType_t dataType;
  int nbytes;
  int arrayCallbacks   = 0;
  int first_frame = 0;
  int last_frame = 0;
  int frames = 0;
  int num_cines = 0;
  int metaExposure = 0;
  int metaRate = 0;
  int metaFrame = 0; //Frame as specified in cine
  int lastfr = 0;
  int irigYear = 0;
  int trigSecs = 0;
  int trigUSecs = 0;
  int abort = 0;
  int markSaved = 0;
  unsigned int first_tv_sec = 0;
  unsigned int first_tv_usec = 0;
  int status = asynSuccess;

  status = getCameraDataStruc("irig", paramMap_);
  status = stringToInteger(paramMap_["irig.yearbegin"].getValue(), irigYear);

  //Read total number of cines
  getIntegerParam(PHANTOM_GetCineCount_, &num_cines);

  int cine{start_cine};
  do{ //do/while loop ending when cine == end_cine

    if(status != asynSuccess){
      break;
    }
    //Allows range to loop back around
    if(cine > num_cines){ 
      cine = 1;
    }
    frame = 0;

    // Read the cine meta data
    sprintf(command, "c%d.rate", cine);
    status = stringToInteger(paramMap_[command].getValue(), metaRate);
    sprintf(command, "c%d.exp", cine);
    status = stringToInteger(paramMap_[command].getValue(), metaExposure);
    sprintf(command, "c%d.lastfr", cine);
    status = stringToInteger(paramMap_[command].getValue(), lastfr);
    lastfr++;
    sprintf(command, "c%d.trigtime.secs", cine);
    status = stringToInteger(paramMap_[command].getValue(), trigSecs);
    sprintf(command, "c%d.trigtime.frac", cine);
    status = stringToInteger(paramMap_[command].getValue(), trigUSecs);

    // Loop over meta array to read values
    for (int mc = 0; mc < (int)metaArray_.size(); mc++){
      sprintf(command, metaArray_[mc]->param_.c_str(), cine);
      metaArray_[mc]->setStringVal(paramMap_[command].getValue());
    }

    //Determine first and last frame to read
    if(uni_frame_lim){
      first_frame = start_frame;
      last_frame = end_frame;
    } else {
      if (cine == start_cine){
        first_frame = start_frame;
      } else{
        getIntegerParam(PHANTOM_CnFirstFrame_[cine], &first_frame);
      } 
      if(cine == end_cine){
        last_frame = end_frame;
      } else{
        getIntegerParam(PHANTOM_CnLastFrame_[cine], &last_frame);
      }
    }
    frames = last_frame - first_frame + 1;

    // Read the frame size for the selected cine
    getIntegerParam(PHANTOM_CnWidth_[cine], &width);
    getIntegerParam(PHANTOM_CnHeight_[cine], &height);
    // Calculate the number of bytes to read
    // Depends on the chosen bit depth
    nBytes = (int)((double)width * (double)height * ((float)bitDepth_/8));
    debug(functionName, "Width", width);
    debug(functionName, "Height", height);
    debug(functionName, "nBytes", nBytes);


    // Flush the data connection
    pasynOctetSyncIO->flush(dataChannel_);
    sprintf(command, "img {cine:%d, start:%d, cnt:%d, fmt:%s}", cine, first_frame, frames, phantomToken_.c_str());  
    status = sendSimpleCommand(command, &response);
    debug(functionName, "Command", command);
    debug(functionName, "Response", response);
    if (frame == 0){
      short_time_stamp32 tss = timestampData_[total_frame];
      first_tv_sec = (ntohl(tss.csecs) / 100) + irigYear;
      first_tv_usec = ((ntohl(tss.csecs) % 100) * 10000) + (ntohs(tss.frac) >> 2);
    }
    while ((frame < frames) && (status == asynSuccess)){
      getIntegerParam(PHANTOM_DownloadAbort_, &abort);
      if(abort){
        setStringParam(ADStatusMessage, "Download aborting");
        //To abort cleanly we disconnect from the port to restart the datastream
        debug(functionName, "Running common connect");
        status = pasynCommonSyncIO->connect(dataPort_, 0, &commonDataport_, NULL);
        if (status){
          debug(functionName, "Common connect failed");
        }
        debug(functionName, "Running pasynCommonSyncIO->disconnectDevice");
        status = pasynCommonSyncIO->disconnectDevice(commonDataport_);
        if (status) {
          debug(functionName, "Disconnect device failed");
        }
        debug(functionName, "Running pasynCommonSyncIO->connectDevice");
        status = pasynCommonSyncIO->connectDevice(commonDataport_);
        if (status) {
          debug(functionName, "Connect device failed");
        }
        debug(functionName, "Running attachToPort");
        status = attachToPort("dataPort");
        if(status){
          debug(functionName, "Failed to attach ");
        }
        break;
      }

      metaFrame = start_frame+frame;
      frame++;
      total_frame++;
      setIntegerParam(PHANTOM_DownloadCount_, total_frame);
      callParamCallbacks();
          
      //Time profiling
      struct timespec endTime;
      clock_gettime(CLOCK_MONOTONIC_RAW, &endTime);
      uint64_t delta_ms = (endTime.tv_sec - frameStart_.tv_sec) * 1000 + (endTime.tv_nsec - frameStart_.tv_nsec) / 1000000; 
      debug(functionName, "Time taken for driver to process new data", (int)delta_ms);
      //printf("Time taken for driver to process new data %d\n", (int)delta_ms);
      //

      status = this->readFrame(nBytes);

      // Lock the number of bytes to prevent race condition
      // Send event to conversion threads to start converting their slice of the new data
      // Unlock
      // Wait for all conv threads to send the finished event
      // Lock
      conversionBitDepth_ = bitDepth_;
      conversionBytes_ = nBytes;
      for (int i =0; i<PHANTOM_CONV_THREADS; i++){
        epicsEventSignal(convStartEvt_[i]);
      }
      this->unlock();
      for (int i =0; i<PHANTOM_CONV_THREADS; i++){
        status = epicsEventWaitWithTimeout(convFinishEvt_[i], 0.1);
        if (status == epicsEventWaitTimeout){
          printf("Asyn timeout on conversion! This shouldnt happen\n");
        }
      }
      this->lock();

      if (status == asynSuccess){
        // Allocate NDArray memory
        dims[0] = width;
        dims[1] = height;
        setIntegerParam(NDArraySizeX, width);
        setIntegerParam(NDArraySizeY, height);
        this->unlock();
        nbytes = (dims[0] * dims[1]) * sizeof(int16_t);
        dataType= NDUInt16;
        pImage = this->pNDArrayPool->alloc(2, dims, dataType, nbytes, NULL);

        if (bitDepth_==10 || bitDepth_==8 || bitDepth_==12){
          //Use converted data array
          memcpy(pImage->pData, flashData_, nbytes);
        }
        else {
          memcpy(pImage->pData, data_, nbytes);
        }
        pImage->dims[0].size = dims[0];
        pImage->dims[1].size = dims[1];
        // Add the frame number attribute
        pImage->pAttributeList->add("number", "Frame number", NDAttrInt32, (void *)(&metaFrame));
        // Add the download start frame
        pImage->pAttributeList->add("rec_first_frame", "First frame of recording", NDAttrInt32, (void *)(&start_frame));
        // Add the download frame count
        pImage->pAttributeList->add("rec_frame_count", "Frame count of recording", NDAttrInt32, (void *)(&frames));
        // Add the partition number
        pImage->pAttributeList->add("partition", "Partition number", NDAttrInt32, (void *)(&cine));
        // Add the post trigger frame count
        pImage->pAttributeList->add("post_trig_frames", "Post trigger frame count", NDAttrInt32, (void *)(&lastfr));
        // Add the image UniqueID
        pImage->pAttributeList->add("NDArrayUniqueId", "uniqueId", NDAttrInt32, (void *)(&total_frame));      // Loop over meta array to create attributes
        // Add the pixel token
        pImage->pAttributeList->add("pixel_token", "Phantom pixel type token", NDAttrString, (void *)(phantomToken_.c_str()));
        pImage->uniqueId = total_frame;
        for (int mc = 0; mc < (int)metaArray_.size(); mc++){
          if (metaArray_[mc]->type_ == NDAttrInt8){
            pImage->pAttributeList->add(metaArray_[mc]->name_.c_str(),
                metaArray_[mc]->desc_.c_str(),
                NDAttrInt8,
                (void *)(&metaArray_[mc]->cval_));
          } else if (metaArray_[mc]->type_ == NDAttrInt32){
            pImage->pAttributeList->add(metaArray_[mc]->name_.c_str(),
                metaArray_[mc]->desc_.c_str(),
                NDAttrInt32,
                (void *)(&metaArray_[mc]->ival_));
          } else if (metaArray_[mc]->type_ == NDAttrFloat64){
            pImage->pAttributeList->add(metaArray_[mc]->name_.c_str(),
                metaArray_[mc]->desc_.c_str(),
                NDAttrFloat64,
                (void *)(&metaArray_[mc]->dval_));
          } else if (metaArray_[mc]->type_ == NDAttrString){
            char sval[256];
            strncpy(sval, metaArray_[mc]->sval_.c_str(), 256);
            pImage->pAttributeList->add(metaArray_[mc]->name_.c_str(),
                metaArray_[mc]->desc_.c_str(),
                NDAttrString,
                (void *)(sval));
          }
        }

        // Add the timing attributes
        short_time_stamp32 ts = timestampData_[total_frame-1];
        unsigned int tv_sec = (ntohl(ts.csecs) / 100) + irigYear;
        pImage->pAttributeList->add("ts_sec", "Timestamp of frames (seconds since 1970)", NDAttrUInt32, (void *)(&tv_sec));
        unsigned int tv_usec = ((ntohl(ts.csecs) % 100) * 10000) + (ntohs(ts.frac) >> 2);
        pImage->pAttributeList->add("ts_usec", "Timestamp of frames (microseconds)", NDAttrUInt32, (void *)(&tv_usec));
        char locked = ntohs(ts.frac) & 0x01;
        pImage->pAttributeList->add("irig_sync", "IRIG synchronized", NDAttrInt8, (void *)(&locked));
        char event_active = (ntohs(ts.frac) & 0x02) >> 1;
        pImage->pAttributeList->add("event_input", "Event Input (1 = open)", NDAttrInt8, (void *)(&event_active));
        //unsigned int exp_time = ntohs(ts.exptime);
        unsigned int exp_time = (ntohs(ts.exptime)*1000) + (int)(floor((double)ntohs(ts.exptime32)/65535.0*1000.0 + 0.5));
        pImage->pAttributeList->add("exp_time", "Exposure time (nanoseconds)", NDAttrUInt32, (void *)(&exp_time));
        int tfts = tv_sec - trigSecs;
        int tftus = tv_usec - trigUSecs;
        if (tftus < 0){
          tfts--;
          tftus += 1000000;
        }
        int tft = (tfts * 1000000) + tftus;
        pImage->pAttributeList->add("tft", "Time from trigger (microseconds)", NDAttrInt32, (void *)(&tft));

        int ifts = tv_sec - first_tv_sec;
        int iftus = tv_usec - first_tv_usec;
        if (iftus < 0){
          ifts--;
          iftus += 1000000;
        }
        int ift = (ifts * 1000000) + iftus;
        pImage->pAttributeList->add("ift", "Inter frame time (microseconds)", NDAttrInt32, (void *)(&ift));
        first_tv_sec = tv_sec;
        first_tv_usec = tv_usec;

        this->lock();
        getIntegerParam(NDArrayCallbacks, &arrayCallbacks);
        if (arrayCallbacks){
          // Must release the lock here, or we can get into a deadlock, because we can
          // block on the plugin lock, and the plugin can be calling us
          this->unlock();
          debug(functionName, "Calling NDArray callback");
          doCallbacksGenericPointer(pImage, NDArrayData, 0);
          this->lock();
        }

        // Free the image buffer
        pImage->release();
      }
    }
    //mark cine as saved/reusable after download
    if(!abort){
      getIntegerParam(PHANTOM_MarkCineSaved_, &markSaved);
      if(markSaved){
        sprintf(command, "rel %d", cine);
        status = sendSimpleCommand(command, &response);
        debug(functionName, "Command", command);
        debug(functionName, "Response", response);
      }
    }

  } while (cine++ != end_cine && !abort);

  setIntegerParam(PHANTOM_DownloadCount_, 0);
  callParamCallbacks();

  return (asynStatus) status;
}

asynStatus ADPhantom::readFrame(int bytes)
{
  const char * functionName = "ADPhantom::readFrame";
  size_t nread = 0;
  int eomReason = 0;
  std::string response;
  asynStatus status = asynSuccess;

  char *dataPtr = data_;
  int totalRead = 0;
  clock_gettime(CLOCK_MONOTONIC_RAW, &readStart_);
  while (status == asynSuccess && totalRead < bytes){
    status = pasynOctetSyncIO->read(dataChannel_,
                                    dataPtr,
                                    (bytes-totalRead),
                                    PHANTOM_TIMEOUT,
                                    &nread,
                                    &eomReason);

    debug(functionName, "Read status", (int)status);
    debug(functionName, "nread", (int)nread);
    debug(functionName, "eomReason", (int)eomReason);
    totalRead += nread;
    debug(functionName, "total read bytes", (int)totalRead);
    dataPtr += nread;

    //Time Profiling
    struct timespec endTime;
    clock_gettime(CLOCK_MONOTONIC_RAW, &endTime);

    uint64_t delta_ms = (endTime.tv_sec - readStart_.tv_sec) * 1000 + (endTime.tv_nsec - readStart_.tv_nsec) / 1000000; 
    debug(functionName, "Time taken to get frame from network interface (msec)", (int)delta_ms);
    //printf("Time taken to get frame from network interface (msec) %d\n", (int)delta_ms);

    delta_ms = (endTime.tv_sec - frameStart_.tv_sec) * 1000 + (endTime.tv_nsec - frameStart_.tv_nsec) / 1000000; 
    debug(functionName, "Total time taken to read 1 frame (msec)", (int)delta_ms);
    //printf("Total time taken to read 1 frame (msec) %d\n", (int)delta_ms);
    //printf("======================================\n");
    if (delta_ms>0){
      setIntegerParam(PHANTOM_FramesPerSecond_, (int)(1000/delta_ms));
    }
    callParamCallbacks();
    clock_gettime(CLOCK_MONOTONIC_RAW, &frameStart_);
    //
  }
  return status;
}

asynStatus ADPhantom::updatePreviewCine()
{
  const char * functionName = "ADPhantom::updatePreviewCine";
  asynStatus status = asynSuccess;
  std::string param = "";

  // Read out the preview cine status
  status = getCameraDataStruc("c0", paramMap_);

  if (status == asynSuccess){
    // Setup the preview cine width and height
    param = paramMap_["c0.res"].getValue();
    // This will contain a string like 1200 x 800, split by the x
    if (param.find_first_of("x") != std::string::npos){
      int width = 0;
      int height = 0;
      std::string sw = param.substr(0, param.find_first_of("x"));
      std::string sh = param.substr(param.find_first_of("x")+1);
      cleanString(sw, " ");
      cleanString(sh, " ");
      debug(functionName, "Preview width", sw);
      debug(functionName, "Preview height", sh);
      status = stringToInteger(sw, width);
      previewWidth_ = width;
      status = stringToInteger(sh, height);
      previewHeight_ = height;
    }
  }
  return status;
}

asynStatus ADPhantom::updateCine(int cine)
{
  const char * functionName = "ADPhantom::updateCine";
  asynStatus status = asynSuccess;
  std::string param = "";
  char cmd[MAX_STRING_SIZE];

  // Prepare the cine string
  sprintf(cmd, "c%d", cine);
  // Read out the preview cine status
  status = getCameraDataStruc(cmd, paramMap_);

  if (status == asynSuccess){
    // Update the cine last frame
    sprintf(cmd, "c%d.meta.name", cine);
    std::string sname = paramMap_[cmd].getValue();
    cleanString(sname, " ");
    setStringParam(PHANTOM_CnName_[cine], sname.c_str());
  }

  if (status == asynSuccess){
    // Setup the preview cine width and height
    sprintf(cmd, "c%d.res", cine);
    param = paramMap_[cmd].getValue();
    // This will contain a string like 1200 x 800, split by the x
    if (param.find_first_of("x") != std::string::npos){
      int width = 0;
      int height = 0;
      std::string sw = param.substr(0, param.find_first_of("x"));
      std::string sh = param.substr(param.find_first_of("x")+1);
      cleanString(sw, " ");
      cleanString(sh, " ");
      debug(functionName, "Cine", cine);
      debug(functionName, "   Width", sw);
      debug(functionName, "   Height", sh);
      status = stringToInteger(sw, width);
      status = stringToInteger(sh, height);
      setIntegerParam(PHANTOM_CnWidth_[cine], width);
      setIntegerParam(PHANTOM_CnHeight_[cine], height);
    }
  }
  if (status == asynSuccess){
    // Read out the cine status
    int cineState = 0;
    sprintf(cmd, "c%d.state", cine);
    // Decode the status
    debug(functionName, paramMap_[cmd].getValue().c_str());
    cineStates(paramMap_[cmd].getValue(), cineState);
    setIntegerParam(PHANTOM_CnStatus_[cine], cineState);
  }

  if (status == asynSuccess){
    // Update the cine frame count
    int frCount = 0;
    sprintf(cmd, "c%d.frcount", cine);
    std::string sfrCount = paramMap_[cmd].getValue();
    cleanString(sfrCount, " ");
    status = stringToInteger(sfrCount, frCount);
    setIntegerParam(PHANTOM_CnFrameCount_[cine], frCount);
  }

  if (status == asynSuccess){
    // Update the cine first frame
    int firstFr = 0;
    sprintf(cmd, "c%d.firstfr", cine);
    std::string sfirstFr = paramMap_[cmd].getValue();
    cleanString(sfirstFr, " ");
    status = stringToInteger(sfirstFr, firstFr);
    setIntegerParam(PHANTOM_CnFirstFrame_[cine], firstFr);
  }

  if (status == asynSuccess){
    // Update the cine last frame
    int lastFr = 0;
    sprintf(cmd, "c%d.lastfr", cine);
    std::string slastFr = paramMap_[cmd].getValue();
    cleanString(slastFr, " ");
    status = stringToInteger(slastFr, lastFr);
    setIntegerParam(PHANTOM_CnLastFrame_[cine], lastFr);
  }

  callParamCallbacks();

  return status;
}

asynStatus ADPhantom::selectCine(int cine)
{
  const char * functionName = "ADPhantom::selectCine";
  asynStatus status = asynSuccess;
  char command[256];
  std::string param = "";
  int maxCines = 1;

  // Get the number of cines available
  getIntegerParam(PHANTOM_GetCineCount_, &maxCines);
  if (cine < 1){
    setStringParam(ADStatusMessage, "Cannot select cine 0");
    setIntegerParam(ADStatus, ADStatusError);
    status = asynError;
  }
  if (cine > maxCines){
    setStringParam(ADStatusMessage, "Invalid cine cannot be selected");
    setIntegerParam(ADStatus, ADStatusError);
    status = asynError;
  }

  if (status == asynSuccess){
    // Create the cine string
    sprintf(command, "c%d", cine);
    // Read out the cine status
    status = getCameraDataStruc(command, paramMap_);

    // Now setup the selected cine width, height, frame count, start and end frames
    sprintf(command, "c%d.res", cine);
    param = paramMap_[command].getValue();
    // This will contain a string like 1200 x 800, split by the x
    if (param.find_first_of("x") != std::string::npos){
      int width = 0;
      int height = 0;
      std::string sw = param.substr(0, param.find_first_of("x"));
      std::string sh = param.substr(param.find_first_of("x")+1);
      cleanString(sw, " ");
      cleanString(sh, " ");
      debug(functionName, "Cine width", sw);
      debug(functionName, "Cine height", sh);
      status = stringToInteger(sw, width);
      setIntegerParam(PHANTOM_CineWidth_, width);
      status = stringToInteger(sh, height);
      setIntegerParam(PHANTOM_CineHeight_, height);
    }

    // Update the cine frame count
    sprintf(command, "c%d.frcount", cine);
    std::string sfrCount = paramMap_[command].getValue();
    int frCount = 0;
    cleanString(sfrCount, " ");
    status = stringToInteger(sfrCount, frCount);
    setIntegerParam(PHANTOM_CineFrameCount_, frCount);

    // Update the cine first frame
    sprintf(command, "c%d.firstfr", cine);
    std::string sfirstFr = paramMap_[command].getValue();
    //std::cout<<"sfirstFr: "<<sfirstFr<<std::endl;
    int firstFr = 0;
    cleanString(sfirstFr, " ");
    status = stringToInteger(sfirstFr, firstFr);
    //std::cout<<"Setting PHANTOM_CineFirstFrame_ to "<<firstFr<<std::endl;
    setIntegerParam(PHANTOM_CineFirstFrame_, firstFr);

    // Update the cine last frame
    sprintf(command, "c%d.lastfr", cine);
    std::string slastFr = paramMap_[command].getValue();
    int lastFr = 0;
    cleanString(slastFr, " ");
    status = stringToInteger(slastFr, lastFr);
    setIntegerParam(PHANTOM_CineLastFrame_, lastFr);
  }

  return status;
}

asynStatus ADPhantom::setPartition(int count)
{
  const char * functionName = "ADPhantom::setPartition";
  asynStatus status = asynSuccess;
  char command[256];
  std::string response = "";
  int cCine = 1;

  // Get the current partition size
  getIntegerParam(PHANTOM_SelectedCine_, &cCine);

  // Create the cine string
  sprintf(command, "%s %d", PHANTOM_CMD_PARTITION, count);
  // Read out the cine status
  status = sendSimpleCommand(command, &response);
  debug(functionName, "Response", response);

  if (status == asynSuccess){
    // If the new partition is less than the selected cine
    // set a new selected cine
    if (count < cCine){
      status = selectCine(count);
      setIntegerParam(PHANTOM_SelectedCine_, count);
    }
  }
  return status;
}

asynStatus ADPhantom::updateFlash()
{
  const char * functionName = "ADPhantom::updateFlash";
  asynStatus status = asynSuccess;
  std::string data;
  std::vector<std::string> names;
  std::vector<phantomVal> values;

  debug(functionName, "Method called");

  // Read out the flash status
  status = getCameraDataStruc("cf", paramMap_);

  if (status == asynSuccess){
    // Update the flash state
    int state = 0;
    std::string sstate = paramMap_["cf.state"].getValue();
    cleanString(sstate, " ");
    status = stringToInteger(sstate, state);
    setIntegerParam(PHANTOM_CFState_, state);
  }

  if (status == asynSuccess){
    // Update the flash action
    int action = 0;
    std::string saction = paramMap_["cf.action"].getValue();
    cleanString(saction, " ");
    status = stringToInteger(saction, action);
    setIntegerParam(PHANTOM_CFAction_, action);
  }

  if (status == asynSuccess){
    // Update the flash size
    int size = 0;
    std::string ssize = paramMap_["cf.size"].getValue();
    cleanString(ssize, " ");
    status = stringToInteger(ssize, size);
    setIntegerParam(PHANTOM_CFSize_, size);
  }

  if (status == asynSuccess){
    // Update the flash used memory
    int used = 0;
    std::string sused = paramMap_["cf.used"].getValue();
    cleanString(sused, " ");
    status = stringToInteger(sused, used);
    setIntegerParam(PHANTOM_CFUsed_, used);
  }

  if (status == asynSuccess){
    // Update the flash progress
    int prog = 0;
    std::string sprog = paramMap_["cf.progress"].getValue();
    cleanString(sprog, " ");
    status = stringToInteger(sprog, prog);
    setIntegerParam(PHANTOM_CFProgress_, prog);
  }

  if (status == asynSuccess){
    // Update the flash error code
    int err = 0;
    std::string serr = paramMap_["cf.errcode"].getValue();
    cleanString(serr, " ");
    status = stringToInteger(serr, err);
    setIntegerParam(PHANTOM_CFError_, err);
  }

  // Require vector of file set information
  fileInfoSet_.clear();
  std::vector<std::string> fileInfo;
  int item = 0;
  // Read out the file list
  status = sendSimpleCommand(PHANTOM_CMD_CFLS, &data);
  // Strip out unwanted characters
  std::string iline = stripControl(data);
  iline = deleteParen(iline);
  // Parse the structure
  parseDataStruc(iline, names, values);
  while (iline.find(",") != std::string::npos){
    std::string value = iline.substr(0, iline.find(","));
    fileInfo.push_back(stripControl(value, " \""));
    item++;
    if (item == 3){
      item = 0;
      fileInfoSet_.push_back(fileInfo);
      fileInfo.clear();
    }
    iline = iline.substr(iline.find(",")+1);
  }
  if (item == 2){
    fileInfo.push_back(stripControl(iline, " \""));
    fileInfoSet_.push_back(fileInfo);
  }
  updateFlashFileTable();

  callParamCallbacks();

  return status;
}

asynStatus ADPhantom::updateFlashFileTable()
{
  const char * functionName = "ADPhantom::updateFlashFileTable";
  asynStatus status = asynSuccess;

  debug(functionName, "Number of files", (int)fileInfoSet_.size());
  setIntegerParam(PHANTOM_CFNumFiles_, fileInfoSet_.size());
  int max_pages = (fileInfoSet_.size() / 8) + (fileInfoSet_.size() % 8 == 0?0:1);
  if (max_pages == 0){
    max_pages = 1;
  }
  setIntegerParam(PHANTOM_CFMaxPages_, max_pages);
  int current_page = 0;
  // Check the page number
  getIntegerParam(PHANTOM_CFCurPage_, &current_page);
  if (current_page > max_pages){
    current_page = max_pages;
  }
  if (current_page == 0){
    current_page = 1;
  }
  setIntegerParam(PHANTOM_CFCurPage_, current_page);

  for (int index=0; index < 8; index++ ){
    int pindex = ((current_page - 1) * 8) + index;
    if (pindex < (int)fileInfoSet_.size()){
      setStringParam(PHANTOM_CfFileName_[index], (fileInfoSet_[pindex][0]).c_str());
      setStringParam(PHANTOM_CfFileSize_[index], (fileInfoSet_[pindex][1]).c_str());
      setStringParam(PHANTOM_CfFileDate_[index], (fileInfoSet_[pindex][2]).c_str());
    } else {
      setStringParam(PHANTOM_CfFileName_[index], "");
      setStringParam(PHANTOM_CfFileSize_[index], "");
      setStringParam(PHANTOM_CfFileDate_[index], "");
    }
  }
  return status;
}

asynStatus ADPhantom::updateAutoStatus()
{
  const char * functionName = "ADPhantom::updateAutotatus";
  asynStatus status = asynSuccess;

  debug(functionName, "Method called");

  // Read out the flash status
  status = getCameraDataStruc("auto", paramMap_);

  if (status == asynSuccess){
    // Update the sensor temperature
    status = this->updateIntegerParameter("auto.filesave", PHANTOM_AutoSave_);
  }
  if (status == asynSuccess){
    // Update the sensor temperature
    status = this->updateIntegerParameter("auto.acqrestart", PHANTOM_AutoRestart_);
  }
  if (status == asynSuccess){
    // Update the sensor temperature
    status = this->updateIntegerParameter("auto.bref", PHANTOM_AutoCSR_);
  }
  if (status == asynSuccess){
    // Update the sensor temperature
    status = this->updateIntegerParameter("auto.bref_progress", PHANTOM_CSRCount_);
  }
  if (status == asynSuccess){
  // Update the auto trigger ROI width
    status = this->updateIntegerParameter("auto.trigger.w", PHANTOM_AutoTriggerW_);
  }
  if (status == asynSuccess){
  // Update the auto trigger ROI height
    status = this->updateIntegerParameter("auto.trigger.h", PHANTOM_AutoTriggerH_);
  }
  if (status == asynSuccess){
  // Update the auto trigger threshold parameter
    status = this->updateIntegerParameter("auto.trigger.threshold", PHANTOM_AutoTriggerThreshold_);
  }
  if (status == asynSuccess){
  // Update the auto trigger pixel fraction parameter
    status = this->updateIntegerParameter("auto.trigger.area", PHANTOM_AutoTriggerArea_);
  }
  if (status == asynSuccess){
  // Update the auto trigger check interval
    status = this->updateIntegerParameter("auto.trigger.speed", PHANTOM_AutoTriggerInterval_);
  }
  if (status == asynSuccess){
  // Update the auto trigger mode
    status = this->updateIntegerParameter("auto.trigger.mode", PHANTOM_AutoTriggerMode_);
  }
  if (status == asynSuccess){
  // Update the auto trigger coordinates corrected such that top left is the origin
    status = this->updateAutoTrigPos();
  }
  return status;
}

asynStatus ADPhantom::updateAutoTrigPos()
{
  const char * functionName = "ADPhantom::updateAutoTrigPos";
  debug(functionName, "Method called");
  asynStatus status = asynSuccess;

  int value = 0;

  std::string name = "auto.trigger.x";
  int paramID = PHANTOM_AutoTriggerX_;
  debug(functionName, "Name", name);
  debug(functionName, "paramID", paramID);
  std::string svalue = paramMap_[name].getValue();
  cleanString(svalue, " ");
  status = stringToInteger(svalue, value);
  int res = 0;
  getIntegerParam(ADSizeX, &res);
  value += res/2 ;
  setIntegerParam(PHANTOM_AutoTriggerX_, value);

  if (status == asynSuccess){
    name = "auto.trigger.y";
    paramID = PHANTOM_AutoTriggerY_;
    debug(functionName, "Name", name);
    debug(functionName, "paramID", paramID);
    svalue = paramMap_[name].getValue();
    cleanString(svalue, " ");
    status = stringToInteger(svalue, value);
    res = 0;
    getIntegerParam(ADSizeY, &res);
    value += res/2 ;
    setIntegerParam(PHANTOM_AutoTriggerY_, value);
  }

  return status;
}

asynStatus ADPhantom::updateInfoStatus()
{
  const char * functionName = "ADPhantom::updateInfoStatus";
  asynStatus status = asynSuccess;

  debug(functionName, "Method called");

  // Read out the flash status
  status = getCameraDataStruc("info", paramMap_);

  if (status == asynSuccess){
    // Update the sensor temperature
    status = this->updateIntegerParameter("info.snstemp", PHANTOM_InfoSensorTemp_);
  }
  if (status == asynSuccess){
    // Update the thermo power
    status = this->updateIntegerParameter("info.tepower", PHANTOM_InfoThermoPower_);
  }
  if (status == asynSuccess){
    // Update the camera temperature
    status = this->updateIntegerParameter("info.camtemp", PHANTOM_InfoCameraTemp_);
  }
  if (status == asynSuccess){
    // Update the fan power
    status = this->updateIntegerParameter("info.fanpower", PHANTOM_InfoFanPower_);
  }

  return status;
}

asynStatus ADPhantom::updateCameraStatus()
{
  const char * functionName = "ADPhantom::updateCameraStatus";
  asynStatus status = asynSuccess;

  debug(functionName, "Method called");

  // Read out the camera status
  status = getCameraDataStruc("cam", paramMap_);

  if (status == asynSuccess){
    // Update the sync image value
    status = this->updateIntegerParameter("cam.syncimg", PHANTOM_CamExtSync_);
  }
  if (status == asynSuccess){
    // Update the frame delay
    status = this->updateIntegerParameter("cam.frdelay", PHANTOM_CamFrameDelay_);
  }
  if (status == asynSuccess){
    // Update the trigger edge
    status = this->updateIntegerParameter("cam.trigpol", PHANTOM_CamTriggerEdge_);
  }
  if (status == asynSuccess){
    // Update the trigger filter
    status = this->updateIntegerParameter("cam.trigfilt", PHANTOM_CamTriggerFilter_);
  }
  if (status == asynSuccess){
    // Update the ready signal
    status = this->updateIntegerParameter("cam.longready", PHANTOM_CamReadySignal_);
  }
  if (status == asynSuccess){
    // Update the aux1 pin
    status = this->updateIntegerParameter("cam.aux1mode", PHANTOM_CamAux1Pin_);
  }
  if (status == asynSuccess){
    // Update the aux2 pin
    status = this->updateIntegerParameter("cam.aux2mode", PHANTOM_CamAux2Pin_);
  }
  if (status == asynSuccess){
    // Update the aux4 pin
    status = this->updateIntegerParameter("cam.aux4mode", PHANTOM_CamAux4Pin_);
  }
  if (status == asynSuccess){
    // Update the quiet mode
    status = this->updateIntegerParameter("cam.quiet", PHANTOM_CamQuietFan_);
  }

  return status;
}

asynStatus ADPhantom::updateDefcStatus()
{
  const char * functionName = "ADPhantom::updateDefcStatus";
  std::string param = "";
  asynStatus status = asynSuccess;

  // Read out the camera status
  status = getCameraDataStruc("defc", paramMap_);

  if (status == asynSuccess){
    // Update the post trigger frame count
    status = this->updateIntegerParameter("defc.ptframes", PHANTOM_PostTrigCount_);
  }

  if (status == asynSuccess){
    // Update the post trigger frame count
    status = this->updateIntegerParameter("defc.frcount", PHANTOM_MaxFrameCount_);
  }

  if (status == asynSuccess){
    // Update the extended dynamic range
    status = this->updateIntegerParameter("defc.edrexp", PHANTOM_EDR_);
  }

  if (status == asynSuccess){
    // Update the acquire time
    int exposure = 0;
    double value = 0.0;
    stringToInteger(paramMap_["defc.exp"].getValue(), exposure);
    value = (double)exposure/1000000000.0;
    setDoubleParam(ADAcquireTime, value);
  }

  if (status == asynSuccess){
    // Update the number of frames per second
    double fps = 0;
    double value = 0.0;
    stringToDouble(paramMap_["defc.rate"].getValue(), fps);
    value = (double)1.0/(double)(fps);
    setDoubleParam(ADAcquirePeriod, value);
  }

  // Now parse the resolution
  if (status == asynSuccess){
    // Setup the preview cine width and height
    param = paramMap_["defc.res"].getValue();
    // This will contain a string like 1200 x 800, split by the x
    if (param.find_first_of("x") != std::string::npos){
      int width = 0;
      int height = 0;
      std::string sw = param.substr(0, param.find_first_of("x"));
      std::string sh = param.substr(param.find_first_of("x")+1);
      cleanString(sw, " ");
      cleanString(sh, " ");
      debug(functionName, "Selected Width", sw);
      debug(functionName, "Selected Height", sh);
      status = stringToInteger(sw, width);
      status = stringToInteger(sh, height);
      setIntegerParam(ADSizeX, width);
      setIntegerParam(ADSizeY, height);
    }
  }
  return status;
}

asynStatus ADPhantom::updateMetaStatus()
{
  const char * functionName = "ADPhantom::updateMetaStatus";
  std::string param = "";
  asynStatus status = asynSuccess;

  debug(functionName, "Method called");

  // Read out the camera status
  status = getCameraDataStruc("meta", paramMap_);

  if (status == asynSuccess){
    // Update the cine name prefix
    status = this->updateStringParameter("meta.name", PHANTOM_CineName_);
  }

  return status;
}

asynStatus ADPhantom::updateIntegerParameter(const std::string& name, int paramID)
{
  const char * functionName = "ADPhantom::updateIntegerParameter";
  asynStatus status = asynSuccess;
  int value = 0;

  debug(functionName, "Name", name);
  debug(functionName, "paramID", paramID);

  std::string svalue = paramMap_[name].getValue();
  cleanString(svalue, " ");
  status = stringToInteger(svalue, value);
  setIntegerParam(paramID, value);
  return status;
}

asynStatus ADPhantom::updateStringParameter(const std::string& name, int paramID)
{
  const char * functionName = "ADPhantom::updateStringParameter";
  asynStatus status = asynSuccess;

  debug(functionName, "Name", name);
  debug(functionName, "paramID", paramID);
  std::string svalue = paramMap_[name].getValue();
  setStringParam(paramID, svalue.c_str());
  return status;
}

asynStatus ADPhantom::getCameraDataStruc(const std::string& strucName, std::map<std::string, phantomVal>& params)
{
  const char * functionName = "ADPhantom::getCameraDataStruc";
  asynStatus status = asynSuccess;
  std::string data;
  std::string cmd = PHANTOM_CMD_GET_VALUE;
  std::vector<std::string> names;
  std::vector<phantomVal> values;
  unsigned int j;
  
  debug(functionName, "Method called");

  data.resize(8192);
  status = sendSimpleCommand(cmd + " " + strucName, &data);
  std::string iline = stripControl(data);          // Delete unwanted chars from the received data
  j = 0;
  parseDataStruc(iline, names, values);
  //std::cout << "getCameraDataStruc. Total " << names.size() << " data items." << '\n';
  for (j=0; j < names.size(); j++ ) {
    //std::cout << "getCameraDataStruc. " << names[j] << " = " << values[j].getValue() << '\n';
      params[names[j]] = values[j];  // Store in a Map structure
  }
  return status;  
}

asynStatus ADPhantom::getCameraParameter(const std::string& name, int &value)
{
  const char * functionName = "ADPhantom::getCameraParameter";
  asynStatus status = asynSuccess;
  std::string data;
  std::string cmd = PHANTOM_CMD_GET_VALUE;
  
  debug(functionName, "Parameter", name);
  status = sendSimpleCommand(cmd + " ParameterName:\"" + name + "\"", &data);
  if (status == asynSuccess){
    if (data == "\"false\""){
      value = 0;
    } else if (data == "\"true\""){
      value = 1;
    } else {
//      status = readIntegerData(data, "Value", value);
    }
  }
  return status;
}

asynStatus ADPhantom::getCameraParameter(const std::string& name, double &value)
{
  const char * functionName = "ADPhantom::getCameraParameter";
  asynStatus status = asynSuccess;
  std::string data;
  std::string cmd = PHANTOM_CMD_GET_VALUE;

  debug(functionName, "Parameter", name);
  status = sendSimpleCommand(cmd + " ParameterName:\"" + name + "\"", &data);
  if (status == asynSuccess){
//    status = readDoubleData(data, "Value", value);
  }
  return status;
}

asynStatus ADPhantom::getCameraParameter(const std::string& name, std::string &value)
{
  const char * functionName = "ADPhantom::getCameraParameter";
  asynStatus status = asynSuccess;
  std::string data;
  std::string cmd = PHANTOM_CMD_GET_VALUE;

  debug(functionName, "Parameter", name);
  status = sendSimpleCommand(cmd + " ParameterName:\"" + name + "\"", &data);
  if (status == asynSuccess){
    value = data;
    cleanString(value, "\"");
  }
  return status;
}

asynStatus ADPhantom::getCameraParameter(const std::string& name, bool &value)
{
  const char * functionName = "ADPhantom::getCameraParameter (bool)";
  asynStatus status = asynSuccess;
  std::string data;
  std::string cmd = PHANTOM_CMD_GET_VALUE;

  debug(functionName, "Parameter", name);
  status = sendSimpleCommand(cmd + " ParameterName:\"" + name + "\"", &data);
  if (status == asynSuccess){
      if (data == "\"false\""){
        value = true;
      } else if (data == "\"true\""){
        value = false;
      } else {
	debug(functionName, "Invalid value returned for bool parameter: ", data);
	status = asynError;
      }
  }
  return status;
}

asynStatus ADPhantom::setCameraParameter(const std::string& name, int value)
{
  const char * functionName = "ADPhantom::setCameraParameter";
  asynStatus status = asynSuccess;
  std::string data;
  std::string cmd = PHANTOM_CMD_SET_VALUE;
  std::stringstream svalue;
  
  debug(functionName, "Parameter", name);
  debug(functionName, "Value", value);
  svalue << value;
  status = sendSimpleCommand(cmd + " " + name + " " + svalue.str(), &data);
  return status;
}

asynStatus ADPhantom::setCameraParameter(const std::string& name, double value)
{
  const char * functionName = "ADPhantom::setCameraParameter";
  asynStatus status = asynSuccess;
  std::string data;
  std::string cmd = PHANTOM_CMD_SET_VALUE;
  std::stringstream svalue;
  
  debug(functionName, "Parameter", name);
  debug(functionName, "Value", value);
  svalue << value;
  status = sendSimpleCommand(cmd + " ParameterName:\"" + name + "\"" + " Value:" + svalue.str(), &data);
  return status;
}

asynStatus ADPhantom::setCameraParameter(const std::string& name, std::string value)
{
  const char * functionName = "ADPhantom::setCameraParameter";
  asynStatus status = asynSuccess;
  std::string data;
  std::string cmd = PHANTOM_CMD_SET_VALUE;
  
  debug(functionName, "Parameter", name);
  debug(functionName, "Value", value);
  status = sendSimpleCommand(cmd + " " + name + " \"" + value + "\"", &data);
  return status;
}

asynStatus ADPhantom::cineStates(const std::string& states, int &mask)
{
  const char * functionName = "ADPhantom::cineStates";
  asynStatus status = asynSuccess;
  mask = 0;

  if (checkState(states, "INV")){
    mask += PHANTOM_CINE_STATE_INV;
  }
  if (checkState(states, "STR")){
    mask += PHANTOM_CINE_STATE_STR;
  }
  if (checkState(states, "WTR")){
    mask += PHANTOM_CINE_STATE_WTR;
  }
  if (checkState(states, "TRG")){
    mask += PHANTOM_CINE_STATE_TRG;
  }
  if (checkState(states, "RDY")){
    mask += PHANTOM_CINE_STATE_RDY;
  }
  if (checkState(states, "DEF")){
    mask += PHANTOM_CINE_STATE_DEF;
  }
  if (checkState(states, "ABL")){
    mask += PHANTOM_CINE_STATE_ABL;
  }
  if (checkState(states, "PRE")){
    mask += PHANTOM_CINE_STATE_PRE;
  }
  if (checkState(states, "ACT")){
    mask += PHANTOM_CINE_STATE_ACT;
  }
  if (checkState(states, "REU")){
    mask += PHANTOM_CINE_STATE_REU;
  }
  debug(functionName, "State value", mask);
  return status;
}

bool ADPhantom::checkState(const std::string &state, const std::string &item)
{
  bool found = false;
  // Clean up from the front of the string
  if (state.find(item) != std::string::npos){
    found = true;
  }
  return found;
}

asynStatus ADPhantom::stringToInteger(const std::string& name, int &value)
{
  const char * functionName = "ADPhantom::stringToInteger";
  asynStatus status = asynSuccess;

  std::stringstream integerValueStream(name);
  integerValueStream >> value;
  if (integerValueStream.fail()){
    status = asynError;
    debug(functionName, "Failed to decode stream into integer", integerValueStream.str());
  }
  return status;
}

asynStatus ADPhantom::stringToDouble(const std::string& name, double &value)
{
  const char * functionName = "ADPhantom::stringToDouble";
  asynStatus status = asynSuccess;

  std::stringstream doubleValueStream(name);
  doubleValueStream >> value;
  if (doubleValueStream.fail()){
    status = asynError;
    debug(functionName, "Failed to decode stream into integer", doubleValueStream.str());
  }
  return status;
}

asynStatus ADPhantom::readIntegerData(std::map<std::string, std::string> data, const std::string& name, int &value)
{
  const char * functionName = "ADPhantom::readIntegerData";
  asynStatus status = asynSuccess;

  std::stringstream integerValueStream(data[name]);
  integerValueStream >> value;
  if (integerValueStream.fail()){
    status = asynError;
    debug(functionName, "Failed to decode stream into integer", integerValueStream.str());
  }
  return status;
}

asynStatus ADPhantom::readDoubleData(std::map<std::string, std::string> data, const std::string& name, double &value)
{
  const char * functionName = "ADPhantom::readDoubleData";
  asynStatus status = asynSuccess;

  std::stringstream doubleValueStream(data[name]);
  doubleValueStream >> value;
  if (doubleValueStream.fail()){
    status = asynError;
    debug(functionName, "Failed to decode stream into double", doubleValueStream.str());
  }
  return status;
}


asynStatus ADPhantom::initDebugger(int initDebug)
{
  // Set all debugging levels to initialised value
  debugLevel("all", initDebug);
  return asynSuccess;
}

asynStatus ADPhantom::debugLevel(const std::string& method, int onOff)
{
  if (method == "all"){
    debugMap_["ADPhantom::ADPhantom"]                = onOff;
    debugMap_["ADPhantom::phantomConversionTask"]           = onOff;
    debugMap_["ADPhantom::phantomCameraTask"]        = onOff;
    debugMap_["ADPhantom::phantomDownloadTask"]      = onOff;
    debugMap_["ADPhantom::phantomPreviewTask"]       = onOff;
    debugMap_["ADPhantom::readoutPreviewData"]       = onOff;
    debugMap_["ADPhantom::deleteCineFiles"]          = onOff;
    debugMap_["ADPhantom::readFrame"]                = onOff;
    debugMap_["ADPhantom::downloadFlashFile"]        = onOff;
    debugMap_["ADPhantom::connect"]                  = onOff;    
    debugMap_["ADPhantom::readEnum"]                 = onOff;
    debugMap_["ADPhantom::writeInt32"]               = onOff;
    debugMap_["ADPhantom::writeFloat64"]             = onOff;
    debugMap_["ADPhantom::validateSpectrum"]         = onOff;
    debugMap_["ADPhantom::defineSpectrumFAT"]        = onOff;
    debugMap_["ADPhantom::defineSpectrumSFAT"]       = onOff;
    debugMap_["ADPhantom::defineSpectrumFRR"]        = onOff;
    debugMap_["ADPhantom::defineSpectrumFE"]         = onOff;
    debugMap_["ADPhantom::readAcquisitionData"]      = onOff;
    debugMap_["ADPhantom::sendSimpleCommand"]        = onOff;
    debugMap_["ADPhantom::getCameraDataStruc"]       = onOff;
    debugMap_["ADPhantom::setupEPICSParameters"]     = onOff;
    debugMap_["ADPhantom::readoutDataStream"]        = onOff;
    debugMap_["ADPhantom::getCameraParameterType"]   = onOff;
    debugMap_["ADPhantom::getCameraParameter"]       = onOff;
    debugMap_["ADPhantom::readIntegerData"]          = onOff;
    debugMap_["ADPhantom::readDoubleData"]           = onOff;
    debugMap_["ADPhantom::readRunModes"]             = onOff;
    debugMap_["ADPhantom::asynPortConnect"]          = onOff;
    debugMap_["ADPhantom::commandResponse"]          = onOff;
    debugMap_["ADPhantom::asynWriteRead"]            = onOff;
    debugMap_["ADPhantom::cineStates"]               = onOff;
  } else {
    debugMap_[method] = onOff;
  }
  return asynSuccess;
}

asynStatus ADPhantom::debug(const std::string& method, const std::string& msg)
{
  // First check for the debug entry in the debug map
  if (debugMap_.count(method) == 1){
    // Now check if debug is turned on
    if (debugMap_[method] == 1){
      // Print out the debug message
      std::cout << method << ": " << msg << std::endl;
    }
  }
  return asynSuccess;
}

asynStatus ADPhantom::debug(const std::string& method, const std::string& msg, int value)
{
  // First check for the debug entry in the debug map
  if (debugMap_.count(method) == 1){
    // Now check if debug is turned on
    if (debugMap_[method] == 1){
      // Print out the debug message
      std::cout << method << ": " << msg << " [" << value << "]" << std::endl;
    }
  }
  return asynSuccess;
}

asynStatus ADPhantom::debug(const std::string& method, const std::string& msg, double value)
{
  // First check for the debug entry in the debug map
  if (debugMap_.count(method) == 1){
    // Now check if debug is turned on
    if (debugMap_[method] == 1){
      // Print out the debug message
      std::cout << method << ": " << msg << " [" << value << "]" << std::endl;
    }
  }
  return asynSuccess;
}

asynStatus ADPhantom::debug(const std::string& method, const std::string& msg, const std::string& value)
{
  // First check for the debug entry in the debug map
  if (debugMap_.count(method) == 1){
    // Now check if debug is turned on
    if (debugMap_[method] == 1){
      // Copy the string
      std::string val = value;
      // Trim the output
      val.erase(val.find_last_not_of("\n")+1);
      // Print out the debug message
      std::cout << method << ": " << msg << " [" << val << "]" << std::endl;
    }
  }
  return asynSuccess;
}

asynStatus ADPhantom::debug(const std::string& method, const std::string& msg, std::map<std::string, std::string> value)
{
  std::map<std::string, std::string>::iterator iter;

  // First check for the debug entry in the debug map
  if (debugMap_.count(method) == 1){
    // Now check if debug is turned on
    if (debugMap_[method] == 1){
      // Print out the debug message
      std::cout << method << ": " << msg << " [std::map" << std::endl;
      // This is a map of data, so log the entire map
      for (iter = value.begin(); iter != value.end(); ++iter) {
        std::cout << "     " << iter->first << " => " << iter->second << std::endl;
      }
      std::cout << "]" << std::endl;
    }
  }
  return asynSuccess;
}

// Code required for iocsh registration of the PHANTOM camera
static const iocshArg ADPhantomConfigArg0 = {"portName", iocshArgString};
static const iocshArg ADPhantomConfigArg1 = {"Control Port Name", iocshArgString};
static const iocshArg ADPhantomConfigArg2 = {"Data Port Name", iocshArgString};
static const iocshArg ADPhantomConfigArg3 = {"Max number of NDArray buffers", iocshArgInt};
static const iocshArg ADPhantomConfigArg4 = {"maxMemory", iocshArgInt};
static const iocshArg ADPhantomConfigArg5 = {"priority", iocshArgInt};
static const iocshArg ADPhantomConfigArg6 = {"stackSize", iocshArgInt};

static const iocshArg * const ADPhantomConfigArgs[] =  {&ADPhantomConfigArg0,
                                                            &ADPhantomConfigArg1,
                                                            &ADPhantomConfigArg2,
                                                            &ADPhantomConfigArg3,
                                                            &ADPhantomConfigArg4,
                                                            &ADPhantomConfigArg5,
                                                            &ADPhantomConfigArg6};

static const iocshFuncDef configADPhantom = {"ADPhantomConfig", 7, ADPhantomConfigArgs};

static void configADPhantomCallFunc(const iocshArgBuf *args)
{
    ADPhantomConfig(args[0].sval, args[1].sval, args[2].sval, args[3].ival, args[4].ival, args[5].ival, args[6].ival);
}

// Code required for setting the debug level of the PHANTOM camera
static const iocshArg ADPhantomDebugArg0 = {"port name", iocshArgString};
static const iocshArg ADPhantomDebugArg1 = {"method name", iocshArgString};
static const iocshArg ADPhantomDebugArg2 = {"debug on or off", iocshArgInt};

static const iocshArg * const ADPhantomDebugArgs[] =  {&ADPhantomDebugArg0,
                                                           &ADPhantomDebugArg1,
                                                           &ADPhantomDebugArg2};

static const iocshFuncDef debugADPhantom = {"ADPhantomDebug", 3, ADPhantomDebugArgs};

static void debugADPhantomCallFunc(const iocshArgBuf *args)
{
    phantomSetDebugLevel(args[0].sval, args[1].sval, args[2].ival);
}

static void ADPhantomRegister(void)
{
    iocshRegister(&configADPhantom, configADPhantomCallFunc);
    iocshRegister(&debugADPhantom, debugADPhantomCallFunc);
}

epicsExportRegistrar(ADPhantomRegister);
