//==============================================================================
//File name:    "lpc21isp.cpp"
//Purpose:      Source File, CPage2 property page
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "malloc.h"
#include "lpc21isp.h"
#include "lpcprog.h"
#include "datatx.h"
#include "cmd.h"
#include "Page6.h"

extern CPage6 *pPG6;

CDataTX *pBLE = NULL;
void SetTXpointer(void *p_BT)
{
  pBLE = (CDataTX *)p_BT;
}

int debug_level = 2;

static const unsigned int SectorTable_210x[] =
{
    8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192,
    8192, 8192, 8192, 8192, 8192, 8192, 8192
};

static const unsigned int SectorTable_2103[] =
{
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096
};

static const unsigned int SectorTable_2109[] =
{
    8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192
};

static const unsigned int SectorTable_211x[] =
{
    8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192,
    8192, 8192, 8192, 8192, 8192, 8192, 8192,
};

static const unsigned int SectorTable_212x[] =
{
    8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192,
    65536, 65536, 8192, 8192, 8192, 8192, 8192, 8192, 8192
};

// Used for devices with 500K (LPC2138 and LPC2148) and
// for devices with 504K (1 extra 4k block at the end)
static const unsigned int SectorTable_213x[] =
{
     4096,  4096,  4096,  4096,  4096,  4096,  4096,  4096,
    32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768,
    32768, 32768, 32768, 32768, 32768, 32768,  4096,  4096,
     4096,  4096,  4096,  4096
};

// Used for LPC11xx devices
static const unsigned int SectorTable_11xx[] =
{
     4096,  4096,  4096,  4096,  4096,  4096,  4096,  4096,
     4096,  4096,  4096,  4096,  4096,  4096,  4096,  4096,
     4096,  4096,  4096,  4096,  4096,  4096,  4096,  4096,
     4096,  4096,  4096,  4096,  4096,  4096,  4096,  4096
};

// Used for LPC17xx devices
static const unsigned int SectorTable_17xx[] =
{
     4096,  4096,  4096,  4096,  4096,  4096,  4096,  4096,
     4096,  4096,  4096,  4096,  4096,  4096,  4096,  4096,
    32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768,
    32768, 32768, 32768, 32768, 32768, 32768
};

// Used for LPC18xx devices
static const unsigned int SectorTable_18xx[] =
{
     8192,  8192,  8192,  8192,  8192,  8192,  8192,  8192,
    65536, 65536, 65536, 65536, 65536, 65536, 65536
};

// Used for LPC43xx devices
static const unsigned int SectorTable_43xx[] =
{
     8192,  8192,  8192,  8192,  8192,  8192,  8192,  8192,
    65536, 65536, 65536, 65536, 65536, 65536, 65536
};

// Used for LPC8xx devices
static const unsigned int SectorTable_8xx[] =
{
     1024,  1024,  1024,  1024,  1024,  1024,  1024,  1024,
     1024,  1024,  1024,  1024,  1024,  1024,  1024,  1024
};

static int unsigned SectorTable_RAM[]  = { 65000 };

static LPC_DEVICE_TYPE LPCtypes[] =
{
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, CHIP_VARIANT_NONE },  /* unknown */

   // id,        id2,  use id2, name of product,          flash size, ram size, total number of sector, max copy size, sector table, chip variant

   { 0x00008100, 0x00000000, 0, "810M021FN8",                      4,   1,  4,  256, SectorTable_8xx,  CHIP_VARIANT_LPC8XX  },
   { 0x00008110, 0x00000000, 0, "811M001FDH16",                    8,   2,  8, 1024, SectorTable_8xx,  CHIP_VARIANT_LPC8XX  },
   { 0x00008120, 0x00000000, 0, "812M101FDH16",                   16,   4, 16, 1024, SectorTable_8xx,  CHIP_VARIANT_LPC8XX  },
   { 0x00008121, 0x00000000, 0, "812M101FD20",                    16,   4, 16, 1024, SectorTable_8xx,  CHIP_VARIANT_LPC8XX  },
   { 0x00008122, 0x00000000, 0, "812M101FDH20",                   16,   4, 16, 1024, SectorTable_8xx,  CHIP_VARIANT_LPC8XX  },

   { 0x2500102B, 0x00000000, 0, "1102",                           32,   8,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },

   { 0x0A07102B, 0x00000000, 0, "1110.../002",                     4,   1,  1, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x1A07102B, 0x00000000, 0, "1110.../002",                     4,   1,  1, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x0A16D02B, 0x00000000, 0, "1111.../002",                     8,   2,  2, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x1A16D02B, 0x00000000, 0, "1111.../002",                     8,   2,  2, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x041E502B, 0x00000000, 0, "1111.../101",                     8,   2,  2, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x2516D02B, 0x00000000, 0, "1111.../102",                     8,   2,  2, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x00010013, 0x00000000, 0, "1111.../103",                     8,   2,  2, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x0416502B, 0x00000000, 0, "1111.../201",                     8,   4,  2, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x2516902B, 0x00000000, 0, "1111.../202",                     8,   4,  2, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x00010012, 0x00000000, 0, "1111.../203",                     8,   4,  2, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x042D502B, 0x00000000, 0, "1112.../101",                    16,   2,  4, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x2524D02B, 0x00000000, 0, "1112.../102",                    16,   2,  4, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x0A24902B, 0x00000000, 0, "1112.../102",                    16,   4,  4, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x1A24902B, 0x00000000, 0, "1112.../102",                    16,   4,  4, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x00020023, 0x00000000, 0, "1112.../103",                    16,   2,  4, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x0425502B, 0x00000000, 0, "1112.../201",                    16,   4,  4, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x2524902B, 0x00000000, 0, "1112.../202",                    16,   4,  4, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x00020022, 0x00000000, 0, "1112.../203",                    16,   4,  4, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x0434502B, 0x00000000, 0, "1113.../201",                    24,   4,  6, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x2532902B, 0x00000000, 0, "1113.../202",                    24,   4,  6, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x00030032, 0x00000000, 0, "1113.../203",                    24,   4,  6, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x0434102B, 0x00000000, 0, "1113.../301",                    24,   8,  6, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x2532102B, 0x00000000, 0, "1113.../302",                    24,   8,  6, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x00030030, 0x00000000, 0, "1113.../303",                    24,   8,  6, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x0A40902B, 0x00000000, 0, "1114.../102",                    32,   4,  8, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x1A40902B, 0x00000000, 0, "1114.../102",                    32,   4,  8, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x0444502B, 0x00000000, 0, "1114.../201",                    32,   4,  8, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x2540902B, 0x00000000, 0, "1114.../202",                    32,   4,  8, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x00040042, 0x00000000, 0, "1114.../203",                    32,   8,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x0444102B, 0x00000000, 0, "1114.../301",                    32,   8,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x2540102B, 0x00000000, 0, "1114.../302",                    32,   8,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x00040040, 0x00000000, 0, "1114.../303",                    32,   8,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x00040060, 0x00000000, 0, "1114.../323",                    32,   8, 12, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x00040070, 0x00000000, 0, "1114.../333",                    32,   8, 14, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x00050080, 0x00000000, 0, "1115.../303",                    64,   8, 16, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },

   { 0x1421102B, 0x00000000, 0, "11C12.../301",                   16,   8,  4, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x1440102B, 0x00000000, 0, "11C14.../301",                   32,   8,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x1431102B, 0x00000000, 0, "11C22.../301",                   16,   8,  4, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
   { 0x1430102B, 0x00000000, 0, "11C24.../301",                   32,   8,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },

   { 0x293E902B, 0x00000000, 0, "11E11FHN33/101",                  8,   4,  2, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10518 Rev. 3 -- 25 Nov 2013 */
   { 0x2954502B, 0x00000000, 0, "11E12FBD48/201",                 16,   6,  4, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10518 Rev. 3 -- 25 Nov 2013 */
   { 0x296A102B, 0x00000000, 0, "11E13FBD48/301",                 24,   8,  6, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10518 Rev. 3 -- 25 Nov 2013 */
   { 0x2980102B, 0x00000000, 0, "11E14(FHN33,FBD48,FBD64)/401",   32,  10,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10518 Rev. 3 -- 25 Nov 2013 */
   { 0x00009C41, 0x00000000, 0, "11E36(FBD64,FHN33)/501",         96,  12, 24, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10518 Rev. 3 -- 25 Nov 2013 */
   { 0x00007C45, 0x00000000, 0, "11E37HFBD64/401",               128,  10, 32, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10518 Rev. 3 -- 25 Nov 2013 */
   { 0x00007C41, 0x00000000, 0, "11E37(FBD48,FBD64)/501",        128,  12, 32, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10518 Rev. 3 -- 25 Nov 2013 */

   { 0x095C802B, 0x00000000, 0, "11U12(FHN33,FBD48)/201",         16,   6,  4, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
   { 0x295C802B, 0x00000000, 0, "11U12(FHN33,FBD48)/201",         16,   6,  4, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
   { 0x097A802B, 0x00000000, 0, "11U13FBD48/201",                 24,   6,  6, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
   { 0x297A802B, 0x00000000, 0, "11U13FBD48/201",                 24,   6,  6, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
   { 0x0998802B, 0x00000000, 0, "11U14FHN33/201",                 32,   6,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
   { 0x2998802B, 0x00000000, 0, "11U14(FHN,FHI)33/201",           32,   6,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
   { 0x0998802B, 0x00000000, 0, "11U14(FBD,FET)48/201",           32,   6,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
   { 0x2998802B, 0x00000000, 0, "11U14(FBD,FET)48/201",           32,   6,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
   { 0x2972402B, 0x00000000, 0, "11U23FBD48/301",                 24,   8,  6, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
   { 0x2988402B, 0x00000000, 0, "11U24(FHI33,FBD48,FET48)/301",   32,   8,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
   { 0x2980002B, 0x00000000, 0, "11U24(FHN33,FBD48,FBD64)/401",   32,  10,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
   { 0x0003D440, 0x00000000, 0, "11U34(FHN33,FBD48)/311",         40,   8, 10, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
   { 0x0001CC40, 0x00000000, 0, "11U34(FHN33,FBD48)/421",         48,  10, 12, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
   { 0x0001BC40, 0x00000000, 0, "11U35(FHN33,FBD48,FBD64)/401",   64,  10, 16, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
   { 0x0000BC40, 0x00000000, 0, "11U35(FHI33,FET48)/501",         64,  12, 16, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
   { 0x00019C40, 0x00000000, 0, "11U36(FBD48,FBD64)/401",         96,  10, 24, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
   { 0x00017C40, 0x00000000, 0, "11U37FBD48/401",                128,  10, 32, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
   { 0x00007C44, 0x00000000, 0, "11U37HFBD64/401",               128,  10, 32, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
   { 0x00007C40, 0x00000000, 0, "11U37FBD64/501",                128,  12, 32, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */

   { 0x3640C02B, 0x00000000, 0, "1224.../101",                    32,   8,  4, 2048, SectorTable_17xx, CHIP_VARIANT_LPC11XX },
   { 0x3642C02B, 0x00000000, 0, "1224.../121",                    48,  12, 32, 4096, SectorTable_17xx, CHIP_VARIANT_LPC11XX },
   { 0x3650002B, 0x00000000, 0, "1225.../301",                    64,  16, 32, 4096, SectorTable_17xx, CHIP_VARIANT_LPC11XX },
   { 0x3652002B, 0x00000000, 0, "1225.../321",                    80,  20, 32, 4096, SectorTable_17xx, CHIP_VARIANT_LPC11XX },
   { 0x3660002B, 0x00000000, 0, "1226",                           96,  24, 32, 4096, SectorTable_17xx, CHIP_VARIANT_LPC11XX },
   { 0x3670002B, 0x00000000, 0, "1227",                          128,  32, 32, 4096, SectorTable_17xx, CHIP_VARIANT_LPC11XX },

   { 0x2C42502B, 0x00000000, 0, "1311",                            8,   4,  2, 1024, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
   { 0x1816902B, 0x00000000, 0, "1311/01",                         8,   4,  2, 1024, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
   { 0x2C40102B, 0x00000000, 0, "1313",                           32,   8,  8, 4096, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
   { 0x1830102B, 0x00000000, 0, "1313/01",                        32,   8,  8, 4096, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
   { 0x3A010523, 0x00000000, 0, "1315",                           32,   8,  8, 4096, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
   { 0x1A018524, 0x00000000, 0, "1316",                           48,   8, 12, 4096, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
   { 0x1A020525, 0x00000000, 0, "1317",                           64,   8, 16, 4096, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
   { 0x3D01402B, 0x00000000, 0, "1342",                           16,   4,  4, 1024, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
   { 0x3D00002B, 0x00000000, 0, "1343",                           32,   8,  8, 4096, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
   { 0x28010541, 0x00000000, 0, "1345",                           32,   8,  8, 4096, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
   { 0x08018542, 0x00000000, 0, "1346",                           48,   8, 12, 4096, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
   { 0x08020543, 0x00000000, 0, "1347",                           64,   8, 16, 4096, SectorTable_17xx, CHIP_VARIANT_LPC13XX },

   { 0x25001118, 0x00000000, 0, "1751",                           32,   8,  8, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x25001121, 0x00000000, 0, "1752",                           64,  16, 16, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x25011722, 0x00000000, 0, "1754",                          128,  32, 18, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x25011723, 0x00000000, 0, "1756",                          256,  32, 22, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x25013F37, 0x00000000, 0, "1758",                          512,  64, 30, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x25113737, 0x00000000, 0, "1759",                          512,  64, 30, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x26012033, 0x00000000, 0, "1763",                          256,  64, 22, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x26011922, 0x00000000, 0, "1764",                          128,  32, 18, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x26013733, 0x00000000, 0, "1765",                          256,  64, 22, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x26013F33, 0x00000000, 0, "1766",                          256,  64, 22, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x26012837, 0x00000000, 0, "1767",                          512,  64, 30, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x26013F37, 0x00000000, 0, "1768",                          512,  64, 30, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x26113F37, 0x00000000, 0, "1769",                          512,  64, 30, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },

   { 0x27011132, 0x00000000, 0, "1774",                          128,  40, 18, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x27191F43, 0x00000000, 0, "1776",                          256,  80, 22, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x27193747, 0x00000000, 0, "1777",                          512,  96, 30, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x27193F47, 0x00000000, 0, "1778",                          512,  96, 30, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x281D1743, 0x00000000, 0, "1785",                          256,  80, 22, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x281D1F43, 0x00000000, 0, "1786",                          256,  80, 22, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x281D3747, 0x00000000, 0, "1787",                          512,  96, 30, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
   { 0x281D3F47, 0x00000000, 0, "1788",                          512,  96, 30, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },

   // LPC18xx
   { 0xF00B1B3F, 0x00000000, 1, "1810",                            0,  32,  0, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX }, // Flashless
   { 0xF001D830, 0x00000000, 1, "1812",                          512,  32, 15, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
   { 0xF001D830, 0x00000000, 1, "1813",                          512,  32, 11, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
   { 0xF001D830, 0x00000000, 1, "1815",                          768,  32, 13, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
   { 0xF001D830, 0x00000000, 1, "1817",                         1024,  32, 15, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
   { 0xF00A9B3C, 0x00000000, 1, "1820",                            0,  32,  0, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX }, // Flashless
   { 0xF001D830, 0x00000000, 1, "1822",                          512,  32, 15, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
   { 0xF001D830, 0x00000000, 1, "1823",                          512,  32, 11, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
   { 0xF001D830, 0x00000000, 1, "1825",                          768,  32, 13, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
   { 0xF001D830, 0x00000000, 1, "1827",                         1024,  32, 15, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
   { 0xF0009A30, 0x00000000, 1, "1830",                            0,  32,  0, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX }, // Flashless
   { 0xF001DA30, 0x00000044, 1, "1833",                          512,  32, 11, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
   { 0xF001DA30, 0x00000000, 1, "1837",                         1024,  32, 15, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
   { 0xF0009830, 0x00000000, 1, "1850",                            0,  32,  0, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX }, // Flashless
   { 0xF001D830, 0x00000044, 1, "1853",                          512,  32, 11, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
   { 0xF001D830, 0x00000000, 1, "1857",                         1024,  32, 15, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },

   { 0x0004FF11, 0x00000000, 0, "2103",                           32,   8,  8, 4096, SectorTable_2103, CHIP_VARIANT_LPC2XXX },
   { 0xFFF0FF12, 0x00000000, 0, "2104",                          128,  16, 15, 8192, SectorTable_210x, CHIP_VARIANT_LPC2XXX },
   { 0xFFF0FF22, 0x00000000, 0, "2105",                          128,  32, 15, 8192, SectorTable_210x, CHIP_VARIANT_LPC2XXX },
   { 0xFFF0FF32, 0x00000000, 0, "2106",                          128,  64, 15, 8192, SectorTable_210x, CHIP_VARIANT_LPC2XXX },
   { 0x0201FF01, 0x00000000, 0, "2109",                           64,   8,  8, 4096, SectorTable_2109, CHIP_VARIANT_LPC2XXX },
   { 0x0101FF12, 0x00000000, 0, "2114",                          128,  16, 15, 8192, SectorTable_211x, CHIP_VARIANT_LPC2XXX },
   { 0x0201FF12, 0x00000000, 0, "2119",                          128,  16, 15, 8192, SectorTable_211x, CHIP_VARIANT_LPC2XXX },
   { 0x0101FF13, 0x00000000, 0, "2124",                          256,  16, 17, 8192, SectorTable_212x, CHIP_VARIANT_LPC2XXX },
   { 0x0201FF13, 0x00000000, 0, "2129",                          256,  16, 17, 8192, SectorTable_212x, CHIP_VARIANT_LPC2XXX },
   { 0x0002FF01, 0x00000000, 0, "2131",                           32,   8,  8, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0002FF11, 0x00000000, 0, "2132",                           64,  16,  9, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0002FF12, 0x00000000, 0, "2134",                          128,  16, 11, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0002FF23, 0x00000000, 0, "2136",                          256,  32, 15, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0002FF25, 0x00000000, 0, "2138",                          512,  32, 27, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0402FF01, 0x00000000, 0, "2141",                           32,   8,  8, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0402FF11, 0x00000000, 0, "2142",                           64,  16,  9, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0402FF12, 0x00000000, 0, "2144",                          128,  16, 11, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0402FF23, 0x00000000, 0, "2146",                          256,  40, 15, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0402FF25, 0x00000000, 0, "2148",                          512,  40, 27, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0301FF13, 0x00000000, 0, "2194",                          256,  16, 17, 8192, SectorTable_212x, CHIP_VARIANT_LPC2XXX },
   { 0x0301FF12, 0x00000000, 0, "2210",                            0,  16,  0, 8192, SectorTable_211x, CHIP_VARIANT_LPC2XXX }, /* table is a "don't care" */
   { 0x0401FF12, 0x00000000, 0, "2212",                          128,  16, 15, 8192, SectorTable_211x, CHIP_VARIANT_LPC2XXX },
   { 0x0601FF13, 0x00000000, 0, "2214",                          256,  16, 17, 8192, SectorTable_212x, CHIP_VARIANT_LPC2XXX },
   /*                           "2290"; same id as the LPC2210 */
   { 0x0401FF13, 0x00000000, 0, "2292",                          256,  16, 17, 8192, SectorTable_212x, CHIP_VARIANT_LPC2XXX },
   { 0x0501FF13, 0x00000000, 0, "2294",                          256,  16, 17, 8192, SectorTable_212x, CHIP_VARIANT_LPC2XXX },
   { 0x1600F701, 0x00000000, 0, "2361",                          128,  34, 11, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX }, /* From UM10211 Rev. 4.1 -- 5 Sep 2012 */
   { 0x1600FF22, 0x00000000, 0, "2362",                          128,  34, 11, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX }, /* From UM10211 Rev. 4.1 -- 5 Sep 2012 */
   { 0x0603FB02, 0x00000000, 0, "2364",                          128,  34, 11, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX }, /* From UM10211 Rev. 01 -- 6 July 2007 */
   { 0x1600F902, 0x00000000, 0, "2364",                          128,  34, 11, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1600E823, 0x00000000, 0, "2365",                          256,  58, 15, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0603FB23, 0x00000000, 0, "2366",                          256,  58, 15, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX }, /* From UM10211 Rev. 01 -- 6 July 2007 */
   { 0x1600F923, 0x00000000, 0, "2366",                          256,  58, 15, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1600E825, 0x00000000, 0, "2367",                          512,  58, 15, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0603FB25, 0x00000000, 0, "2368",                          512,  58, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX }, /* From UM10211 Rev. 01 -- 6 July 2007 */
   { 0x1600F925, 0x00000000, 0, "2368",                          512,  58, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1700E825, 0x00000000, 0, "2377",                          512,  58, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0703FF25, 0x00000000, 0, "2378",                          512,  58, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX }, /* From UM10211 Rev. 01 -- 6 July 2007 */
   { 0x1600FD25, 0x00000000, 0, "2378",                          512,  58, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX }, /* From UM10211 Rev. 01 -- 29 October 2007 */
   { 0x1700FD25, 0x00000000, 0, "2378",                          512,  58, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1700FF35, 0x00000000, 0, "2387",                          512,  98, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX }, /* From UM10211 Rev. 03 -- 25 August 2008 */
   { 0x1800F935, 0x00000000, 0, "2387",                          512,  98, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1800FF35, 0x00000000, 0, "2388",                          512,  98, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1500FF35, 0x00000000, 0, "2458",                          512,  98, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1600FF30, 0x00000000, 0, "2460",                            0,  98,  0, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1600FF35, 0x00000000, 0, "2468",                          512,  98, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1701FF30, 0x00000000, 0, "2470",                            0,  98,  0, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1701FF35, 0x00000000, 0, "2478",                          512,  98, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },

   { 0xA00A8B3F, 0x00000000, 1, "4310",                            0, 168,  0, 4096, SectorTable_43xx, CHIP_VARIANT_LPC43XX }, /* From UM10503 Rev. 1.4 -- 3 Sep 2012 */
   { 0xA00BCB3F, 0x00000080, 1, "4312",                          512, 104, 15, 4096, SectorTable_43xx, CHIP_VARIANT_LPC43XX }, /* info not yet available */
   { 0xA00BCB3F, 0x00000044, 1, "4313",                          512, 104, 11, 4096, SectorTable_43xx, CHIP_VARIANT_LPC43XX }, /* info not yet available */
   { 0xA001CB3F, 0x00000022, 1, "4315",                          768, 136, 13, 4096, SectorTable_43xx, CHIP_VARIANT_LPC43XX }, /* info not yet available */
   { 0xA001CB3F, 0x00000000, 1, "4317",                         1024, 136, 15, 4096, SectorTable_43xx, CHIP_VARIANT_LPC43XX }, /* info not yet available */
   { 0xA0008B3C, 0x00000000, 1, "4320",                            0, 200,  0, 4096, SectorTable_43xx, CHIP_VARIANT_LPC43XX }, /* From UM10503 Rev. 1.4 -- 3 Sep 2012 */
   { 0xA00BCB3C, 0x00000080, 1, "4322",                          512, 104, 15, 4096, SectorTable_43xx, CHIP_VARIANT_LPC43XX }, /* info not yet available */
   { 0xA00BCB3C, 0x00000044, 1, "4323",                          512, 104, 11, 4096, SectorTable_43xx, CHIP_VARIANT_LPC43XX }, /* info not yet available */
   { 0xA001CB3C, 0x00000022, 1, "4325",                          768, 136, 13, 4096, SectorTable_43xx, CHIP_VARIANT_LPC43XX }, /* info not yet available */
   { 0xA001CB3C, 0x00000000, 1, "4327",                         1024, 136, 15, 4096, SectorTable_43xx, CHIP_VARIANT_LPC43XX }, /* info not yet available */
   { 0xA0000A30, 0x00000000, 1, "4330",                            0, 264,  0, 4096, SectorTable_43xx, CHIP_VARIANT_LPC43XX }, /* From UM10503 Rev. 1.4 -- 3 Sep 2012 */
   { 0xA001CA30, 0x00000044, 1, "4333",                          512, 512, 11, 4096, SectorTable_43xx, CHIP_VARIANT_LPC43XX }, /* info not yet available */
   { 0xA001CA30, 0x00000000, 1, "4337",                         1024, 512, 15, 4096, SectorTable_43xx, CHIP_VARIANT_LPC43XX }, /* info not yet available */
   { 0xA0000830, 0x00000000, 1, "4350",                            0, 264,  0, 4096, SectorTable_43xx, CHIP_VARIANT_LPC43XX }, /* From UM10503 Rev. 1.4 -- 3 Sep 2012 */
   { 0xA001C830, 0x00000044, 1, "4353",                          512, 512, 11, 4096, SectorTable_43xx, CHIP_VARIANT_LPC43XX }, /* From UM10503 Rev. 1.4 -- 3 Sep 2012 */
   { 0xA001C830, 0x00000000, 1, "4357",                         1024, 512, 15, 4096, SectorTable_43xx, CHIP_VARIANT_LPC43XX }  /* From UM10503 Rev. 1.4 -- 3 Sep 2012 */
};

/***************************** NXP Download *********************************/
/**  Download the file from the internal memory image to the NXP microcontroller.
*   This function is visible from outside if COMPILE_FOR_LPC21
*/

/***************************** FormatCommand ********************************/
/**  2013-06-28 Torsten Lang, Uwe Schneider GmbH
According to the various NXP user manuals the ISP bootloader commands should
be terminated with <CR><LF>, the echo and/or answer should have the same line
termination. So far for the theory...
In fact, the bootloader also accepts <LF> as line termination, but it may or
may not echo the linebreak character. Some bootloaders convert the character
into <CR><LF>, some leave the <LF> and append another one (<LF><LF> in the
answer). Furthermore, during uuencoded data transfer the bootloader may or
may not append an additional <LF> character at the end of the answer
(leading to a <CR><LF><LF> sequence).
A reliable way to handle these deviations from the UM is to strictly send the
commands according to the description in the UM and to re-format commands
and answers after a transfer.
FormatCommand works in a way that it drops any leading linefeeds which only
can be surplus <LF> characters from a previous answer. It then converts any
sequence of <CR> and <LF> into a single <LF> character.
FormatCommand can work in place, meaning that In==Out is allowed!
\param [in]  In  Pointer to input buffer.
\param [out] Out Pointer to output buffer.
*/
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
static void FormatCommand(const char *In, char *Out)
{
  size_t i, j;
  for (i = 0, j = 0; In[j] != '\0'; i++, j++)
  {
    if ((In[j] == '\r') || (In[j] == '\n'))
    {
      if (i > 0) // Ignore leading line breaks (they must be leftovers from a previous answer)
      {
        Out[i] = '\n';
      }
      else
      {
        i--;
      }
      while ((In[j+1] == '\r') || (In[j+1] == '\n'))
      {
        j++;
      }
    }
    else
    {
      Out[i] = In[j];
    }
  }
  Out[i] = '\0';
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
static int SendAndVerify(ISP_ENVIRONMENT *IspEnvironment, const char *Command,
                                 char *AnswerBuffer, int AnswerLength)
{
  unsigned long realsize;
  int cmdlen;
  char *FormattedCommand;

  BT_Send(Command);
  ReceiveBT(IspEnvironment, AnswerBuffer, AnswerLength - 1, &realsize, 2, 5000);

  cmdlen = strlen(Command);
  FormattedCommand = (char *)alloca(cmdlen+1);
  FormatCommand(Command, FormattedCommand);
  FormatCommand(AnswerBuffer, AnswerBuffer);
  cmdlen = strlen(FormattedCommand);
  
  return (strncmp(AnswerBuffer, FormattedCommand, cmdlen) == 0
        && strcmp(AnswerBuffer + cmdlen, "0\n") == 0);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
static void NxpOutputErrorMessage(unsigned char ErrorNumber)
{
    switch (ErrorNumber)
    {
    case   0:
        DebugPrintf( "CMD_SUCCESS\n");
        break;

    case   1:
        DebugPrintf( "INVALID_COMMAND\n");
        break;

    case   2:
        DebugPrintf( "SRC_ADDR_ERROR: Source address is not on word boundary.\n");
        break;

    case   3:
        DebugPrintf( "DST_ADDR_ERROR: Destination address is not on a correct boundary.\n");
        break;

    case   4:
        DebugPrintf( "SRC_ADDR_NOT_MAPPED: Source address is not mapped in the memory map.\n"
                       "                     Count value is taken into consideration where applicable.\n");
        break;

    case   5:
        DebugPrintf( "DST_ADDR_NOT_MAPPED: Destination address is not mapped in the memory map.\n"
                       "                     Count value is taken into consideration where applicable.\n");
        break;

    case   6:
        DebugPrintf( "COUNT_ERROR: Byte count is not multiple of 4 or is not a permitted value.\n");
        break;

    case   7:
        DebugPrintf( "INVALID_SECTOR: Sector number is invalid or end sector number is\n"
                       "                greater than start sector number.\n");
        break;

    case   8:
        DebugPrintf( "SECTOR_NOT_BLANK\n");
        break;

    case   9:
        DebugPrintf( "SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION:\n"
                       "Command to prepare sector for write operation was not executed.\n");
        break;

    case  10:
        DebugPrintf( "COMPARE_ERROR: Source and destination data not equal.\n");
        break;

    case  11:
        DebugPrintf( "BUSY: Flash programming hardware interface is busy.\n");
        break;

    case  12:
        DebugPrintf( "PARAM_ERROR: Insufficient number of parameters or invalid parameter.\n");
        break;

    case  13:
        DebugPrintf( "ADDR_ERROR: Address is not on word boundary.\n");
        break;

    case  14:
        DebugPrintf( "ADDR_NOT_MAPPED: Address is not mapped in the memory map.\n"
                       "                 Count value is taken in to consideration where applicable.\n");
        break;

    case  15:
        DebugPrintf( "CMD_LOCKED\n");
        break;

    case  16:
        DebugPrintf( "INVALID_CODE: Unlock code is invalid.\n");
        break;

    case  17:
        DebugPrintf( "INVALID_BAUD_RATE: Invalid baud rate setting.\n");
        break;

    case  18:
        DebugPrintf( "INVALID_STOP_BIT: Invalid stop bit setting.\n");
        break;

    case  19:
        DebugPrintf( "CODE READ PROTECTION ENABLED\n");
        break;

    case 255:
        break;

    default:
        DebugPrintf( "unknown error %u\n", ErrorNumber);
        break;
    }

    //DebugPrintf( "error (%u), see  NxpOutputErrorMessage() in lpc21isp.c for help \n\r", ErrorNumber);
}

/***************************** GetAndReportErrorNumber ***************************/
/**  Find error number in string.  This will normally be the string
returned from the microcontroller.
\param [in] Answer the buffer to search for the error number.
\return the error number found, if no linefeed found before the end of the
string an error value of 255 is returned. If a non-numeric value is found
then it is printed to stdout and an error value of 255 is returned.
*/
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
static unsigned char GetAndReportErrorNumber(const char *Answer)
{
    unsigned char Result = 0xFF;                            // Error !!!
    unsigned int i = 0;

    while (1)
    {
        if (Answer[i] == 0x00)
        {
            break;
        }

        if (Answer[i] == 0x0a)
        {
            i++;

            if (Answer[i] < '0' || Answer[i] > '9')
            {
                DebugPrintf( "ErrorString: %s", &Answer[i]);
                break;
            }

            Result = (unsigned char) (atoi(&Answer[i]));
            break;
        }

        i++;
    }

    NxpOutputErrorMessage(Result);

    return Result;
}
#include <stdio.h>
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int NxpDownload(ISP_ENVIRONMENT *IspEnvironment)
{
  unsigned long realsize;
  char Answer[128];
  char ExpectedAnswer[128];
  char temp[128];
  char send[128];
  /*const*/ char *strippedAnswer, *endPtr;
  int strippedsize;
  int nQuestionMarks;
  int found;
  unsigned long Sector;
  unsigned long SectorLength;
  unsigned long SectorStart, SectorOffset, SectorChunk;
  char tmpString[128];
  char uuencode_table[64];
  int Line;
  unsigned long tmpStringPos;
  unsigned long BlockOffset;
  unsigned long Block;
  unsigned long Pos;
  unsigned long Id[2];
  unsigned long Id1Masked;
  unsigned long CopyLength;
  int c,k=0,i;
  unsigned long ivt_CRC;          // CRC over interrupt vector table
  unsigned long block_CRC;
  time_t tStartUpload=0, tDoneUpload=0;
  char tmp_string[64];
  char * cmd_string;

  int repeat = 0;
  int result;
  
  // Puffer for data to resend after "RESEND\r\n" Target responce
  char sendbuf0[128];
  char sendbuf1[128];
  char sendbuf2[128];
  char sendbuf3[128];
  char sendbuf4[128];
  char sendbuf5[128];
  char sendbuf6[128];
  char sendbuf7[128];
  char sendbuf8[128];
  char sendbuf9[128];
  char sendbuf10[128];
  char sendbuf11[128];
  char sendbuf12[128];
  char sendbuf13[128];
  char sendbuf14[128];
  char sendbuf15[128];
  char sendbuf16[128];
  char sendbuf17[128];
  char sendbuf18[128];
  char sendbuf19[128];

  char * sendbuf[20] = { sendbuf0,  sendbuf1,  sendbuf2,  sendbuf3,  sendbuf4,
                         sendbuf5,  sendbuf6,  sendbuf7,  sendbuf8,  sendbuf9,
                         sendbuf10, sendbuf11, sendbuf12, sendbuf13, sendbuf14,
                         sendbuf15, sendbuf16, sendbuf17, sendbuf18, sendbuf19};
  
  //�������� ����� ������ 
  pPG6->txt_info.Empty();

  //����������� ��������� ���� � ����� ����������������
  DebugPrintf("Jump to bootloader mode\r\n");
   
  result = pBLE->Tx(CMD_NRF_OVEN_PROG_MODE, NULL, 0);
  if(!result){ return (NO_ANSWER_QM);}

 

  //���� �������� � �������� 
  Sleep(1400);
  DebugPrintf("Connect ");
  found = 0;

  //������������� auto boud
  for(int i=0; i<12; i++)
  {	  
	 //�������� ? ��� ������ ���������
     //result = BT_Send("?");
	 pPG6->ev_BootDataRx.ResetEvent();
	 result = pBLE->Tx(CMD_NRF_OVEN_PROG_DATA, (char*)"?", 1);
	 if(!result)
	 { 
	   return (NO_ANSWER_QM);
	 }
	 
	 Sleep(10);
	 memset(Answer, 0, sizeof(Answer));

	 //���� ������ 
	 result = BT_Receive(Answer, sizeof(Answer), 500);
	 if(result==1)
	 {
	    std::string boot_answ(Answer); 
		result = boot_answ.find("Synchronized");
		
		if(result>=0)
		{
		  found = 1;
		  break;
		}
	 }
	  
	 DebugPrintf(".");
  }

  if(found==0)
  {
    DebugPrintf(" Error\r\n");
	  
	//no answer on '?'\n
	DebugPrintf("Bootloader no answer, Reset\r\n");
	
	pBLE->Tx(CMD_NRF_OVEN_RESET, NULL, 0);
	 
	Sleep(1000);
	
	//�������� ����� ������ 
    pPG6->txt_info.Empty();
	pPG6->m_edit_info.SetWindowTextA("");

    return (NO_ANSWER_QM);
  }

  DebugPrintf( " OK\r\n");


  Sleep(50);

  //===========================================================
  //�������� �������� ���������� � boot mode �� ���
  //53 79 6E 63 68 72 6F 6E 69 7A 65 64 0D 0A 
  found=0;
  memset(send,0,sizeof(send));
  memcpy(send, "Synchronized", 12);
  //result = BT_Send(send);
  pPG6->ev_BootDataRx.ResetEvent();
  result = pBLE->Tx(CMD_NRF_OVEN_PROG_DATA, (char*)send, 12);

  //���� ������  Synchronized OK
  result = BT_Receive(Answer, sizeof(Answer), 4000);
  if(result==1)
  {
	 std::string boot_answ(Answer); 
     result = boot_answ.find("Synchronized");
		
	 if(result>=0)
	 {
		found = 1;
	 }
  }	

  if (found==0)
  {
    DebugPrintf( "No answer on 'Synchronized'\r\n");
    return (NO_ANSWER_SYNC);
  }

  DebugPrintf("Synchronized OK\r\n");

  //===========================================================
  DebugPrintf("Setting oscillator\r\n");    
  memset(send,0,sizeof(send));
  memcpy(send, "014740\r\n", 6);
  
  found = 0;
  BT_Send(send);
  memset(Answer,0,sizeof(Answer));
  result = BT_Receive(Answer, sizeof(Answer), 1000);
  if(result==1)
  {
	std::string boot_answ(Answer); 
	result = boot_answ.find("OK");
	if(result>=0)
	{
	  found = 1;
	}
    
	result = boot_answ.find("014740\n1\r\n");	
	if(result>=0)
	{
	  found = 1;
	}
  }  
	
  if(found==0)
  {
    DebugPrintf( "No answer on Oscillator-Command\r\n");
    return (NO_ANSWER_OSC);
  }


  //===========================================================
  DebugPrintf("Unlock\r\n");

  cmd_string = "U 23130\r\n";

  if(!SendAndVerify(IspEnvironment, cmd_string, Answer, sizeof Answer))
  {
    DebugPrintf( "Unlock-Command: Error\r\n");
    return -1;//(UNLOCK_ERROR + GetAndReportErrorNumber(Answer));
  }
   
  //===========================================================
  DebugPrintf( "Read bootcode version: ");
  cmd_string = "K\r\n";

  BT_Send(cmd_string);
  ReceiveBT(IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 4,5000);

  FormatCommand(cmd_string, temp);
  FormatCommand(Answer, Answer);
  if(strncmp(Answer, temp, strlen(temp)) != 0)
  {
    DebugPrintf( "no answer on Read Boot Code Version\r\n");
    return (NO_ANSWER_RBV);
  }

  if(strncmp(Answer + strlen(temp), "0\n", 2) == 0)
  {
    strippedAnswer = Answer + strlen(temp) + 2;
        /*
        int maj, min, build;
        if (sscanf(strippedAnswer, "%d %d %d", &build, &min, &maj) == 2) {
        maj = min;
        min = build;
        build = 0;
        } // if
        DebugPrintf( "%d.%d.%d\n", maj, min, build);
        */
        DebugPrintf( strippedAnswer);
  }
  else
  {
        DebugPrintf( "unknown\r\n");
  }
   
  
  //===========================================================
  DebugPrintf("Read part ID: ");

  cmd_string = "J\r\n";
  BT_Send(cmd_string);

  ReceiveBT(IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 3, 5000);

  FormatCommand(cmd_string, temp);
  FormatCommand(Answer, Answer);
  if (strncmp(Answer, temp, strlen(temp)) != 0)
  {
    DebugPrintf( "no answer on Read Part Id\n");
    return (NO_ANSWER_RPID);
  }

    strippedAnswer = (strncmp(Answer, "J\n0\n", 4) == 0) ? Answer + 4 : Answer;

    Id[0] = strtoul(strippedAnswer, &endPtr, 10);
    Id[1] = 0UL;
    *endPtr = '\0'; /* delete \r\n */
    for (i = sizeof LPCtypes / sizeof LPCtypes[0] - 1; i > 0 && LPCtypes[i].id != Id[0]; i--)
        /* nothing */;
    IspEnvironment->DetectedDevice = i;
    if (LPCtypes[IspEnvironment->DetectedDevice].EvalId2 != 0)
    {
        /* Read out the second configuration word and run the search again */
        *endPtr = '\n';
        endPtr++;
        if ((endPtr[0] == '\0') || (endPtr[strlen(endPtr)-1] != '\n'))
        {
            /* No or incomplete word 2 */
            ReceiveBT(IspEnvironment, endPtr, sizeof(Answer)-(endPtr-Answer)-1, &realsize, 1, 100);
        }

        FormatCommand(endPtr, endPtr);
        if ((*endPtr == '\0') || (*endPtr == '\n'))
        {
            DebugPrintf( "incomplete answer on Read Part Id (second configuration word missing)\n");
            return (NO_ANSWER_RPID);
        }

        Id[1] = strtoul(endPtr, &endPtr, 10);
        *endPtr = '\0'; /* delete \r\n */

        Id1Masked = Id[1] & 0xFF;

        /* now search the table again */
        for (i = sizeof LPCtypes / sizeof LPCtypes[0] - 1; i > 0 && (LPCtypes[i].id != Id[0] || LPCtypes[i].id2 != Id1Masked); i--)
            /* nothing */;
        IspEnvironment->DetectedDevice = i;
    }
    if (IspEnvironment->DetectedDevice == 0) {
        DebugPrintf( "unknown");
    }
    else {
        DebugPrintf( "LPC%s, %d kiB FLASH / %d kiB SRAM",
            LPCtypes[IspEnvironment->DetectedDevice].Product,
            LPCtypes[IspEnvironment->DetectedDevice].FlashSize,
            LPCtypes[IspEnvironment->DetectedDevice].RAMSize);
    }
    if (LPCtypes[IspEnvironment->DetectedDevice].EvalId2 != 0)
    {
        DebugPrintf( " (0x%08lX / 0x%08lX -> %08lX)\n", Id[0], Id[1], Id1Masked);
    }
    else
    {
        DebugPrintf( " (0x%08lX)\n", Id[0]);
    }

    if (!IspEnvironment->DetectOnly)
    {
        // Build up uuencode table
        uuencode_table[0] = 0x60;           // 0x20 is translated to 0x60 !

        for (i = 1; i < 64; i++)
        {
            uuencode_table[i] = (char)(0x20 + i);
        }

        if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC2XXX)
        {
            // Patch 0x14, otherwise it is not running and jumps to boot mode

            ivt_CRC = 0;

            // Clear the vector at 0x14 so it doesn't affect the checksum:
            for (i = 0; i < 4; i++)
            {
                IspEnvironment->BinaryContent[i + 0x14] = 0;
            }

            // Calculate a native checksum of the little endian vector table:
            for (i = 0; i < (4 * 8);) {
                ivt_CRC += IspEnvironment->BinaryContent[i++];
                ivt_CRC += IspEnvironment->BinaryContent[i++] << 8;
                ivt_CRC += IspEnvironment->BinaryContent[i++] << 16;
                ivt_CRC += IspEnvironment->BinaryContent[i++] << 24;
            }

            /* Negate the result and place in the vector at 0x14 as little endian
            * again. The resulting vector table should checksum to 0. */
            ivt_CRC = (unsigned long) (0 - ivt_CRC);
            for (i = 0; i < 4; i++)
            {
                IspEnvironment->BinaryContent[i + 0x14] = (unsigned char)(ivt_CRC >> (8 * i));
            }

            DebugPrintf("Position 0x14 patched: ivt_CRC = 0x%08lX\n", ivt_CRC);
        }
        else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX ||
                LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX ||
                LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC17XX ||
                LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC13XX ||
                LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC11XX ||
                LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC8XX)
        {
            // Patch 0x1C, otherwise it is not running and jumps to boot mode

            ivt_CRC = 0;

            // Clear the vector at 0x1C so it doesn't affect the checksum:
            for (i = 0; i < 4; i++)
            {
                IspEnvironment->BinaryContent[i + 0x1C] = 0;
            }

            // Calculate a native checksum of the little endian vector table:
            for (i = 0; i < (4 * 8);) {
                ivt_CRC += IspEnvironment->BinaryContent[i++];
                ivt_CRC += IspEnvironment->BinaryContent[i++] << 8;
                ivt_CRC += IspEnvironment->BinaryContent[i++] << 16;
                ivt_CRC += IspEnvironment->BinaryContent[i++] << 24;
            }

            /* Negate the result and place in the vector at 0x1C as little endian
            * again. The resulting vector table should checksum to 0. */
            ivt_CRC = (unsigned long) (0 - ivt_CRC);
            for (i = 0; i < 4; i++)
            {
                IspEnvironment->BinaryContent[i + 0x1C] = (unsigned char)(ivt_CRC >> (8 * i));
            }

            DebugPrintf("Position 0x1C patched: ivt_CRC = 0x%08lX\n", ivt_CRC);
        }
        else
        {
          DebugPrintf( "Internal error: wrong chip variant %d (detected device %d)\n", LPCtypes[IspEnvironment->DetectedDevice].ChipVariant, IspEnvironment->DetectedDevice);
          exit(1);
        }
    }

#if 0
    DebugPrintf( "Read Unique ID:\n");

    cmd_string = "N\r\n";

    SendComPort(IspEnvironment, cmd_string);

    ReceiveBT(IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 5,5000);

    FormatCommand(cmd_string, temp);
    FormatCommand(Answer, Answer);
    if (strncmp(Answer, temp, strlen(temp)) != 0)
    {
        DebugPrintf( "no answer on Read Unique ID\n");
        return (NO_ANSWER_RBV);
    }

    if (strncmp(Answer + strlen(cmd_string), "0\n", 2) == 0)
    {
        strippedAnswer = Answer + strlen(temp) + 2;
        DebugPrintf( strippedAnswer);
    }
    else
    {
        DebugPrintf( "unknown\n");
    }
#endif // 0

    /* In case of a download to RAM, use full RAM for downloading
    * set the flash parameters to full RAM also.
    * This makes sure that all code is downloaded as one big sector
    */

    if ( (IspEnvironment->BinaryOffset >= ReturnValueLpcRamStart(IspEnvironment))
       &&(IspEnvironment->BinaryOffset + IspEnvironment->BinaryLength <= ReturnValueLpcRamStart(IspEnvironment)+(LPCtypes[IspEnvironment->DetectedDevice].RAMSize*1024)))
    {
        LPCtypes[IspEnvironment->DetectedDevice].FlashSectors = 1;
        LPCtypes[IspEnvironment->DetectedDevice].MaxCopySize  = LPCtypes[IspEnvironment->DetectedDevice].RAMSize*1024 - (ReturnValueLpcRamBase(IspEnvironment) - ReturnValueLpcRamStart(IspEnvironment));
        LPCtypes[IspEnvironment->DetectedDevice].SectorTable  = SectorTable_RAM;
        SectorTable_RAM[0] = LPCtypes[IspEnvironment->DetectedDevice].MaxCopySize;
    }
    if (IspEnvironment->DetectOnly)
        return (0);

    if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC8XX)
    {
      // XON/XOFF must be switched off for LPC8XX
      // otherwise problem during binary transmission of data to LPC8XX
      DebugPrintf("Switch off XON/XOFF !!!\n");
      ControlXonXoffSerialPort(IspEnvironment, 0);
    }

    // Start with sector 1 and go upward... Sector 0 containing the interrupt vectors
    // will be loaded last, since it contains a checksum and device will re-enter
    // bootloader mode as long as this checksum is invalid.
    DebugPrintf( "Will start programming at Sector 1 if possible, and conclude with Sector 0 to ensure that checksum is written last.\n");
    if (LPCtypes[IspEnvironment->DetectedDevice].SectorTable[0] >= IspEnvironment->BinaryLength)
    {
        Sector = 0;
        SectorStart = 0;
    }
    else
    {
        SectorStart = LPCtypes[IspEnvironment->DetectedDevice].SectorTable[0];
        Sector = 1;
    }

    if (IspEnvironment->WipeDevice == 1)
    {
        DebugPrintf( "Wiping Device. ");

        if (LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX ||
            LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX)
        {
            // TODO: Quick and dirty hack to address bank 0
            sprintf(tmpString, "P %d %d 0\r\n", 0, LPCtypes[IspEnvironment->DetectedDevice].FlashSectors-1);
        }
        else
        {
            sprintf(tmpString, "P %d %d\r\n", 0, LPCtypes[IspEnvironment->DetectedDevice].FlashSectors-1);
        }

        if (!SendAndVerify(IspEnvironment, tmpString, Answer, sizeof Answer))
        {
            DebugPrintf( "Wrong answer on Prepare-Command\n");
            return (WRONG_ANSWER_PREP + GetAndReportErrorNumber(Answer));
        }

        if (LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX ||
            LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX)
        {
            // TODO: Quick and dirty hack to address bank 0
            sprintf(tmpString, "E %d %d 0\r\n", 0, LPCtypes[IspEnvironment->DetectedDevice].FlashSectors-1);
        }
        else
        {
            sprintf(tmpString, "E %d %d\r\n", 0, LPCtypes[IspEnvironment->DetectedDevice].FlashSectors-1);
        }
        if (!SendAndVerify(IspEnvironment, tmpString, Answer, sizeof Answer))
        {
            DebugPrintf( "Wrong answer on Erase-Command\n");
            return (WRONG_ANSWER_ERAS + GetAndReportErrorNumber(Answer));
        }
        DebugPrintf( "OK \n");

        if (LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX ||
            LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX)
        {
          DebugPrintf( "ATTENTION: Only bank A was wiped!!!\n");
        }
    }
    else{
        //no wiping requested: erasing sector 0 first
        DebugPrintf( "Erasing sector 0 first, to invalidate checksum. ");

        if (LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX ||
            LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX)
        {
            // TODO: Quick and dirty hack to address bank 0
            sprintf(tmpString, "P %d %d 0\r\n", 0, 0);
        }
        else
        {
            sprintf(tmpString, "P %d %d\r\n", 0, 0);
        }

        if (!SendAndVerify(IspEnvironment, tmpString, Answer, sizeof Answer))
        {
            DebugPrintf( "Wrong answer on Prepare-Command\n");
            return (WRONG_ANSWER_PREP + GetAndReportErrorNumber(Answer));
        }

        if (LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX ||
            LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX)
        {
            // TODO: Quick and dirty hack to address bank 0
            sprintf(tmpString, "E %d %d 0\r\n", 0, 0);
        }
        else
        {
            sprintf(tmpString, "E %d %d\r\n", 0, 0);
        }

        if (!SendAndVerify(IspEnvironment, tmpString, Answer, sizeof Answer))
        {
            DebugPrintf( "Wrong answer on Erase-Command\n");
            return (WRONG_ANSWER_ERAS + GetAndReportErrorNumber(Answer));
        }
        DebugPrintf( "OK \n");
    }
    while (1)
    {
        if (Sector >= LPCtypes[IspEnvironment->DetectedDevice].FlashSectors)
        {
            DebugPrintf( "Program too large; running out of Flash sectors.\n");
            return (PROGRAM_TOO_LARGE);
        }

        DebugPrintf( "Sector %ld: ", Sector);
        fflush(stdout);

        if ( (IspEnvironment->BinaryOffset <  ReturnValueLpcRamStart(IspEnvironment))  // Skip Erase when running from RAM
           ||(IspEnvironment->BinaryOffset >= ReturnValueLpcRamStart(IspEnvironment)+(LPCtypes[IspEnvironment->DetectedDevice].RAMSize*1024)))
        {
            if (LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX ||
                LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX)
            {
                // TODO: Quick and dirty hack to address bank 0
                sprintf(tmpString, "P %ld %ld 0\r\n", Sector, Sector);
            }
            else
            {
                sprintf(tmpString, "P %ld %ld\r\n", Sector, Sector);
            }

            if (!SendAndVerify(IspEnvironment, tmpString, Answer, sizeof Answer))
            {
                DebugPrintf( "Wrong answer on Prepare-Command (1) (Sector %ld)\n", Sector);
                return (WRONG_ANSWER_PREP + GetAndReportErrorNumber(Answer));
            }

            DebugPrintf( ".");
            fflush(stdout);
            if (IspEnvironment->WipeDevice == 0 && (Sector!=0)) //Sector 0 already erased
            {
                if (LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX ||
                    LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX)
                {
                    // TODO: Quick and dirty hack to address bank 0
                    sprintf(tmpString, "E %ld %ld 0\r\n", Sector, Sector);
                }
                else
                {
                    sprintf(tmpString, "E %ld %ld\r\n", Sector, Sector);
                }

                if (!SendAndVerify(IspEnvironment, tmpString, Answer, sizeof Answer))
                {
                    DebugPrintf( "Wrong answer on Erase-Command (Sector %ld)\n", Sector);
                    return (WRONG_ANSWER_ERAS + GetAndReportErrorNumber(Answer));
                }

                DebugPrintf( ".");
                fflush(stdout);
            }
        }

        SectorLength = LPCtypes[IspEnvironment->DetectedDevice].SectorTable[Sector];
        if (SectorLength > IspEnvironment->BinaryLength - SectorStart)
        {
            SectorLength = IspEnvironment->BinaryLength - SectorStart;
        }

        for (SectorOffset = 0; SectorOffset < SectorLength; SectorOffset += SectorChunk)
        {
            // Check if we are to write only 0xFFs - it would be just a waste of time..
            if (SectorOffset == 0) {
                for (SectorOffset = 0; SectorOffset < SectorLength; ++SectorOffset)
                {
                    if (IspEnvironment->BinaryContent[SectorStart + SectorOffset] != 0xFF)
                        break;
                }
                if (SectorOffset == SectorLength) // all data contents were 0xFFs
                {
                    DebugPrintf( "Whole sector contents is 0xFFs, skipping programming.");
                    fflush(stdout);
                    break;
                }
                SectorOffset = 0; // re-set otherwise
            }

            if (SectorOffset > 0)
            {
                // Add a visible marker between segments in a sector
                DebugPrintf( "|");  /* means: partial segment copied */
                fflush(stdout);
            }

            // If the Flash ROM sector size is bigger than the number of bytes
            // we can copy from RAM to Flash, we must "chop up" the sector and
            // copy these individually.
            // This is especially needed in the case where a Flash sector is
            // bigger than the amount of SRAM.
            SectorChunk = SectorLength - SectorOffset;
            if (SectorChunk > (unsigned)LPCtypes[IspEnvironment->DetectedDevice].MaxCopySize)
            {
                SectorChunk = LPCtypes[IspEnvironment->DetectedDevice].MaxCopySize;
            }

            // Write multiple of 45 * 4 Byte blocks to RAM, but copy maximum of on sector to Flash
            // In worst case we transfer up to 180 byte too much to RAM
            // but then we can always use full 45 byte blocks and length is multiple of 4
            CopyLength = SectorChunk;

            if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC2XXX ||
               LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC17XX ||
               LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC13XX ||
               LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC11XX ||
               LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX ||
               LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX)
            {
                if ((CopyLength % (45 * 4)) != 0)
                {
                    CopyLength += ((45 * 4) - (CopyLength % (45 * 4)));
                }
            }

            sprintf(tmpString, "W %ld %ld\r\n", ReturnValueLpcRamBase(IspEnvironment), CopyLength);

            if (!SendAndVerify(IspEnvironment, tmpString, Answer, sizeof Answer))
            {
                DebugPrintf( "Wrong answer on Write-Command\n");
                return (WRONG_ANSWER_WRIT + GetAndReportErrorNumber(Answer));
            }

            DebugPrintf( ".");
            fflush(stdout);

            if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC2XXX ||
               LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC17XX ||
               LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC13XX ||
               LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC11XX ||
               LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX ||
               LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX)
            {
                block_CRC = 0;
                Line = 0;

                // Transfer blocks of 45 * 4 bytes to RAM
                for (Pos = SectorStart + SectorOffset; (Pos < SectorStart + SectorOffset + CopyLength) && (Pos < IspEnvironment->BinaryLength); Pos += (45 * 4))
                {
                    for (Block = 0; Block < 4; Block++)  // Each block 45 bytes
                    {
                        DebugPrintf( ".");
                        fflush(stdout);

#if defined INTEGRATED_IN_WIN_APP
                        // inform the calling application about having written another chuck of data
                        /// AppWritten(45);
#endif

                        // Uuencode one 45 byte block
                        tmpStringPos = 0;

#if !defined COMPILE_FOR_LPC21
                        sendbuf[Line][tmpStringPos++] = (char)(' ' + 45);    // Encode Length of block
#else
                        tmpString[tmpStringPos++] = (char)(' ' + 45);        // Encode Length of block
#endif

                        for (BlockOffset = 0; BlockOffset < 45; BlockOffset++)
                        {
                            if ( (IspEnvironment->BinaryOffset <  ReturnValueLpcRamStart(IspEnvironment))
                               ||(IspEnvironment->BinaryOffset >= ReturnValueLpcRamStart(IspEnvironment)+(LPCtypes[IspEnvironment->DetectedDevice].RAMSize*1024)))
                            { // Flash: use full memory
                                c = IspEnvironment->BinaryContent[Pos + Block * 45 + BlockOffset];
                            }
                            else
                            { // RAM: Skip first 0x200 bytes, these are used by the download program in LPC21xx
                                c = IspEnvironment->BinaryContent[Pos + Block * 45 + BlockOffset + 0x200];
                            }

                            block_CRC += c;

                            k = (k << 8) + (c & 255);

                            if ((BlockOffset % 3) == 2)   // Collecting always 3 Bytes, then do processing in 4 Bytes
                            {
#if !defined COMPILE_FOR_LPC21
                                sendbuf[Line][tmpStringPos++] = uuencode_table[(k >> 18) & 63];
                                sendbuf[Line][tmpStringPos++] = uuencode_table[(k >> 12) & 63];
                                sendbuf[Line][tmpStringPos++] = uuencode_table[(k >>  6) & 63];
                                sendbuf[Line][tmpStringPos++] = uuencode_table[ k        & 63];
#else
                                tmpString[tmpStringPos++] = uuencode_table[(k >> 18) & 63];
                                tmpString[tmpStringPos++] = uuencode_table[(k >> 12) & 63];
                                tmpString[tmpStringPos++] = uuencode_table[(k >>  6) & 63];
                                tmpString[tmpStringPos++] = uuencode_table[ k        & 63];
#endif
                            }
                        }


#if !defined COMPILE_FOR_LPC21
                        sendbuf[Line][tmpStringPos++] = '\r';
                        sendbuf[Line][tmpStringPos++] = '\n';
                        sendbuf[Line][tmpStringPos++] = 0;

                        BT_Send(sendbuf[Line]);
                        // receive only for debug proposes
                        ReceiveBT(IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 1, 5000);
                        FormatCommand(sendbuf[Line], tmpString);
                        FormatCommand(Answer, Answer);
                        if (strncmp(Answer, tmpString, strlen(tmpString)) != 0)
                        {
                            DebugPrintf( "Error on writing data (1)\n");
                            return (ERROR_WRITE_DATA);
                        }
#else
                        tmpString[tmpStringPos++] = '\r';
                        tmpString[tmpStringPos++] = '\n';
                        tmpString[tmpStringPos++] = 0;

                        SendComPort(IspEnvironment, tmpString);
                        ReceiveBT(IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 1, 5000);
                        FormatCommand(tmpString, tmpString);
                        FormatCommand(Answer, Answer);
                        if (strncmp(Answer, tmpString, tmpStringPos) != 0)
                        {
                            DebugPrintf( "Error on writing data (1)\n");
                            return (ERROR_WRITE_DATA);
                        }
#endif

                        Line++;

                        DebugPrintf("Line = %d\n", Line);

                        if (Line == 20)
                        {
#if !defined COMPILE_FOR_LPC21
                            for (repeat = 0; repeat < 3; repeat++)
                            {

                                // DebugPrintf( "block_CRC = %ld\n", block_CRC);

                                sprintf(tmpString, "%ld\r\n", block_CRC);

                                BT_Send(tmpString);

                                ReceiveBT(IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 2, 5000);

                                sprintf(tmpString, "%ld\nOK\n", block_CRC);

                                FormatCommand(tmpString, tmpString);
                                FormatCommand(Answer, Answer);
                                if (strcmp(Answer, tmpString) != 0)
                                {
                                    for (i = 0; i < Line; i++)
                                    {
                                        BT_Send(sendbuf[i]);
                                        ReceiveBT(IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 1, 5000);
                                    }
                                }
                                else
                                    break;
                            }

                            if (repeat >= 3)
                            {
                                DebugPrintf( "Error on writing block_CRC (1)\n");
                                return (ERROR_WRITE_CRC);
                            }
#else
                            // DebugPrintf( "block_CRC = %ld\n", block_CRC);
                            sprintf(tmpString, "%ld\r\n", block_CRC);
                            SendComPort(IspEnvironment, tmpString);

                            ReceiveBT(IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 2,5000);

                            sprintf(tmpString, "%ld\nOK\n", block_CRC);
                            FormatCommand(tmpString, tmpString);
                            FormatCommand(Answer, Answer);
                            if (strcmp(Answer, tmpString) != 0)
                            {
                                DebugPrintf( "Error on writing block_CRC (2)\n");
                                return (ERROR_WRITE_CRC);
                            }
#endif
                            Line = 0;
                            block_CRC = 0;
                        }
                    }
                }

                if (Line != 0)
                {
#if !defined COMPILE_FOR_LPC21
                    for (repeat = 0; repeat < 3; repeat++)
                    {
                        sprintf(tmpString, "%ld\r\n", block_CRC);

                        BT_Send(tmpString);

                        ReceiveBT(IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 2,5000);

                        sprintf(tmpString, "%ld\nOK\n", block_CRC);

                        FormatCommand(tmpString, tmpString);
                        FormatCommand(Answer, Answer);
                        if (strcmp(Answer, tmpString) != 0)
                        {
                            for (i = 0; i < Line; i++)
                            {
                                BT_Send(sendbuf[i]);
                                ReceiveBT(IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 1,5000);
                            }
                        }
                        else
                            break;
                    }

                    if (repeat >= 3)
                    {
                        DebugPrintf( "Error on writing block_CRC (3)\n");
                        return (ERROR_WRITE_CRC2);
                    }
#else
                    sprintf(tmpString, "%ld\r\n", block_CRC);
                    SendComPort(IspEnvironment, tmpString);

                    ReceiveBT(IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 2,5000);

                    sprintf(tmpString, "%ld\nOK\n", block_CRC);
                    FormatCommand(tmpString, tmpString);
                    FormatCommand(Answer, Answer);
                    if (strcmp(Answer, tmpString) != 0)
                    {
                        DebugPrintf( "Error on writing block_CRC (4)\n");
                        return (ERROR_WRITE_CRC2);
                    }
#endif
                }
            }
            else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC8XX)
            {
                unsigned char BigAnswer[4096];
                unsigned long CopyLengthPartialOffset = 0;
                unsigned long CopyLengthPartialRemainingBytes;

                while(CopyLengthPartialOffset < CopyLength)
                {
                    CopyLengthPartialRemainingBytes = CopyLength - CopyLengthPartialOffset;
                    if(CopyLengthPartialRemainingBytes > 256)
                    {
                      // There seems to be an error in LPC812:
                      // When too much bytes are written at high speed,
                      // bytes get lost
                      // Workaround: Use smaller blocks
                      CopyLengthPartialRemainingBytes = 256;
                    }

///                    SendComPortBlock(IspEnvironment, &IspEnvironment->BinaryContent[SectorStart + SectorOffset + CopyLengthPartialOffset], CopyLengthPartialRemainingBytes);

///                    if (ReceiveBTBlockComplete(IspEnvironment, &BigAnswer, CopyLengthPartialRemainingBytes, 10000) != 0)
///                    {
///                        return (ERROR_WRITE_DATA);
///                    }

                    if(memcmp(&IspEnvironment->BinaryContent[SectorStart + SectorOffset + CopyLengthPartialOffset], BigAnswer, CopyLengthPartialRemainingBytes))
                    {
                        return (ERROR_WRITE_DATA);
                    }

                    CopyLengthPartialOffset += CopyLengthPartialRemainingBytes;
                }
            }

            if ( (IspEnvironment->BinaryOffset <  ReturnValueLpcRamStart(IspEnvironment))
               ||(IspEnvironment->BinaryOffset >= ReturnValueLpcRamStart(IspEnvironment)+(LPCtypes[IspEnvironment->DetectedDevice].RAMSize*1024)))
            {
                // Prepare command must be repeated before every write
                if (LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX ||
                    LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX)
                {
                    // TODO: Quick and dirty hack to address bank 0
                    sprintf(tmpString, "P %ld %ld 0\r\n", Sector, Sector);
                }
                else
                {
                    sprintf(tmpString, "P %ld %ld\r\n", Sector, Sector);
                }

                if (!SendAndVerify(IspEnvironment, tmpString, Answer, sizeof Answer))
                {
                    DebugPrintf( "Wrong answer on Prepare-Command (2) (Sector %ld)\n", Sector);
                    return (WRONG_ANSWER_PREP2 + GetAndReportErrorNumber(Answer));
                }

                // Round CopyLength up to one of the following values: 512, 1024,
                // 4096, 8192; but do not exceed the maximum copy size (usually
                // 8192, but chip-dependent)
                if (CopyLength < 512)
                {
                    CopyLength = 512;
                }
                else if (SectorLength < 1024)
                {
                    CopyLength = 1024;
                }
                else if (SectorLength < 4096)
                {
                    CopyLength = 4096;
                }
                else
                {
                    CopyLength = 8192;
                }
                if (CopyLength > (unsigned)LPCtypes[IspEnvironment->DetectedDevice].MaxCopySize)
                {
                    CopyLength = LPCtypes[IspEnvironment->DetectedDevice].MaxCopySize;
                }

                sprintf(tmpString, "C %ld %ld %ld\r\n", IspEnvironment->BinaryOffset + SectorStart + SectorOffset, ReturnValueLpcRamBase(IspEnvironment), CopyLength);

                if (!SendAndVerify(IspEnvironment, tmpString, Answer, sizeof Answer))
                {
                    DebugPrintf( "Wrong answer on Copy-Command\n");
                    return (WRONG_ANSWER_COPY + GetAndReportErrorNumber(Answer));
                }

                if (IspEnvironment->Verify)
                {

                    //Avoid compare first 64 bytes.
                    //Because first 64 bytes are re-mapped to flash boot sector,
                    //and the compare result may not be correct.
                    if (SectorStart + SectorOffset<64)
                    {
                        sprintf(tmpString, "M %d %ld %ld\r\n", 64, ReturnValueLpcRamBase(IspEnvironment) + (64 - SectorStart - SectorOffset), CopyLength-(64 - SectorStart - SectorOffset));
                    }
                    else
                    {
                        sprintf(tmpString, "M %ld %ld %ld\r\n", SectorStart + SectorOffset, ReturnValueLpcRamBase(IspEnvironment), CopyLength);
                    }

                    if (!SendAndVerify(IspEnvironment, tmpString, Answer, sizeof Answer))
                    {
                        DebugPrintf( "Wrong answer on Compare-Command\n");
                        return (WRONG_ANSWER_COPY + GetAndReportErrorNumber(Answer));
                    }
                }
            }
        }

        DebugPrintf( "\n");
        fflush(stdout);

        if ((SectorStart + SectorLength) >= IspEnvironment->BinaryLength && Sector!=0)
        {
            Sector = 0;
            SectorStart = 0;
        }
        else if (Sector == 0) {
            break;
        }
        else {
            SectorStart += LPCtypes[IspEnvironment->DetectedDevice].SectorTable[Sector];
            Sector++;
        }
    }

    tDoneUpload = time(NULL);
    if (IspEnvironment->Verify)
        DebugPrintf( "Download Finished and Verified correct... taking %d seconds\n", tDoneUpload - tStartUpload);
    else
        DebugPrintf( "Download Finished... taking %d seconds\n", tDoneUpload - tStartUpload);

    // For LPC18xx set boot bank to 0
    if (LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX ||
        LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX)
    {
        if (!SendAndVerify(IspEnvironment, "S 0\r\n", Answer, sizeof Answer))
        {
            DebugPrintf( "Wrong answer on SetActiveBootFlashBank-Command\n");
            return (WRONG_ANSWER_BTBNK + GetAndReportErrorNumber(Answer));
        }
    }

    if(IspEnvironment->DoNotStart == 0)
    {
        DebugPrintf( "Now launching the brand new code\n");
        fflush(stdout);

        if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC2XXX)
        {
            sprintf(tmpString, "G %ld A\r\n", IspEnvironment->StartAddress);
        }
        else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX ||
                LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX ||
                LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC17XX ||
                LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC13XX ||
                LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC11XX)
        {
            sprintf(tmpString, "G %ld T\r\n", IspEnvironment->StartAddress & ~1);
        }
        else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC8XX)
        {
            sprintf(tmpString, "G 0 T\r\n");
        }
        else
        {
            DebugPrintf( "Internal Error %s %d\n", __FILE__, __LINE__);
            exit(1);
        }

        BT_Send(tmpString); //goto 0 : run this fresh new downloaded code code
        if ( (IspEnvironment->BinaryOffset <  ReturnValueLpcRamStart(IspEnvironment))
           ||(IspEnvironment->BinaryOffset >= ReturnValueLpcRamStart(IspEnvironment)+(LPCtypes[IspEnvironment->DetectedDevice].RAMSize*1024)))
        { // Skip response on G command - show response on Terminal instead
            ReceiveBT(IspEnvironment, Answer, sizeof(Answer)-1, &realsize, 2, 5000);
            /* the reply string is frequently terminated with a -1 (EOF) because the
            * connection gets broken; zero-terminate the string ourselves
            */
            while (realsize > 0 && ((signed char) Answer[(int)realsize - 1]) < 0)
                realsize--;
            Answer[(int)realsize] = '\0';
            /* Better to check only the first 9 chars instead of complete receive buffer,
            * because the answer can contain the output by the started programm
            */
            if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC2XXX)
            {
                sprintf(ExpectedAnswer, "G %ld A\n0", IspEnvironment->StartAddress);
            }
            else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX ||
                    LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX ||
                    LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC17XX ||
                    LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC13XX ||
                    LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC11XX)
            {
                sprintf(ExpectedAnswer, "G %ld T\n0", IspEnvironment->StartAddress & ~1);
            }
            else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC8XX)
            {
                sprintf(ExpectedAnswer, "G 0 T\n0");
            }
            else
            {
                DebugPrintf( "Internal Error %s %d\n", __FILE__, __LINE__);
                exit(1);
            }

            FormatCommand(Answer, Answer);
            if (realsize == 0 || strncmp((const char *)Answer, /*cmd_string*/ExpectedAnswer, strlen(/*cmd_string*/ExpectedAnswer)) != 0)
            {
                DebugPrintf( "Failed to run the new downloaded code: ");
                return (FAILED_RUN + GetAndReportErrorNumber(Answer));
            }
        }

        fflush(stdout);
    }
    return (0);
}
//#endif // LPC_SUPPORT

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
unsigned long ReturnValueLpcRamStart(ISP_ENVIRONMENT *IspEnvironment)
{
  if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX)
  {
    return LPC_RAMSTART_LPC43XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC2XXX)
  {
    return LPC_RAMSTART_LPC2XXX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX)
  {
    return LPC_RAMSTART_LPC18XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC17XX)
  {
    return LPC_RAMSTART_LPC17XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC13XX)
  {
    return LPC_RAMSTART_LPC13XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC11XX)
  {
    return LPC_RAMSTART_LPC11XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC8XX)
  {
    return LPC_RAMSTART_LPC8XX;
  }
  DebugPrintf( "Error in ReturnValueLpcRamStart (%d)\n", LPCtypes[IspEnvironment->DetectedDevice].ChipVariant);
  //exit(1);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
unsigned long ReturnValueLpcRamBase(ISP_ENVIRONMENT *IspEnvironment)
{
  if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX)
  {
    return LPC_RAMBASE_LPC43XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC2XXX)
  {
    return LPC_RAMBASE_LPC2XXX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX)
  {
    return LPC_RAMBASE_LPC18XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC17XX)
  {
    return LPC_RAMBASE_LPC17XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC13XX)
  {
    return LPC_RAMBASE_LPC13XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC11XX)
  {
    return LPC_RAMBASE_LPC11XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC8XX)
  {
    return LPC_RAMBASE_LPC8XX;
  }
  DebugPrintf( "Error in ReturnValueLpcRamBase (%d)\n", LPCtypes[IspEnvironment->DetectedDevice].ChipVariant);
  //exit(1);
}
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
 /**  Sends a block of bytes out the opened com port.
\param [in] s block to send.
\param [in] n size of the block.
*/
void SendComPortBlock(ISP_ENVIRONMENT *IspEnvironment, const void *s, size_t n)
{
 /*

    unsigned long realsize;
    size_t m;
    unsigned long rxsize;
    char * pch;
    char * rxpch;


    DumpString(4, s, n, "Sending ");

#if defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN

    if (IspEnvironment->HalfDuplex == 0)
    {
        WriteFile(IspEnvironment->hCom, s, n, &realsize, NULL);
    }
    else
    {
        pch = (char *)s;
        rxpch = RxTmpBuf;
        pRxTmpBuf = RxTmpBuf;

        // avoid buffer otherflow
        if (n > sizeof (RxTmpBuf))
            n = sizeof (RxTmpBuf);

        for (m = 0; m < n; m++)
        {
            WriteFile(IspEnvironment->hCom, pch, 1, &realsize, NULL);

            if ((*pch != '?') || (n != 1))
            {
                do
                {
                    ReadFile(IspEnvironment->hCom, rxpch, 1, &rxsize, NULL);
                }while (rxsize == 0);
            }
            pch++;
            rxpch++;
        }
        *rxpch = 0;        // terminate echo string
    }
#endif // defined COMPILE_FOR_WINDOWS || defined COMPILE_FOR_CYGWIN

#if defined COMPILE_FOR_LINUX || defined COMPILE_FOR_LPC21

    write(IspEnvironment->fdCom, s, n);

#endif // defined COMPILE_FOR_LINUX || defined COMPILE_FOR_LPC21

    if (IspEnvironment->WriteDelay == 1)
    {
        Sleep(100); // 100 ms delay after each block (makes lpc21isp to work with bad UARTs)
    }  */
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
/***************************** ReceiveBT ***************************/
/**  Receives a buffer from the open com port. Returns when the buffer is
filled, the numer of requested linefeeds has been received or the timeout
period has passed. The bootloaders may send 0x0d,0x0a,0x0a or 0x0d,0x0a as
linefeed pattern
2013-06-28 Torsten Lang
Note: We *could* filter out surplus 0x0a characters like in <CR><LF><LF>
but as we don't know how the individual bootloader behaves we don't want
to wait for possible surplus <LF> (which would slow down the transfer).
Thus, we just terminate after the expected number of <CR><LF> sequences
and leave it to the command handler in lpcprog.c to filter out surplus
<LF> characters which then occur as leading character in answers or
echoed commands.
\param [in] ISPEnvironment.
\param [out] Answer buffer to hold the bytes read from the serial port.
\param [in] MaxSize the size of buffer pointed to by Answer.
\param [out] RealSize pointer to a long that returns the amout of the
buffer that is actually used.
\param [in] WantedNr0x0A the maximum number of linefeeds to accept before
returning.
\param [in] timeOutMilliseconds the maximum amount of time to wait before
reading with an incomplete buffer.
*/
void ReceiveBT(ISP_ENVIRONMENT *IspEnvironment,
                                    const char *Ans, unsigned long MaxSize,
                                    unsigned long *RealSize, unsigned long WantedNr0x0A,
                                    unsigned timeOutMilliseconds)
{
    unsigned long tmp_realsize;
    unsigned long nr_of_0x0A = 0;
    unsigned long nr_of_0x0D = 0;
    int eof = 0;
    unsigned long p;
    unsigned char *Answer;
    unsigned char *endPtr;
    char tmp_string[32];
    static char residual_data[128] = {'\0'};
    int lf = 0;
	int length;

    Answer  = (unsigned char*) Ans;

    *RealSize = 0;
    endPtr = NULL;

	/*
    do
    {
        if (residual_data[0] == '\0')
        {
            /* Receive new data 
            ReceiveComPortBlock(IspEnvironment, Answer + (*RealSize), MaxSize - (*RealSize), &tmp_realsize);
        }
        else
        {
            /* Take over any old residual data 
            strcpy((char *)Answer, residual_data);
            tmp_realsize = strlen((char *)Answer);
            residual_data[0] = '\0';
        }

        if (tmp_realsize != 0)
        {
            for (p = (*RealSize); p < (*RealSize) + tmp_realsize; p++)
            {
                /* Torsten Lang 2013-05-06 Scan for 0x0d,0x0a,0x0a and 0x0d,0x0a as linefeed pattern
                if (Answer[p] == 0x0a)
                {
                    if (lf != 0)
                    {
                        nr_of_0x0A++;
                        lf = 0;
                        if (nr_of_0x0A >= WantedNr0x0A)
                        {
                            endPtr = &Answer[p+1];
                        }
                    }
                }
                else if (Answer[p] == 0x0d)
                {
                    nr_of_0x0D++;
                    lf = 1;
                }
                else if (((signed char) Answer[p]) < 0)
                {
                    eof = 1;
                    lf  = 0;
                }
                else if (lf != 0)
                {
                    nr_of_0x0D++;
                    nr_of_0x0A++;
                    lf = 0;
                    if (nr_of_0x0A >= WantedNr0x0A)
                    {
                        endPtr = &Answer[p+1];
                    }
                }
            }
            (*RealSize) += tmp_realsize;
        }
    } while (((*RealSize) < MaxSize) && (SerialTimeoutCheck(IspEnvironment) == 0) && (nr_of_0x0A < WantedNr0x0A) && !eof);
	 */
	int result = BT_Receive(residual_data, sizeof(residual_data), timeOutMilliseconds);

	length = strlen(residual_data);
	
	memcpy((void*)Ans, residual_data, length);

	/*
    // Torsten Lang 2013-05-06 Store residual data and cut answer after expected nr. of 0x0a 
    Answer[*RealSize] = '\0';
    if (endPtr != NULL)
    {
        strcpy(residual_data, (char *)endPtr);
        *endPtr = '\0';
        // Torsten Lang 2013-06-28 Update size info
        
    }  */
	
	*RealSize = length; //endPtr-Answer;
    
	//sprintf(tmp_string, "Answer(Length=%ld): ", (*RealSize));
    //DumpString(3, Answer, (*RealSize), tmp_string);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
/***************************** ReceiveComPortBlockComplete **************/
/**  Receives a fixed block from the open com port. Returns when the
block is completely filled or the timeout period has passed
\param [out] block buffer to hold the bytes read from the serial port.
\param [in] size the size of the buffer pointed to by block.
\param [in] timeOut the maximum amount of time to wait before guvung up on
completing the read.
\return 0 if successful, non-zero otherwise.
*/
int ReceiveComPortBlockComplete(ISP_ENVIRONMENT *IspEnvironment,
                                                    void *block, size_t size, unsigned timeout)
{
    unsigned long realsize = 0, read;
    char *result;
    char tmp_string[32];

    result = (char*) block;

   /// SerialTimeoutSet(IspEnvironment, timeout);
   /*
    do
    {
       ReceiveComPortBlock(IspEnvironment, result + realsize, size - realsize, &read);

        realsize += read;

    } while ((realsize < size) && (SerialTimeoutCheck(IspEnvironment) == 0));

    sprintf(tmp_string, "Answer(Length=%ld): ", realsize);
    DumpString(3, result, realsize, tmp_string);
   */
    if (realsize != size)
    {
        return 1;
    }
    return 0;
}


void ControlXonXoffSerialPort(ISP_ENVIRONMENT *IspEnvironment, unsigned char XonXoff)
{
    DCB dcb;

    GetCommState(IspEnvironment->hCom, &dcb);

    if(XonXoff)
    {
        dcb.fOutX = TRUE;
        dcb.fInX  = TRUE;
    }
    else
    {
        dcb.fOutX = FALSE;
        dcb.fInX  = FALSE;
    }

    if (SetCommState(IspEnvironment->hCom, &dcb) == 0)
    {
        DebugPrintf( "Can't set XonXoff ! - Error: %ld", GetLastError());
        exit(3);
    }
}
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void PrepareKeyboardTtySettings(void){}
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void ResetKeyboardTtySettings(void){}
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void AppException(int exception_level){}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void DebugPrintf(const char *fmt, ...)
{
  char pTemp[2000];
  memset(pTemp, 0, sizeof(pTemp));
  va_list ap;
      
  va_start(ap, fmt);
  //vprintf(fmt, ap);
  vsprintf(pTemp, fmt, ap);      
  va_end(ap);

  std::string msg(pTemp);
  
  pPG6->txt_info += msg.c_str();
  
  pPG6->m_edit_info.SetWindowTextA(pPG6->txt_info);
  pPG6->m_edit_info.UpdateWindow();

  ///TRACE(pTemp);
}



//------------------------------------------------------------------------------
//Function:	string to send
//------------------------------------------------------------------------------
int BT_Send(const char *pString)
{
  int length = strlen(pString);
  pPG6->ev_BootDataRx.ResetEvent();
/*
  unsigned long realsize;
  size_t m;
  unsigned long rxsize;
  char * pch;
  char * rxpch;
*/
  ///DumpString(4, s, n, "Sending ");
  
  /*
  if (IspEnvironment->HalfDuplex == 0)
  {
        WriteFile(IspEnvironment->hCom, s, n, &realsize, NULL);
  }
    else
    {
        pch = (char *)s;
        rxpch = RxTmpBuf;
        pRxTmpBuf = RxTmpBuf;

        // avoid buffer otherflow
        if (n > sizeof (RxTmpBuf))
            n = sizeof (RxTmpBuf);

        for (m = 0; m < n; m++)
        {
            WriteFile(IspEnvironment->hCom, pch, 1, &realsize, NULL);

            if ((*pch != '?') || (n != 1))
            {
                do
                {
                    ReadFile(IspEnvironment->hCom, rxpch, 1, &rxsize, NULL);
                }while (rxsize == 0);
            }
            pch++;
            rxpch++;
        }
        *rxpch = 0;        // terminate echo string
  
  } */

  int result = pBLE->Tx(CMD_NRF_OVEN_PROG_DATA, (char*)pString, length);

  //100 ms delay after each block (makes lpc21isp to work with bad UARTs)
  //Sleep(100); 

  return result;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int BT_Receive(void *pBuf, int size, unsigned timeout)
{  
  //pPG6->ev_BootDataRx.ResetEvent();
  //memset(pBuf, 0, sizeof(size));
  
  DWORD ret;
  ret = WaitForSingleObject(pPG6->ev_BootDataRx.m_hObject, timeout);
  
  int copy_length;
  //TRACE1("%d\n", ret);

  if(ret==0)
  {
	int length = pPG6->boot_txt.length();
	if(length<=size) copy_length = length;
	else { copy_length = size;}

	memcpy(pBuf, pPG6->boot_txt.c_str(), copy_length);
	return 1;
  }

  /*
  if(ok==1) TRACE1("%s\n", buf2);
  */

  return -1;
}
