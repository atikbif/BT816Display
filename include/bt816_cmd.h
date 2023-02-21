/*
 * bt816_cmd.h
 *
 *  Created on: 2022 Sep 13
 *      Author: Roman
 */

#ifndef INCLUDE_BT816_CMD_H_
#define INCLUDE_BT816_CMD_H_

#include <stdint.h>

#define DL_VERTEX_FORMAT 0x27000000UL
#define VERTEX_FORMAT(frac) ((DL_VERTEX_FORMAT) | ((frac) & 7UL))

#define DL_DISPLAY       0x00000000UL
#define DL_BITMAP_SOURCE 0x01000000UL
#define DL_CLEAR_COLOR_RGB 0x02000000UL
#define DL_TAG           0x03000000UL
#define DL_COLOR_RGB     0x04000000UL
#define DL_BITMAP_HANDLE 0x05000000UL
#define DL_CELL          0x06000000UL
#define DL_BITMAP_LAYOUT 0x07000000UL
#define DL_BITMAP_SIZE   0x08000000UL
#define DL_ALPHA_FUNC    0x09000000UL
#define DL_STENCIL_FUNC  0x0A000000UL
#define DL_BLEND_FUNC    0x0B000000UL
#define DL_STENCIL_OP    0x0C000000UL
#define DL_POINT_SIZE    0x0D000000UL
#define DL_LINE_WIDTH    0x0E000000UL
#define DL_CLEAR_COLOR_A 0x0F000000UL
#define DL_COLOR_A       0x10000000UL
#define DL_CLEAR_STENCIL 0x11000000UL
#define DL_CLEAR_TAG     0x12000000UL
#define DL_STENCIL_MASK  0x13000000UL
#define DL_TAG_MASK      0x14000000UL
#define DL_BITMAP_TRANSFORM_A 0x15000000UL
#define DL_BITMAP_TRANSFORM_B 0x16000000UL
#define DL_BITMAP_TRANSFORM_C 0x17000000UL
#define DL_BITMAP_TRANSFORM_D 0x18000000UL
#define DL_BITMAP_TRANSFORM_E 0x19000000UL
#define DL_BITMAP_TRANSFORM_F 0x1A000000UL
#define DL_SCISSOR_XY    0x1B000000UL
#define DL_SCISSOR_SIZE  0x1C000000UL
#define DL_CALL          0x1D000000UL
#define DL_JUMP          0x1E000000UL
#define DL_BEGIN         0x1F000000UL
#define DL_COLOR_MASK    0x20000000UL
#define DL_END           0x21000000UL
#define DL_SAVE_CONTEXT  0x22000000UL
#define DL_RESTORE_CONTEXT 0x23000000UL
#define DL_RETURN        0x24000000UL
#define DL_MACRO         0x25000000UL
#define DL_CLEAR         0x26000000UL
#define DL_VERTEX_FORMAT 0x27000000UL
#define DL_BITMAP_LAYOUT_H 0x28000000UL
#define DL_BITMAP_SIZE_H 0x29000000UL
#define DL_PALETTE_SOURCE 0x2A000000UL
#define DL_VERTEX_TRANSLATE_X 0x2B000000UL
#define DL_VERTEX_TRANSLATE_Y 0x2C000000UL
#define DL_NOP            0x2D000000UL

#define DL_VERTEX2F       0x40000000UL
#define DL_VERTEX2II      0x80000000UL

/* Alpha blending */
#define BT816_ZERO                 0UL
#define BT816_ONE                  1UL
#define BT816_SRC_ALPHA            2UL
#define BT816_DST_ALPHA            3UL
#define BT816_ONE_MINUS_SRC_ALPHA  4UL
#define BT816_ONE_MINUS_DST_ALPHA  5UL

/* Graphics primitives */
#define BT816_BITMAPS              1UL
#define BT816_POINTS               2UL
#define BT816_LINES                3UL
#define BT816_LINE_STRIP           4UL
#define BT816_EDGE_STRIP_R         5UL
#define BT816_EDGE_STRIP_L         6UL
#define BT816_EDGE_STRIP_A         7UL
#define BT816_EDGE_STRIP_B         8UL
#define BT816_RECTS                9UL

/* some pre-definded colors */
#define RED     0xff0000UL
#define ORANGE  0xffa500UL
#define GREEN   0x00ff00UL
#define BLUE    0x0000ffUL
#define BLUE_1  0x5dade2L
#define YELLOW  0xffff00UL
#define MAGENTA 0xff00ffUL
#define PURPLE  0x800080UL
#define WHITE   0xffffffUL
#define BLACK   0x000000UL

#define RAM_G_SIZE   (1024U*1024UL)
#define CMDFIFO_SIZE (4U*1024UL)
#define RAM_DL_SIZE  (8U*1024UL)

#define OPT_MEDIAFIFO        16U
#define OPT_FULLSCREEN       8U
#define OPT_NOTEAR           4U
#define OPT_SOUND            32U

#define OPT_FLASH  64U
#define OPT_OVERLAY 128U
#define OPT_FORMAT 4096U
#define OPT_FILL   8192U

#define RAM_ERR_REPORT				0x309800

/* Widget command options */
#define BT816_OPT_MONO             1U
#define BT816_OPT_NODL             2U
#define BT816_OPT_FLAT             256U
#define BT816_OPT_CENTERX          512U
#define BT816_OPT_CENTERY          1024U
#define BT816_OPT_CENTER           (BT816_OPT_CENTERX | BT816_OPT_CENTERX)
#define BT816_OPT_NOBACK           4096U
#define BT816_OPT_NOTICKS          8192U
#define BT816_OPT_NOHM             16384U
#define BT816_OPT_NOPOINTER        16384U
#define BT816_OPT_NOSECS           32768U
#define BT816_OPT_NOHANDS          49152U
#define BT816_OPT_RIGHTX           2048U
#define BT816_OPT_SIGNED           256U

#define BT816_OPT_MEDIAFIFO        16U
#define BT816_OPT_FULLSCREEN       8U
#define BT816_OPT_NOTEAR           4U
#define BT816_OPT_SOUND            32U

#define CLR_COL       0x4U
#define CLR_STN       0x2U
#define CLR_TAG       0x1U

#define BITMAP_TRANSFORM_A(a) ((DL_BITMAP_TRANSFORM_A) | ((a) & 0x1FFFFUL))
#define BITMAP_TRANSFORM_B(b) ((DL_BITMAP_TRANSFORM_B) | ((b) & 0x1FFFFUL))
#define BITMAP_TRANSFORM_D(d) ((DL_BITMAP_TRANSFORM_D) | ((d) & 0x1FFFFUL))
#define BITMAP_TRANSFORM_E(e) ((DL_BITMAP_TRANSFORM_E) | ((e) & 0x1FFFFUL))

#define BITMAP_TRANSFORM_C(c) ((DL_BITMAP_TRANSFORM_C) | ((c) & 0x1FFFFUL))
#define BITMAP_TRANSFORM_F(f) ((DL_BITMAP_TRANSFORM_F) | ((f) & 0x1FFFFUL))

#define BLEND_FUNC(src,dst) ((DL_BLEND_FUNC) | (((src) & 7UL) << 3U) | ((dst) & 7UL))
#define CALL(dest) ((DL_CALL) | ((dest) & 0xFFFFUL))
#define CELL(cell) ((DL_CELL) | ((cell) & 0x7FUL))
#define CLEAR(c,s,t) ((DL_CLEAR) | (((c) & 1UL) << 2U) | (((s) & 1UL) << 1U) | ((t) & 1UL))
#define CLEAR_COLOR_A(alpha) ((DL_CLEAR_COLOR_A) | ((alpha) & 0xFFUL))
#define CLEAR_COLOR_RGB(red,green,blue) ((DL_CLEAR_COLOR_RGB) | (((red) & 0xFFUL) << 16U) | (((green) & 0xFFUL) << 8U) | ((blue) & 0xFFUL))
#define CLEAR_STENCIL(s) ((DL_CLEAR_STENCIL) | ((s) & 0xFFUL))
#define CLEAR_TAG(s) ((DL_CLEAR_TAG) | ((s) & 0xFFUL))
#define COLOR_A(alpha) ((DL_COLOR_A) | ((alpha) & 0xFFUL))
#define COLOR_MASK(r,g,b,a) ((DL_COLOR_MASK) | (((r) & 1UL) << 3U) | (((g) & 1UL) << 2U) | (((b) & 1UL) << 1U) | ((a) & 1UL))
#define COLOR_RGB(red,green,blue) ((DL_COLOR_RGB) | (((red) & 0xFFUL) << 16U) | (((green) & 0xFFUL) << 8U) | ((blue) & 0xFFUL))
#define JUMP(dest) ((DL_JUMP) | ((dest) & 0xFFFFUL))
#define LINE_WIDTH(width) ((DL_LINE_WIDTH) | (((uint32_t) (width)) & 0xFFFUL))
#define MACRO(m) ((DL_MACRO) | ((m) & 1UL))
#define PALETTE_SOURCE(addr) ((DL_PALETTE_SOURCE) | ((addr) & 0x3FFFFF3UL))
#define POINT_SIZE(size) ((DL_POINT_SIZE) | ((size) & 0x1FFFUL))
#define SCISSOR_SIZE(width,height) ((DL_SCISSOR_SIZE) | (((width) & 0xFFFUL) << 12U) | ((height) & 0xFFFUL))
#define SCISSOR_XY(x,y) ((DL_SCISSOR_XY) | (((x) & 0x7FFUL) << 11U) | ((y) & 0x7FFUL))
#define STENCIL_FUNC(func,ref,mask) ((DL_STENCIL_FUNC) | (((func) & 7UL) << 16U) | (((ref) & 0xFFUL) << 8U)|((mask) & 0xFFUL))
#define STENCIL_MASK(mask) ((DL_STENCIL_MASK) | ((mask) & 0xFFUL))
#define STENCIL_OP(sfail,spass) ((DL_STENCIL_OP) | (((sfail) & 7UL) << 3U) | ((spass) & 7UL))
#define TAG(s) ((DL_TAG) | ((s) & 0xFFUL))
#define TAG_MASK(mask) ((DL_TAG_MASK) | ((mask) & 1UL))
#define VERTEX2F(x,y) ((DL_VERTEX2F) | ((((uint32_t) (x)) & 0x7FFFUL) << 15U) | (((uint32_t) (y)) & 0x7FFFUL))
#define VERTEX2II(x,y,handle,cell) ((DL_VERTEX2II) | (((x) & 0x1FFUL) << 21U) | (((y) & 0x1FFUL) << 12U) | (((handle) & 0x1FUL) << 7U) | ((cell) & 0x7FUL))
#define VERTEX_FORMAT(frac) ((DL_VERTEX_FORMAT) | ((frac) & 7UL))
#define VERTEX_TRANSLATE_X(x) ((DL_VERTEX_TRANSLATE_X) | ((x) & 0x1FFFFUL))
#define VERTEX_TRANSLATE_Y(y) ((DL_VERTEX_TRANSLATE_Y) | ((y) & 0x1FFFFUL))

#define BT816_ID			(0x011608)

/* Commands */
#define CMD_APPEND       0xFFFFFF1EUL
#define CMD_BGCOLOR      0xFFFFFF09UL
#define CMD_BUTTON       0xFFFFFF0DUL
#define CMD_CALIBRATE    0xFFFFFF15UL
#define CMD_CLOCK        0xFFFFFF14UL
#define CMD_COLDSTART    0xFFFFFF32UL
#define CMD_DIAL         0xFFFFFF2DUL
#define CMD_DLSTART      0xFFFFFF00UL
#define CMD_FGCOLOR      0xFFFFFF0AUL
#define CMD_GAUGE        0xFFFFFF13UL
#define CMD_GETMATRIX    0xFFFFFF33UL
#define CMD_GETPROPS     0xFFFFFF25UL
#define CMD_GETPTR       0xFFFFFF23UL
#define CMD_GRADCOLOR    0xFFFFFF34UL
#define CMD_GRADIENT     0xFFFFFF0BUL
#define CMD_INFLATE      0xFFFFFF22UL
#define CMD_INTERRUPT    0xFFFFFF02UL
#define CMD_KEYS         0xFFFFFF0EUL
#define CMD_LOADIDENTITY 0xFFFFFF26UL
#define CMD_LOADIMAGE    0xFFFFFF24UL
#define CMD_LOGO         0xFFFFFF31UL
#define CMD_MEDIAFIFO    0xFFFFFF39UL
#define CMD_MEMCPY       0xFFFFFF1DUL
#define CMD_MEMCRC       0xFFFFFF18UL
#define CMD_MEMSET       0xFFFFFF1BUL
#define CMD_MEMWRITE     0xFFFFFF1AUL
#define CMD_MEMZERO      0xFFFFFF1CUL
#define CMD_NUMBER       0xFFFFFF2EUL
#define CMD_PLAYVIDEO    0xFFFFFF3AUL
#define CMD_PROGRESS     0xFFFFFF0FUL
#define CMD_REGREAD      0xFFFFFF19UL
#define CMD_ROMFONT      0xFFFFFF3FUL
#define CMD_ROTATE       0xFFFFFF29UL
#define CMD_SCALE        0xFFFFFF28UL
#define CMD_SCREENSAVER  0xFFFFFF2FUL
#define CMD_SCROLLBAR    0xFFFFFF11UL
#define CMD_SETBASE      0xFFFFFF38UL
#define CMD_SETBITMAP    0xFFFFFF43UL
#define CMD_SETFONT      0xFFFFFF2BUL
#define CMD_SETFONT2     0xFFFFFF3BUL
#define CMD_SETMATRIX    0xFFFFFF2AUL
#define CMD_SETROTATE    0xFFFFFF36UL
#define CMD_SETSCRATCH   0xFFFFFF3CUL
#define CMD_SKETCH       0xFFFFFF30UL
#define CMD_SLIDER       0xFFFFFF10UL
#define CMD_SNAPSHOT     0xFFFFFF1FUL
#define CMD_SNAPSHOT2    0xFFFFFF37UL
#define CMD_SPINNER      0xFFFFFF16UL
#define CMD_STOP         0xFFFFFF17UL
#define CMD_SWAP         0xFFFFFF01UL
#define CMD_TEXT         0xFFFFFF0CUL
#define CMD_TOGGLE       0xFFFFFF12UL
#define CMD_TRACK        0xFFFFFF2CUL
#define CMD_TRANSLATE    0xFFFFFF27UL
#define CMD_VIDEOFRAME   0xFFFFFF41UL
#define CMD_VIDEOSTART   0xFFFFFF40UL
#define CMD_BITMAP_TRANSFORM 0xFFFFFF21UL
#define CMD_SYNC             0xFFFFFF42UL /* does not need a dedicated function, just use EVE_cmd_dl(CMD_SYNC) */
#define CMD_FLASHERASE       0xFFFFFF44UL /* does not need a dedicated function, just use EVE_cmd_dl(CMD_FLASHERASE) */
#define CMD_FLASHWRITE       0xFFFFFF45UL
#define CMD_FLASHREAD        0xFFFFFF46UL
#define CMD_FLASHUPDATE      0xFFFFFF47UL
#define CMD_FLASHDETACH      0xFFFFFF48UL /* does not need a dedicated function, just use EVE_cmd_dl(CMD_FLASHDETACH) */
#define CMD_FLASHATTACH      0xFFFFFF49UL /* does not need a dedicated function, just use EVE_cmd_dl(CMD_FLASHATTACH) */
#define CMD_FLASHFAST        0xFFFFFF4AUL
#define CMD_FLASHSPIDESEL    0xFFFFFF4BUL /* does not need a dedicated function, just use EVE_cmd_dl(CMD_FLASHSPIDESEL) */
#define CMD_FLASHSPITX       0xFFFFFF4CUL
#define CMD_FLASHSPIRX       0xFFFFFF4DUL
#define CMD_FLASHSOURCE      0xFFFFFF4EUL
#define CMD_CLEARCACHE       0xFFFFFF4FUL /* does not need a dedicated function, just use EVE_cmd_dl(CMD_CLEARCACHE) */
#define CMD_INFLATE2         0xFFFFFF50UL
#define CMD_ROTATEAROUND     0xFFFFFF51UL
#define CMD_RESETFONTS       0xFFFFFF52UL /* does not need a dedicated function, just use EVE_cmd_dl(CMD_RESETFONTS) */
#define CMD_ANIMSTART        0xFFFFFF53UL
#define CMD_ANIMSTOP         0xFFFFFF54UL
#define CMD_ANIMXY           0xFFFFFF55UL
#define CMD_ANIMDRAW         0xFFFFFF56UL
#define CMD_GRADIENTA        0xFFFFFF57UL
#define CMD_FILLWIDTH        0xFFFFFF58UL
#define CMD_APPENDF          0xFFFFFF59UL
#define CMD_ANIMFRAME        0xFFFFFF5AUL
#define CMD_VIDEOSTARTF      0xFFFFFF5FUL
#define CMD_FONTCACHE		 0xFFFFFF6BUL

/* Registers */
#define REG_ANA_COMP         0x00302184UL /* only listed in datasheet */
#define REG_BIST_EN          0x00302174UL /* only listed in datasheet */
#define REG_CLOCK            0x00302008UL
#define REG_CMDB_SPACE       0x00302574UL
#define REG_CMDB_WRITE       0x00302578UL
#define REG_CMD_DL           0x00302100UL
#define REG_CMD_READ         0x003020f8UL
#define REG_CMD_WRITE        0x003020fcUL
#define REG_CPURESET         0x00302020UL
#define REG_CSPREAD          0x00302068UL
#define REG_CTOUCH_EXTENDED  0x00302108UL
#define REG_CTOUCH_TOUCH0_XY 0x00302124UL /* only listed in datasheet */
#define REG_CTOUCH_TOUCH4_X  0x0030216cUL
#define REG_CTOUCH_TOUCH4_Y  0x00302120UL
#define REG_CTOUCH_TOUCH1_XY 0x0030211cUL
#define REG_CTOUCH_TOUCH2_XY 0x0030218cUL
#define REG_CTOUCH_TOUCH3_XY 0x00302190UL
#define REG_TOUCH_CONFIG     0x00302168UL
#define REG_DATESTAMP        0x00302564UL /* only listed in datasheet */
#define REG_DITHER           0x00302060UL
#define REG_DLSWAP           0x00302054UL
#define REG_FRAMES           0x00302004UL
#define REG_FREQUENCY        0x0030200cUL
#define REG_GPIO             0x00302094UL
#define REG_GPIOX            0x0030209cUL
#define REG_GPIOX_DIR        0x00302098UL
#define REG_GPIO_DIR         0x00302090UL
#define REG_HCYCLE           0x0030202cUL
#define REG_HOFFSET          0x00302030UL
#define REG_HSIZE            0x00302034UL
#define REG_HSYNC0           0x00302038UL
#define REG_HSYNC1           0x0030203cUL
#define REG_ID               0x00302000UL
#define REG_INT_EN           0x003020acUL
#define REG_INT_FLAGS        0x003020a8UL
#define REG_INT_MASK         0x003020b0UL
#define REG_MACRO_0          0x003020d8UL
#define REG_MACRO_1          0x003020dcUL
#define REG_MEDIAFIFO_READ   0x00309014UL /* only listed in programmers guide */
#define REG_MEDIAFIFO_WRITE  0x00309018UL /* only listed in programmers guide */
#define REG_OUTBITS          0x0030205cUL
#define REG_PCLK             0x00302070UL
#define REG_PCLK_POL         0x0030206cUL
#define REG_PLAY             0x0030208cUL
#define REG_PLAYBACK_FORMAT  0x003020c4UL
#define REG_PLAYBACK_FREQ    0x003020c0UL
#define REG_PLAYBACK_LENGTH  0x003020b8UL
#define REG_PLAYBACK_LOOP    0x003020c8UL
#define REG_PLAYBACK_PLAY    0x003020ccUL
#define REG_PLAYBACK_READPTR 0x003020bcUL
#define REG_PLAYBACK_START   0x003020b4UL
#define REG_PWM_DUTY         0x003020d4UL
#define REG_PWM_HZ           0x003020d0UL
#define REG_RENDERMODE       0x00302010UL /* only listed in datasheet */
#define REG_ROTATE           0x00302058UL
#define REG_SNAPFORMAT       0x0030201cUL /* only listed in datasheet */
#define REG_SNAPSHOT         0x00302018UL /* only listed in datasheet */
#define REG_SNAPY            0x00302014UL /* only listed in datasheet */
#define REG_SOUND            0x00302088UL
#define REG_SPI_WIDTH        0x00302188UL /* listed with false offset in programmers guide V1.1 */
#define REG_SWIZZLE          0x00302064UL
#define REG_TAG              0x0030207cUL
#define REG_TAG_X            0x00302074UL
#define REG_TAG_Y            0x00302078UL
#define REG_TAP_CRC          0x00302024UL /* only listed in datasheet */
#define REG_TAP_MASK         0x00302028UL /* only listed in datasheet */
#define REG_TOUCH_ADC_MODE   0x00302108UL
#define REG_TOUCH_CHARGE     0x0030210cUL
#define REG_TOUCH_DIRECT_XY  0x0030218cUL
#define REG_TOUCH_DIRECT_Z1Z2 0x00302190UL
#define REG_TOUCH_MODE       0x00302104UL
#define REG_TOUCH_OVERSAMPLE 0x00302114UL
#define REG_TOUCH_RAW_XY     0x0030211cUL
#define REG_TOUCH_RZ         0x00302120UL
#define REG_TOUCH_RZTHRESH   0x00302118UL
#define REG_TOUCH_SCREEN_XY  0x00302124UL
#define REG_TOUCH_SETTLE     0x00302110UL
#define REG_TOUCH_TAG        0x0030212cUL
#define REG_TOUCH_TAG1       0x00302134UL /* only listed in datasheet */
#define REG_TOUCH_TAG1_XY    0x00302130UL /* only listed in datasheet */
#define REG_TOUCH_TAG2       0x0030213cUL /* only listed in datasheet */
#define REG_TOUCH_TAG2_XY    0x00302138UL /* only listed in datasheet */
#define REG_TOUCH_TAG3       0x00302144UL /* only listed in datasheet */
#define REG_TOUCH_TAG3_XY    0x00302140UL /* only listed in datasheet */
#define REG_TOUCH_TAG4       0x0030214cUL /* only listed in datasheet */
#define REG_TOUCH_TAG4_XY    0x00302148UL /* only listed in datasheet */
#define REG_TOUCH_TAG_XY     0x00302128UL
#define REG_TOUCH_TRANSFORM_A 0x00302150UL
#define REG_TOUCH_TRANSFORM_B 0x00302154UL
#define REG_TOUCH_TRANSFORM_C 0x00302158UL
#define REG_TOUCH_TRANSFORM_D 0x0030215cUL
#define REG_TOUCH_TRANSFORM_E 0x00302160UL
#define REG_TOUCH_TRANSFORM_F 0x00302164UL
#define REG_TRACKER          0x00309000UL /* only listed in programmers guide */
#define REG_TRACKER_1        0x00309004UL /* only listed in programmers guide */
#define REG_TRACKER_2        0x00309008UL /* only listed in programmers guide */
#define REG_TRACKER_3        0x0030900cUL /* only listed in programmers guide */
#define REG_TRACKER_4        0x00309010UL /* only listed in programmers guide */
#define REG_TRIM             0x00302180UL
#define REG_VCYCLE           0x00302040UL
#define REG_VOFFSET          0x00302044UL
#define REG_VOL_PB           0x00302080UL
#define REG_VOL_SOUND        0x00302084UL
#define REG_VSIZE            0x00302048UL
#define REG_VSYNC0           0x0030204cUL
#define REG_VSYNC1           0x00302050UL
#define REG_ADAPTIVE_FRAMERATE 0x0030257cUL
#define REG_PLAYBACK_PAUSE     0x003025ecUL
#define REG_FLASH_STATUS       0x003025f0UL
#define REG_FLASH_SIZE         0x00309024UL
#define REG_PLAY_CONTROL       0x0030914eUL
#define REG_COPRO_PATCH_DTR    0x00309162UL

#define DLSWAP_FRAME  		(2UL)

/* Memory */
#define RAM_G         0x00000000UL
#define ROM_CHIPID    0x000C0000UL
#define ROM_FONT      0x001E0000UL
#define ROM_FONT_ADDR 0x002FFFFCUL
#define RAM_DL        0x00300000UL
#define RAM_REG       0x00302000UL
#define RAM_CMD       0x00308000UL

#define ALPHA_FUNC_NEVER	0
#define ALPHA_FUNC_LESS		1
#define ALPHA_FUNC_LEQUAL	2
#define ALPHA_FUNC_GREATER	3
#define ALPHA_FUNC_GEQUAL	4
#define ALPHA_FUNC_EQUAL	5
#define ALPHA_FUNC_NOTEQUAL	6
#define ALPHA_FUNC_ALWAYS	7

#define BT816_ARGB1555		0
#define BT816_L1				1
#define BT816_L4				2
#define BT816_L8				3
#define BT816_RGB332			4
#define BT816_ARGB2			5
#define BT816_ARGB4			6
#define BT816_RGB565			7
#define BT816_TEXT8x8		9
#define BT816_TEXTVGA		10
#define BT816_BARGRAPH		11
#define BT816_PALETTED565	14
#define BT816_PALETTED444	15
#define BT816_PALETTED8		16
#define BT816_L2				17
#define BT816_COMPRESSED_RGBA_ASTC_4x4_KHR	37808
#define BT816_COMPRESSED_RGBA_ASTC_5x4_KHR	37809
#define BT816_COMPRESSED_RGBA_ASTC_5x5_KHR	37810
#define BT816_COMPRESSED_RGBA_ASTC_6x5_KHR	37811
#define BT816_COMPRESSED_RGBA_ASTC_6x6_KHR	37812
#define BT816_COMPRESSED_RGBA_ASTC_8x5_KHR	37813
#define BT816_COMPRESSED_RGBA_ASTC_8x6_KHR	37814
#define BT816_COMPRESSED_RGBA_ASTC_8x8_KHR	37815
#define BT816_COMPRESSED_RGBA_ASTC_10x5_KHR	37816
#define BT816_COMPRESSED_RGBA_ASTC_10x6_KHR	37817
#define BT816_COMPRESSED_RGBA_ASTC_10x8_KHR	37818
#define BT816_COMPRESSED_RGBA_ASTC_10x10_KHR	37819
#define BT816_COMPRESSED_RGBA_ASTC_12x10_KHR	37820
#define BT816_COMPRESSED_RGBA_ASTC_12x12_KHR	37821

#define BLEND_ZERO					0
#define BLEND_ONE					1
#define BLEND_SRC_ALPHA				2
#define BLEND_DST_ALPHA				3
#define BLEND_ONE_MINUS_SRC_ALPHA	4
#define BLEND_ONE_MINUS_DST_ALPHA	5

#define STENCIL_ZERO		0
#define STENCIL_KEEP		1
#define STENCIL_REPLACE		2
#define STENCIL_INCR		3
#define STENCIL_DECR		4
#define STENCIL_INVERT		5

uint8_t bt816_init();

uint32_t bt816_alpha_func(uint8_t func, uint8_t ref);
uint32_t bt816_begin(uint8_t prim);
uint32_t bt816_bitmap_ext_format(uint16_t format);
uint32_t bt816_bitmap_handle(uint8_t handle);
uint32_t bt816_bitmap_layout(uint8_t format, uint16_t linestride, uint16_t height);
uint32_t bt816_bitmap_layout_h(uint8_t linestride, uint8_t height);
uint32_t bt816_bitmap_size(uint8_t filter, uint8_t wrapx, uint8_t wrapy, uint16_t width, uint16_t height);
uint32_t bt816_bitmap_size_h(uint8_t width, uint8_t height);
uint32_t bt816_bitmap_source(uint32_t addr);
uint32_t bt816_bitmap_transform_a(uint8_t p, uint32_t v);
uint32_t bt816_bitmap_transform_b(uint8_t p, uint32_t v);
uint32_t bt816_bitmap_transform_c(uint32_t v);
uint32_t bt816_bitmap_transform_d(uint8_t p, uint32_t v);
uint32_t bt816_bitmap_transform_e(uint8_t p, uint32_t v);
uint32_t bt816_bitmap_transform_f(uint32_t v);
uint32_t bt816_blend_func(uint8_t src, uint8_t dst);
uint32_t bt816_call(uint16_t dest);
uint32_t bt816_cell(uint8_t cell);
uint32_t bt816_clear(uint8_t color_buf, uint8_t stencil_buf, uint8_t tag_buf);
uint32_t bt816_clear_color_a(uint8_t alpha);
uint32_t bt816_clear_color_rgb(uint8_t r, uint8_t g, uint8_t b);
uint32_t bt816_clear_stencil(uint8_t s);
uint32_t bt816_clear_tag(uint8_t t);
uint32_t bt816_color_a(uint8_t alpha);
uint32_t bt816_color_mask(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
uint32_t bt816_color_rgb(uint8_t r, uint8_t g, uint8_t b);
uint32_t bt816_display();
uint32_t bt816_end();
uint32_t bt816_jump(uint16_t dest);
uint32_t bt816_line_width(uint16_t width);
uint32_t bt816_macro(uint8_t m);
uint32_t bt816_nop();
uint32_t bt816_palette_source(uint32_t addr);
uint32_t bt816_point_size(uint16_t size);
uint32_t bt816_restore_context();
uint32_t bt816_return();
uint32_t bt816_save_context();
uint32_t bt816_scissor_size(uint16_t width, uint16_t height);
uint32_t bt816_scissor_xy(uint16_t x, uint16_t y);
uint32_t bt816_stencil_func(uint8_t func, uint8_t ref, uint8_t mask);
uint32_t bt816_stencil_mask(uint8_t mask);
uint32_t bt816_stencil_op(uint8_t sfall, uint8_t spass);
uint32_t bt816_tag(uint8_t s);
uint32_t bt816_tag_mask(uint8_t mask);
uint32_t bt816_vertex2f(uint16_t x, uint16_t y);
uint32_t bt816_vertex2ii(uint16_t x, uint16_t y, uint8_t handle, uint8_t cell);
uint32_t bt816_vertex_format(uint8_t frac);
uint32_t bt816_vertex_translate_x(uint16_t x);
uint32_t bt816_vertex_translate_y(uint16_t y);

uint8_t bt816_mem_read8(uint32_t addr);
uint16_t bt816_mem_read16(uint32_t addr);
uint32_t bt816_mem_read32(uint32_t addr);
void bt816_mem_write8(uint32_t addr, uint8_t value);
void bt816_mem_write16(uint32_t addr, uint16_t value);
void bt816_mem_write32(uint32_t addr, uint32_t value);

void bt816_cmd_dl(uint32_t command);
void bt816_block_transfer(const uint8_t *p_data, uint32_t len);

void bt816_cmd_dlstart();
void bt816_cmd_coldstart();
void bt816_cmd_swap();
void bt816_cmd_append(uint32_t ptr, uint32_t num);
void bt816_mem_write_buffer(uint32_t bt_addr, const uint8_t *data, uint32_t cnt);

void bt816_cmd_flashattach();
void bt816_cmd_flashdetach();
void bt816_cmd_flasherase();
void bt816_cmd_flashspidesel();
void bt816_cmd_flashsource(uint32_t ptr);
void bt816_cmd_flashspirx(uint32_t dest, uint32_t num);
void bt816_cmd_flashspitx(uint32_t num, const uint8_t *p_data);
uint32_t bt816_cmd_flashfast();
void bt816_cmd_flashupdate (uint32_t dest, uint32_t src, uint32_t num);
void bt816_cmd_flashread(uint32_t dest, uint32_t src, uint32_t num);
void bt816_cmd_flashwrite(uint32_t ptr, uint32_t num, const uint8_t *p_data);

void bt816_cmd_inflate(uint32_t ptr, const uint8_t *p_data, uint32_t len);
void bt816_cmd_inflate2(uint32_t ptr, uint32_t options, const uint8_t *p_data, uint32_t len);
void bt816_cmd_getprops(uint32_t *p_pointer, uint32_t *p_width, uint32_t *p_height);
uint32_t bt816_cmd_getptr();
void bt816_cmd_loadimage(uint32_t ptr, uint32_t options, const uint8_t *p_data, uint32_t len);
void bt816_cmd_mediafifo(uint32_t ptr, uint32_t size);
void bt816_cmd_memcpy(uint32_t dest, uint32_t src, uint32_t num);
uint32_t bt816_cmd_memcrc(uint32_t ptr, uint32_t num);
void bt816_cmd_memset(uint32_t ptr, uint8_t value, uint32_t num);
void bt816_cmd_memwrite(uint32_t mem_addr, uint32_t num, const uint8_t *data_ptr);
void bt816_cmd_memzero(uint32_t ptr, uint32_t num);
void bt816_cmd_playvideo(uint32_t options, const uint8_t *p_data, uint32_t len);
void bt816_cmd_setrotate(uint32_t rotation);
void bt816_cmd_snapshot(uint32_t ptr);
void bt816_cmd_snapshot2(uint32_t fmt, uint32_t ptr, int16_t xc0, int16_t yc0, int16_t width, int16_t height);
void bt816_cmd_track(int16_t xc0, int16_t yc0, int16_t width, int16_t height, int16_t tag);
void bt816_cmd_videoframe(uint32_t dest, uint32_t result_ptr);
void bt816_cmd_animdraw(int32_t chnl);
void bt816_cmd_animdraw(int32_t chnl);
void bt816_cmd_animframe(int16_t xc0, int16_t yc0, uint32_t aoptr, uint32_t frame);
void bt816_cmd_animstart(int32_t chnl, uint32_t aoptr, uint32_t loop);
void bt816_cmd_animstop(int32_t chnl);
void bt816_cmd_animxy(int32_t chnl, int16_t xc0, int16_t yc0);
void bt816_cmd_appendf(uint32_t ptr, uint32_t num);
uint16_t bt816_cmd_bitmap_transform(int32_t xc0, int32_t yc0, int32_t xc1,
                                int32_t yc1, int32_t xc2, int32_t yc2,
                                int32_t tx0, int32_t ty0, int32_t tx1,
                                int32_t ty1, int32_t tx2, int32_t ty2);
void bt816_cmd_fillwidth(uint32_t pixel);
void bt816_cmd_gradienta(int16_t xc0, int16_t yc0, uint32_t argb0, int16_t xc1,
                        int16_t yc1, uint32_t argb1);
void bt816_cmd_rotatearound(int32_t xc0, int32_t yc0, int32_t angle, int32_t scale);
void bt816_cmd_button_var(int16_t xc0, int16_t yc0, int16_t width, int16_t height,
                        int16_t font, uint16_t options, const char *p_text,
                        uint8_t num_args, const uint32_t p_arguments[]);
void bt816_cmd_text_var(int16_t xc0, int16_t yc0, int16_t font,
                        uint16_t options, const char *p_text,
                        uint8_t num_args, const uint32_t p_arguments[]);
void bt816_cmd_toggle_var(int16_t xc0, int16_t yc0, int16_t width, int16_t font,
                        uint16_t options, uint16_t state, const char *p_text,
                        uint8_t num_args, const uint32_t p_arguments[]);
void bt816_cmd_bgcolor(uint32_t color);
void bt816_cmd_button(int16_t xc0, int16_t yc0, int16_t width, int16_t height,
                    int16_t font, uint16_t options, const char *p_text);
void bt816_cmd_clock(int16_t xc0, int16_t yc0, int16_t rad, uint16_t options, uint16_t hours,
                    uint16_t mins, uint16_t secs, uint16_t msecs);
void bt816_cmd_dial(int16_t xc0, int16_t yc0, int16_t rad, uint16_t options, uint16_t val);

uint32_t bt816_cmd_regread(uint32_t ptr);
void bt816_cmd_fgcolor(uint32_t color);
void bt816_cmd_gauge(int16_t xc0, int16_t yc0, int16_t rad, uint16_t options,
                    uint16_t major, uint16_t minor,
                    uint16_t val, uint16_t range);
void bt816_cmd_getmatrix(int32_t *p_a, int32_t *p_b, int32_t *p_c,
                        int32_t *p_d, int32_t *p_e, int32_t *p_f);
void bt816_cmd_gradcolor(uint32_t color);
void bt816_cmd_gradient(int16_t xc0, int16_t yc0, uint32_t rgb0, int16_t xc1,
                        int16_t yc1, uint32_t rgb1);
void bt816_cmd_keys(int16_t xc0, int16_t yc0, int16_t wid, int16_t hgt,
                    int16_t font, uint16_t options, const char *p_text);
void bt816_cmd_number(int16_t xc0, int16_t yc0, int16_t font, uint16_t options,
                    int32_t number);
void bt816_cmd_progress(int16_t xc0, int16_t yc0, int16_t wid, int16_t hgt,
                        uint16_t options, uint16_t val, uint16_t range);
void bt816_cmd_romfont(uint32_t font, uint32_t romslot);
void bt816_cmd_rotate(int32_t angle);
void bt816_cmd_scale(int32_t scx, int32_t scy);
void bt816_cmd_scrollbar(int16_t xc0, int16_t yc0, int16_t width, int16_t height,
            uint16_t options, uint16_t val, uint16_t size, uint16_t range);
void bt816_cmd_setbase(uint32_t base);
void bt816_cmd_setbitmap(uint32_t addr, uint16_t fmt, uint16_t width,
                        uint16_t height);
void bt816_cmd_setfont(uint32_t font, uint32_t ptr);
void bt816_cmd_setfont2(uint32_t font, uint32_t ptr, uint32_t firstchar);
void bt816_cmd_setscratch(uint32_t handle);
void bt816_cmd_sketch(int16_t xc0, int16_t yc0, uint16_t width, uint16_t height,
                    uint32_t ptr, uint16_t format);
void bt816_cmd_slider(int16_t xc0, int16_t yc0, int16_t wid, int16_t hgt,
                    uint16_t options, uint16_t val, uint16_t range);
void bt816_cmd_spinner(int16_t xc0, int16_t yc0, uint16_t style, uint16_t scale);
void bt816_cmd_text(int16_t xc0, int16_t yc0, int16_t font, uint16_t options,
                    const char *p_text);
void bt816_cmd_toggle(int16_t xc0, int16_t yc0, int16_t width, int16_t font,
                    uint16_t options, uint16_t state, const char *p_text);
void bt816_cmd_translate(int32_t tr_x, int32_t tr_y);
void bt816_cmd_fontcache(uint32_t font, uint32_t ram_addr, uint32_t cnt);

uint8_t bt816_init();

#endif /* INCLUDE_BT816_CMD_H_ */
