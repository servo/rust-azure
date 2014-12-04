/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_NVPR_GLDEFS_H_
#define MOZILLA_GFX_NVPR_GLDEFS_H_

#include <stdint.h>

#ifdef WIN32
#define GLAPIENTRY __stdcall
#else
#define GLAPIENTRY
#endif

// OpenGL 1.1
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef signed char GLbyte;   /* 1-byte signed */
typedef short GLshort;  /* 2-byte signed */
typedef int GLint;    /* 4-byte signed */
typedef unsigned char GLubyte;  /* 1-byte unsigned */
typedef unsigned short GLushort; /* 2-byte unsigned */
typedef unsigned int GLuint;   /* 4-byte unsigned */
typedef int GLsizei;  /* 4-byte signed */
typedef float GLfloat;  /* single precision float */
typedef float GLclampf; /* single precision float in [0,1] */
typedef double GLdouble; /* double precision float */
typedef double GLclampd; /* double precision float in [0,1] */

#define GL_FALSE 0
#define GL_TRUE 1
#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_2_BYTES 0x1407
#define GL_3_BYTES 0x1408
#define GL_4_BYTES 0x1409
#define GL_DOUBLE 0x140A
#define GL_POINTS 0x0000
#define GL_LINES 0x0001
#define GL_LINE_LOOP 0x0002
#define GL_LINE_STRIP 0x0003
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_QUADS 0x0007
#define GL_QUAD_STRIP 0x0008
#define GL_POLYGON 0x0009
#define GL_VERTEX_ARRAY 0x8074
#define GL_NORMAL_ARRAY 0x8075
#define GL_COLOR_ARRAY 0x8076
#define GL_INDEX_ARRAY 0x8077
#define GL_TEXTURE_COORD_ARRAY 0x8078
#define GL_EDGE_FLAG_ARRAY 0x8079
#define GL_VERTEX_ARRAY_SIZE 0x807A
#define GL_VERTEX_ARRAY_TYPE 0x807B
#define GL_VERTEX_ARRAY_STRIDE 0x807C
#define GL_NORMAL_ARRAY_TYPE 0x807E
#define GL_NORMAL_ARRAY_STRIDE 0x807F
#define GL_COLOR_ARRAY_SIZE 0x8081
#define GL_COLOR_ARRAY_TYPE 0x8082
#define GL_COLOR_ARRAY_STRIDE 0x8083
#define GL_INDEX_ARRAY_TYPE 0x8085
#define GL_INDEX_ARRAY_STRIDE 0x8086
#define GL_TEXTURE_COORD_ARRAY_SIZE 0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE 0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE 0x808A
#define GL_EDGE_FLAG_ARRAY_STRIDE 0x808C
#define GL_VERTEX_ARRAY_POINTER 0x808E
#define GL_NORMAL_ARRAY_POINTER 0x808F
#define GL_COLOR_ARRAY_POINTER 0x8090
#define GL_INDEX_ARRAY_POINTER 0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER 0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER 0x8093
#define GL_V2F 0x2A20
#define GL_V3F 0x2A21
#define GL_C4UB_V2F 0x2A22
#define GL_C4UB_V3F 0x2A23
#define GL_C3F_V3F 0x2A24
#define GL_N3F_V3F 0x2A25
#define GL_C4F_N3F_V3F 0x2A26
#define GL_T2F_V3F 0x2A27
#define GL_T4F_V4F 0x2A28
#define GL_T2F_C4UB_V3F 0x2A29
#define GL_T2F_C3F_V3F 0x2A2A
#define GL_T2F_N3F_V3F 0x2A2B
#define GL_T2F_C4F_N3F_V3F 0x2A2C
#define GL_T4F_C4F_N3F_V4F 0x2A2D
#define GL_MATRIX_MODE 0x0BA0
#define GL_MODELVIEW 0x1700
#define GL_PROJECTION 0x1701
#define GL_TEXTURE 0x1702
#define GL_POINT_SMOOTH 0x0B10
#define GL_POINT_SIZE 0x0B11
#define GL_POINT_SIZE_GRANULARITY 0x0B13
#define GL_POINT_SIZE_RANGE 0x0B12
#define GL_LINE_SMOOTH 0x0B20
#define GL_LINE_STIPPLE 0x0B24
#define GL_LINE_STIPPLE_PATTERN 0x0B25
#define GL_LINE_STIPPLE_REPEAT 0x0B26
#define GL_LINE_WIDTH 0x0B21
#define GL_LINE_WIDTH_GRANULARITY 0x0B23
#define GL_LINE_WIDTH_RANGE 0x0B22
#define GL_POINT 0x1B00
#define GL_LINE 0x1B01
#define GL_FILL 0x1B02
#define GL_CW 0x0900
#define GL_CCW 0x0901
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_POLYGON_MODE 0x0B40
#define GL_POLYGON_SMOOTH 0x0B41
#define GL_POLYGON_STIPPLE 0x0B42
#define GL_EDGE_FLAG 0x0B43
#define GL_CULL_FACE 0x0B44
#define GL_CULL_FACE_MODE 0x0B45
#define GL_FRONT_FACE 0x0B46
#define GL_POLYGON_OFFSET_FACTOR 0x8038
#define GL_POLYGON_OFFSET_UNITS 0x2A00
#define GL_POLYGON_OFFSET_POINT 0x2A01
#define GL_POLYGON_OFFSET_LINE 0x2A02
#define GL_POLYGON_OFFSET_FILL 0x8037
#define GL_COMPILE 0x1300
#define GL_COMPILE_AND_EXECUTE 0x1301
#define GL_LIST_BASE 0x0B32
#define GL_LIST_INDEX 0x0B33
#define GL_LIST_MODE 0x0B30
#define GL_NEVER 0x0200
#define GL_LESS 0x0201
#define GL_EQUAL 0x0202
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL 0x0206
#define GL_ALWAYS 0x0207
#define GL_DEPTH_TEST 0x0B71
#define GL_DEPTH_BITS 0x0D56
#define GL_DEPTH_CLEAR_VALUE 0x0B73
#define GL_DEPTH_FUNC 0x0B74
#define GL_DEPTH_RANGE 0x0B70
#define GL_DEPTH_WRITEMASK 0x0B72
#define GL_DEPTH_COMPONENT 0x1902
#define GL_LIGHTING 0x0B50
#define GL_LIGHT0 0x4000
#define GL_LIGHT1 0x4001
#define GL_LIGHT2 0x4002
#define GL_LIGHT3 0x4003
#define GL_LIGHT4 0x4004
#define GL_LIGHT5 0x4005
#define GL_LIGHT6 0x4006
#define GL_LIGHT7 0x4007
#define GL_SPOT_EXPONENT 0x1205
#define GL_SPOT_CUTOFF 0x1206
#define GL_CONSTANT_ATTENUATION 0x1207
#define GL_LINEAR_ATTENUATION 0x1208
#define GL_QUADRATIC_ATTENUATION 0x1209
#define GL_AMBIENT 0x1200
#define GL_DIFFUSE 0x1201
#define GL_SPECULAR 0x1202
#define GL_SHININESS 0x1601
#define GL_EMISSION 0x1600
#define GL_POSITION 0x1203
#define GL_SPOT_DIRECTION 0x1204
#define GL_AMBIENT_AND_DIFFUSE 0x1602
#define GL_COLOR_INDEXES 0x1603
#define GL_LIGHT_MODEL_TWO_SIDE 0x0B52
#define GL_LIGHT_MODEL_LOCAL_VIEWER 0x0B51
#define GL_LIGHT_MODEL_AMBIENT 0x0B53
#define GL_FRONT_AND_BACK 0x0408
#define GL_SHADE_MODEL 0x0B54
#define GL_FLAT 0x1D00
#define GL_SMOOTH 0x1D01
#define GL_COLOR_MATERIAL 0x0B57
#define GL_COLOR_MATERIAL_FACE 0x0B55
#define GL_COLOR_MATERIAL_PARAMETER 0x0B56
#define GL_NORMALIZE 0x0BA1
#define GL_CLIP_PLANE0 0x3000
#define GL_CLIP_PLANE1 0x3001
#define GL_CLIP_PLANE2 0x3002
#define GL_CLIP_PLANE3 0x3003
#define GL_CLIP_PLANE4 0x3004
#define GL_CLIP_PLANE5 0x3005
#define GL_ACCUM_RED_BITS 0x0D58
#define GL_ACCUM_GREEN_BITS 0x0D59
#define GL_ACCUM_BLUE_BITS 0x0D5A
#define GL_ACCUM_ALPHA_BITS 0x0D5B
#define GL_ACCUM_CLEAR_VALUE 0x0B80
#define GL_ACCUM 0x0100
#define GL_ADD 0x0104
#define GL_LOAD 0x0101
#define GL_MULT 0x0103
#define GL_RETURN 0x0102
#define GL_ALPHA_TEST 0x0BC0
#define GL_ALPHA_TEST_REF 0x0BC2
#define GL_ALPHA_TEST_FUNC 0x0BC1
#define GL_BLEND 0x0BE2
#define GL_BLEND_SRC 0x0BE1
#define GL_BLEND_DST 0x0BE0
#define GL_ZERO 0
#define GL_ONE 1
#define GL_SRC_COLOR 0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA 0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_DST_COLOR 0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE 0x0308
#define GL_FEEDBACK 0x1C01
#define GL_RENDER 0x1C00
#define GL_SELECT 0x1C02
#define GL_2D 0x0600
#define GL_3D 0x0601
#define GL_3D_COLOR 0x0602
#define GL_3D_COLOR_TEXTURE 0x0603
#define GL_4D_COLOR_TEXTURE 0x0604
#define GL_POINT_TOKEN 0x0701
#define GL_LINE_TOKEN 0x0702
#define GL_LINE_RESET_TOKEN 0x0707
#define GL_POLYGON_TOKEN 0x0703
#define GL_BITMAP_TOKEN 0x0704
#define GL_DRAW_PIXEL_TOKEN 0x0705
#define GL_COPY_PIXEL_TOKEN 0x0706
#define GL_PASS_THROUGH_TOKEN 0x0700
#define GL_FEEDBACK_BUFFER_POINTER 0x0DF0
#define GL_FEEDBACK_BUFFER_SIZE 0x0DF1
#define GL_FEEDBACK_BUFFER_TYPE 0x0DF2
#define GL_SELECTION_BUFFER_POINTER 0x0DF3
#define GL_SELECTION_BUFFER_SIZE 0x0DF4
#define GL_FOG 0x0B60
#define GL_FOG_MODE 0x0B65
#define GL_FOG_DENSITY 0x0B62
#define GL_FOG_COLOR 0x0B66
#define GL_FOG_INDEX 0x0B61
#define GL_FOG_START 0x0B63
#define GL_FOG_END 0x0B64
#define GL_LINEAR 0x2601
#define GL_EXP 0x0800
#define GL_EXP2 0x0801
#define GL_LOGIC_OP 0x0BF1
#define GL_INDEX_LOGIC_OP 0x0BF1
#define GL_COLOR_LOGIC_OP 0x0BF2
#define GL_LOGIC_OP_MODE 0x0BF0
#define GL_CLEAR 0x1500
#define GL_SET 0x150F
#define GL_COPY 0x1503
#define GL_COPY_INVERTED 0x150C
#define GL_NOOP 0x1505
#define GL_INVERT 0x150A
#define GL_AND 0x1501
#define GL_NAND 0x150E
#define GL_OR 0x1507
#define GL_NOR 0x1508
#define GL_XOR 0x1506
#define GL_EQUIV 0x1509
#define GL_AND_REVERSE 0x1502
#define GL_AND_INVERTED 0x1504
#define GL_OR_REVERSE 0x150B
#define GL_OR_INVERTED 0x150D
#define GL_STENCIL_BITS 0x0D57
#define GL_STENCIL_TEST 0x0B90
#define GL_STENCIL_CLEAR_VALUE 0x0B91
#define GL_STENCIL_FUNC 0x0B92
#define GL_STENCIL_VALUE_MASK 0x0B93
#define GL_STENCIL_FAIL 0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL 0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS 0x0B96
#define GL_STENCIL_REF 0x0B97
#define GL_STENCIL_WRITEMASK 0x0B98
#define GL_STENCIL_INDEX 0x1901
#define GL_KEEP 0x1E00
#define GL_REPLACE 0x1E01
#define GL_INCR 0x1E02
#define GL_DECR 0x1E03
#define GL_NONE 0
#define GL_LEFT 0x0406
#define GL_RIGHT 0x0407
#define GL_FRONT_LEFT 0x0400
#define GL_FRONT_RIGHT 0x0401
#define GL_BACK_LEFT 0x0402
#define GL_BACK_RIGHT 0x0403
#define GL_AUX0 0x0409
#define GL_AUX1 0x040A
#define GL_AUX2 0x040B
#define GL_AUX3 0x040C
#define GL_COLOR_INDEX 0x1900
#define GL_RED 0x1903
#define GL_GREEN 0x1904
#define GL_BLUE 0x1905
#define GL_ALPHA 0x1906
#define GL_LUMINANCE 0x1909
#define GL_LUMINANCE_ALPHA 0x190A
#define GL_ALPHA_BITS 0x0D55
#define GL_RED_BITS 0x0D52
#define GL_GREEN_BITS 0x0D53
#define GL_BLUE_BITS 0x0D54
#define GL_INDEX_BITS 0x0D51
#define GL_SUBPIXEL_BITS 0x0D50
#define GL_AUX_BUFFERS 0x0C00
#define GL_READ_BUFFER 0x0C02
#define GL_DRAW_BUFFER 0x0C01
#define GL_DOUBLEBUFFER 0x0C32
#define GL_STEREO 0x0C33
#define GL_BITMAP 0x1A00
#define GL_COLOR 0x1800
#define GL_DEPTH 0x1801
#define GL_STENCIL 0x1802
#define GL_DITHER 0x0BD0
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_MAX_LIST_NESTING 0x0B31
#define GL_MAX_EVAL_ORDER 0x0D30
#define GL_MAX_LIGHTS 0x0D31
#define GL_MAX_CLIP_PLANES 0x0D32
#define GL_MAX_TEXTURE_SIZE 0x0D33
#define GL_MAX_PIXEL_MAP_TABLE 0x0D34
#define GL_MAX_ATTRIB_STACK_DEPTH 0x0D35
#define GL_MAX_MODELVIEW_STACK_DEPTH 0x0D36
#define GL_MAX_NAME_STACK_DEPTH 0x0D37
#define GL_MAX_PROJECTION_STACK_DEPTH 0x0D38
#define GL_MAX_TEXTURE_STACK_DEPTH 0x0D39
#define GL_MAX_VIEWPORT_DIMS 0x0D3A
#define GL_MAX_CLIENT_ATTRIB_STACK_DEPTH 0x0D3B
#define GL_ATTRIB_STACK_DEPTH 0x0BB0
#define GL_CLIENT_ATTRIB_STACK_DEPTH 0x0BB1
#define GL_COLOR_CLEAR_VALUE 0x0C22
#define GL_COLOR_WRITEMASK 0x0C23
#define GL_CURRENT_INDEX 0x0B01
#define GL_CURRENT_COLOR 0x0B00
#define GL_CURRENT_NORMAL 0x0B02
#define GL_CURRENT_RASTER_COLOR 0x0B04
#define GL_CURRENT_RASTER_DISTANCE 0x0B09
#define GL_CURRENT_RASTER_INDEX 0x0B05
#define GL_CURRENT_RASTER_POSITION 0x0B07
#define GL_CURRENT_RASTER_TEXTURE_COORDS 0x0B06
#define GL_CURRENT_RASTER_POSITION_VALID 0x0B08
#define GL_CURRENT_TEXTURE_COORDS 0x0B03
#define GL_INDEX_CLEAR_VALUE 0x0C20
#define GL_INDEX_MODE 0x0C30
#define GL_INDEX_WRITEMASK 0x0C21
#define GL_MODELVIEW_MATRIX 0x0BA6
#define GL_MODELVIEW_STACK_DEPTH 0x0BA3
#define GL_NAME_STACK_DEPTH 0x0D70
#define GL_PROJECTION_MATRIX 0x0BA7
#define GL_PROJECTION_STACK_DEPTH 0x0BA4
#define GL_RENDER_MODE 0x0C40
#define GL_RGBA_MODE 0x0C31
#define GL_TEXTURE_MATRIX 0x0BA8
#define GL_TEXTURE_STACK_DEPTH 0x0BA5
#define GL_VIEWPORT 0x0BA2
#define GL_AUTO_NORMAL 0x0D80
#define GL_MAP1_COLOR_4 0x0D90
#define GL_MAP1_INDEX 0x0D91
#define GL_MAP1_NORMAL 0x0D92
#define GL_MAP1_TEXTURE_COORD_1 0x0D93
#define GL_MAP1_TEXTURE_COORD_2 0x0D94
#define GL_MAP1_TEXTURE_COORD_3 0x0D95
#define GL_MAP1_TEXTURE_COORD_4 0x0D96
#define GL_MAP1_VERTEX_3 0x0D97
#define GL_MAP1_VERTEX_4 0x0D98
#define GL_MAP2_COLOR_4 0x0DB0
#define GL_MAP2_INDEX 0x0DB1
#define GL_MAP2_NORMAL 0x0DB2
#define GL_MAP2_TEXTURE_COORD_1 0x0DB3
#define GL_MAP2_TEXTURE_COORD_2 0x0DB4
#define GL_MAP2_TEXTURE_COORD_3 0x0DB5
#define GL_MAP2_TEXTURE_COORD_4 0x0DB6
#define GL_MAP2_VERTEX_3 0x0DB7
#define GL_MAP2_VERTEX_4 0x0DB8
#define GL_MAP1_GRID_DOMAIN 0x0DD0
#define GL_MAP1_GRID_SEGMENTS 0x0DD1
#define GL_MAP2_GRID_DOMAIN 0x0DD2
#define GL_MAP2_GRID_SEGMENTS 0x0DD3
#define GL_COEFF 0x0A00
#define GL_ORDER 0x0A01
#define GL_DOMAIN 0x0A02
#define GL_PERSPECTIVE_CORRECTION_HINT 0x0C50
#define GL_POINT_SMOOTH_HINT 0x0C51
#define GL_LINE_SMOOTH_HINT 0x0C52
#define GL_POLYGON_SMOOTH_HINT 0x0C53
#define GL_FOG_HINT 0x0C54
#define GL_DONT_CARE 0x1100
#define GL_FASTEST 0x1101
#define GL_NICEST 0x1102
#define GL_SCISSOR_BOX 0x0C10
#define GL_SCISSOR_TEST 0x0C11
#define GL_MAP_COLOR 0x0D10
#define GL_MAP_STENCIL 0x0D11
#define GL_INDEX_SHIFT 0x0D12
#define GL_INDEX_OFFSET 0x0D13
#define GL_RED_SCALE 0x0D14
#define GL_RED_BIAS 0x0D15
#define GL_GREEN_SCALE 0x0D18
#define GL_GREEN_BIAS 0x0D19
#define GL_BLUE_SCALE 0x0D1A
#define GL_BLUE_BIAS 0x0D1B
#define GL_ALPHA_SCALE 0x0D1C
#define GL_ALPHA_BIAS 0x0D1D
#define GL_DEPTH_SCALE 0x0D1E
#define GL_DEPTH_BIAS 0x0D1F
#define GL_PIXEL_MAP_S_TO_S_SIZE 0x0CB1
#define GL_PIXEL_MAP_I_TO_I_SIZE 0x0CB0
#define GL_PIXEL_MAP_I_TO_R_SIZE 0x0CB2
#define GL_PIXEL_MAP_I_TO_G_SIZE 0x0CB3
#define GL_PIXEL_MAP_I_TO_B_SIZE 0x0CB4
#define GL_PIXEL_MAP_I_TO_A_SIZE 0x0CB5
#define GL_PIXEL_MAP_R_TO_R_SIZE 0x0CB6
#define GL_PIXEL_MAP_G_TO_G_SIZE 0x0CB7
#define GL_PIXEL_MAP_B_TO_B_SIZE 0x0CB8
#define GL_PIXEL_MAP_A_TO_A_SIZE 0x0CB9
#define GL_PIXEL_MAP_S_TO_S 0x0C71
#define GL_PIXEL_MAP_I_TO_I 0x0C70
#define GL_PIXEL_MAP_I_TO_R 0x0C72
#define GL_PIXEL_MAP_I_TO_G 0x0C73
#define GL_PIXEL_MAP_I_TO_B 0x0C74
#define GL_PIXEL_MAP_I_TO_A 0x0C75
#define GL_PIXEL_MAP_R_TO_R 0x0C76
#define GL_PIXEL_MAP_G_TO_G 0x0C77
#define GL_PIXEL_MAP_B_TO_B 0x0C78
#define GL_PIXEL_MAP_A_TO_A 0x0C79
#define GL_PACK_ALIGNMENT 0x0D05
#define GL_PACK_LSB_FIRST 0x0D01
#define GL_PACK_ROW_LENGTH 0x0D02
#define GL_PACK_SKIP_PIXELS 0x0D04
#define GL_PACK_SKIP_ROWS 0x0D03
#define GL_PACK_SWAP_BYTES 0x0D00
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_UNPACK_LSB_FIRST 0x0CF1
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#define GL_UNPACK_SKIP_PIXELS 0x0CF4
#define GL_UNPACK_SKIP_ROWS 0x0CF3
#define GL_UNPACK_SWAP_BYTES 0x0CF0
#define GL_ZOOM_X 0x0D16
#define GL_ZOOM_Y 0x0D17
#define GL_TEXTURE_ENV 0x2300
#define GL_TEXTURE_ENV_MODE 0x2200
#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_ENV_COLOR 0x2201
#define GL_TEXTURE_GEN_S 0x0C60
#define GL_TEXTURE_GEN_T 0x0C61
#define GL_TEXTURE_GEN_R 0x0C62
#define GL_TEXTURE_GEN_Q 0x0C63
#define GL_TEXTURE_GEN_MODE 0x2500
#define GL_TEXTURE_BORDER_COLOR 0x1004
#define GL_TEXTURE_WIDTH 0x1000
#define GL_TEXTURE_HEIGHT 0x1001
#define GL_TEXTURE_BORDER 0x1005
#define GL_TEXTURE_COMPONENTS 0x1003
#define GL_TEXTURE_RED_SIZE 0x805C
#define GL_TEXTURE_GREEN_SIZE 0x805D
#define GL_TEXTURE_BLUE_SIZE 0x805E
#define GL_TEXTURE_ALPHA_SIZE 0x805F
#define GL_TEXTURE_LUMINANCE_SIZE 0x8060
#define GL_TEXTURE_INTENSITY_SIZE 0x8061
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_OBJECT_LINEAR 0x2401
#define GL_OBJECT_PLANE 0x2501
#define GL_EYE_LINEAR 0x2400
#define GL_EYE_PLANE 0x2502
#define GL_SPHERE_MAP 0x2402
#define GL_DECAL 0x2101
#define GL_MODULATE 0x2100
#define GL_NEAREST 0x2600
#define GL_REPEAT 0x2901
#define GL_CLAMP 0x2900
#define GL_S 0x2000
#define GL_T 0x2001
#define GL_R 0x2002
#define GL_Q 0x2003
#define GL_VENDOR 0x1F00
#define GL_RENDERER 0x1F01
#define GL_VERSION 0x1F02
#define GL_EXTENSIONS 0x1F03
#define GL_NO_ERROR 0
#define GL_INVALID_ENUM 0x0500
#define GL_INVALID_VALUE 0x0501
#define GL_INVALID_OPERATION 0x0502
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504
#define GL_OUT_OF_MEMORY 0x0505
#define GL_CURRENT_BIT 0x00000001
#define GL_POINT_BIT 0x00000002
#define GL_LINE_BIT 0x00000004
#define GL_POLYGON_BIT 0x00000008
#define GL_POLYGON_STIPPLE_BIT 0x00000010
#define GL_PIXEL_MODE_BIT 0x00000020
#define GL_LIGHTING_BIT 0x00000040
#define GL_FOG_BIT 0x00000080
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_ACCUM_BUFFER_BIT 0x00000200
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_VIEWPORT_BIT 0x00000800
#define GL_TRANSFORM_BIT 0x00001000
#define GL_ENABLE_BIT 0x00002000
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_HINT_BIT 0x00008000
#define GL_EVAL_BIT 0x00010000
#define GL_LIST_BIT 0x00020000
#define GL_TEXTURE_BIT 0x00040000
#define GL_SCISSOR_BIT 0x00080000
#define GL_ALL_ATTRIB_BITS 0x000FFFFF
#define GL_PROXY_TEXTURE_1D 0x8063
#define GL_PROXY_TEXTURE_2D 0x8064
#define GL_TEXTURE_PRIORITY 0x8066
#define GL_TEXTURE_RESIDENT 0x8067
#define GL_TEXTURE_BINDING_1D 0x8068
#define GL_TEXTURE_BINDING_2D 0x8069
#define GL_TEXTURE_INTERNAL_FORMAT 0x1003
#define GL_ALPHA4 0x803B
#define GL_ALPHA8 0x803C
#define GL_ALPHA12 0x803D
#define GL_ALPHA16 0x803E
#define GL_LUMINANCE4 0x803F
#define GL_LUMINANCE8 0x8040
#define GL_LUMINANCE12 0x8041
#define GL_LUMINANCE16 0x8042
#define GL_LUMINANCE4_ALPHA4 0x8043
#define GL_LUMINANCE6_ALPHA2 0x8044
#define GL_LUMINANCE8_ALPHA8 0x8045
#define GL_LUMINANCE12_ALPHA4 0x8046
#define GL_LUMINANCE12_ALPHA12 0x8047
#define GL_LUMINANCE16_ALPHA16 0x8048
#define GL_INTENSITY 0x8049
#define GL_INTENSITY4 0x804A
#define GL_INTENSITY8 0x804B
#define GL_INTENSITY12 0x804C
#define GL_INTENSITY16 0x804D
#define GL_R3_G3_B2 0x2A10
#define GL_RGB4 0x804F
#define GL_RGB5 0x8050
#define GL_RGB8 0x8051
#define GL_RGB10 0x8052
#define GL_RGB12 0x8053
#define GL_RGB16 0x8054
#define GL_RGBA2 0x8055
#define GL_RGBA4 0x8056
#define GL_RGB5_A1 0x8057
#define GL_RGBA8 0x8058
#define GL_RGB10_A2 0x8059
#define GL_RGBA12 0x805A
#define GL_RGBA16 0x805B
#define GL_CLIENT_PIXEL_STORE_BIT 0x00000001
#define GL_CLIENT_VERTEX_ARRAY_BIT 0x00000002
#define GL_ALL_CLIENT_ATTRIB_BITS 0xFFFFFFFF
#define GL_CLIENT_ALL_ATTRIB_BITS 0xFFFFFFFF

typedef void (GLAPIENTRY * GLClearIndex) (GLfloat c);
typedef void (GLAPIENTRY * GLClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void (GLAPIENTRY * GLClear) (GLbitfield mask);
typedef void (GLAPIENTRY * GLIndexMask) (GLuint mask);
typedef void (GLAPIENTRY * GLColorMask) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void (GLAPIENTRY * GLAlphaFunc) (GLenum func, GLclampf ref);
typedef void (GLAPIENTRY * GLBlendFunc) (GLenum sfactor, GLenum dfactor);
typedef void (GLAPIENTRY * GLLogicOp) (GLenum opcode);
typedef void (GLAPIENTRY * GLCullFace) (GLenum mode);
typedef void (GLAPIENTRY * GLFrontFace) (GLenum mode);
typedef void (GLAPIENTRY * GLPointSize) (GLfloat size);
typedef void (GLAPIENTRY * GLLineWidth) (GLfloat width);
typedef void (GLAPIENTRY * GLLineStipple) (GLint factor, GLushort pattern);
typedef void (GLAPIENTRY * GLPolygonMode) (GLenum face, GLenum mode);
typedef void (GLAPIENTRY * GLPolygonOffset) (GLfloat factor, GLfloat units);
typedef void (GLAPIENTRY * GLPolygonStipple) (const GLubyte *mask);
typedef void (GLAPIENTRY * GLGetPolygonStipple) (GLubyte *mask);
typedef void (GLAPIENTRY * GLEdgeFlag) (GLboolean flag);
typedef void (GLAPIENTRY * GLEdgeFlagv) (const GLboolean *flag);
typedef void (GLAPIENTRY * GLScissor) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * GLClipPlane) (GLenum plane, const GLdouble *equation);
typedef void (GLAPIENTRY * GLGetClipPlane) (GLenum plane, GLdouble *equation);
typedef void (GLAPIENTRY * GLDrawBuffer) (GLenum mode);
typedef void (GLAPIENTRY * GLReadBuffer) (GLenum mode);
typedef void (GLAPIENTRY * GLEnable) (GLenum cap);
typedef void (GLAPIENTRY * GLDisable) (GLenum cap);
typedef GLboolean (GLAPIENTRY * GLIsEnabled) (GLenum cap);
typedef void (GLAPIENTRY * GLEnableClientState) (GLenum cap);
typedef void (GLAPIENTRY * GLDisableClientState) (GLenum cap);
typedef void (GLAPIENTRY * GLGetBooleanv) (GLenum pname, GLboolean *params);
typedef void (GLAPIENTRY * GLGetDoublev) (GLenum pname, GLdouble *params);
typedef void (GLAPIENTRY * GLGetFloatv) (GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * GLGetIntegerv) (GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLPushAttrib) (GLbitfield mask);
typedef void (GLAPIENTRY * GLPopAttrib) (void);
typedef void (GLAPIENTRY * GLPushClientAttrib) (GLbitfield mask);
typedef void (GLAPIENTRY * GLPopClientAttrib) (void);
typedef GLint (GLAPIENTRY * GLRenderMode) (GLenum mode);
typedef GLenum (GLAPIENTRY * GLGetError) (void);
typedef const GLubyte * (GLAPIENTRY * GLGetString) (GLenum name);
typedef void (GLAPIENTRY * GLFinish) (void);
typedef void (GLAPIENTRY * GLFlush) (void);
typedef void (GLAPIENTRY * GLHint) (GLenum target, GLenum mode);
typedef void (GLAPIENTRY * GLClearDepth) (GLclampd depth);
typedef void (GLAPIENTRY * GLDepthFunc) (GLenum func);
typedef void (GLAPIENTRY * GLDepthMask) (GLboolean flag);
typedef void (GLAPIENTRY * GLDepthRange) (GLclampd near_val, GLclampd far_val);
typedef void (GLAPIENTRY * GLClearAccum) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (GLAPIENTRY * GLAccum) (GLenum op, GLfloat value);
typedef void (GLAPIENTRY * GLMatrixMode) (GLenum mode);
typedef void (GLAPIENTRY * GLOrtho) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val);
typedef void (GLAPIENTRY * GLFrustum) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val);
typedef void (GLAPIENTRY * GLViewport) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * GLPushMatrix) (void);
typedef void (GLAPIENTRY * GLPopMatrix) (void);
typedef void (GLAPIENTRY * GLLoadIdentity) (void);
typedef void (GLAPIENTRY * GLLoadMatrixd) (const GLdouble *m);
typedef void (GLAPIENTRY * GLLoadMatrixf) (const GLfloat *m);
typedef void (GLAPIENTRY * GLMultMatrixd) (const GLdouble *m);
typedef void (GLAPIENTRY * GLMultMatrixf) (const GLfloat *m);
typedef void (GLAPIENTRY * GLRotated) (GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * GLRotatef) (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * GLScaled) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * GLScalef) (GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * GLTranslated) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * GLTranslatef) (GLfloat x, GLfloat y, GLfloat z);
typedef GLboolean (GLAPIENTRY * GLIsList) (GLuint list);
typedef void (GLAPIENTRY * GLDeleteLists) (GLuint list, GLsizei range);
typedef GLuint (GLAPIENTRY * GLGenLists) (GLsizei range);
typedef void (GLAPIENTRY * GLNewList) (GLuint list, GLenum mode);
typedef void (GLAPIENTRY * GLEndList) (void);
typedef void (GLAPIENTRY * GLCallList) (GLuint list);
typedef void (GLAPIENTRY * GLCallLists) (GLsizei n, GLenum type, const GLvoid *lists);
typedef void (GLAPIENTRY * GLListBase) (GLuint base);
typedef void (GLAPIENTRY * GLBegin) (GLenum mode);
typedef void (GLAPIENTRY * GLEnd) (void);
typedef void (GLAPIENTRY * GLVertex2d) (GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * GLVertex2f) (GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * GLVertex2i) (GLint x, GLint y);
typedef void (GLAPIENTRY * GLVertex2s) (GLshort x, GLshort y);
typedef void (GLAPIENTRY * GLVertex3d) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * GLVertex3f) (GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * GLVertex3i) (GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * GLVertex3s) (GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * GLVertex4d) (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * GLVertex4f) (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * GLVertex4i) (GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * GLVertex4s) (GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * GLVertex2dv) (const GLdouble *v);
typedef void (GLAPIENTRY * GLVertex2fv) (const GLfloat *v);
typedef void (GLAPIENTRY * GLVertex2iv) (const GLint *v);
typedef void (GLAPIENTRY * GLVertex2sv) (const GLshort *v);
typedef void (GLAPIENTRY * GLVertex3dv) (const GLdouble *v);
typedef void (GLAPIENTRY * GLVertex3fv) (const GLfloat *v);
typedef void (GLAPIENTRY * GLVertex3iv) (const GLint *v);
typedef void (GLAPIENTRY * GLVertex3sv) (const GLshort *v);
typedef void (GLAPIENTRY * GLVertex4dv) (const GLdouble *v);
typedef void (GLAPIENTRY * GLVertex4fv) (const GLfloat *v);
typedef void (GLAPIENTRY * GLVertex4iv) (const GLint *v);
typedef void (GLAPIENTRY * GLVertex4sv) (const GLshort *v);
typedef void (GLAPIENTRY * GLNormal3b) (GLbyte nx, GLbyte ny, GLbyte nz);
typedef void (GLAPIENTRY * GLNormal3d) (GLdouble nx, GLdouble ny, GLdouble nz);
typedef void (GLAPIENTRY * GLNormal3f) (GLfloat nx, GLfloat ny, GLfloat nz);
typedef void (GLAPIENTRY * GLNormal3i) (GLint nx, GLint ny, GLint nz);
typedef void (GLAPIENTRY * GLNormal3s) (GLshort nx, GLshort ny, GLshort nz);
typedef void (GLAPIENTRY * GLNormal3bv) (const GLbyte *v);
typedef void (GLAPIENTRY * GLNormal3dv) (const GLdouble *v);
typedef void (GLAPIENTRY * GLNormal3fv) (const GLfloat *v);
typedef void (GLAPIENTRY * GLNormal3iv) (const GLint *v);
typedef void (GLAPIENTRY * GLNormal3sv) (const GLshort *v);
typedef void (GLAPIENTRY * GLIndexd) (GLdouble c);
typedef void (GLAPIENTRY * GLIndexf) (GLfloat c);
typedef void (GLAPIENTRY * GLIndexi) (GLint c);
typedef void (GLAPIENTRY * GLIndexs) (GLshort c);
typedef void (GLAPIENTRY * GLIndexub) (GLubyte c);
typedef void (GLAPIENTRY * GLIndexdv) (const GLdouble *c);
typedef void (GLAPIENTRY * GLIndexfv) (const GLfloat *c);
typedef void (GLAPIENTRY * GLIndexiv) (const GLint *c);
typedef void (GLAPIENTRY * GLIndexsv) (const GLshort *c);
typedef void (GLAPIENTRY * GLIndexubv) (const GLubyte *c);
typedef void (GLAPIENTRY * GLColor3b) (GLbyte red, GLbyte green, GLbyte blue);
typedef void (GLAPIENTRY * GLColor3d) (GLdouble red, GLdouble green, GLdouble blue);
typedef void (GLAPIENTRY * GLColor3f) (GLfloat red, GLfloat green, GLfloat blue);
typedef void (GLAPIENTRY * GLColor3i) (GLint red, GLint green, GLint blue);
typedef void (GLAPIENTRY * GLColor3s) (GLshort red, GLshort green, GLshort blue);
typedef void (GLAPIENTRY * GLColor3ub) (GLubyte red, GLubyte green, GLubyte blue);
typedef void (GLAPIENTRY * GLColor3ui) (GLuint red, GLuint green, GLuint blue);
typedef void (GLAPIENTRY * GLColor3us) (GLushort red, GLushort green, GLushort blue);
typedef void (GLAPIENTRY * GLColor4b) (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
typedef void (GLAPIENTRY * GLColor4d) (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
typedef void (GLAPIENTRY * GLColor4f) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (GLAPIENTRY * GLColor4i) (GLint red, GLint green, GLint blue, GLint alpha);
typedef void (GLAPIENTRY * GLColor4s) (GLshort red, GLshort green, GLshort blue, GLshort alpha);
typedef void (GLAPIENTRY * GLColor4ub) (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
typedef void (GLAPIENTRY * GLColor4ui) (GLuint red, GLuint green, GLuint blue, GLuint alpha);
typedef void (GLAPIENTRY * GLColor4us) (GLushort red, GLushort green, GLushort blue, GLushort alpha);
typedef void (GLAPIENTRY * GLColor3bv) (const GLbyte *v);
typedef void (GLAPIENTRY * GLColor3dv) (const GLdouble *v);
typedef void (GLAPIENTRY * GLColor3fv) (const GLfloat *v);
typedef void (GLAPIENTRY * GLColor3iv) (const GLint *v);
typedef void (GLAPIENTRY * GLColor3sv) (const GLshort *v);
typedef void (GLAPIENTRY * GLColor3ubv) (const GLubyte *v);
typedef void (GLAPIENTRY * GLColor3uiv) (const GLuint *v);
typedef void (GLAPIENTRY * GLColor3usv) (const GLushort *v);
typedef void (GLAPIENTRY * GLColor4bv) (const GLbyte *v);
typedef void (GLAPIENTRY * GLColor4dv) (const GLdouble *v);
typedef void (GLAPIENTRY * GLColor4fv) (const GLfloat *v);
typedef void (GLAPIENTRY * GLColor4iv) (const GLint *v);
typedef void (GLAPIENTRY * GLColor4sv) (const GLshort *v);
typedef void (GLAPIENTRY * GLColor4ubv) (const GLubyte *v);
typedef void (GLAPIENTRY * GLColor4uiv) (const GLuint *v);
typedef void (GLAPIENTRY * GLColor4usv) (const GLushort *v);
typedef void (GLAPIENTRY * GLTexCoord1d) (GLdouble s);
typedef void (GLAPIENTRY * GLTexCoord1f) (GLfloat s);
typedef void (GLAPIENTRY * GLTexCoord1i) (GLint s);
typedef void (GLAPIENTRY * GLTexCoord1s) (GLshort s);
typedef void (GLAPIENTRY * GLTexCoord2d) (GLdouble s, GLdouble t);
typedef void (GLAPIENTRY * GLTexCoord2f) (GLfloat s, GLfloat t);
typedef void (GLAPIENTRY * GLTexCoord2i) (GLint s, GLint t);
typedef void (GLAPIENTRY * GLTexCoord2s) (GLshort s, GLshort t);
typedef void (GLAPIENTRY * GLTexCoord3d) (GLdouble s, GLdouble t, GLdouble r);
typedef void (GLAPIENTRY * GLTexCoord3f) (GLfloat s, GLfloat t, GLfloat r);
typedef void (GLAPIENTRY * GLTexCoord3i) (GLint s, GLint t, GLint r);
typedef void (GLAPIENTRY * GLTexCoord3s) (GLshort s, GLshort t, GLshort r);
typedef void (GLAPIENTRY * GLTexCoord4d) (GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (GLAPIENTRY * GLTexCoord4f) (GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (GLAPIENTRY * GLTexCoord4i) (GLint s, GLint t, GLint r, GLint q);
typedef void (GLAPIENTRY * GLTexCoord4s) (GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (GLAPIENTRY * GLTexCoord1dv) (const GLdouble *v);
typedef void (GLAPIENTRY * GLTexCoord1fv) (const GLfloat *v);
typedef void (GLAPIENTRY * GLTexCoord1iv) (const GLint *v);
typedef void (GLAPIENTRY * GLTexCoord1sv) (const GLshort *v);
typedef void (GLAPIENTRY * GLTexCoord2dv) (const GLdouble *v);
typedef void (GLAPIENTRY * GLTexCoord2fv) (const GLfloat *v);
typedef void (GLAPIENTRY * GLTexCoord2iv) (const GLint *v);
typedef void (GLAPIENTRY * GLTexCoord2sv) (const GLshort *v);
typedef void (GLAPIENTRY * GLTexCoord3dv) (const GLdouble *v);
typedef void (GLAPIENTRY * GLTexCoord3fv) (const GLfloat *v);
typedef void (GLAPIENTRY * GLTexCoord3iv) (const GLint *v);
typedef void (GLAPIENTRY * GLTexCoord3sv) (const GLshort *v);
typedef void (GLAPIENTRY * GLTexCoord4dv) (const GLdouble *v);
typedef void (GLAPIENTRY * GLTexCoord4fv) (const GLfloat *v);
typedef void (GLAPIENTRY * GLTexCoord4iv) (const GLint *v);
typedef void (GLAPIENTRY * GLTexCoord4sv) (const GLshort *v);
typedef void (GLAPIENTRY * GLRasterPos2d) (GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * GLRasterPos2f) (GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * GLRasterPos2i) (GLint x, GLint y);
typedef void (GLAPIENTRY * GLRasterPos2s) (GLshort x, GLshort y);
typedef void (GLAPIENTRY * GLRasterPos3d) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * GLRasterPos3f) (GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * GLRasterPos3i) (GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * GLRasterPos3s) (GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * GLRasterPos4d) (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * GLRasterPos4f) (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * GLRasterPos4i) (GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * GLRasterPos4s) (GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * GLRasterPos2dv) (const GLdouble *v);
typedef void (GLAPIENTRY * GLRasterPos2fv) (const GLfloat *v);
typedef void (GLAPIENTRY * GLRasterPos2iv) (const GLint *v);
typedef void (GLAPIENTRY * GLRasterPos2sv) (const GLshort *v);
typedef void (GLAPIENTRY * GLRasterPos3dv) (const GLdouble *v);
typedef void (GLAPIENTRY * GLRasterPos3fv) (const GLfloat *v);
typedef void (GLAPIENTRY * GLRasterPos3iv) (const GLint *v);
typedef void (GLAPIENTRY * GLRasterPos3sv) (const GLshort *v);
typedef void (GLAPIENTRY * GLRasterPos4dv) (const GLdouble *v);
typedef void (GLAPIENTRY * GLRasterPos4fv) (const GLfloat *v);
typedef void (GLAPIENTRY * GLRasterPos4iv) (const GLint *v);
typedef void (GLAPIENTRY * GLRasterPos4sv) (const GLshort *v);
typedef void (GLAPIENTRY * GLRectd) (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
typedef void (GLAPIENTRY * GLRectf) (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
typedef void (GLAPIENTRY * GLRecti) (GLint x1, GLint y1, GLint x2, GLint y2);
typedef void (GLAPIENTRY * GLRects) (GLshort x1, GLshort y1, GLshort x2, GLshort y2);
typedef void (GLAPIENTRY * GLRectdv) (const GLdouble *v1, const GLdouble *v2);
typedef void (GLAPIENTRY * GLRectfv) (const GLfloat *v1, const GLfloat *v2);
typedef void (GLAPIENTRY * GLRectiv) (const GLint *v1, const GLint *v2);
typedef void (GLAPIENTRY * GLRectsv) (const GLshort *v1, const GLshort *v2);
typedef void (GLAPIENTRY * GLVertexPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
typedef void (GLAPIENTRY * GLNormalPointer) (GLenum type, GLsizei stride, const GLvoid *ptr);
typedef void (GLAPIENTRY * GLColorPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
typedef void (GLAPIENTRY * GLIndexPointer) (GLenum type, GLsizei stride, const GLvoid *ptr);
typedef void (GLAPIENTRY * GLTexCoordPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
typedef void (GLAPIENTRY * GLEdgeFlagPointer) (GLsizei stride, const GLvoid *ptr);
typedef void (GLAPIENTRY * GLGetPointerv) (GLenum pname, GLvoid **params);
typedef void (GLAPIENTRY * GLArrayElement) (GLint i);
typedef void (GLAPIENTRY * GLDrawArrays) (GLenum mode, GLint first, GLsizei count);
typedef void (GLAPIENTRY * GLDrawElements) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
typedef void (GLAPIENTRY * GLInterleavedArrays) (GLenum format, GLsizei stride, const GLvoid *pointer);
typedef void (GLAPIENTRY * GLShadeModel) (GLenum mode);
typedef void (GLAPIENTRY * GLLightf) (GLenum light, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * GLLighti) (GLenum light, GLenum pname, GLint param);
typedef void (GLAPIENTRY * GLLightfv) (GLenum light, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * GLLightiv) (GLenum light, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * GLGetLightfv) (GLenum light, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * GLGetLightiv) (GLenum light, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLLightModelf) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * GLLightModeli) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * GLLightModelfv) (GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * GLLightModeliv) (GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * GLMaterialf) (GLenum face, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * GLMateriali) (GLenum face, GLenum pname, GLint param);
typedef void (GLAPIENTRY * GLMaterialfv) (GLenum face, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * GLMaterialiv) (GLenum face, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * GLGetMaterialfv) (GLenum face, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * GLGetMaterialiv) (GLenum face, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLColorMaterial) (GLenum face, GLenum mode);
typedef void (GLAPIENTRY * GLPixelZoom) (GLfloat xfactor, GLfloat yfactor);
typedef void (GLAPIENTRY * GLPixelStoref) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * GLPixelStorei) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * GLPixelTransferf) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * GLPixelTransferi) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * GLPixelMapfv) (GLenum map, GLsizei mapsize, const GLfloat *values);
typedef void (GLAPIENTRY * GLPixelMapuiv) (GLenum map, GLsizei mapsize, const GLuint *values);
typedef void (GLAPIENTRY * GLPixelMapusv) (GLenum map, GLsizei mapsize, const GLushort *values);
typedef void (GLAPIENTRY * GLGetPixelMapfv) (GLenum map, GLfloat *values);
typedef void (GLAPIENTRY * GLGetPixelMapuiv) (GLenum map, GLuint *values);
typedef void (GLAPIENTRY * GLGetPixelMapusv) (GLenum map, GLushort *values);
typedef void (GLAPIENTRY * GLBitmap) (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
typedef void (GLAPIENTRY * GLReadPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
typedef void (GLAPIENTRY * GLDrawPixels) (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLCopyPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
typedef void (GLAPIENTRY * GLStencilFunc) (GLenum func, GLint ref, GLuint mask);
typedef void (GLAPIENTRY * GLStencilMask) (GLuint mask);
typedef void (GLAPIENTRY * GLStencilOp) (GLenum fail, GLenum zfail, GLenum zpass);
typedef void (GLAPIENTRY * GLClearStencil) (GLint s);
typedef void (GLAPIENTRY * GLTexGend) (GLenum coord, GLenum pname, GLdouble param);
typedef void (GLAPIENTRY * GLTexGenf) (GLenum coord, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * GLTexGeni) (GLenum coord, GLenum pname, GLint param);
typedef void (GLAPIENTRY * GLTexGendv) (GLenum coord, GLenum pname, const GLdouble *params);
typedef void (GLAPIENTRY * GLTexGenfv) (GLenum coord, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * GLTexGeniv) (GLenum coord, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * GLGetTexGendv) (GLenum coord, GLenum pname, GLdouble *params);
typedef void (GLAPIENTRY * GLGetTexGenfv) (GLenum coord, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * GLGetTexGeniv) (GLenum coord, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLTexEnvf) (GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * GLTexEnvi) (GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * GLTexEnvfv) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * GLTexEnviv) (GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * GLGetTexEnvfv) (GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * GLGetTexEnviv) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLTexParameterf) (GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * GLTexParameteri) (GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * GLTexParameterfv) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * GLTexParameteriv) (GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * GLGetTexParameterfv) (GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * GLGetTexParameteriv) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetTexLevelParameterfv) (GLenum target, GLint level, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * GLGetTexLevelParameteriv) (GLenum target, GLint level, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLTexImage1D) (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLTexImage2D) (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLGetTexImage) (GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
typedef void (GLAPIENTRY * GLGenTextures) (GLsizei n, GLuint *textures);
typedef void (GLAPIENTRY * GLDeleteTextures) (GLsizei n, const GLuint *textures);
typedef void (GLAPIENTRY * GLBindTexture) (GLenum target, GLuint texture);
typedef void (GLAPIENTRY * GLPrioritizeTextures) (GLsizei n, const GLuint *textures, const GLclampf *priorities);
typedef GLboolean (GLAPIENTRY * GLAreTexturesResident) (GLsizei n, const GLuint *textures, GLboolean *residences);
typedef GLboolean (GLAPIENTRY * GLIsTexture) (GLuint texture);
typedef void (GLAPIENTRY * GLTexSubImage1D) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLCopyTexImage1D) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (GLAPIENTRY * GLCopyTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAPIENTRY * GLCopyTexSubImage1D) (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * GLCopyTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * GLMap1d) (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
typedef void (GLAPIENTRY * GLMap1f) (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
typedef void (GLAPIENTRY * GLMap2d) (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
typedef void (GLAPIENTRY * GLMap2f) (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
typedef void (GLAPIENTRY * GLGetMapdv) (GLenum target, GLenum query, GLdouble *v);
typedef void (GLAPIENTRY * GLGetMapfv) (GLenum target, GLenum query, GLfloat *v);
typedef void (GLAPIENTRY * GLGetMapiv) (GLenum target, GLenum query, GLint *v);
typedef void (GLAPIENTRY * GLEvalCoord1d) (GLdouble u);
typedef void (GLAPIENTRY * GLEvalCoord1f) (GLfloat u);
typedef void (GLAPIENTRY * GLEvalCoord1dv) (const GLdouble *u);
typedef void (GLAPIENTRY * GLEvalCoord1fv) (const GLfloat *u);
typedef void (GLAPIENTRY * GLEvalCoord2d) (GLdouble u, GLdouble v);
typedef void (GLAPIENTRY * GLEvalCoord2f) (GLfloat u, GLfloat v);
typedef void (GLAPIENTRY * GLEvalCoord2dv) (const GLdouble *u);
typedef void (GLAPIENTRY * GLEvalCoord2fv) (const GLfloat *u);
typedef void (GLAPIENTRY * GLMapGrid1d) (GLint un, GLdouble u1, GLdouble u2);
typedef void (GLAPIENTRY * GLMapGrid1f) (GLint un, GLfloat u1, GLfloat u2);
typedef void (GLAPIENTRY * GLMapGrid2d) (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
typedef void (GLAPIENTRY * GLMapGrid2f) (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY * GLEvalPoint1) (GLint i);
typedef void (GLAPIENTRY * GLEvalPoint2) (GLint i, GLint j);
typedef void (GLAPIENTRY * GLEvalMesh1) (GLenum mode, GLint i1, GLint i2);
typedef void (GLAPIENTRY * GLEvalMesh2) (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
typedef void (GLAPIENTRY * GLFogf) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * GLFogi) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * GLFogfv) (GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * GLFogiv) (GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * GLFeedbackBuffer) (GLsizei size, GLenum type, GLfloat *buffer);
typedef void (GLAPIENTRY * GLPassThrough) (GLfloat token);
typedef void (GLAPIENTRY * GLSelectBuffer) (GLsizei size, GLuint *buffer);
typedef void (GLAPIENTRY * GLInitNames) (void);
typedef void (GLAPIENTRY * GLLoadName) (GLuint name);
typedef void (GLAPIENTRY * GLPushName) (GLuint name);
typedef void (GLAPIENTRY * GLPopName) (void);


// OpenGL 1.2
#define GL_UNSIGNED_BYTE_3_3_2 0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#define GL_UNSIGNED_INT_8_8_8_8 0x8035
#define GL_UNSIGNED_INT_10_10_10_2 0x8036
#define GL_TEXTURE_BINDING_3D 0x806A
#define GL_PACK_SKIP_IMAGES 0x806B
#define GL_PACK_IMAGE_HEIGHT 0x806C
#define GL_UNPACK_SKIP_IMAGES 0x806D
#define GL_UNPACK_IMAGE_HEIGHT 0x806E
#define GL_TEXTURE_3D 0x806F
#define GL_PROXY_TEXTURE_3D 0x8070
#define GL_TEXTURE_DEPTH 0x8071
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_MAX_3D_TEXTURE_SIZE 0x8073
#define GL_UNSIGNED_BYTE_2_3_3_REV 0x8362
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV 0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV 0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV 0x8366
#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
#define GL_UNSIGNED_INT_2_10_10_10_REV 0x8368
#define GL_BGR 0x80E0
#define GL_BGRA 0x80E1
#define GL_MAX_ELEMENTS_VERTICES 0x80E8
#define GL_MAX_ELEMENTS_INDICES 0x80E9
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_TEXTURE_MIN_LOD 0x813A
#define GL_TEXTURE_MAX_LOD 0x813B
#define GL_TEXTURE_BASE_LEVEL 0x813C
#define GL_TEXTURE_MAX_LEVEL 0x813D
#define GL_SMOOTH_POINT_SIZE_RANGE 0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY 0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE 0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY 0x0B23
#define GL_ALIASED_LINE_WIDTH_RANGE 0x846E
#define GL_RESCALE_NORMAL 0x803A
#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81F8
#define GL_SINGLE_COLOR 0x81F9
#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
#define GL_ALIASED_POINT_SIZE_RANGE 0x846D

typedef void (GLAPIENTRY * GLBlendColor) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (GLAPIENTRY * GLBlendEquation) (GLenum mode);
typedef void (GLAPIENTRY * GLDrawRangeElements) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
typedef void (GLAPIENTRY * GLTexImage3D) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLTexSubImage3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLCopyTexSubImage3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);


// OpenGL 1.3
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE31 0x84DF
#define GL_ACTIVE_TEXTURE 0x84E0
#define GL_MULTISAMPLE 0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#define GL_SAMPLE_ALPHA_TO_ONE 0x809F
#define GL_SAMPLE_COVERAGE 0x80A0
#define GL_SAMPLE_BUFFERS 0x80A8
#define GL_SAMPLES 0x80A9
#define GL_SAMPLE_COVERAGE_VALUE 0x80AA
#define GL_SAMPLE_COVERAGE_INVERT 0x80AB
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP 0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP 0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE 0x851C
#define GL_COMPRESSED_RGB 0x84ED
#define GL_COMPRESSED_RGBA 0x84EE
#define GL_TEXTURE_COMPRESSION_HINT 0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE 0x86A0
#define GL_TEXTURE_COMPRESSED 0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS 0x86A3
#define GL_CLAMP_TO_BORDER 0x812D
#define GL_CLIENT_ACTIVE_TEXTURE 0x84E1
#define GL_MAX_TEXTURE_UNITS 0x84E2
#define GL_TRANSPOSE_MODELVIEW_MATRIX 0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX 0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX 0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX 0x84E6
#define GL_MULTISAMPLE_BIT 0x20000000
#define GL_NORMAL_MAP 0x8511
#define GL_REFLECTION_MAP 0x8512
#define GL_COMPRESSED_ALPHA 0x84E9
#define GL_COMPRESSED_LUMINANCE 0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA 0x84EB
#define GL_COMPRESSED_INTENSITY 0x84EC
#define GL_COMBINE 0x8570
#define GL_COMBINE_RGB 0x8571
#define GL_COMBINE_ALPHA 0x8572
#define GL_SOURCE0_RGB 0x8580
#define GL_SOURCE1_RGB 0x8581
#define GL_SOURCE2_RGB 0x8582
#define GL_SOURCE0_ALPHA 0x8588
#define GL_SOURCE1_ALPHA 0x8589
#define GL_SOURCE2_ALPHA 0x858A
#define GL_OPERAND0_RGB 0x8590
#define GL_OPERAND1_RGB 0x8591
#define GL_OPERAND2_RGB 0x8592
#define GL_OPERAND0_ALPHA 0x8598
#define GL_OPERAND1_ALPHA 0x8599
#define GL_OPERAND2_ALPHA 0x859A
#define GL_RGB_SCALE 0x8573
#define GL_ADD_SIGNED 0x8574
#define GL_INTERPOLATE 0x8575
#define GL_SUBTRACT 0x84E7
#define GL_CONSTANT 0x8576
#define GL_PRIMARY_COLOR 0x8577
#define GL_PREVIOUS 0x8578
#define GL_DOT3_RGB 0x86AE
#define GL_DOT3_RGBA 0x86AF

typedef void (GLAPIENTRY * GLActiveTexture) (GLenum texture);
typedef void (GLAPIENTRY * GLSampleCoverage) (GLfloat value, GLboolean invert);
typedef void (GLAPIENTRY * GLCompressedTexImage3D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
typedef void (GLAPIENTRY * GLCompressedTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
typedef void (GLAPIENTRY * GLCompressedTexImage1D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
typedef void (GLAPIENTRY * GLCompressedTexSubImage3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
typedef void (GLAPIENTRY * GLCompressedTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
typedef void (GLAPIENTRY * GLCompressedTexSubImage1D) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
typedef void (GLAPIENTRY * GLGetCompressedTexImage) (GLenum target, GLint level, GLvoid *img);
typedef void (GLAPIENTRY * GLClientActiveTexture) (GLenum texture);
typedef void (GLAPIENTRY * GLMultiTexCoord1d) (GLenum target, GLdouble s);
typedef void (GLAPIENTRY * GLMultiTexCoord1dv) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * GLMultiTexCoord1f) (GLenum target, GLfloat s);
typedef void (GLAPIENTRY * GLMultiTexCoord1fv) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * GLMultiTexCoord1i) (GLenum target, GLint s);
typedef void (GLAPIENTRY * GLMultiTexCoord1iv) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * GLMultiTexCoord1s) (GLenum target, GLshort s);
typedef void (GLAPIENTRY * GLMultiTexCoord1sv) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * GLMultiTexCoord2d) (GLenum target, GLdouble s, GLdouble t);
typedef void (GLAPIENTRY * GLMultiTexCoord2dv) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * GLMultiTexCoord2f) (GLenum target, GLfloat s, GLfloat t);
typedef void (GLAPIENTRY * GLMultiTexCoord2fv) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * GLMultiTexCoord2i) (GLenum target, GLint s, GLint t);
typedef void (GLAPIENTRY * GLMultiTexCoord2iv) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * GLMultiTexCoord2s) (GLenum target, GLshort s, GLshort t);
typedef void (GLAPIENTRY * GLMultiTexCoord2sv) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * GLMultiTexCoord3d) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
typedef void (GLAPIENTRY * GLMultiTexCoord3dv) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * GLMultiTexCoord3f) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
typedef void (GLAPIENTRY * GLMultiTexCoord3fv) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * GLMultiTexCoord3i) (GLenum target, GLint s, GLint t, GLint r);
typedef void (GLAPIENTRY * GLMultiTexCoord3iv) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * GLMultiTexCoord3s) (GLenum target, GLshort s, GLshort t, GLshort r);
typedef void (GLAPIENTRY * GLMultiTexCoord3sv) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * GLMultiTexCoord4d) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (GLAPIENTRY * GLMultiTexCoord4dv) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * GLMultiTexCoord4f) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (GLAPIENTRY * GLMultiTexCoord4fv) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * GLMultiTexCoord4i) (GLenum target, GLint s, GLint t, GLint r, GLint q);
typedef void (GLAPIENTRY * GLMultiTexCoord4iv) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * GLMultiTexCoord4s) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (GLAPIENTRY * GLMultiTexCoord4sv) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * GLLoadTransposeMatrixf) (const GLfloat *m);
typedef void (GLAPIENTRY * GLLoadTransposeMatrixd) (const GLdouble *m);
typedef void (GLAPIENTRY * GLMultTransposeMatrixf) (const GLfloat *m);
typedef void (GLAPIENTRY * GLMultTransposeMatrixd) (const GLdouble *m);


// OpenGL 1.4
#define GL_BLEND_DST_RGB 0x80C8
#define GL_BLEND_SRC_RGB 0x80C9
#define GL_BLEND_DST_ALPHA 0x80CA
#define GL_BLEND_SRC_ALPHA 0x80CB
#define GL_POINT_FADE_THRESHOLD_SIZE 0x8128
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_DEPTH_COMPONENT32 0x81A7
#define GL_MIRRORED_REPEAT 0x8370
#define GL_MAX_TEXTURE_LOD_BIAS 0x84FD
#define GL_TEXTURE_LOD_BIAS 0x8501
#define GL_INCR_WRAP 0x8507
#define GL_DECR_WRAP 0x8508
#define GL_TEXTURE_DEPTH_SIZE 0x884A
#define GL_TEXTURE_COMPARE_MODE 0x884C
#define GL_TEXTURE_COMPARE_FUNC 0x884D
#define GL_POINT_SIZE_MIN 0x8126
#define GL_POINT_SIZE_MAX 0x8127
#define GL_POINT_DISTANCE_ATTENUATION 0x8129
#define GL_GENERATE_MIPMAP 0x8191
#define GL_GENERATE_MIPMAP_HINT 0x8192
#define GL_FOG_COORDINATE_SOURCE 0x8450
#define GL_FOG_COORDINATE 0x8451
#define GL_FRAGMENT_DEPTH 0x8452
#define GL_CURRENT_FOG_COORDINATE 0x8453
#define GL_FOG_COORDINATE_ARRAY_TYPE 0x8454
#define GL_FOG_COORDINATE_ARRAY_STRIDE 0x8455
#define GL_FOG_COORDINATE_ARRAY_POINTER 0x8456
#define GL_FOG_COORDINATE_ARRAY 0x8457
#define GL_COLOR_SUM 0x8458
#define GL_CURRENT_SECONDARY_COLOR 0x8459
#define GL_SECONDARY_COLOR_ARRAY_SIZE 0x845A
#define GL_SECONDARY_COLOR_ARRAY_TYPE 0x845B
#define GL_SECONDARY_COLOR_ARRAY_STRIDE 0x845C
#define GL_SECONDARY_COLOR_ARRAY_POINTER 0x845D
#define GL_SECONDARY_COLOR_ARRAY 0x845E
#define GL_TEXTURE_FILTER_CONTROL 0x8500
#define GL_DEPTH_TEXTURE_MODE 0x884B
#define GL_COMPARE_R_TO_TEXTURE 0x884E

typedef void (GLAPIENTRY * GLBlendFuncSeparate) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void (GLAPIENTRY * GLMultiDrawArrays) (GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount);
typedef void (GLAPIENTRY * GLMultiDrawElements) (GLenum mode, const GLsizei *count, GLenum type, const GLvoid *const*indices, GLsizei drawcount);
typedef void (GLAPIENTRY * GLPointParameterf) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * GLPointParameterfv) (GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * GLPointParameteri) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * GLPointParameteriv) (GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * GLFogCoordf) (GLfloat coord);
typedef void (GLAPIENTRY * GLFogCoordfv) (const GLfloat *coord);
typedef void (GLAPIENTRY * GLFogCoordd) (GLdouble coord);
typedef void (GLAPIENTRY * GLFogCoorddv) (const GLdouble *coord);
typedef void (GLAPIENTRY * GLFogCoordPointer) (GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (GLAPIENTRY * GLSecondaryColor3b) (GLbyte red, GLbyte green, GLbyte blue);
typedef void (GLAPIENTRY * GLSecondaryColor3bv) (const GLbyte *v);
typedef void (GLAPIENTRY * GLSecondaryColor3d) (GLdouble red, GLdouble green, GLdouble blue);
typedef void (GLAPIENTRY * GLSecondaryColor3dv) (const GLdouble *v);
typedef void (GLAPIENTRY * GLSecondaryColor3f) (GLfloat red, GLfloat green, GLfloat blue);
typedef void (GLAPIENTRY * GLSecondaryColor3fv) (const GLfloat *v);
typedef void (GLAPIENTRY * GLSecondaryColor3i) (GLint red, GLint green, GLint blue);
typedef void (GLAPIENTRY * GLSecondaryColor3iv) (const GLint *v);
typedef void (GLAPIENTRY * GLSecondaryColor3s) (GLshort red, GLshort green, GLshort blue);
typedef void (GLAPIENTRY * GLSecondaryColor3sv) (const GLshort *v);
typedef void (GLAPIENTRY * GLSecondaryColor3ub) (GLubyte red, GLubyte green, GLubyte blue);
typedef void (GLAPIENTRY * GLSecondaryColor3ubv) (const GLubyte *v);
typedef void (GLAPIENTRY * GLSecondaryColor3ui) (GLuint red, GLuint green, GLuint blue);
typedef void (GLAPIENTRY * GLSecondaryColor3uiv) (const GLuint *v);
typedef void (GLAPIENTRY * GLSecondaryColor3us) (GLushort red, GLushort green, GLushort blue);
typedef void (GLAPIENTRY * GLSecondaryColor3usv) (const GLushort *v);
typedef void (GLAPIENTRY * GLSecondaryColorPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (GLAPIENTRY * GLWindowPos2d) (GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * GLWindowPos2dv) (const GLdouble *v);
typedef void (GLAPIENTRY * GLWindowPos2f) (GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * GLWindowPos2fv) (const GLfloat *v);
typedef void (GLAPIENTRY * GLWindowPos2i) (GLint x, GLint y);
typedef void (GLAPIENTRY * GLWindowPos2iv) (const GLint *v);
typedef void (GLAPIENTRY * GLWindowPos2s) (GLshort x, GLshort y);
typedef void (GLAPIENTRY * GLWindowPos2sv) (const GLshort *v);
typedef void (GLAPIENTRY * GLWindowPos3d) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * GLWindowPos3dv) (const GLdouble *v);
typedef void (GLAPIENTRY * GLWindowPos3f) (GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * GLWindowPos3fv) (const GLfloat *v);
typedef void (GLAPIENTRY * GLWindowPos3i) (GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * GLWindowPos3iv) (const GLint *v);
typedef void (GLAPIENTRY * GLWindowPos3s) (GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * GLWindowPos3sv) (const GLshort *v);


// OpenGL 1.5
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;

#define GL_BUFFER_SIZE 0x8764
#define GL_BUFFER_USAGE 0x8765
#define GL_QUERY_COUNTER_BITS 0x8864
#define GL_CURRENT_QUERY 0x8865
#define GL_QUERY_RESULT 0x8866
#define GL_QUERY_RESULT_AVAILABLE 0x8867
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_ARRAY_BUFFER_BINDING 0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING 0x8895
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_READ_ONLY 0x88B8
#define GL_WRITE_ONLY 0x88B9
#define GL_READ_WRITE 0x88BA
#define GL_BUFFER_ACCESS 0x88BB
#define GL_BUFFER_MAPPED 0x88BC
#define GL_BUFFER_MAP_POINTER 0x88BD
#define GL_STREAM_DRAW 0x88E0
#define GL_STREAM_READ 0x88E1
#define GL_STREAM_COPY 0x88E2
#define GL_STATIC_DRAW 0x88E4
#define GL_STATIC_READ 0x88E5
#define GL_STATIC_COPY 0x88E6
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_DYNAMIC_READ 0x88E9
#define GL_DYNAMIC_COPY 0x88EA
#define GL_SAMPLES_PASSED 0x8914
#define GL_SRC1_ALPHA 0x8589
#define GL_VERTEX_ARRAY_BUFFER_BINDING 0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING 0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING 0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING 0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING 0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING 0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING 0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING 0x889E
#define GL_FOG_COORD_SRC 0x8450
#define GL_FOG_COORD 0x8451
#define GL_CURRENT_FOG_COORD 0x8453
#define GL_FOG_COORD_ARRAY_TYPE 0x8454
#define GL_FOG_COORD_ARRAY_STRIDE 0x8455
#define GL_FOG_COORD_ARRAY_POINTER 0x8456
#define GL_FOG_COORD_ARRAY 0x8457
#define GL_FOG_COORD_ARRAY_BUFFER_BINDING 0x889D
#define GL_SRC0_RGB 0x8580
#define GL_SRC1_RGB 0x8581
#define GL_SRC2_RGB 0x8582
#define GL_SRC0_ALPHA 0x8588
#define GL_SRC2_ALPHA 0x858A

typedef void (GLAPIENTRY * GLGenQueries) (GLsizei n, GLuint *ids);
typedef void (GLAPIENTRY * GLDeleteQueries) (GLsizei n, const GLuint *ids);
typedef GLboolean (GLAPIENTRY * GLIsQuery) (GLuint id);
typedef void (GLAPIENTRY * GLBeginQuery) (GLenum target, GLuint id);
typedef void (GLAPIENTRY * GLEndQuery) (GLenum target);
typedef void (GLAPIENTRY * GLGetQueryiv) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetQueryObjectiv) (GLuint id, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetQueryObjectuiv) (GLuint id, GLenum pname, GLuint *params);
typedef void (GLAPIENTRY * GLBindBuffer) (GLenum target, GLuint buffer);
typedef void (GLAPIENTRY * GLDeleteBuffers) (GLsizei n, const GLuint *buffers);
typedef void (GLAPIENTRY * GLGenBuffers) (GLsizei n, GLuint *buffers);
typedef GLboolean (GLAPIENTRY * GLIsBuffer) (GLuint buffer);
typedef void (GLAPIENTRY * GLBufferData) (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
typedef void (GLAPIENTRY * GLBufferSubData) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
typedef void (GLAPIENTRY * GLGetBufferSubData) (GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data);
typedef void * (GLAPIENTRY * GLMapBuffer) (GLenum target, GLenum access);
typedef GLboolean (GLAPIENTRY * GLUnmapBuffer) (GLenum target);
typedef void (GLAPIENTRY * GLGetBufferParameteriv) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetBufferPointerv) (GLenum target, GLenum pname, GLvoid **params);


// OpenGL 2.0
typedef char GLchar;

#define GL_BLEND_EQUATION_RGB 0x8009
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED 0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE 0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE 0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE 0x8625
#define GL_CURRENT_VERTEX_ATTRIB 0x8626
#define GL_VERTEX_PROGRAM_POINT_SIZE 0x8642
#define GL_VERTEX_ATTRIB_ARRAY_POINTER 0x8645
#define GL_STENCIL_BACK_FUNC 0x8800
#define GL_STENCIL_BACK_FAIL 0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL 0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS 0x8803
#define GL_MAX_DRAW_BUFFERS 0x8824
#define GL_DRAW_BUFFER0 0x8825
#define GL_DRAW_BUFFER1 0x8826
#define GL_DRAW_BUFFER2 0x8827
#define GL_DRAW_BUFFER3 0x8828
#define GL_DRAW_BUFFER4 0x8829
#define GL_DRAW_BUFFER5 0x882A
#define GL_DRAW_BUFFER6 0x882B
#define GL_DRAW_BUFFER7 0x882C
#define GL_DRAW_BUFFER8 0x882D
#define GL_DRAW_BUFFER9 0x882E
#define GL_DRAW_BUFFER10 0x882F
#define GL_DRAW_BUFFER11 0x8830
#define GL_DRAW_BUFFER12 0x8831
#define GL_DRAW_BUFFER13 0x8832
#define GL_DRAW_BUFFER14 0x8833
#define GL_DRAW_BUFFER15 0x8834
#define GL_BLEND_EQUATION_ALPHA 0x883D
#define GL_MAX_VERTEX_ATTRIBS 0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_MAX_TEXTURE_IMAGE_UNITS 0x8872
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS 0x8B4A
#define GL_MAX_VARYING_FLOATS 0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_SHADER_TYPE 0x8B4F
#define GL_FLOAT_VEC2 0x8B50
#define GL_FLOAT_VEC3 0x8B51
#define GL_FLOAT_VEC4 0x8B52
#define GL_INT_VEC2 0x8B53
#define GL_INT_VEC3 0x8B54
#define GL_INT_VEC4 0x8B55
#define GL_BOOL 0x8B56
#define GL_BOOL_VEC2 0x8B57
#define GL_BOOL_VEC3 0x8B58
#define GL_BOOL_VEC4 0x8B59
#define GL_FLOAT_MAT2 0x8B5A
#define GL_FLOAT_MAT3 0x8B5B
#define GL_FLOAT_MAT4 0x8B5C
#define GL_SAMPLER_1D 0x8B5D
#define GL_SAMPLER_2D 0x8B5E
#define GL_SAMPLER_3D 0x8B5F
#define GL_SAMPLER_CUBE 0x8B60
#define GL_SAMPLER_1D_SHADOW 0x8B61
#define GL_SAMPLER_2D_SHADOW 0x8B62
#define GL_DELETE_STATUS 0x8B80
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_VALIDATE_STATUS 0x8B83
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_ATTACHED_SHADERS 0x8B85
#define GL_ACTIVE_UNIFORMS 0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH 0x8B87
#define GL_SHADER_SOURCE_LENGTH 0x8B88
#define GL_ACTIVE_ATTRIBUTES 0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH 0x8B8A
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#define GL_CURRENT_PROGRAM 0x8B8D
#define GL_POINT_SPRITE_COORD_ORIGIN 0x8CA0
#define GL_LOWER_LEFT 0x8CA1
#define GL_UPPER_LEFT 0x8CA2
#define GL_STENCIL_BACK_REF 0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK 0x8CA4
#define GL_STENCIL_BACK_WRITEMASK 0x8CA5
#define GL_VERTEX_PROGRAM_TWO_SIDE 0x8643
#define GL_POINT_SPRITE 0x8861
#define GL_COORD_REPLACE 0x8862
#define GL_MAX_TEXTURE_COORDS 0x8871

typedef void (GLAPIENTRY * GLBlendEquationSeparate) (GLenum modeRGB, GLenum modeAlpha);
typedef void (GLAPIENTRY * GLDrawBuffers) (GLsizei n, const GLenum *bufs);
typedef void (GLAPIENTRY * GLStencilOpSeparate) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef void (GLAPIENTRY * GLStencilFuncSeparate) (GLenum face, GLenum func, GLint ref, GLuint mask);
typedef void (GLAPIENTRY * GLStencilMaskSeparate) (GLenum face, GLuint mask);
typedef void (GLAPIENTRY * GLAttachShader) (GLuint program, GLuint shader);
typedef void (GLAPIENTRY * GLBindAttribLocation) (GLuint program, GLuint index, const GLchar *name);
typedef void (GLAPIENTRY * GLCompileShader) (GLuint shader);
typedef GLuint (GLAPIENTRY * GLCreateProgram) (void);
typedef GLuint (GLAPIENTRY * GLCreateShader) (GLenum type);
typedef void (GLAPIENTRY * GLDeleteProgram) (GLuint program);
typedef void (GLAPIENTRY * GLDeleteShader) (GLuint shader);
typedef void (GLAPIENTRY * GLDetachShader) (GLuint program, GLuint shader);
typedef void (GLAPIENTRY * GLDisableVertexAttribArray) (GLuint index);
typedef void (GLAPIENTRY * GLEnableVertexAttribArray) (GLuint index);
typedef void (GLAPIENTRY * GLGetActiveAttrib) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
typedef void (GLAPIENTRY * GLGetActiveUniform) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
typedef void (GLAPIENTRY * GLGetAttachedShaders) (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
typedef GLint (GLAPIENTRY * GLGetAttribLocation) (GLuint program, const GLchar *name);
typedef void (GLAPIENTRY * GLGetProgramiv) (GLuint program, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetProgramInfoLog) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (GLAPIENTRY * GLGetShaderiv) (GLuint shader, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetShaderInfoLog) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (GLAPIENTRY * GLGetShaderSource) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
typedef GLint (GLAPIENTRY * GLGetUniformLocation) (GLuint program, const GLchar *name);
typedef void (GLAPIENTRY * GLGetUniformfv) (GLuint program, GLint location, GLfloat *params);
typedef void (GLAPIENTRY * GLGetUniformiv) (GLuint program, GLint location, GLint *params);
typedef void (GLAPIENTRY * GLGetVertexAttribdv) (GLuint index, GLenum pname, GLdouble *params);
typedef void (GLAPIENTRY * GLGetVertexAttribfv) (GLuint index, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * GLGetVertexAttribiv) (GLuint index, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetVertexAttribPointerv) (GLuint index, GLenum pname, GLvoid **pointer);
typedef GLboolean (GLAPIENTRY * GLIsProgram) (GLuint program);
typedef GLboolean (GLAPIENTRY * GLIsShader) (GLuint shader);
typedef void (GLAPIENTRY * GLLinkProgram) (GLuint program);
typedef void (GLAPIENTRY * GLShaderSource) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (GLAPIENTRY * GLUseProgram) (GLuint program);
typedef void (GLAPIENTRY * GLUniform1f) (GLint location, GLfloat v0);
typedef void (GLAPIENTRY * GLUniform2f) (GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAPIENTRY * GLUniform3f) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY * GLUniform4f) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAPIENTRY * GLUniform1i) (GLint location, GLint v0);
typedef void (GLAPIENTRY * GLUniform2i) (GLint location, GLint v0, GLint v1);
typedef void (GLAPIENTRY * GLUniform3i) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAPIENTRY * GLUniform4i) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAPIENTRY * GLUniform1fv) (GLint location, GLsizei count, const GLfloat *value);
typedef void (GLAPIENTRY * GLUniform2fv) (GLint location, GLsizei count, const GLfloat *value);
typedef void (GLAPIENTRY * GLUniform3fv) (GLint location, GLsizei count, const GLfloat *value);
typedef void (GLAPIENTRY * GLUniform4fv) (GLint location, GLsizei count, const GLfloat *value);
typedef void (GLAPIENTRY * GLUniform1iv) (GLint location, GLsizei count, const GLint *value);
typedef void (GLAPIENTRY * GLUniform2iv) (GLint location, GLsizei count, const GLint *value);
typedef void (GLAPIENTRY * GLUniform3iv) (GLint location, GLsizei count, const GLint *value);
typedef void (GLAPIENTRY * GLUniform4iv) (GLint location, GLsizei count, const GLint *value);
typedef void (GLAPIENTRY * GLUniformMatrix2fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLUniformMatrix3fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLUniformMatrix4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLValidateProgram) (GLuint program);
typedef void (GLAPIENTRY * GLVertexAttrib1d) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * GLVertexAttrib1dv) (GLuint index, const GLdouble *v);
typedef void (GLAPIENTRY * GLVertexAttrib1f) (GLuint index, GLfloat x);
typedef void (GLAPIENTRY * GLVertexAttrib1fv) (GLuint index, const GLfloat *v);
typedef void (GLAPIENTRY * GLVertexAttrib1s) (GLuint index, GLshort x);
typedef void (GLAPIENTRY * GLVertexAttrib1sv) (GLuint index, const GLshort *v);
typedef void (GLAPIENTRY * GLVertexAttrib2d) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * GLVertexAttrib2dv) (GLuint index, const GLdouble *v);
typedef void (GLAPIENTRY * GLVertexAttrib2f) (GLuint index, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * GLVertexAttrib2fv) (GLuint index, const GLfloat *v);
typedef void (GLAPIENTRY * GLVertexAttrib2s) (GLuint index, GLshort x, GLshort y);
typedef void (GLAPIENTRY * GLVertexAttrib2sv) (GLuint index, const GLshort *v);
typedef void (GLAPIENTRY * GLVertexAttrib3d) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * GLVertexAttrib3dv) (GLuint index, const GLdouble *v);
typedef void (GLAPIENTRY * GLVertexAttrib3f) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * GLVertexAttrib3fv) (GLuint index, const GLfloat *v);
typedef void (GLAPIENTRY * GLVertexAttrib3s) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * GLVertexAttrib3sv) (GLuint index, const GLshort *v);
typedef void (GLAPIENTRY * GLVertexAttrib4Nbv) (GLuint index, const GLbyte *v);
typedef void (GLAPIENTRY * GLVertexAttrib4Niv) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * GLVertexAttrib4Nsv) (GLuint index, const GLshort *v);
typedef void (GLAPIENTRY * GLVertexAttrib4Nub) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (GLAPIENTRY * GLVertexAttrib4Nubv) (GLuint index, const GLubyte *v);
typedef void (GLAPIENTRY * GLVertexAttrib4Nuiv) (GLuint index, const GLuint *v);
typedef void (GLAPIENTRY * GLVertexAttrib4Nusv) (GLuint index, const GLushort *v);
typedef void (GLAPIENTRY * GLVertexAttrib4bv) (GLuint index, const GLbyte *v);
typedef void (GLAPIENTRY * GLVertexAttrib4d) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * GLVertexAttrib4dv) (GLuint index, const GLdouble *v);
typedef void (GLAPIENTRY * GLVertexAttrib4f) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * GLVertexAttrib4fv) (GLuint index, const GLfloat *v);
typedef void (GLAPIENTRY * GLVertexAttrib4iv) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * GLVertexAttrib4s) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * GLVertexAttrib4sv) (GLuint index, const GLshort *v);
typedef void (GLAPIENTRY * GLVertexAttrib4ubv) (GLuint index, const GLubyte *v);
typedef void (GLAPIENTRY * GLVertexAttrib4uiv) (GLuint index, const GLuint *v);
typedef void (GLAPIENTRY * GLVertexAttrib4usv) (GLuint index, const GLushort *v);
typedef void (GLAPIENTRY * GLVertexAttribPointer) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);


// OpenGL 2.1
#define GL_PIXEL_PACK_BUFFER 0x88EB
#define GL_PIXEL_UNPACK_BUFFER 0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING 0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING 0x88EF
#define GL_FLOAT_MAT2x3 0x8B65
#define GL_FLOAT_MAT2x4 0x8B66
#define GL_FLOAT_MAT3x2 0x8B67
#define GL_FLOAT_MAT3x4 0x8B68
#define GL_FLOAT_MAT4x2 0x8B69
#define GL_FLOAT_MAT4x3 0x8B6A
#define GL_SRGB 0x8C40
#define GL_SRGB8 0x8C41
#define GL_SRGB_ALPHA 0x8C42
#define GL_SRGB8_ALPHA8 0x8C43
#define GL_COMPRESSED_SRGB 0x8C48
#define GL_COMPRESSED_SRGB_ALPHA 0x8C49
#define GL_CURRENT_RASTER_SECONDARY_COLOR 0x845F
#define GL_SLUMINANCE_ALPHA 0x8C44
#define GL_SLUMINANCE8_ALPHA8 0x8C45
#define GL_SLUMINANCE 0x8C46
#define GL_SLUMINANCE8 0x8C47
#define GL_COMPRESSED_SLUMINANCE 0x8C4A
#define GL_COMPRESSED_SLUMINANCE_ALPHA 0x8C4B

typedef void (GLAPIENTRY * GLUniformMatrix2x3fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLUniformMatrix3x2fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLUniformMatrix2x4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLUniformMatrix4x2fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLUniformMatrix3x4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLUniformMatrix4x3fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);


// OpenGL 3.0
#define GL_COMPARE_REF_TO_TEXTURE 0x884E
#define GL_CLIP_DISTANCE0 0x3000
#define GL_CLIP_DISTANCE1 0x3001
#define GL_CLIP_DISTANCE2 0x3002
#define GL_CLIP_DISTANCE3 0x3003
#define GL_CLIP_DISTANCE4 0x3004
#define GL_CLIP_DISTANCE5 0x3005
#define GL_CLIP_DISTANCE6 0x3006
#define GL_CLIP_DISTANCE7 0x3007
#define GL_MAX_CLIP_DISTANCES 0x0D32
#define GL_MAJOR_VERSION 0x821B
#define GL_MINOR_VERSION 0x821C
#define GL_NUM_EXTENSIONS 0x821D
#define GL_CONTEXT_FLAGS 0x821E
#define GL_COMPRESSED_RED 0x8225
#define GL_COMPRESSED_RG 0x8226
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT 0x00000001
#define GL_RGBA32F 0x8814
#define GL_RGB32F 0x8815
#define GL_RGBA16F 0x881A
#define GL_RGB16F 0x881B
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER 0x88FD
#define GL_MAX_ARRAY_TEXTURE_LAYERS 0x88FF
#define GL_MIN_PROGRAM_TEXEL_OFFSET 0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET 0x8905
#define GL_CLAMP_READ_COLOR 0x891C
#define GL_FIXED_ONLY 0x891D
#define GL_MAX_VARYING_COMPONENTS 0x8B4B
#define GL_TEXTURE_1D_ARRAY 0x8C18
#define GL_PROXY_TEXTURE_1D_ARRAY 0x8C19
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_PROXY_TEXTURE_2D_ARRAY 0x8C1B
#define GL_TEXTURE_BINDING_1D_ARRAY 0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY 0x8C1D
#define GL_R11F_G11F_B10F 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV 0x8C3B
#define GL_RGB9_E5 0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV 0x8C3E
#define GL_TEXTURE_SHARED_SIZE 0x8C3F
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS 0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
#define GL_PRIMITIVES_GENERATED 0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
#define GL_RASTERIZER_DISCARD 0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
#define GL_INTERLEAVED_ATTRIBS 0x8C8C
#define GL_SEPARATE_ATTRIBS 0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER 0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
#define GL_RGBA32UI 0x8D70
#define GL_RGB32UI 0x8D71
#define GL_RGBA16UI 0x8D76
#define GL_RGB16UI 0x8D77
#define GL_RGBA8UI 0x8D7C
#define GL_RGB8UI 0x8D7D
#define GL_RGBA32I 0x8D82
#define GL_RGB32I 0x8D83
#define GL_RGBA16I 0x8D88
#define GL_RGB16I 0x8D89
#define GL_RGBA8I 0x8D8E
#define GL_RGB8I 0x8D8F
#define GL_RED_INTEGER 0x8D94
#define GL_GREEN_INTEGER 0x8D95
#define GL_BLUE_INTEGER 0x8D96
#define GL_RGB_INTEGER 0x8D98
#define GL_RGBA_INTEGER 0x8D99
#define GL_BGR_INTEGER 0x8D9A
#define GL_BGRA_INTEGER 0x8D9B
#define GL_SAMPLER_1D_ARRAY 0x8DC0
#define GL_SAMPLER_2D_ARRAY 0x8DC1
#define GL_SAMPLER_1D_ARRAY_SHADOW 0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW 0x8DC4
#define GL_SAMPLER_CUBE_SHADOW 0x8DC5
#define GL_UNSIGNED_INT_VEC2 0x8DC6
#define GL_UNSIGNED_INT_VEC3 0x8DC7
#define GL_UNSIGNED_INT_VEC4 0x8DC8
#define GL_INT_SAMPLER_1D 0x8DC9
#define GL_INT_SAMPLER_2D 0x8DCA
#define GL_INT_SAMPLER_3D 0x8DCB
#define GL_INT_SAMPLER_CUBE 0x8DCC
#define GL_INT_SAMPLER_1D_ARRAY 0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY 0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_1D 0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D 0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D 0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE 0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY 0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY 0x8DD7
#define GL_QUERY_WAIT 0x8E13
#define GL_QUERY_NO_WAIT 0x8E14
#define GL_QUERY_BY_REGION_WAIT 0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT 0x8E16
#define GL_BUFFER_ACCESS_FLAGS 0x911F
#define GL_BUFFER_MAP_LENGTH 0x9120
#define GL_BUFFER_MAP_OFFSET 0x9121
#define GL_DEPTH_COMPONENT32F 0x8CAC
#define GL_DEPTH32F_STENCIL8 0x8CAD
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 0x8DAD
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_DEFAULT 0x8218
#define GL_FRAMEBUFFER_UNDEFINED 0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A
#define GL_MAX_RENDERBUFFER_SIZE 0x84E8
#define GL_DEPTH_STENCIL 0x84F9
#define GL_UNSIGNED_INT_24_8 0x84FA
#define GL_DEPTH24_STENCIL8 0x88F0
#define GL_TEXTURE_STENCIL_SIZE 0x88F1
#define GL_TEXTURE_RED_TYPE 0x8C10
#define GL_TEXTURE_GREEN_TYPE 0x8C11
#define GL_TEXTURE_BLUE_TYPE 0x8C12
#define GL_TEXTURE_ALPHA_TYPE 0x8C13
#define GL_TEXTURE_DEPTH_TYPE 0x8C16
#define GL_UNSIGNED_NORMALIZED 0x8C17
#define GL_FRAMEBUFFER_BINDING 0x8CA6
#define GL_DRAW_FRAMEBUFFER_BINDING 0x8CA6
#define GL_RENDERBUFFER_BINDING 0x8CA7
#define GL_READ_FRAMEBUFFER 0x8CA8
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING 0x8CAA
#define GL_RENDERBUFFER_SAMPLES 0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED 0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS 0x8CDF
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_COLOR_ATTACHMENT3 0x8CE3
#define GL_COLOR_ATTACHMENT4 0x8CE4
#define GL_COLOR_ATTACHMENT5 0x8CE5
#define GL_COLOR_ATTACHMENT6 0x8CE6
#define GL_COLOR_ATTACHMENT7 0x8CE7
#define GL_COLOR_ATTACHMENT8 0x8CE8
#define GL_COLOR_ATTACHMENT9 0x8CE9
#define GL_COLOR_ATTACHMENT10 0x8CEA
#define GL_COLOR_ATTACHMENT11 0x8CEB
#define GL_COLOR_ATTACHMENT12 0x8CEC
#define GL_COLOR_ATTACHMENT13 0x8CED
#define GL_COLOR_ATTACHMENT14 0x8CEE
#define GL_COLOR_ATTACHMENT15 0x8CEF
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_STENCIL_ATTACHMENT 0x8D20
#define GL_FRAMEBUFFER 0x8D40
#define GL_RENDERBUFFER 0x8D41
#define GL_RENDERBUFFER_WIDTH 0x8D42
#define GL_RENDERBUFFER_HEIGHT 0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT 0x8D44
#define GL_STENCIL_INDEX1 0x8D46
#define GL_STENCIL_INDEX4 0x8D47
#define GL_STENCIL_INDEX8 0x8D48
#define GL_STENCIL_INDEX16 0x8D49
#define GL_RENDERBUFFER_RED_SIZE 0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE 0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE 0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE 0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE 0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE 0x8D55
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_MAX_SAMPLES 0x8D57
#define GL_INDEX 0x8222
#define GL_TEXTURE_LUMINANCE_TYPE 0x8C14
#define GL_TEXTURE_INTENSITY_TYPE 0x8C15
#define GL_FRAMEBUFFER_SRGB 0x8DB9
#define GL_HALF_FLOAT 0x140B
#define GL_MAP_READ_BIT 0x0001
#define GL_MAP_WRITE_BIT 0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT 0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT 0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT 0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT 0x0020
#define GL_COMPRESSED_RED_RGTC1 0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1 0x8DBC
#define GL_COMPRESSED_RG_RGTC2 0x8DBD
#define GL_COMPRESSED_SIGNED_RG_RGTC2 0x8DBE
#define GL_RG 0x8227
#define GL_RG_INTEGER 0x8228
#define GL_R8 0x8229
#define GL_R16 0x822A
#define GL_RG8 0x822B
#define GL_RG16 0x822C
#define GL_R16F 0x822D
#define GL_R32F 0x822E
#define GL_RG16F 0x822F
#define GL_RG32F 0x8230
#define GL_R8I 0x8231
#define GL_R8UI 0x8232
#define GL_R16I 0x8233
#define GL_R16UI 0x8234
#define GL_R32I 0x8235
#define GL_R32UI 0x8236
#define GL_RG8I 0x8237
#define GL_RG8UI 0x8238
#define GL_RG16I 0x8239
#define GL_RG16UI 0x823A
#define GL_RG32I 0x823B
#define GL_RG32UI 0x823C
#define GL_VERTEX_ARRAY_BINDING 0x85B5
#define GL_CLAMP_VERTEX_COLOR 0x891A
#define GL_CLAMP_FRAGMENT_COLOR 0x891B
#define GL_ALPHA_INTEGER 0x8D97

typedef void (GLAPIENTRY * GLColorMaski) (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
typedef void (GLAPIENTRY * GLGetBooleani_v) (GLenum target, GLuint index, GLboolean *data);
typedef void (GLAPIENTRY * GLGetIntegeri_v) (GLenum target, GLuint index, GLint *data);
typedef void (GLAPIENTRY * GLEnablei) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * GLDisablei) (GLenum target, GLuint index);
typedef GLboolean (GLAPIENTRY * GLIsEnabledi) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * GLBeginTransformFeedback) (GLenum primitiveMode);
typedef void (GLAPIENTRY * GLEndTransformFeedback) (void);
typedef void (GLAPIENTRY * GLBindBufferRange) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAPIENTRY * GLBindBufferBase) (GLenum target, GLuint index, GLuint buffer);
typedef void (GLAPIENTRY * GLTransformFeedbackVaryings) (GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode);
typedef void (GLAPIENTRY * GLGetTransformFeedbackVarying) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
typedef void (GLAPIENTRY * GLClampColor) (GLenum target, GLenum clamp);
typedef void (GLAPIENTRY * GLBeginConditionalRender) (GLuint id, GLenum mode);
typedef void (GLAPIENTRY * GLEndConditionalRender) (void);
typedef void (GLAPIENTRY * GLVertexAttribIPointer) (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (GLAPIENTRY * GLGetVertexAttribIiv) (GLuint index, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetVertexAttribIuiv) (GLuint index, GLenum pname, GLuint *params);
typedef void (GLAPIENTRY * GLVertexAttribI1i) (GLuint index, GLint x);
typedef void (GLAPIENTRY * GLVertexAttribI2i) (GLuint index, GLint x, GLint y);
typedef void (GLAPIENTRY * GLVertexAttribI3i) (GLuint index, GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * GLVertexAttribI4i) (GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * GLVertexAttribI1ui) (GLuint index, GLuint x);
typedef void (GLAPIENTRY * GLVertexAttribI2ui) (GLuint index, GLuint x, GLuint y);
typedef void (GLAPIENTRY * GLVertexAttribI3ui) (GLuint index, GLuint x, GLuint y, GLuint z);
typedef void (GLAPIENTRY * GLVertexAttribI4ui) (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * GLVertexAttribI1iv) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * GLVertexAttribI2iv) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * GLVertexAttribI3iv) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * GLVertexAttribI4iv) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * GLVertexAttribI1uiv) (GLuint index, const GLuint *v);
typedef void (GLAPIENTRY * GLVertexAttribI2uiv) (GLuint index, const GLuint *v);
typedef void (GLAPIENTRY * GLVertexAttribI3uiv) (GLuint index, const GLuint *v);
typedef void (GLAPIENTRY * GLVertexAttribI4uiv) (GLuint index, const GLuint *v);
typedef void (GLAPIENTRY * GLVertexAttribI4bv) (GLuint index, const GLbyte *v);
typedef void (GLAPIENTRY * GLVertexAttribI4sv) (GLuint index, const GLshort *v);
typedef void (GLAPIENTRY * GLVertexAttribI4ubv) (GLuint index, const GLubyte *v);
typedef void (GLAPIENTRY * GLVertexAttribI4usv) (GLuint index, const GLushort *v);
typedef void (GLAPIENTRY * GLGetUniformuiv) (GLuint program, GLint location, GLuint *params);
typedef void (GLAPIENTRY * GLBindFragDataLocation) (GLuint program, GLuint color, const GLchar *name);
typedef GLint (GLAPIENTRY * GLGetFragDataLocation) (GLuint program, const GLchar *name);
typedef void (GLAPIENTRY * GLUniform1ui) (GLint location, GLuint v0);
typedef void (GLAPIENTRY * GLUniform2ui) (GLint location, GLuint v0, GLuint v1);
typedef void (GLAPIENTRY * GLUniform3ui) (GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (GLAPIENTRY * GLUniform4ui) (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GLAPIENTRY * GLUniform1uiv) (GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * GLUniform2uiv) (GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * GLUniform3uiv) (GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * GLUniform4uiv) (GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * GLTexParameterIiv) (GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * GLTexParameterIuiv) (GLenum target, GLenum pname, const GLuint *params);
typedef void (GLAPIENTRY * GLGetTexParameterIiv) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetTexParameterIuiv) (GLenum target, GLenum pname, GLuint *params);
typedef void (GLAPIENTRY * GLClearBufferiv) (GLenum buffer, GLint drawbuffer, const GLint *value);
typedef void (GLAPIENTRY * GLClearBufferuiv) (GLenum buffer, GLint drawbuffer, const GLuint *value);
typedef void (GLAPIENTRY * GLClearBufferfv) (GLenum buffer, GLint drawbuffer, const GLfloat *value);
typedef void (GLAPIENTRY * GLClearBufferfi) (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
typedef const GLubyte * (GLAPIENTRY * GLGetStringi) (GLenum name, GLuint index);
typedef GLboolean (GLAPIENTRY * GLIsRenderbuffer) (GLuint renderbuffer);
typedef void (GLAPIENTRY * GLBindRenderbuffer) (GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * GLDeleteRenderbuffers) (GLsizei n, const GLuint *renderbuffers);
typedef void (GLAPIENTRY * GLGenRenderbuffers) (GLsizei n, GLuint *renderbuffers);
typedef void (GLAPIENTRY * GLRenderbufferStorage) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * GLGetRenderbufferParameteriv) (GLenum target, GLenum pname, GLint *params);
typedef GLboolean (GLAPIENTRY * GLIsFramebuffer) (GLuint framebuffer);
typedef void (GLAPIENTRY * GLBindFramebuffer) (GLenum target, GLuint framebuffer);
typedef void (GLAPIENTRY * GLDeleteFramebuffers) (GLsizei n, const GLuint *framebuffers);
typedef void (GLAPIENTRY * GLGenFramebuffers) (GLsizei n, GLuint *framebuffers);
typedef GLenum (GLAPIENTRY * GLCheckFramebufferStatus) (GLenum target);
typedef void (GLAPIENTRY * GLFramebufferTexture1D) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * GLFramebufferTexture2D) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * GLFramebufferTexture3D) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (GLAPIENTRY * GLFramebufferRenderbuffer) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAPIENTRY * GLGetFramebufferAttachmentParameteriv) (GLenum target, GLenum attachment, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGenerateMipmap) (GLenum target);
typedef void (GLAPIENTRY * GLBlitFramebuffer) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void (GLAPIENTRY * GLRenderbufferStorageMultisample) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * GLFramebufferTextureLayer) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void * (GLAPIENTRY * GLMapBufferRange) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void (GLAPIENTRY * GLFlushMappedBufferRange) (GLenum target, GLintptr offset, GLsizeiptr length);
typedef void (GLAPIENTRY * GLBindVertexArray) (GLuint array);
typedef void (GLAPIENTRY * GLDeleteVertexArrays) (GLsizei n, const GLuint *arrays);
typedef void (GLAPIENTRY * GLGenVertexArrays) (GLsizei n, GLuint *arrays);
typedef GLboolean (GLAPIENTRY * GLIsVertexArray) (GLuint array);


// OpenGL 3.1
#define GL_SAMPLER_2D_RECT 0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW 0x8B64
#define GL_SAMPLER_BUFFER 0x8DC2
#define GL_INT_SAMPLER_2D_RECT 0x8DCD
#define GL_INT_SAMPLER_BUFFER 0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT 0x8DD5
#define GL_UNSIGNED_INT_SAMPLER_BUFFER 0x8DD8
#define GL_TEXTURE_BUFFER 0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE 0x8C2B
#define GL_TEXTURE_BINDING_BUFFER 0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING 0x8C2D
#define GL_TEXTURE_RECTANGLE 0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE 0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE 0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE 0x84F8
#define GL_R8_SNORM 0x8F94
#define GL_RG8_SNORM 0x8F95
#define GL_RGB8_SNORM 0x8F96
#define GL_RGBA8_SNORM 0x8F97
#define GL_R16_SNORM 0x8F98
#define GL_RG16_SNORM 0x8F99
#define GL_RGB16_SNORM 0x8F9A
#define GL_RGBA16_SNORM 0x8F9B
#define GL_SIGNED_NORMALIZED 0x8F9C
#define GL_PRIMITIVE_RESTART 0x8F9D
#define GL_PRIMITIVE_RESTART_INDEX 0x8F9E
#define GL_COPY_READ_BUFFER 0x8F36
#define GL_COPY_WRITE_BUFFER 0x8F37
#define GL_UNIFORM_BUFFER 0x8A11
#define GL_UNIFORM_BUFFER_BINDING 0x8A28
#define GL_UNIFORM_BUFFER_START 0x8A29
#define GL_UNIFORM_BUFFER_SIZE 0x8A2A
#define GL_MAX_VERTEX_UNIFORM_BLOCKS 0x8A2B
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS 0x8A2D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS 0x8A2E
#define GL_MAX_UNIFORM_BUFFER_BINDINGS 0x8A2F
#define GL_MAX_UNIFORM_BLOCK_SIZE 0x8A30
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS 0x8A31
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS 0x8A33
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 0x8A34
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH 0x8A35
#define GL_ACTIVE_UNIFORM_BLOCKS 0x8A36
#define GL_UNIFORM_TYPE 0x8A37
#define GL_UNIFORM_SIZE 0x8A38
#define GL_UNIFORM_NAME_LENGTH 0x8A39
#define GL_UNIFORM_BLOCK_INDEX 0x8A3A
#define GL_UNIFORM_OFFSET 0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE 0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE 0x8A3D
#define GL_UNIFORM_IS_ROW_MAJOR 0x8A3E
#define GL_UNIFORM_BLOCK_BINDING 0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE 0x8A40
#define GL_UNIFORM_BLOCK_NAME_LENGTH 0x8A41
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS 0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES 0x8A43
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER 0x8A44
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER 0x8A46
#define GL_INVALID_INDEX 0xFFFFFFFFu

typedef void (GLAPIENTRY * GLDrawArraysInstanced) (GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
typedef void (GLAPIENTRY * GLDrawElementsInstanced) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei instancecount);
typedef void (GLAPIENTRY * GLTexBuffer) (GLenum target, GLenum internalformat, GLuint buffer);
typedef void (GLAPIENTRY * GLPrimitiveRestartIndex) (GLuint index);
typedef void (GLAPIENTRY * GLCopyBufferSubData) (GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
typedef void (GLAPIENTRY * GLGetUniformIndices) (GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices);
typedef void (GLAPIENTRY * GLGetActiveUniformsiv) (GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetActiveUniformName) (GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName);
typedef GLuint (GLAPIENTRY * GLGetUniformBlockIndex) (GLuint program, const GLchar *uniformBlockName);
typedef void (GLAPIENTRY * GLGetActiveUniformBlockiv) (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetActiveUniformBlockName) (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
typedef void (GLAPIENTRY * GLUniformBlockBinding) (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);


// OpenGL 3.2
typedef struct __GLsync *GLsync;
typedef uint64_t GLuint64;
typedef int64_t GLint64;

#define GL_CONTEXT_CORE_PROFILE_BIT 0x00000001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT 0x00000002
#define GL_LINES_ADJACENCY 0x000A
#define GL_LINE_STRIP_ADJACENCY 0x000B
#define GL_TRIANGLES_ADJACENCY 0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY 0x000D
#define GL_PROGRAM_POINT_SIZE 0x8642
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS 0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS 0x8DA8
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_GEOMETRY_VERTICES_OUT 0x8916
#define GL_GEOMETRY_INPUT_TYPE 0x8917
#define GL_GEOMETRY_OUTPUT_TYPE 0x8918
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS 0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS 0x8DE1
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS 0x9122
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS 0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS 0x9124
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS 0x9125
#define GL_CONTEXT_PROFILE_MASK 0x9126
#define GL_DEPTH_CLAMP 0x864F
#define GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION 0x8E4C
#define GL_FIRST_VERTEX_CONVENTION 0x8E4D
#define GL_LAST_VERTEX_CONVENTION 0x8E4E
#define GL_PROVOKING_VERTEX 0x8E4F
#define GL_TEXTURE_CUBE_MAP_SEAMLESS 0x884F
#define GL_MAX_SERVER_WAIT_TIMEOUT 0x9111
#define GL_OBJECT_TYPE 0x9112
#define GL_SYNC_CONDITION 0x9113
#define GL_SYNC_STATUS 0x9114
#define GL_SYNC_FLAGS 0x9115
#define GL_SYNC_FENCE 0x9116
#define GL_SYNC_GPU_COMMANDS_COMPLETE 0x9117
#define GL_UNSIGNALED 0x9118
#define GL_SIGNALED 0x9119
#define GL_ALREADY_SIGNALED 0x911A
#define GL_TIMEOUT_EXPIRED 0x911B
#define GL_CONDITION_SATISFIED 0x911C
#define GL_WAIT_FAILED 0x911D
#define GL_TIMEOUT_IGNORED 0xFFFFFFFFFFFFFFFFull
#define GL_SYNC_FLUSH_COMMANDS_BIT 0x00000001
#define GL_SAMPLE_POSITION 0x8E50
#define GL_SAMPLE_MASK 0x8E51
#define GL_SAMPLE_MASK_VALUE 0x8E52
#define GL_MAX_SAMPLE_MASK_WORDS 0x8E59
#define GL_TEXTURE_2D_MULTISAMPLE 0x9100
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE 0x9101
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9102
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9103
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE 0x9104
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY 0x9105
#define GL_TEXTURE_SAMPLES 0x9106
#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS 0x9107
#define GL_SAMPLER_2D_MULTISAMPLE 0x9108
#define GL_INT_SAMPLER_2D_MULTISAMPLE 0x9109
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE 0x910A
#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910B
#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910C
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910D
#define GL_MAX_COLOR_TEXTURE_SAMPLES 0x910E
#define GL_MAX_DEPTH_TEXTURE_SAMPLES 0x910F
#define GL_MAX_INTEGER_SAMPLES 0x9110

typedef void (GLAPIENTRY * GLDrawElementsBaseVertex) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
typedef void (GLAPIENTRY * GLDrawRangeElementsBaseVertex) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
typedef void (GLAPIENTRY * GLDrawElementsInstancedBaseVertex) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei instancecount, GLint basevertex);
typedef void (GLAPIENTRY * GLMultiDrawElementsBaseVertex) (GLenum mode, const GLsizei *count, GLenum type, const GLvoid *const*indices, GLsizei drawcount, const GLint *basevertex);
typedef void (GLAPIENTRY * GLProvokingVertex) (GLenum mode);
typedef GLsync (GLAPIENTRY * GLFenceSync) (GLenum condition, GLbitfield flags);
typedef GLboolean (GLAPIENTRY * GLIsSync) (GLsync sync);
typedef void (GLAPIENTRY * GLDeleteSync) (GLsync sync);
typedef GLenum (GLAPIENTRY * GLClientWaitSync) (GLsync sync, GLbitfield flags, GLuint64 timeout);
typedef void (GLAPIENTRY * GLWaitSync) (GLsync sync, GLbitfield flags, GLuint64 timeout);
typedef void (GLAPIENTRY * GLGetInteger64v) (GLenum pname, GLint64 *params);
typedef void (GLAPIENTRY * GLGetSynciv) (GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
typedef void (GLAPIENTRY * GLGetInteger64i_v) (GLenum target, GLuint index, GLint64 *data);
typedef void (GLAPIENTRY * GLGetBufferParameteri64v) (GLenum target, GLenum pname, GLint64 *params);
typedef void (GLAPIENTRY * GLFramebufferTexture) (GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAPIENTRY * GLTexImage2DMultisample) (GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * GLTexImage3DMultisample) (GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * GLGetMultisamplefv) (GLenum pname, GLuint index, GLfloat *val);
typedef void (GLAPIENTRY * GLSampleMaski) (GLuint index, GLbitfield mask);


// OpenGL 3.3
#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR 0x88FE
#define GL_SRC1_COLOR 0x88F9
#define GL_ONE_MINUS_SRC1_COLOR 0x88FA
#define GL_ONE_MINUS_SRC1_ALPHA 0x88FB
#define GL_MAX_DUAL_SOURCE_DRAW_BUFFERS 0x88FC
#define GL_ANY_SAMPLES_PASSED 0x8C2F
#define GL_SAMPLER_BINDING 0x8919
#define GL_RGB10_A2UI 0x906F
#define GL_TEXTURE_SWIZZLE_R 0x8E42
#define GL_TEXTURE_SWIZZLE_G 0x8E43
#define GL_TEXTURE_SWIZZLE_B 0x8E44
#define GL_TEXTURE_SWIZZLE_A 0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA 0x8E46
#define GL_TIME_ELAPSED 0x88BF
#define GL_TIMESTAMP 0x8E28
#define GL_INT_2_10_10_10_REV 0x8D9F

typedef void (GLAPIENTRY * GLBindFragDataLocationIndexed) (GLuint program, GLuint colorNumber, GLuint index, const GLchar *name);
typedef GLint (GLAPIENTRY * GLGetFragDataIndex) (GLuint program, const GLchar *name);
typedef void (GLAPIENTRY * GLGenSamplers) (GLsizei count, GLuint *samplers);
typedef void (GLAPIENTRY * GLDeleteSamplers) (GLsizei count, const GLuint *samplers);
typedef GLboolean (GLAPIENTRY * GLIsSampler) (GLuint sampler);
typedef void (GLAPIENTRY * GLBindSampler) (GLuint unit, GLuint sampler);
typedef void (GLAPIENTRY * GLSamplerParameteri) (GLuint sampler, GLenum pname, GLint param);
typedef void (GLAPIENTRY * GLSamplerParameteriv) (GLuint sampler, GLenum pname, const GLint *param);
typedef void (GLAPIENTRY * GLSamplerParameterf) (GLuint sampler, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * GLSamplerParameterfv) (GLuint sampler, GLenum pname, const GLfloat *param);
typedef void (GLAPIENTRY * GLSamplerParameterIiv) (GLuint sampler, GLenum pname, const GLint *param);
typedef void (GLAPIENTRY * GLSamplerParameterIuiv) (GLuint sampler, GLenum pname, const GLuint *param);
typedef void (GLAPIENTRY * GLGetSamplerParameteriv) (GLuint sampler, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetSamplerParameterIiv) (GLuint sampler, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetSamplerParameterfv) (GLuint sampler, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * GLGetSamplerParameterIuiv) (GLuint sampler, GLenum pname, GLuint *params);
typedef void (GLAPIENTRY * GLQueryCounter) (GLuint id, GLenum target);
typedef void (GLAPIENTRY * GLGetQueryObjecti64v) (GLuint id, GLenum pname, GLint64 *params);
typedef void (GLAPIENTRY * GLGetQueryObjectui64v) (GLuint id, GLenum pname, GLuint64 *params);
typedef void (GLAPIENTRY * GLVertexAttribDivisor) (GLuint index, GLuint divisor);
typedef void (GLAPIENTRY * GLVertexAttribP1ui) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAPIENTRY * GLVertexAttribP1uiv) (GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
typedef void (GLAPIENTRY * GLVertexAttribP2ui) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAPIENTRY * GLVertexAttribP2uiv) (GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
typedef void (GLAPIENTRY * GLVertexAttribP3ui) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAPIENTRY * GLVertexAttribP3uiv) (GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
typedef void (GLAPIENTRY * GLVertexAttribP4ui) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAPIENTRY * GLVertexAttribP4uiv) (GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
typedef void (GLAPIENTRY * GLVertexP2ui) (GLenum type, GLuint value);
typedef void (GLAPIENTRY * GLVertexP2uiv) (GLenum type, const GLuint *value);
typedef void (GLAPIENTRY * GLVertexP3ui) (GLenum type, GLuint value);
typedef void (GLAPIENTRY * GLVertexP3uiv) (GLenum type, const GLuint *value);
typedef void (GLAPIENTRY * GLVertexP4ui) (GLenum type, GLuint value);
typedef void (GLAPIENTRY * GLVertexP4uiv) (GLenum type, const GLuint *value);
typedef void (GLAPIENTRY * GLTexCoordP1ui) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * GLTexCoordP1uiv) (GLenum type, const GLuint *coords);
typedef void (GLAPIENTRY * GLTexCoordP2ui) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * GLTexCoordP2uiv) (GLenum type, const GLuint *coords);
typedef void (GLAPIENTRY * GLTexCoordP3ui) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * GLTexCoordP3uiv) (GLenum type, const GLuint *coords);
typedef void (GLAPIENTRY * GLTexCoordP4ui) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * GLTexCoordP4uiv) (GLenum type, const GLuint *coords);
typedef void (GLAPIENTRY * GLMultiTexCoordP1ui) (GLenum texture, GLenum type, GLuint coords);
typedef void (GLAPIENTRY * GLMultiTexCoordP1uiv) (GLenum texture, GLenum type, const GLuint *coords);
typedef void (GLAPIENTRY * GLMultiTexCoordP2ui) (GLenum texture, GLenum type, GLuint coords);
typedef void (GLAPIENTRY * GLMultiTexCoordP2uiv) (GLenum texture, GLenum type, const GLuint *coords);
typedef void (GLAPIENTRY * GLMultiTexCoordP3ui) (GLenum texture, GLenum type, GLuint coords);
typedef void (GLAPIENTRY * GLMultiTexCoordP3uiv) (GLenum texture, GLenum type, const GLuint *coords);
typedef void (GLAPIENTRY * GLMultiTexCoordP4ui) (GLenum texture, GLenum type, GLuint coords);
typedef void (GLAPIENTRY * GLMultiTexCoordP4uiv) (GLenum texture, GLenum type, const GLuint *coords);
typedef void (GLAPIENTRY * GLNormalP3ui) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * GLNormalP3uiv) (GLenum type, const GLuint *coords);
typedef void (GLAPIENTRY * GLColorP3ui) (GLenum type, GLuint color);
typedef void (GLAPIENTRY * GLColorP3uiv) (GLenum type, const GLuint *color);
typedef void (GLAPIENTRY * GLColorP4ui) (GLenum type, GLuint color);
typedef void (GLAPIENTRY * GLColorP4uiv) (GLenum type, const GLuint *color);
typedef void (GLAPIENTRY * GLSecondaryColorP3ui) (GLenum type, GLuint color);
typedef void (GLAPIENTRY * GLSecondaryColorP3uiv) (GLenum type, const GLuint *color);


// OpenGL 4.0
#define GL_SAMPLE_SHADING 0x8C36
#define GL_MIN_SAMPLE_SHADING_VALUE 0x8C37
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5F
#define GL_TEXTURE_CUBE_MAP_ARRAY 0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY 0x900A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARRAY 0x900B
#define GL_SAMPLER_CUBE_MAP_ARRAY 0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW 0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY 0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY 0x900F
#define GL_DRAW_INDIRECT_BUFFER 0x8F3F
#define GL_DRAW_INDIRECT_BUFFER_BINDING 0x8F43
#define GL_GEOMETRY_SHADER_INVOCATIONS 0x887F
#define GL_MAX_GEOMETRY_SHADER_INVOCATIONS 0x8E5A
#define GL_MIN_FRAGMENT_INTERPOLATION_OFFSET 0x8E5B
#define GL_MAX_FRAGMENT_INTERPOLATION_OFFSET 0x8E5C
#define GL_FRAGMENT_INTERPOLATION_OFFSET_BITS 0x8E5D
#define GL_MAX_VERTEX_STREAMS 0x8E71
#define GL_DOUBLE_VEC2 0x8FFC
#define GL_DOUBLE_VEC3 0x8FFD
#define GL_DOUBLE_VEC4 0x8FFE
#define GL_DOUBLE_MAT2 0x8F46
#define GL_DOUBLE_MAT3 0x8F47
#define GL_DOUBLE_MAT4 0x8F48
#define GL_DOUBLE_MAT2x3 0x8F49
#define GL_DOUBLE_MAT2x4 0x8F4A
#define GL_DOUBLE_MAT3x2 0x8F4B
#define GL_DOUBLE_MAT3x4 0x8F4C
#define GL_DOUBLE_MAT4x2 0x8F4D
#define GL_DOUBLE_MAT4x3 0x8F4E
#define GL_ACTIVE_SUBROUTINES 0x8DE5
#define GL_ACTIVE_SUBROUTINE_UNIFORMS 0x8DE6
#define GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS 0x8E47
#define GL_ACTIVE_SUBROUTINE_MAX_LENGTH 0x8E48
#define GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH 0x8E49
#define GL_MAX_SUBROUTINES 0x8DE7
#define GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS 0x8DE8
#define GL_NUM_COMPATIBLE_SUBROUTINES 0x8E4A
#define GL_COMPATIBLE_SUBROUTINES 0x8E4B
#define GL_PATCHES 0x000E
#define GL_PATCH_VERTICES 0x8E72
#define GL_PATCH_DEFAULT_INNER_LEVEL 0x8E73
#define GL_PATCH_DEFAULT_OUTER_LEVEL 0x8E74
#define GL_TESS_CONTROL_OUTPUT_VERTICES 0x8E75
#define GL_TESS_GEN_MODE 0x8E76
#define GL_TESS_GEN_SPACING 0x8E77
#define GL_TESS_GEN_VERTEX_ORDER 0x8E78
#define GL_TESS_GEN_POINT_MODE 0x8E79
#define GL_ISOLINES 0x8E7A
#define GL_FRACTIONAL_ODD 0x8E7B
#define GL_FRACTIONAL_EVEN 0x8E7C
#define GL_MAX_PATCH_VERTICES 0x8E7D
#define GL_MAX_TESS_GEN_LEVEL 0x8E7E
#define GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS 0x8E7F
#define GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS 0x8E80
#define GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS 0x8E81
#define GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS 0x8E82
#define GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS 0x8E83
#define GL_MAX_TESS_PATCH_COMPONENTS 0x8E84
#define GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS 0x8E85
#define GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS 0x8E86
#define GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS 0x8E89
#define GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS 0x8E8A
#define GL_MAX_TESS_CONTROL_INPUT_COMPONENTS 0x886C
#define GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS 0x886D
#define GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS 0x8E1E
#define GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS 0x8E1F
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER 0x84F0
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER 0x84F1
#define GL_TESS_EVALUATION_SHADER 0x8E87
#define GL_TESS_CONTROL_SHADER 0x8E88
#define GL_TRANSFORM_FEEDBACK 0x8E22
#define GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED 0x8E23
#define GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE 0x8E24
#define GL_TRANSFORM_FEEDBACK_BINDING 0x8E25
#define GL_MAX_TRANSFORM_FEEDBACK_BUFFERS 0x8E70

typedef void (GLAPIENTRY * GLMinSampleShading) (GLfloat value);
typedef void (GLAPIENTRY * GLBlendEquationi) (GLuint buf, GLenum mode);
typedef void (GLAPIENTRY * GLBlendEquationSeparatei) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void (GLAPIENTRY * GLBlendFunci) (GLuint buf, GLenum src, GLenum dst);
typedef void (GLAPIENTRY * GLBlendFuncSeparatei) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
typedef void (GLAPIENTRY * GLDrawArraysIndirect) (GLenum mode, const GLvoid *indirect);
typedef void (GLAPIENTRY * GLDrawElementsIndirect) (GLenum mode, GLenum type, const GLvoid *indirect);
typedef void (GLAPIENTRY * GLUniform1d) (GLint location, GLdouble x);
typedef void (GLAPIENTRY * GLUniform2d) (GLint location, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * GLUniform3d) (GLint location, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * GLUniform4d) (GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * GLUniform1dv) (GLint location, GLsizei count, const GLdouble *value);
typedef void (GLAPIENTRY * GLUniform2dv) (GLint location, GLsizei count, const GLdouble *value);
typedef void (GLAPIENTRY * GLUniform3dv) (GLint location, GLsizei count, const GLdouble *value);
typedef void (GLAPIENTRY * GLUniform4dv) (GLint location, GLsizei count, const GLdouble *value);
typedef void (GLAPIENTRY * GLUniformMatrix2dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * GLUniformMatrix3dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * GLUniformMatrix4dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * GLUniformMatrix2x3dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * GLUniformMatrix2x4dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * GLUniformMatrix3x2dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * GLUniformMatrix3x4dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * GLUniformMatrix4x2dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * GLUniformMatrix4x3dv) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * GLGetUniformdv) (GLuint program, GLint location, GLdouble *params);
typedef GLint (GLAPIENTRY * GLGetSubroutineUniformLocation) (GLuint program, GLenum shadertype, const GLchar *name);
typedef GLuint (GLAPIENTRY * GLGetSubroutineIndex) (GLuint program, GLenum shadertype, const GLchar *name);
typedef void (GLAPIENTRY * GLGetActiveSubroutineUniformiv) (GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values);
typedef void (GLAPIENTRY * GLGetActiveSubroutineUniformName) (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
typedef void (GLAPIENTRY * GLGetActiveSubroutineName) (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
typedef void (GLAPIENTRY * GLUniformSubroutinesuiv) (GLenum shadertype, GLsizei count, const GLuint *indices);
typedef void (GLAPIENTRY * GLGetUniformSubroutineuiv) (GLenum shadertype, GLint location, GLuint *params);
typedef void (GLAPIENTRY * GLGetProgramStageiv) (GLuint program, GLenum shadertype, GLenum pname, GLint *values);
typedef void (GLAPIENTRY * GLPatchParameteri) (GLenum pname, GLint value);
typedef void (GLAPIENTRY * GLPatchParameterfv) (GLenum pname, const GLfloat *values);
typedef void (GLAPIENTRY * GLBindTransformFeedback) (GLenum target, GLuint id);
typedef void (GLAPIENTRY * GLDeleteTransformFeedbacks) (GLsizei n, const GLuint *ids);
typedef void (GLAPIENTRY * GLGenTransformFeedbacks) (GLsizei n, GLuint *ids);
typedef GLboolean (GLAPIENTRY * GLIsTransformFeedback) (GLuint id);
typedef void (GLAPIENTRY * GLPauseTransformFeedback) (void);
typedef void (GLAPIENTRY * GLResumeTransformFeedback) (void);
typedef void (GLAPIENTRY * GLDrawTransformFeedback) (GLenum mode, GLuint id);
typedef void (GLAPIENTRY * GLDrawTransformFeedbackStream) (GLenum mode, GLuint id, GLuint stream);
typedef void (GLAPIENTRY * GLBeginQueryIndexed) (GLenum target, GLuint index, GLuint id);
typedef void (GLAPIENTRY * GLEndQueryIndexed) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * GLGetQueryIndexediv) (GLenum target, GLuint index, GLenum pname, GLint *params);


// OpenGL 4.1
#define GL_FIXED 0x140C
#define GL_IMPLEMENTATION_COLOR_READ_TYPE 0x8B9A
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B
#define GL_LOW_FLOAT 0x8DF0
#define GL_MEDIUM_FLOAT 0x8DF1
#define GL_HIGH_FLOAT 0x8DF2
#define GL_LOW_INT 0x8DF3
#define GL_MEDIUM_INT 0x8DF4
#define GL_HIGH_INT 0x8DF5
#define GL_SHADER_COMPILER 0x8DFA
#define GL_SHADER_BINARY_FORMATS 0x8DF8
#define GL_NUM_SHADER_BINARY_FORMATS 0x8DF9
#define GL_MAX_VERTEX_UNIFORM_VECTORS 0x8DFB
#define GL_MAX_VARYING_VECTORS 0x8DFC
#define GL_MAX_FRAGMENT_UNIFORM_VECTORS 0x8DFD
#define GL_RGB565 0x8D62
#define GL_PROGRAM_BINARY_RETRIEVABLE_HINT 0x8257
#define GL_PROGRAM_BINARY_LENGTH 0x8741
#define GL_NUM_PROGRAM_BINARY_FORMATS 0x87FE
#define GL_PROGRAM_BINARY_FORMATS 0x87FF
#define GL_VERTEX_SHADER_BIT 0x00000001
#define GL_FRAGMENT_SHADER_BIT 0x00000002
#define GL_GEOMETRY_SHADER_BIT 0x00000004
#define GL_TESS_CONTROL_SHADER_BIT 0x00000008
#define GL_TESS_EVALUATION_SHADER_BIT 0x00000010
#define GL_ALL_SHADER_BITS 0xFFFFFFFF
#define GL_PROGRAM_SEPARABLE 0x8258
#define GL_ACTIVE_PROGRAM 0x8259
#define GL_PROGRAM_PIPELINE_BINDING 0x825A
#define GL_MAX_VIEWPORTS 0x825B
#define GL_VIEWPORT_SUBPIXEL_BITS 0x825C
#define GL_VIEWPORT_BOUNDS_RANGE 0x825D
#define GL_LAYER_PROVOKING_VERTEX 0x825E
#define GL_VIEWPORT_INDEX_PROVOKING_VERTEX 0x825F
#define GL_UNDEFINED_VERTEX 0x8260

typedef void (GLAPIENTRY * GLReleaseShaderCompiler) (void);
typedef void (GLAPIENTRY * GLShaderBinary) (GLsizei count, const GLuint *shaders, GLenum binaryformat, const GLvoid *binary, GLsizei length);
typedef void (GLAPIENTRY * GLGetShaderPrecisionFormat) (GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
typedef void (GLAPIENTRY * GLDepthRangef) (GLfloat n, GLfloat f);
typedef void (GLAPIENTRY * GLClearDepthf) (GLfloat d);
typedef void (GLAPIENTRY * GLGetProgramBinary) (GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary);
typedef void (GLAPIENTRY * GLProgramBinary) (GLuint program, GLenum binaryFormat, const GLvoid *binary, GLsizei length);
typedef void (GLAPIENTRY * GLProgramParameteri) (GLuint program, GLenum pname, GLint value);
typedef void (GLAPIENTRY * GLUseProgramStages) (GLuint pipeline, GLbitfield stages, GLuint program);
typedef void (GLAPIENTRY * GLActiveShaderProgram) (GLuint pipeline, GLuint program);
typedef GLuint (GLAPIENTRY * GLCreateShaderProgramv) (GLenum type, GLsizei count, const GLchar *const*strings);
typedef void (GLAPIENTRY * GLBindProgramPipeline) (GLuint pipeline);
typedef void (GLAPIENTRY * GLDeleteProgramPipelines) (GLsizei n, const GLuint *pipelines);
typedef void (GLAPIENTRY * GLGenProgramPipelines) (GLsizei n, GLuint *pipelines);
typedef GLboolean (GLAPIENTRY * GLIsProgramPipeline) (GLuint pipeline);
typedef void (GLAPIENTRY * GLGetProgramPipelineiv) (GLuint pipeline, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLProgramUniform1i) (GLuint program, GLint location, GLint v0);
typedef void (GLAPIENTRY * GLProgramUniform1iv) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (GLAPIENTRY * GLProgramUniform1f) (GLuint program, GLint location, GLfloat v0);
typedef void (GLAPIENTRY * GLProgramUniform1fv) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniform1d) (GLuint program, GLint location, GLdouble v0);
typedef void (GLAPIENTRY * GLProgramUniform1dv) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void (GLAPIENTRY * GLProgramUniform1ui) (GLuint program, GLint location, GLuint v0);
typedef void (GLAPIENTRY * GLProgramUniform1uiv) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * GLProgramUniform2i) (GLuint program, GLint location, GLint v0, GLint v1);
typedef void (GLAPIENTRY * GLProgramUniform2iv) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (GLAPIENTRY * GLProgramUniform2f) (GLuint program, GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAPIENTRY * GLProgramUniform2fv) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniform2d) (GLuint program, GLint location, GLdouble v0, GLdouble v1);
typedef void (GLAPIENTRY * GLProgramUniform2dv) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void (GLAPIENTRY * GLProgramUniform2ui) (GLuint program, GLint location, GLuint v0, GLuint v1);
typedef void (GLAPIENTRY * GLProgramUniform2uiv) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * GLProgramUniform3i) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAPIENTRY * GLProgramUniform3iv) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (GLAPIENTRY * GLProgramUniform3f) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY * GLProgramUniform3fv) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniform3d) (GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2);
typedef void (GLAPIENTRY * GLProgramUniform3dv) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void (GLAPIENTRY * GLProgramUniform3ui) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (GLAPIENTRY * GLProgramUniform3uiv) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * GLProgramUniform4i) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAPIENTRY * GLProgramUniform4iv) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (GLAPIENTRY * GLProgramUniform4f) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAPIENTRY * GLProgramUniform4fv) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniform4d) (GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
typedef void (GLAPIENTRY * GLProgramUniform4dv) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void (GLAPIENTRY * GLProgramUniform4ui) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GLAPIENTRY * GLProgramUniform4uiv) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix2fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix3fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix4fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix2dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix3dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix4dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix2x3fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix3x2fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix2x4fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix4x2fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix3x4fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix4x3fv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix2x3dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix3x2dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix2x4dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix4x2dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix3x4dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix4x3dv) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * GLValidateProgramPipeline) (GLuint pipeline);
typedef void (GLAPIENTRY * GLGetProgramPipelineInfoLog) (GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (GLAPIENTRY * GLVertexAttribL1d) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * GLVertexAttribL2d) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * GLVertexAttribL3d) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * GLVertexAttribL4d) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * GLVertexAttribL1dv) (GLuint index, const GLdouble *v);
typedef void (GLAPIENTRY * GLVertexAttribL2dv) (GLuint index, const GLdouble *v);
typedef void (GLAPIENTRY * GLVertexAttribL3dv) (GLuint index, const GLdouble *v);
typedef void (GLAPIENTRY * GLVertexAttribL4dv) (GLuint index, const GLdouble *v);
typedef void (GLAPIENTRY * GLVertexAttribLPointer) (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (GLAPIENTRY * GLGetVertexAttribLdv) (GLuint index, GLenum pname, GLdouble *params);
typedef void (GLAPIENTRY * GLViewportArrayv) (GLuint first, GLsizei count, const GLfloat *v);
typedef void (GLAPIENTRY * GLViewportIndexedf) (GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
typedef void (GLAPIENTRY * GLViewportIndexedfv) (GLuint index, const GLfloat *v);
typedef void (GLAPIENTRY * GLScissorArrayv) (GLuint first, GLsizei count, const GLint *v);
typedef void (GLAPIENTRY * GLScissorIndexed) (GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * GLScissorIndexedv) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * GLDepthRangeArrayv) (GLuint first, GLsizei count, const GLdouble *v);
typedef void (GLAPIENTRY * GLDepthRangeIndexed) (GLuint index, GLdouble n, GLdouble f);
typedef void (GLAPIENTRY * GLGetFloati_v) (GLenum target, GLuint index, GLfloat *data);
typedef void (GLAPIENTRY * GLGetDoublei_v) (GLenum target, GLuint index, GLdouble *data);


// OpenGL 4.2
#define GL_UNPACK_COMPRESSED_BLOCK_WIDTH 0x9127
#define GL_UNPACK_COMPRESSED_BLOCK_HEIGHT 0x9128
#define GL_UNPACK_COMPRESSED_BLOCK_DEPTH 0x9129
#define GL_UNPACK_COMPRESSED_BLOCK_SIZE 0x912A
#define GL_PACK_COMPRESSED_BLOCK_WIDTH 0x912B
#define GL_PACK_COMPRESSED_BLOCK_HEIGHT 0x912C
#define GL_PACK_COMPRESSED_BLOCK_DEPTH 0x912D
#define GL_PACK_COMPRESSED_BLOCK_SIZE 0x912E
#define GL_NUM_SAMPLE_COUNTS 0x9380
#define GL_MIN_MAP_BUFFER_ALIGNMENT 0x90BC
#define GL_ATOMIC_COUNTER_BUFFER 0x92C0
#define GL_ATOMIC_COUNTER_BUFFER_BINDING 0x92C1
#define GL_ATOMIC_COUNTER_BUFFER_START 0x92C2
#define GL_ATOMIC_COUNTER_BUFFER_SIZE 0x92C3
#define GL_ATOMIC_COUNTER_BUFFER_DATA_SIZE 0x92C4
#define GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS 0x92C5
#define GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES 0x92C6
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER 0x92C7
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER 0x92C8
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER 0x92C9
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER 0x92CA
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER 0x92CB
#define GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS 0x92CC
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS 0x92CD
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS 0x92CE
#define GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS 0x92CF
#define GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS 0x92D0
#define GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS 0x92D1
#define GL_MAX_VERTEX_ATOMIC_COUNTERS 0x92D2
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS 0x92D3
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS 0x92D4
#define GL_MAX_GEOMETRY_ATOMIC_COUNTERS 0x92D5
#define GL_MAX_FRAGMENT_ATOMIC_COUNTERS 0x92D6
#define GL_MAX_COMBINED_ATOMIC_COUNTERS 0x92D7
#define GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE 0x92D8
#define GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS 0x92DC
#define GL_ACTIVE_ATOMIC_COUNTER_BUFFERS 0x92D9
#define GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX 0x92DA
#define GL_UNSIGNED_INT_ATOMIC_COUNTER 0x92DB
#define GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT 0x00000001
#define GL_ELEMENT_ARRAY_BARRIER_BIT 0x00000002
#define GL_UNIFORM_BARRIER_BIT 0x00000004
#define GL_TEXTURE_FETCH_BARRIER_BIT 0x00000008
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT 0x00000020
#define GL_COMMAND_BARRIER_BIT 0x00000040
#define GL_PIXEL_BUFFER_BARRIER_BIT 0x00000080
#define GL_TEXTURE_UPDATE_BARRIER_BIT 0x00000100
#define GL_BUFFER_UPDATE_BARRIER_BIT 0x00000200
#define GL_FRAMEBUFFER_BARRIER_BIT 0x00000400
#define GL_TRANSFORM_FEEDBACK_BARRIER_BIT 0x00000800
#define GL_ATOMIC_COUNTER_BARRIER_BIT 0x00001000
#define GL_ALL_BARRIER_BITS 0xFFFFFFFF
#define GL_MAX_IMAGE_UNITS 0x8F38
#define GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS 0x8F39
#define GL_IMAGE_BINDING_NAME 0x8F3A
#define GL_IMAGE_BINDING_LEVEL 0x8F3B
#define GL_IMAGE_BINDING_LAYERED 0x8F3C
#define GL_IMAGE_BINDING_LAYER 0x8F3D
#define GL_IMAGE_BINDING_ACCESS 0x8F3E
#define GL_IMAGE_1D 0x904C
#define GL_IMAGE_2D 0x904D
#define GL_IMAGE_3D 0x904E
#define GL_IMAGE_2D_RECT 0x904F
#define GL_IMAGE_CUBE 0x9050
#define GL_IMAGE_BUFFER 0x9051
#define GL_IMAGE_1D_ARRAY 0x9052
#define GL_IMAGE_2D_ARRAY 0x9053
#define GL_IMAGE_CUBE_MAP_ARRAY 0x9054
#define GL_IMAGE_2D_MULTISAMPLE 0x9055
#define GL_IMAGE_2D_MULTISAMPLE_ARRAY 0x9056
#define GL_INT_IMAGE_1D 0x9057
#define GL_INT_IMAGE_2D 0x9058
#define GL_INT_IMAGE_3D 0x9059
#define GL_INT_IMAGE_2D_RECT 0x905A
#define GL_INT_IMAGE_CUBE 0x905B
#define GL_INT_IMAGE_BUFFER 0x905C
#define GL_INT_IMAGE_1D_ARRAY 0x905D
#define GL_INT_IMAGE_2D_ARRAY 0x905E
#define GL_INT_IMAGE_CUBE_MAP_ARRAY 0x905F
#define GL_INT_IMAGE_2D_MULTISAMPLE 0x9060
#define GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY 0x9061
#define GL_UNSIGNED_INT_IMAGE_1D 0x9062
#define GL_UNSIGNED_INT_IMAGE_2D 0x9063
#define GL_UNSIGNED_INT_IMAGE_3D 0x9064
#define GL_UNSIGNED_INT_IMAGE_2D_RECT 0x9065
#define GL_UNSIGNED_INT_IMAGE_CUBE 0x9066
#define GL_UNSIGNED_INT_IMAGE_BUFFER 0x9067
#define GL_UNSIGNED_INT_IMAGE_1D_ARRAY 0x9068
#define GL_UNSIGNED_INT_IMAGE_2D_ARRAY 0x9069
#define GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY 0x906A
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE 0x906B
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY 0x906C
#define GL_MAX_IMAGE_SAMPLES 0x906D
#define GL_IMAGE_BINDING_FORMAT 0x906E
#define GL_IMAGE_FORMAT_COMPATIBILITY_TYPE 0x90C7
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_SIZE 0x90C8
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_CLASS 0x90C9
#define GL_MAX_VERTEX_IMAGE_UNIFORMS 0x90CA
#define GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS 0x90CB
#define GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS 0x90CC
#define GL_MAX_GEOMETRY_IMAGE_UNIFORMS 0x90CD
#define GL_MAX_FRAGMENT_IMAGE_UNIFORMS 0x90CE
#define GL_MAX_COMBINED_IMAGE_UNIFORMS 0x90CF
#define GL_TEXTURE_IMMUTABLE_FORMAT 0x912F

typedef void (GLAPIENTRY * GLDrawArraysInstancedBaseInstance) (GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance);
typedef void (GLAPIENTRY * GLDrawElementsInstancedBaseInstance) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLuint baseinstance);
typedef void (GLAPIENTRY * GLDrawElementsInstancedBaseVertexBaseInstance) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);
typedef void (GLAPIENTRY * GLGetInternalformati64v) (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64 *params);
typedef void (GLAPIENTRY * GLGetActiveAtomicCounterBufferiv) (GLuint program, GLuint bufferIndex, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLBindImageTexture) (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
typedef void (GLAPIENTRY * GLMemoryBarrier) (GLbitfield barriers);
typedef void (GLAPIENTRY * GLTexStorage1D) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void (GLAPIENTRY * GLTexStorage2D) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * GLTexStorage3D) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
typedef void (GLAPIENTRY * GLDrawTransformFeedbackInstanced) (GLenum mode, GLuint id, GLsizei instancecount);
typedef void (GLAPIENTRY * GLDrawTransformFeedbackStreamInstanced) (GLenum mode, GLuint id, GLuint stream, GLsizei instancecount);


// OpenGL 4.3
typedef void (GLAPIENTRY * GLDEBUGPROC) (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

#define GL_NUM_SHADING_LANGUAGE_VERSIONS 0x82E9
#define GL_VERTEX_ATTRIB_ARRAY_LONG 0x874E
#define GL_COMPRESSED_RGB8_ETC2 0x9274
#define GL_COMPRESSED_SRGB8_ETC2 0x9275
#define GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9276
#define GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9277
#define GL_COMPRESSED_RGBA8_ETC2_EAC 0x9278
#define GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC 0x9279
#define GL_COMPRESSED_R11_EAC 0x9270
#define GL_COMPRESSED_SIGNED_R11_EAC 0x9271
#define GL_COMPRESSED_RG11_EAC 0x9272
#define GL_COMPRESSED_SIGNED_RG11_EAC 0x9273
#define GL_PRIMITIVE_RESTART_FIXED_INDEX 0x8D69
#define GL_ANY_SAMPLES_PASSED_CONSERVATIVE 0x8D6A
#define GL_MAX_ELEMENT_INDEX 0x8D6B
#define GL_COMPUTE_SHADER 0x91B9
#define GL_MAX_COMPUTE_UNIFORM_BLOCKS 0x91BB
#define GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS 0x91BC
#define GL_MAX_COMPUTE_IMAGE_UNIFORMS 0x91BD
#define GL_MAX_COMPUTE_SHARED_MEMORY_SIZE 0x8262
#define GL_MAX_COMPUTE_UNIFORM_COMPONENTS 0x8263
#define GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS 0x8264
#define GL_MAX_COMPUTE_ATOMIC_COUNTERS 0x8265
#define GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS 0x8266
#define GL_MAX_COMPUTE_LOCAL_INVOCATIONS 0x90EB
#define GL_MAX_COMPUTE_WORK_GROUP_COUNT 0x91BE
#define GL_MAX_COMPUTE_WORK_GROUP_SIZE 0x91BF
#define GL_COMPUTE_LOCAL_WORK_SIZE 0x8267
#define GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER 0x90EC
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER 0x90ED
#define GL_DISPATCH_INDIRECT_BUFFER 0x90EE
#define GL_DISPATCH_INDIRECT_BUFFER_BINDING 0x90EF
#define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH 0x8243
#define GL_DEBUG_CALLBACK_FUNCTION 0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM 0x8245
#define GL_DEBUG_SOURCE_API 0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM 0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY 0x8249
#define GL_DEBUG_SOURCE_APPLICATION 0x824A
#define GL_DEBUG_SOURCE_OTHER 0x824B
#define GL_DEBUG_TYPE_ERROR 0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR 0x824E
#define GL_DEBUG_TYPE_PORTABILITY 0x824F
#define GL_DEBUG_TYPE_PERFORMANCE 0x8250
#define GL_DEBUG_TYPE_OTHER 0x8251
#define GL_MAX_DEBUG_MESSAGE_LENGTH 0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES 0x9144
#define GL_DEBUG_LOGGED_MESSAGES 0x9145
#define GL_DEBUG_SEVERITY_HIGH 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM 0x9147
#define GL_DEBUG_SEVERITY_LOW 0x9148
#define GL_DEBUG_TYPE_MARKER 0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP 0x8269
#define GL_DEBUG_TYPE_POP_GROUP 0x826A
#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B
#define GL_MAX_DEBUG_GROUP_STACK_DEPTH 0x826C
#define GL_DEBUG_GROUP_STACK_DEPTH 0x826D
#define GL_BUFFER 0x82E0
#define GL_SHADER 0x82E1
#define GL_PROGRAM 0x82E2
#define GL_QUERY 0x82E3
#define GL_PROGRAM_PIPELINE 0x82E4
#define GL_SAMPLER 0x82E6
#define GL_MAX_LABEL_LENGTH 0x82E8
#define GL_DEBUG_OUTPUT 0x92E0
#define GL_CONTEXT_FLAG_DEBUG_BIT 0x00000002
#define GL_MAX_UNIFORM_LOCATIONS 0x826E
#define GL_FRAMEBUFFER_DEFAULT_WIDTH 0x9310
#define GL_FRAMEBUFFER_DEFAULT_HEIGHT 0x9311
#define GL_FRAMEBUFFER_DEFAULT_LAYERS 0x9312
#define GL_FRAMEBUFFER_DEFAULT_SAMPLES 0x9313
#define GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS 0x9314
#define GL_MAX_FRAMEBUFFER_WIDTH 0x9315
#define GL_MAX_FRAMEBUFFER_HEIGHT 0x9316
#define GL_MAX_FRAMEBUFFER_LAYERS 0x9317
#define GL_MAX_FRAMEBUFFER_SAMPLES 0x9318
#define GL_INTERNALFORMAT_SUPPORTED 0x826F
#define GL_INTERNALFORMAT_PREFERRED 0x8270
#define GL_INTERNALFORMAT_RED_SIZE 0x8271
#define GL_INTERNALFORMAT_GREEN_SIZE 0x8272
#define GL_INTERNALFORMAT_BLUE_SIZE 0x8273
#define GL_INTERNALFORMAT_ALPHA_SIZE 0x8274
#define GL_INTERNALFORMAT_DEPTH_SIZE 0x8275
#define GL_INTERNALFORMAT_STENCIL_SIZE 0x8276
#define GL_INTERNALFORMAT_SHARED_SIZE 0x8277
#define GL_INTERNALFORMAT_RED_TYPE 0x8278
#define GL_INTERNALFORMAT_GREEN_TYPE 0x8279
#define GL_INTERNALFORMAT_BLUE_TYPE 0x827A
#define GL_INTERNALFORMAT_ALPHA_TYPE 0x827B
#define GL_INTERNALFORMAT_DEPTH_TYPE 0x827C
#define GL_INTERNALFORMAT_STENCIL_TYPE 0x827D
#define GL_MAX_WIDTH 0x827E
#define GL_MAX_HEIGHT 0x827F
#define GL_MAX_DEPTH 0x8280
#define GL_MAX_LAYERS 0x8281
#define GL_MAX_COMBINED_DIMENSIONS 0x8282
#define GL_COLOR_COMPONENTS 0x8283
#define GL_DEPTH_COMPONENTS 0x8284
#define GL_STENCIL_COMPONENTS 0x8285
#define GL_COLOR_RENDERABLE 0x8286
#define GL_DEPTH_RENDERABLE 0x8287
#define GL_STENCIL_RENDERABLE 0x8288
#define GL_FRAMEBUFFER_RENDERABLE 0x8289
#define GL_FRAMEBUFFER_RENDERABLE_LAYERED 0x828A
#define GL_FRAMEBUFFER_BLEND 0x828B
#define GL_READ_PIXELS 0x828C
#define GL_READ_PIXELS_FORMAT 0x828D
#define GL_READ_PIXELS_TYPE 0x828E
#define GL_TEXTURE_IMAGE_FORMAT 0x828F
#define GL_TEXTURE_IMAGE_TYPE 0x8290
#define GL_GET_TEXTURE_IMAGE_FORMAT 0x8291
#define GL_GET_TEXTURE_IMAGE_TYPE 0x8292
#define GL_MIPMAP 0x8293
#define GL_MANUAL_GENERATE_MIPMAP 0x8294
#define GL_AUTO_GENERATE_MIPMAP 0x8295
#define GL_COLOR_ENCODING 0x8296
#define GL_SRGB_READ 0x8297
#define GL_SRGB_WRITE 0x8298
#define GL_FILTER 0x829A
#define GL_VERTEX_TEXTURE 0x829B
#define GL_TESS_CONTROL_TEXTURE 0x829C
#define GL_TESS_EVALUATION_TEXTURE 0x829D
#define GL_GEOMETRY_TEXTURE 0x829E
#define GL_FRAGMENT_TEXTURE 0x829F
#define GL_COMPUTE_TEXTURE 0x82A0
#define GL_TEXTURE_SHADOW 0x82A1
#define GL_TEXTURE_GATHER 0x82A2
#define GL_TEXTURE_GATHER_SHADOW 0x82A3
#define GL_SHADER_IMAGE_LOAD 0x82A4
#define GL_SHADER_IMAGE_STORE 0x82A5
#define GL_SHADER_IMAGE_ATOMIC 0x82A6
#define GL_IMAGE_TEXEL_SIZE 0x82A7
#define GL_IMAGE_COMPATIBILITY_CLASS 0x82A8
#define GL_IMAGE_PIXEL_FORMAT 0x82A9
#define GL_IMAGE_PIXEL_TYPE 0x82AA
#define GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST 0x82AC
#define GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST 0x82AD
#define GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE 0x82AE
#define GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE 0x82AF
#define GL_TEXTURE_COMPRESSED_BLOCK_WIDTH 0x82B1
#define GL_TEXTURE_COMPRESSED_BLOCK_HEIGHT 0x82B2
#define GL_TEXTURE_COMPRESSED_BLOCK_SIZE 0x82B3
#define GL_CLEAR_BUFFER 0x82B4
#define GL_TEXTURE_VIEW 0x82B5
#define GL_VIEW_COMPATIBILITY_CLASS 0x82B6
#define GL_FULL_SUPPORT 0x82B7
#define GL_CAVEAT_SUPPORT 0x82B8
#define GL_IMAGE_CLASS_4_X_32 0x82B9
#define GL_IMAGE_CLASS_2_X_32 0x82BA
#define GL_IMAGE_CLASS_1_X_32 0x82BB
#define GL_IMAGE_CLASS_4_X_16 0x82BC
#define GL_IMAGE_CLASS_2_X_16 0x82BD
#define GL_IMAGE_CLASS_1_X_16 0x82BE
#define GL_IMAGE_CLASS_4_X_8 0x82BF
#define GL_IMAGE_CLASS_2_X_8 0x82C0
#define GL_IMAGE_CLASS_1_X_8 0x82C1
#define GL_IMAGE_CLASS_11_11_10 0x82C2
#define GL_IMAGE_CLASS_10_10_10_2 0x82C3
#define GL_VIEW_CLASS_128_BITS 0x82C4
#define GL_VIEW_CLASS_96_BITS 0x82C5
#define GL_VIEW_CLASS_64_BITS 0x82C6
#define GL_VIEW_CLASS_48_BITS 0x82C7
#define GL_VIEW_CLASS_32_BITS 0x82C8
#define GL_VIEW_CLASS_24_BITS 0x82C9
#define GL_VIEW_CLASS_16_BITS 0x82CA
#define GL_VIEW_CLASS_8_BITS 0x82CB
#define GL_VIEW_CLASS_S3TC_DXT1_RGB 0x82CC
#define GL_VIEW_CLASS_S3TC_DXT1_RGBA 0x82CD
#define GL_VIEW_CLASS_S3TC_DXT3_RGBA 0x82CE
#define GL_VIEW_CLASS_S3TC_DXT5_RGBA 0x82CF
#define GL_VIEW_CLASS_RGTC1_RED 0x82D0
#define GL_VIEW_CLASS_RGTC2_RG 0x82D1
#define GL_VIEW_CLASS_BPTC_UNORM 0x82D2
#define GL_VIEW_CLASS_BPTC_FLOAT 0x82D3
#define GL_UNIFORM 0x92E1
#define GL_UNIFORM_BLOCK 0x92E2
#define GL_PROGRAM_INPUT 0x92E3
#define GL_PROGRAM_OUTPUT 0x92E4
#define GL_BUFFER_VARIABLE 0x92E5
#define GL_SHADER_STORAGE_BLOCK 0x92E6
#define GL_VERTEX_SUBROUTINE 0x92E8
#define GL_TESS_CONTROL_SUBROUTINE 0x92E9
#define GL_TESS_EVALUATION_SUBROUTINE 0x92EA
#define GL_GEOMETRY_SUBROUTINE 0x92EB
#define GL_FRAGMENT_SUBROUTINE 0x92EC
#define GL_COMPUTE_SUBROUTINE 0x92ED
#define GL_VERTEX_SUBROUTINE_UNIFORM 0x92EE
#define GL_TESS_CONTROL_SUBROUTINE_UNIFORM 0x92EF
#define GL_TESS_EVALUATION_SUBROUTINE_UNIFORM 0x92F0
#define GL_GEOMETRY_SUBROUTINE_UNIFORM 0x92F1
#define GL_FRAGMENT_SUBROUTINE_UNIFORM 0x92F2
#define GL_COMPUTE_SUBROUTINE_UNIFORM 0x92F3
#define GL_TRANSFORM_FEEDBACK_VARYING 0x92F4
#define GL_ACTIVE_RESOURCES 0x92F5
#define GL_MAX_NAME_LENGTH 0x92F6
#define GL_MAX_NUM_ACTIVE_VARIABLES 0x92F7
#define GL_MAX_NUM_COMPATIBLE_SUBROUTINES 0x92F8
#define GL_NAME_LENGTH 0x92F9
#define GL_TYPE 0x92FA
#define GL_ARRAY_SIZE 0x92FB
#define GL_OFFSET 0x92FC
#define GL_BLOCK_INDEX 0x92FD
#define GL_ARRAY_STRIDE 0x92FE
#define GL_MATRIX_STRIDE 0x92FF
#define GL_IS_ROW_MAJOR 0x9300
#define GL_ATOMIC_COUNTER_BUFFER_INDEX 0x9301
#define GL_BUFFER_BINDING 0x9302
#define GL_BUFFER_DATA_SIZE 0x9303
#define GL_NUM_ACTIVE_VARIABLES 0x9304
#define GL_ACTIVE_VARIABLES 0x9305
#define GL_REFERENCED_BY_VERTEX_SHADER 0x9306
#define GL_REFERENCED_BY_TESS_CONTROL_SHADER 0x9307
#define GL_REFERENCED_BY_TESS_EVALUATION_SHADER 0x9308
#define GL_REFERENCED_BY_GEOMETRY_SHADER 0x9309
#define GL_REFERENCED_BY_FRAGMENT_SHADER 0x930A
#define GL_REFERENCED_BY_COMPUTE_SHADER 0x930B
#define GL_TOP_LEVEL_ARRAY_SIZE 0x930C
#define GL_TOP_LEVEL_ARRAY_STRIDE 0x930D
#define GL_LOCATION 0x930E
#define GL_LOCATION_INDEX 0x930F
#define GL_IS_PER_PATCH 0x92E7
#define GL_SHADER_STORAGE_BUFFER 0x90D2
#define GL_SHADER_STORAGE_BUFFER_BINDING 0x90D3
#define GL_SHADER_STORAGE_BUFFER_START 0x90D4
#define GL_SHADER_STORAGE_BUFFER_SIZE 0x90D5
#define GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS 0x90D6
#define GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS 0x90D7
#define GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS 0x90D8
#define GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS 0x90D9
#define GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS 0x90DA
#define GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS 0x90DB
#define GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS 0x90DC
#define GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS 0x90DD
#define GL_MAX_SHADER_STORAGE_BLOCK_SIZE 0x90DE
#define GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT 0x90DF
#define GL_SHADER_STORAGE_BARRIER_BIT 0x00002000
#define GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES 0x8F39
#define GL_DEPTH_STENCIL_TEXTURE_MODE 0x90EA
#define GL_TEXTURE_BUFFER_OFFSET 0x919D
#define GL_TEXTURE_BUFFER_SIZE 0x919E
#define GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT 0x919F
#define GL_TEXTURE_VIEW_MIN_LEVEL 0x82DB
#define GL_TEXTURE_VIEW_NUM_LEVELS 0x82DC
#define GL_TEXTURE_VIEW_MIN_LAYER 0x82DD
#define GL_TEXTURE_VIEW_NUM_LAYERS 0x82DE
#define GL_TEXTURE_IMMUTABLE_LEVELS 0x82DF
#define GL_VERTEX_ATTRIB_BINDING 0x82D4
#define GL_VERTEX_ATTRIB_RELATIVE_OFFSET 0x82D5
#define GL_VERTEX_BINDING_DIVISOR 0x82D6
#define GL_VERTEX_BINDING_OFFSET 0x82D7
#define GL_VERTEX_BINDING_STRIDE 0x82D8
#define GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET 0x82D9
#define GL_MAX_VERTEX_ATTRIB_BINDINGS 0x82DA
#define GL_DISPLAY_LIST 0x82E7

typedef void (GLAPIENTRY * GLClearBufferData) (GLenum target, GLenum internalformat, GLenum format, GLenum type, const void *data);
typedef void (GLAPIENTRY * GLClearBufferSubData) (GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
typedef void (GLAPIENTRY * GLDispatchCompute) (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
typedef void (GLAPIENTRY * GLDispatchComputeIndirect) (GLintptr indirect);
typedef void (GLAPIENTRY * GLCopyImageSubData) (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
typedef void (GLAPIENTRY * GLFramebufferParameteri) (GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * GLGetFramebufferParameteriv) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLInvalidateTexSubImage) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);
typedef void (GLAPIENTRY * GLInvalidateTexImage) (GLuint texture, GLint level);
typedef void (GLAPIENTRY * GLInvalidateBufferSubData) (GLuint buffer, GLintptr offset, GLsizeiptr length);
typedef void (GLAPIENTRY * GLInvalidateBufferData) (GLuint buffer);
typedef void (GLAPIENTRY * GLInvalidateFramebuffer) (GLenum target, GLsizei numAttachments, const GLenum *attachments);
typedef void (GLAPIENTRY * GLInvalidateSubFramebuffer) (GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * GLMultiDrawArraysIndirect) (GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride);
typedef void (GLAPIENTRY * GLMultiDrawElementsIndirect) (GLenum mode, GLenum type, const void *indirect, GLsizei drawcount, GLsizei stride);
typedef void (GLAPIENTRY * GLGetProgramInterfaceiv) (GLuint program, GLenum programInterface, GLenum pname, GLint *params);
typedef GLuint (GLAPIENTRY * GLGetProgramResourceIndex) (GLuint program, GLenum programInterface, const GLchar *name);
typedef void (GLAPIENTRY * GLGetProgramResourceName) (GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);
typedef void (GLAPIENTRY * GLGetProgramResourceiv) (GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params);
typedef GLint (GLAPIENTRY * GLGetProgramResourceLocation) (GLuint program, GLenum programInterface, const GLchar *name);
typedef GLint (GLAPIENTRY * GLGetProgramResourceLocationIndex) (GLuint program, GLenum programInterface, const GLchar *name);
typedef void (GLAPIENTRY * GLShaderStorageBlockBinding) (GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);
typedef void (GLAPIENTRY * GLTexBufferRange) (GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAPIENTRY * GLTexStorage2DMultisample) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * GLTexStorage3DMultisample) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * GLTextureStorage2DMultisampleEXT) (GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * GLTextureStorage3DMultisampleEXT) (GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * GLTextureView) (GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
typedef void (GLAPIENTRY * GLBindVertexBuffer) (GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
typedef void (GLAPIENTRY * GLVertexAttribFormat) (GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
typedef void (GLAPIENTRY * GLVertexAttribIFormat) (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (GLAPIENTRY * GLVertexAttribLFormat) (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (GLAPIENTRY * GLVertexAttribBinding) (GLuint attribindex, GLuint bindingindex);
typedef void (GLAPIENTRY * GLVertexBindingDivisor) (GLuint bindingindex, GLuint divisor);
typedef void (GLAPIENTRY * GLDebugMessageControl) (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
typedef void (GLAPIENTRY * GLDebugMessageInsert) (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
typedef void (GLAPIENTRY * GLDebugMessageCallback) (GLDEBUGPROC callback, const void *userParam);
typedef GLuint (GLAPIENTRY * GLGetDebugMessageLog) (GLuint count, GLsizei bufsize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
typedef void (GLAPIENTRY * GLPushDebugGroup) (GLenum source, GLuint id, GLsizei length, const GLchar *message);
typedef void (GLAPIENTRY * GLPopDebugGroup) (void);
typedef void (GLAPIENTRY * GLObjectLabel) (GLenum identifier, GLuint name, GLsizei length, const GLchar *label);
typedef void (GLAPIENTRY * GLGetObjectLabel) (GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label);
typedef void (GLAPIENTRY * GLObjectPtrLabel) (const void *ptr, GLsizei length, const GLchar *label);
typedef void (GLAPIENTRY * GLGetObjectPtrLabel) (const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label);


// EXT_texture_filter_anisotropic
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF


// EXT_direct_state_access
#define GL_PROGRAM_MATRIX_EXT 0x8E2D
#define GL_TRANSPOSE_PROGRAM_MATRIX_EXT 0x8E2E
#define GL_PROGRAM_MATRIX_STACK_DEPTH_EXT 0x8E2F

typedef void (GLAPIENTRY * GLMatrixLoadfEXT) (GLenum mode, const GLfloat *m);
typedef void (GLAPIENTRY * GLMatrixLoaddEXT) (GLenum mode, const GLdouble *m);
typedef void (GLAPIENTRY * GLMatrixMultfEXT) (GLenum mode, const GLfloat *m);
typedef void (GLAPIENTRY * GLMatrixMultdEXT) (GLenum mode, const GLdouble *m);
typedef void (GLAPIENTRY * GLMatrixLoadIdentityEXT) (GLenum mode);
typedef void (GLAPIENTRY * GLMatrixRotatefEXT) (GLenum mode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * GLMatrixRotatedEXT) (GLenum mode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * GLMatrixScalefEXT) (GLenum mode, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * GLMatrixScaledEXT) (GLenum mode, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * GLMatrixTranslatefEXT) (GLenum mode, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * GLMatrixTranslatedEXT) (GLenum mode, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * GLMatrixFrustumEXT) (GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef void (GLAPIENTRY * GLMatrixOrthoEXT) (GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef void (GLAPIENTRY * GLMatrixPopEXT) (GLenum mode);
typedef void (GLAPIENTRY * GLMatrixPushEXT) (GLenum mode);
typedef void (GLAPIENTRY * GLClientAttribDefaultEXT) (GLbitfield mask);
typedef void (GLAPIENTRY * GLPushClientAttribDefaultEXT) (GLbitfield mask);
typedef void (GLAPIENTRY * GLTextureParameterfEXT) (GLuint texture, GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * GLTextureParameterfvEXT) (GLuint texture, GLenum target, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * GLTextureParameteriEXT) (GLuint texture, GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * GLTextureParameterivEXT) (GLuint texture, GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * GLTextureImage1DEXT) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLTextureImage2DEXT) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLTextureSubImage1DEXT) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLTextureSubImage2DEXT) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLCopyTextureImage1DEXT) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (GLAPIENTRY * GLCopyTextureImage2DEXT) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAPIENTRY * GLCopyTextureSubImage1DEXT) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * GLCopyTextureSubImage2DEXT) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * GLGetTextureImageEXT) (GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
typedef void (GLAPIENTRY * GLGetTextureParameterfvEXT) (GLuint texture, GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * GLGetTextureParameterivEXT) (GLuint texture, GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetTextureLevelParameterfvEXT) (GLuint texture, GLenum target, GLint level, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * GLGetTextureLevelParameterivEXT) (GLuint texture, GLenum target, GLint level, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLTextureImage3DEXT) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLTextureSubImage3DEXT) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLCopyTextureSubImage3DEXT) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * GLBindMultiTextureEXT) (GLenum texunit, GLenum target, GLuint texture);
typedef void (GLAPIENTRY * GLMultiTexCoordPointerEXT) (GLenum texunit, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (GLAPIENTRY * GLMultiTexEnvfEXT) (GLenum texunit, GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * GLMultiTexEnvfvEXT) (GLenum texunit, GLenum target, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * GLMultiTexEnviEXT) (GLenum texunit, GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * GLMultiTexEnvivEXT) (GLenum texunit, GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * GLMultiTexGendEXT) (GLenum texunit, GLenum coord, GLenum pname, GLdouble param);
typedef void (GLAPIENTRY * GLMultiTexGendvEXT) (GLenum texunit, GLenum coord, GLenum pname, const GLdouble *params);
typedef void (GLAPIENTRY * GLMultiTexGenfEXT) (GLenum texunit, GLenum coord, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * GLMultiTexGenfvEXT) (GLenum texunit, GLenum coord, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * GLMultiTexGeniEXT) (GLenum texunit, GLenum coord, GLenum pname, GLint param);
typedef void (GLAPIENTRY * GLMultiTexGenivEXT) (GLenum texunit, GLenum coord, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * GLGetMultiTexEnvfvEXT) (GLenum texunit, GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * GLGetMultiTexEnvivEXT) (GLenum texunit, GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetMultiTexGendvEXT) (GLenum texunit, GLenum coord, GLenum pname, GLdouble *params);
typedef void (GLAPIENTRY * GLGetMultiTexGenfvEXT) (GLenum texunit, GLenum coord, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * GLGetMultiTexGenivEXT) (GLenum texunit, GLenum coord, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLMultiTexParameteriEXT) (GLenum texunit, GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * GLMultiTexParameterivEXT) (GLenum texunit, GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * GLMultiTexParameterfEXT) (GLenum texunit, GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * GLMultiTexParameterfvEXT) (GLenum texunit, GLenum target, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * GLMultiTexImage1DEXT) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLMultiTexImage2DEXT) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLMultiTexSubImage1DEXT) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLMultiTexSubImage2DEXT) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLCopyMultiTexImage1DEXT) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (GLAPIENTRY * GLCopyMultiTexImage2DEXT) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAPIENTRY * GLCopyMultiTexSubImage1DEXT) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * GLCopyMultiTexSubImage2DEXT) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * GLGetMultiTexImageEXT) (GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
typedef void (GLAPIENTRY * GLGetMultiTexParameterfvEXT) (GLenum texunit, GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * GLGetMultiTexParameterivEXT) (GLenum texunit, GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetMultiTexLevelParameterfvEXT) (GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * GLGetMultiTexLevelParameterivEXT) (GLenum texunit, GLenum target, GLint level, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLMultiTexImage3DEXT) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLMultiTexSubImage3DEXT) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * GLCopyMultiTexSubImage3DEXT) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * GLEnableClientStateIndexedEXT) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * GLDisableClientStateIndexedEXT) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * GLGetFloatIndexedvEXT) (GLenum target, GLuint index, GLfloat *data);
typedef void (GLAPIENTRY * GLGetDoubleIndexedvEXT) (GLenum target, GLuint index, GLdouble *data);
typedef void (GLAPIENTRY * GLGetPointerIndexedvEXT) (GLenum target, GLuint index, GLvoid **data);
typedef void (GLAPIENTRY * GLEnableIndexedEXT) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * GLDisableIndexedEXT) (GLenum target, GLuint index);
typedef GLboolean (GLAPIENTRY * GLIsEnabledIndexedEXT) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * GLGetIntegerIndexedvEXT) (GLenum target, GLuint index, GLint *data);
typedef void (GLAPIENTRY * GLGetBooleanIndexedvEXT) (GLenum target, GLuint index, GLboolean *data);
typedef void (GLAPIENTRY * GLCompressedTextureImage3DEXT) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *bits);
typedef void (GLAPIENTRY * GLCompressedTextureImage2DEXT) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *bits);
typedef void (GLAPIENTRY * GLCompressedTextureImage1DEXT) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *bits);
typedef void (GLAPIENTRY * GLCompressedTextureSubImage3DEXT) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *bits);
typedef void (GLAPIENTRY * GLCompressedTextureSubImage2DEXT) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *bits);
typedef void (GLAPIENTRY * GLCompressedTextureSubImage1DEXT) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *bits);
typedef void (GLAPIENTRY * GLGetCompressedTextureImageEXT) (GLuint texture, GLenum target, GLint lod, GLvoid *img);
typedef void (GLAPIENTRY * GLCompressedMultiTexImage3DEXT) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *bits);
typedef void (GLAPIENTRY * GLCompressedMultiTexImage2DEXT) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *bits);
typedef void (GLAPIENTRY * GLCompressedMultiTexImage1DEXT) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *bits);
typedef void (GLAPIENTRY * GLCompressedMultiTexSubImage3DEXT) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *bits);
typedef void (GLAPIENTRY * GLCompressedMultiTexSubImage2DEXT) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *bits);
typedef void (GLAPIENTRY * GLCompressedMultiTexSubImage1DEXT) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *bits);
typedef void (GLAPIENTRY * GLGetCompressedMultiTexImageEXT) (GLenum texunit, GLenum target, GLint lod, GLvoid *img);
typedef void (GLAPIENTRY * GLMatrixLoadTransposefEXT) (GLenum mode, const GLfloat *m);
typedef void (GLAPIENTRY * GLMatrixLoadTransposedEXT) (GLenum mode, const GLdouble *m);
typedef void (GLAPIENTRY * GLMatrixMultTransposefEXT) (GLenum mode, const GLfloat *m);
typedef void (GLAPIENTRY * GLMatrixMultTransposedEXT) (GLenum mode, const GLdouble *m);
typedef void (GLAPIENTRY * GLNamedBufferDataEXT) (GLuint buffer, GLsizeiptr size, const GLvoid *data, GLenum usage);
typedef void (GLAPIENTRY * GLNamedBufferSubDataEXT) (GLuint buffer, GLintptr offset, GLsizeiptr size, const GLvoid *data);
typedef void * (GLAPIENTRY * GLMapNamedBufferEXT) (GLuint buffer, GLenum access);
typedef GLboolean (GLAPIENTRY * GLUnmapNamedBufferEXT) (GLuint buffer);
typedef void (GLAPIENTRY * GLGetNamedBufferParameterivEXT) (GLuint buffer, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetNamedBufferPointervEXT) (GLuint buffer, GLenum pname, GLvoid **params);
typedef void (GLAPIENTRY * GLGetNamedBufferSubDataEXT) (GLuint buffer, GLintptr offset, GLsizeiptr size, GLvoid *data);
typedef void (GLAPIENTRY * GLProgramUniform1fEXT) (GLuint program, GLint location, GLfloat v0);
typedef void (GLAPIENTRY * GLProgramUniform2fEXT) (GLuint program, GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAPIENTRY * GLProgramUniform3fEXT) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY * GLProgramUniform4fEXT) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAPIENTRY * GLProgramUniform1iEXT) (GLuint program, GLint location, GLint v0);
typedef void (GLAPIENTRY * GLProgramUniform2iEXT) (GLuint program, GLint location, GLint v0, GLint v1);
typedef void (GLAPIENTRY * GLProgramUniform3iEXT) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAPIENTRY * GLProgramUniform4iEXT) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAPIENTRY * GLProgramUniform1fvEXT) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniform2fvEXT) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniform3fvEXT) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniform4fvEXT) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniform1ivEXT) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (GLAPIENTRY * GLProgramUniform2ivEXT) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (GLAPIENTRY * GLProgramUniform3ivEXT) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (GLAPIENTRY * GLProgramUniform4ivEXT) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix2fvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix3fvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix4fvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix2x3fvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix3x2fvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix2x4fvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix4x2fvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix3x4fvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLProgramUniformMatrix4x3fvEXT) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * GLTextureBufferEXT) (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer);
typedef void (GLAPIENTRY * GLMultiTexBufferEXT) (GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer);
typedef void (GLAPIENTRY * GLTextureParameterIivEXT) (GLuint texture, GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * GLTextureParameterIuivEXT) (GLuint texture, GLenum target, GLenum pname, const GLuint *params);
typedef void (GLAPIENTRY * GLGetTextureParameterIivEXT) (GLuint texture, GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetTextureParameterIuivEXT) (GLuint texture, GLenum target, GLenum pname, GLuint *params);
typedef void (GLAPIENTRY * GLMultiTexParameterIivEXT) (GLenum texunit, GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * GLMultiTexParameterIuivEXT) (GLenum texunit, GLenum target, GLenum pname, const GLuint *params);
typedef void (GLAPIENTRY * GLGetMultiTexParameterIivEXT) (GLenum texunit, GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetMultiTexParameterIuivEXT) (GLenum texunit, GLenum target, GLenum pname, GLuint *params);
typedef void (GLAPIENTRY * GLProgramUniform1uiEXT) (GLuint program, GLint location, GLuint v0);
typedef void (GLAPIENTRY * GLProgramUniform2uiEXT) (GLuint program, GLint location, GLuint v0, GLuint v1);
typedef void (GLAPIENTRY * GLProgramUniform3uiEXT) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (GLAPIENTRY * GLProgramUniform4uiEXT) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GLAPIENTRY * GLProgramUniform1uivEXT) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * GLProgramUniform2uivEXT) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * GLProgramUniform3uivEXT) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * GLProgramUniform4uivEXT) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * GLNamedProgramLocalParameters4fvEXT) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat *params);
typedef void (GLAPIENTRY * GLNamedProgramLocalParameterI4iEXT) (GLuint program, GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * GLNamedProgramLocalParameterI4ivEXT) (GLuint program, GLenum target, GLuint index, const GLint *params);
typedef void (GLAPIENTRY * GLNamedProgramLocalParametersI4ivEXT) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLint *params);
typedef void (GLAPIENTRY * GLNamedProgramLocalParameterI4uiEXT) (GLuint program, GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * GLNamedProgramLocalParameterI4uivEXT) (GLuint program, GLenum target, GLuint index, const GLuint *params);
typedef void (GLAPIENTRY * GLNamedProgramLocalParametersI4uivEXT) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLuint *params);
typedef void (GLAPIENTRY * GLGetNamedProgramLocalParameterIivEXT) (GLuint program, GLenum target, GLuint index, GLint *params);
typedef void (GLAPIENTRY * GLGetNamedProgramLocalParameterIuivEXT) (GLuint program, GLenum target, GLuint index, GLuint *params);
typedef void (GLAPIENTRY * GLEnableClientStateiEXT) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * GLDisableClientStateiEXT) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * GLGetFloati_vEXT) (GLenum pname, GLuint index, GLfloat *params);
typedef void (GLAPIENTRY * GLGetDoublei_vEXT) (GLenum pname, GLuint index, GLdouble *params);
typedef void (GLAPIENTRY * GLGetPointeri_vEXT) (GLenum pname, GLuint index, GLvoid **params);
typedef void (GLAPIENTRY * GLNamedProgramStringEXT) (GLuint program, GLenum target, GLenum format, GLsizei len, const GLvoid *string);
typedef void (GLAPIENTRY * GLNamedProgramLocalParameter4dEXT) (GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * GLNamedProgramLocalParameter4dvEXT) (GLuint program, GLenum target, GLuint index, const GLdouble *params);
typedef void (GLAPIENTRY * GLNamedProgramLocalParameter4fEXT) (GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * GLNamedProgramLocalParameter4fvEXT) (GLuint program, GLenum target, GLuint index, const GLfloat *params);
typedef void (GLAPIENTRY * GLGetNamedProgramLocalParameterdvEXT) (GLuint program, GLenum target, GLuint index, GLdouble *params);
typedef void (GLAPIENTRY * GLGetNamedProgramLocalParameterfvEXT) (GLuint program, GLenum target, GLuint index, GLfloat *params);
typedef void (GLAPIENTRY * GLGetNamedProgramivEXT) (GLuint program, GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGetNamedProgramStringEXT) (GLuint program, GLenum target, GLenum pname, GLvoid *string);
typedef void (GLAPIENTRY * GLNamedRenderbufferStorageEXT) (GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * GLGetNamedRenderbufferParameterivEXT) (GLuint renderbuffer, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLNamedRenderbufferStorageMultisampleEXT) (GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * GLNamedRenderbufferStorageMultisampleCoverageEXT) (GLuint renderbuffer, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
typedef GLenum (GLAPIENTRY * GLCheckNamedFramebufferStatusEXT) (GLuint framebuffer, GLenum target);
typedef void (GLAPIENTRY * GLNamedFramebufferTexture1DEXT) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * GLNamedFramebufferTexture2DEXT) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * GLNamedFramebufferTexture3DEXT) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (GLAPIENTRY * GLNamedFramebufferRenderbufferEXT) (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAPIENTRY * GLGetNamedFramebufferAttachmentParameterivEXT) (GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLGenerateTextureMipmapEXT) (GLuint texture, GLenum target);
typedef void (GLAPIENTRY * GLGenerateMultiTexMipmapEXT) (GLenum texunit, GLenum target);
typedef void (GLAPIENTRY * GLFramebufferDrawBufferEXT) (GLuint framebuffer, GLenum mode);
typedef void (GLAPIENTRY * GLFramebufferDrawBuffersEXT) (GLuint framebuffer, GLsizei n, const GLenum *bufs);
typedef void (GLAPIENTRY * GLFramebufferReadBufferEXT) (GLuint framebuffer, GLenum mode);
typedef void (GLAPIENTRY * GLGetFramebufferParameterivEXT) (GLuint framebuffer, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * GLNamedCopyBufferSubDataEXT) (GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
typedef void (GLAPIENTRY * GLNamedFramebufferTextureEXT) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAPIENTRY * GLNamedFramebufferTextureLayerEXT) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void (GLAPIENTRY * GLNamedFramebufferTextureFaceEXT) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLenum face);
typedef void (GLAPIENTRY * GLTextureRenderbufferEXT) (GLuint texture, GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * GLMultiTexRenderbufferEXT) (GLenum texunit, GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * GLVertexArrayVertexOffsetEXT) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * GLVertexArrayColorOffsetEXT) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * GLVertexArrayEdgeFlagOffsetEXT) (GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * GLVertexArrayIndexOffsetEXT) (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * GLVertexArrayNormalOffsetEXT) (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * GLVertexArrayTexCoordOffsetEXT) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * GLVertexArrayMultiTexCoordOffsetEXT) (GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * GLVertexArrayFogCoordOffsetEXT) (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * GLVertexArraySecondaryColorOffsetEXT) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * GLVertexArrayVertexAttribOffsetEXT) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * GLVertexArrayVertexAttribIOffsetEXT) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * GLEnableVertexArrayEXT) (GLuint vaobj, GLenum array);
typedef void (GLAPIENTRY * GLDisableVertexArrayEXT) (GLuint vaobj, GLenum array);
typedef void (GLAPIENTRY * GLEnableVertexArrayAttribEXT) (GLuint vaobj, GLuint index);
typedef void (GLAPIENTRY * GLDisableVertexArrayAttribEXT) (GLuint vaobj, GLuint index);
typedef void (GLAPIENTRY * GLGetVertexArrayIntegervEXT) (GLuint vaobj, GLenum pname, GLint *param);
typedef void (GLAPIENTRY * GLGetVertexArrayPointervEXT) (GLuint vaobj, GLenum pname, GLvoid **param);
typedef void (GLAPIENTRY * GLGetVertexArrayIntegeri_vEXT) (GLuint vaobj, GLuint index, GLenum pname, GLint *param);
typedef void (GLAPIENTRY * GLGetVertexArrayPointeri_vEXT) (GLuint vaobj, GLuint index, GLenum pname, GLvoid **param);
typedef void * (GLAPIENTRY * GLMapNamedBufferRangeEXT) (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void (GLAPIENTRY * GLFlushMappedNamedBufferRangeEXT) (GLuint buffer, GLintptr offset, GLsizeiptr length);


// ARB_texture_storage
typedef void (GLAPIENTRY * GLTextureStorage1DEXT) (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void (GLAPIENTRY * GLTextureStorage2DEXT) (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * GLTextureStorage3DEXT) (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);


// NV_path_rendering
#define GL_PATH_FORMAT_SVG_NV 0x9070
#define GL_PATH_FORMAT_PS_NV 0x9071
#define GL_STANDARD_FONT_NAME_NV 0x9072
#define GL_SYSTEM_FONT_NAME_NV 0x9073
#define GL_FILE_NAME_NV 0x9074
#define GL_PATH_STROKE_WIDTH_NV 0x9075
#define GL_PATH_END_CAPS_NV 0x9076
#define GL_PATH_INITIAL_END_CAP_NV 0x9077
#define GL_PATH_TERMINAL_END_CAP_NV 0x9078
#define GL_PATH_JOIN_STYLE_NV 0x9079
#define GL_PATH_MITER_LIMIT_NV 0x907A
#define GL_PATH_DASH_CAPS_NV 0x907B
#define GL_PATH_INITIAL_DASH_CAP_NV 0x907C
#define GL_PATH_TERMINAL_DASH_CAP_NV 0x907D
#define GL_PATH_DASH_OFFSET_NV 0x907E
#define GL_PATH_CLIENT_LENGTH_NV 0x907F
#define GL_PATH_FILL_MODE_NV 0x9080
#define GL_PATH_FILL_MASK_NV 0x9081
#define GL_PATH_FILL_COVER_MODE_NV 0x9082
#define GL_PATH_STROKE_COVER_MODE_NV 0x9083
#define GL_PATH_STROKE_MASK_NV 0x9084
#define GL_COUNT_UP_NV 0x9088
#define GL_COUNT_DOWN_NV 0x9089
#define GL_PATH_OBJECT_BOUNDING_BOX_NV 0x908A
#define GL_CONVEX_HULL_NV 0x908B
#define GL_BOUNDING_BOX_NV 0x908D
#define GL_TRANSLATE_X_NV 0x908E
#define GL_TRANSLATE_Y_NV 0x908F
#define GL_TRANSLATE_2D_NV 0x9090
#define GL_TRANSLATE_3D_NV 0x9091
#define GL_AFFINE_2D_NV 0x9092
#define GL_AFFINE_3D_NV 0x9094
#define GL_TRANSPOSE_AFFINE_2D_NV 0x9096
#define GL_TRANSPOSE_AFFINE_3D_NV 0x9098
#define GL_UTF8_NV 0x909A
#define GL_UTF16_NV 0x909B
#define GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV 0x909C
#define GL_PATH_COMMAND_COUNT_NV 0x909D
#define GL_PATH_COORD_COUNT_NV 0x909E
#define GL_PATH_DASH_ARRAY_COUNT_NV 0x909F
#define GL_PATH_COMPUTED_LENGTH_NV 0x90A0
#define GL_PATH_FILL_BOUNDING_BOX_NV 0x90A1
#define GL_PATH_STROKE_BOUNDING_BOX_NV 0x90A2
#define GL_SQUARE_NV 0x90A3
#define GL_ROUND_NV 0x90A4
#define GL_TRIANGULAR_NV 0x90A5
#define GL_BEVEL_NV 0x90A6
#define GL_MITER_REVERT_NV 0x90A7
#define GL_MITER_TRUNCATE_NV 0x90A8
#define GL_SKIP_MISSING_GLYPH_NV 0x90A9
#define GL_USE_MISSING_GLYPH_NV 0x90AA
#define GL_PATH_ERROR_POSITION_NV 0x90AB
#define GL_PATH_FOG_GEN_MODE_NV 0x90AC
#define GL_ACCUM_ADJACENT_PAIRS_NV 0x90AD
#define GL_ADJACENT_PAIRS_NV 0x90AE
#define GL_FIRST_TO_REST_NV 0x90AF
#define GL_PATH_GEN_MODE_NV 0x90B0
#define GL_PATH_GEN_COEFF_NV 0x90B1
#define GL_PATH_GEN_COLOR_FORMAT_NV 0x90B2
#define GL_PATH_GEN_COMPONENTS_NV 0x90B3
#define GL_PATH_STENCIL_FUNC_NV 0x90B7
#define GL_PATH_STENCIL_REF_NV 0x90B8
#define GL_PATH_STENCIL_VALUE_MASK_NV 0x90B9
#define GL_PATH_STENCIL_DEPTH_OFFSET_FACTOR_NV 0x90BD
#define GL_PATH_STENCIL_DEPTH_OFFSET_UNITS_NV 0x90BE
#define GL_PATH_COVER_DEPTH_FUNC_NV 0x90BF
#define GL_PATH_DASH_OFFSET_RESET_NV 0x90B4
#define GL_MOVE_TO_RESETS_NV 0x90B5
#define GL_MOVE_TO_CONTINUES_NV 0x90B6
#define GL_CLOSE_PATH_NV 0x00
#define GL_MOVE_TO_NV 0x02
#define GL_RELATIVE_MOVE_TO_NV 0x03
#define GL_LINE_TO_NV 0x04
#define GL_RELATIVE_LINE_TO_NV 0x05
#define GL_HORIZONTAL_LINE_TO_NV 0x06
#define GL_RELATIVE_HORIZONTAL_LINE_TO_NV 0x07
#define GL_VERTICAL_LINE_TO_NV 0x08
#define GL_RELATIVE_VERTICAL_LINE_TO_NV 0x09
#define GL_QUADRATIC_CURVE_TO_NV 0x0A
#define GL_RELATIVE_QUADRATIC_CURVE_TO_NV 0x0B
#define GL_CUBIC_CURVE_TO_NV 0x0C
#define GL_RELATIVE_CUBIC_CURVE_TO_NV 0x0D
#define GL_SMOOTH_QUADRATIC_CURVE_TO_NV 0x0E
#define GL_RELATIVE_SMOOTH_QUADRATIC_CURVE_TO_NV 0x0F
#define GL_SMOOTH_CUBIC_CURVE_TO_NV 0x10
#define GL_RELATIVE_SMOOTH_CUBIC_CURVE_TO_NV 0x11
#define GL_SMALL_CCW_ARC_TO_NV 0x12
#define GL_RELATIVE_SMALL_CCW_ARC_TO_NV 0x13
#define GL_SMALL_CW_ARC_TO_NV 0x14
#define GL_RELATIVE_SMALL_CW_ARC_TO_NV 0x15
#define GL_LARGE_CCW_ARC_TO_NV 0x16
#define GL_RELATIVE_LARGE_CCW_ARC_TO_NV 0x17
#define GL_LARGE_CW_ARC_TO_NV 0x18
#define GL_RELATIVE_LARGE_CW_ARC_TO_NV 0x19
#define GL_RESTART_PATH_NV 0xF0
#define GL_DUP_FIRST_CUBIC_CURVE_TO_NV 0xF2
#define GL_DUP_LAST_CUBIC_CURVE_TO_NV 0xF4
#define GL_RECT_NV 0xF6
#define GL_CIRCULAR_CCW_ARC_TO_NV 0xF8
#define GL_CIRCULAR_CW_ARC_TO_NV 0xFA
#define GL_CIRCULAR_TANGENT_ARC_TO_NV 0xFC
#define GL_ARC_TO_NV 0xFE
#define GL_RELATIVE_ARC_TO_NV 0xFF
#define GL_BOLD_BIT_NV 0x01
#define GL_ITALIC_BIT_NV 0x02
#define GL_GLYPH_WIDTH_BIT_NV 0x01
#define GL_GLYPH_HEIGHT_BIT_NV 0x02
#define GL_GLYPH_HORIZONTAL_BEARING_X_BIT_NV 0x04
#define GL_GLYPH_HORIZONTAL_BEARING_Y_BIT_NV 0x08
#define GL_GLYPH_HORIZONTAL_BEARING_ADVANCE_BIT_NV 0x10
#define GL_GLYPH_VERTICAL_BEARING_X_BIT_NV 0x20
#define GL_GLYPH_VERTICAL_BEARING_Y_BIT_NV 0x40
#define GL_GLYPH_VERTICAL_BEARING_ADVANCE_BIT_NV 0x80
#define GL_GLYPH_HAS_KERNING_BIT_NV 0x100
#define GL_FONT_X_MIN_BOUNDS_BIT_NV 0x00010000
#define GL_FONT_Y_MIN_BOUNDS_BIT_NV 0x00020000
#define GL_FONT_X_MAX_BOUNDS_BIT_NV 0x00040000
#define GL_FONT_Y_MAX_BOUNDS_BIT_NV 0x00080000
#define GL_FONT_UNITS_PER_EM_BIT_NV 0x00100000
#define GL_FONT_ASCENDER_BIT_NV 0x00200000
#define GL_FONT_DESCENDER_BIT_NV 0x00400000
#define GL_FONT_HEIGHT_BIT_NV 0x00800000
#define GL_FONT_MAX_ADVANCE_WIDTH_BIT_NV 0x01000000
#define GL_FONT_MAX_ADVANCE_HEIGHT_BIT_NV 0x02000000
#define GL_FONT_UNDERLINE_POSITION_BIT_NV 0x04000000
#define GL_FONT_UNDERLINE_THICKNESS_BIT_NV 0x08000000
#define GL_FONT_HAS_KERNING_BIT_NV 0x10000000
#define GL_PRIMARY_COLOR_NV 0x852C
#define GL_SECONDARY_COLOR_NV 0x852D

typedef GLuint (GLAPIENTRY * GLGenPathsNV) (GLsizei range);
typedef void (GLAPIENTRY * GLDeletePathsNV) (GLuint path, GLsizei range);
typedef GLboolean (GLAPIENTRY * GLIsPathNV) (GLuint path);
typedef void (GLAPIENTRY * GLPathCommandsNV) (GLuint path, GLsizei numCommands, const GLubyte *commands, GLsizei numCoords, GLenum coordType, const GLvoid *coords);
typedef void (GLAPIENTRY * GLPathCoordsNV) (GLuint path, GLsizei numCoords, GLenum coordType, const GLvoid *coords);
typedef void (GLAPIENTRY * GLPathSubCommandsNV) (GLuint path, GLsizei commandStart, GLsizei commandsToDelete, GLsizei numCommands, const GLubyte *commands, GLsizei numCoords, GLenum coordType, const GLvoid *coords);
typedef void (GLAPIENTRY * GLPathSubCoordsNV) (GLuint path, GLsizei coordStart, GLsizei numCoords, GLenum coordType, const GLvoid *coords);
typedef void (GLAPIENTRY * GLPathStringNV) (GLuint path, GLenum format, GLsizei length, const GLvoid *pathString);
typedef void (GLAPIENTRY * GLPathGlyphsNV) (GLuint firstPathName, GLenum fontTarget, const GLvoid *fontName, GLbitfield fontStyle, GLsizei numGlyphs, GLenum type, const GLvoid *charcodes, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
typedef void (GLAPIENTRY * GLPathGlyphRangeNV) (GLuint firstPathName, GLenum fontTarget, const GLvoid *fontName, GLbitfield fontStyle, GLuint firstGlyph, GLsizei numGlyphs, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
typedef void (GLAPIENTRY * GLWeightPathsNV) (GLuint resultPath, GLsizei numPaths, const GLuint *paths, const GLfloat *weights);
typedef void (GLAPIENTRY * GLCopyPathNV) (GLuint resultPath, GLuint srcPath);
typedef void (GLAPIENTRY * GLInterpolatePathsNV) (GLuint resultPath, GLuint pathA, GLuint pathB, GLfloat weight);
typedef void (GLAPIENTRY * GLTransformPathNV) (GLuint resultPath, GLuint srcPath, GLenum transformType, const GLfloat *transformValues);
typedef void (GLAPIENTRY * GLPathParameterivNV) (GLuint path, GLenum pname, const GLint *value);
typedef void (GLAPIENTRY * GLPathParameteriNV) (GLuint path, GLenum pname, GLint value);
typedef void (GLAPIENTRY * GLPathParameterfvNV) (GLuint path, GLenum pname, const GLfloat *value);
typedef void (GLAPIENTRY * GLPathParameterfNV) (GLuint path, GLenum pname, GLfloat value);
typedef void (GLAPIENTRY * GLPathDashArrayNV) (GLuint path, GLsizei dashCount, const GLfloat *dashArray);
typedef void (GLAPIENTRY * GLPathStencilFuncNV) (GLenum func, GLint ref, GLuint mask);
typedef void (GLAPIENTRY * GLPathStencilDepthOffsetNV) (GLfloat factor, GLfloat units);
typedef void (GLAPIENTRY * GLStencilFillPathNV) (GLuint path, GLenum fillMode, GLuint mask);
typedef void (GLAPIENTRY * GLStencilStrokePathNV) (GLuint path, GLint reference, GLuint mask);
typedef void (GLAPIENTRY * GLStencilFillPathInstancedNV) (GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLenum fillMode, GLuint mask, GLenum transformType, const GLfloat *transformValues);
typedef void (GLAPIENTRY * GLStencilStrokePathInstancedNV) (GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLint reference, GLuint mask, GLenum transformType, const GLfloat *transformValues);
typedef void (GLAPIENTRY * GLPathCoverDepthFuncNV) (GLenum func);
typedef void (GLAPIENTRY * GLPathColorGenNV) (GLenum color, GLenum genMode, GLenum colorFormat, const GLfloat *coeffs);
typedef void (GLAPIENTRY * GLPathTexGenNV) (GLenum texCoordSet, GLenum genMode, GLint components, const GLfloat *coeffs);
typedef void (GLAPIENTRY * GLPathFogGenNV) (GLenum genMode);
typedef void (GLAPIENTRY * GLCoverFillPathNV) (GLuint path, GLenum coverMode);
typedef void (GLAPIENTRY * GLCoverStrokePathNV) (GLuint path, GLenum coverMode);
typedef void (GLAPIENTRY * GLCoverFillPathInstancedNV) (GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
typedef void (GLAPIENTRY * GLCoverStrokePathInstancedNV) (GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
typedef void (GLAPIENTRY * GLGetPathParameterivNV) (GLuint path, GLenum pname, GLint *value);
typedef void (GLAPIENTRY * GLGetPathParameterfvNV) (GLuint path, GLenum pname, GLfloat *value);
typedef void (GLAPIENTRY * GLGetPathCommandsNV) (GLuint path, GLubyte *commands);
typedef void (GLAPIENTRY * GLGetPathCoordsNV) (GLuint path, GLfloat *coords);
typedef void (GLAPIENTRY * GLGetPathDashArrayNV) (GLuint path, GLfloat *dashArray);
typedef void (GLAPIENTRY * GLGetPathMetricsNV) (GLbitfield metricQueryMask, GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLsizei stride, GLfloat *metrics);
typedef void (GLAPIENTRY * GLGetPathMetricRangeNV) (GLbitfield metricQueryMask, GLuint firstPathName, GLsizei numPaths, GLsizei stride, GLfloat *metrics);
typedef void (GLAPIENTRY * GLGetPathSpacingNV) (GLenum pathListMode, GLsizei numPaths, GLenum pathNameType, const GLvoid *paths, GLuint pathBase, GLfloat advanceScale, GLfloat kerningScale, GLenum transformType, GLfloat *returnedSpacing);
typedef void (GLAPIENTRY * GLGetPathColorGenivNV) (GLenum color, GLenum pname, GLint *value);
typedef void (GLAPIENTRY * GLGetPathColorGenfvNV) (GLenum color, GLenum pname, GLfloat *value);
typedef void (GLAPIENTRY * GLGetPathTexGenivNV) (GLenum texCoordSet, GLenum pname, GLint *value);
typedef void (GLAPIENTRY * GLGetPathTexGenfvNV) (GLenum texCoordSet, GLenum pname, GLfloat *value);
typedef GLboolean (GLAPIENTRY * GLIsPointInFillPathNV) (GLuint path, GLuint mask, GLfloat x, GLfloat y);
typedef GLboolean (GLAPIENTRY * GLIsPointInStrokePathNV) (GLuint path, GLfloat x, GLfloat y);
typedef GLfloat (GLAPIENTRY * GLGetPathLengthNV) (GLuint path, GLsizei startSegment, GLsizei numSegments);
typedef GLboolean (GLAPIENTRY * GLPointAlongPathNV) (GLuint path, GLsizei startSegment, GLsizei numSegments, GLfloat distance, GLfloat *x, GLfloat *y, GLfloat *tangentX, GLfloat *tangentY);


// NV_blend_equation_advanced
#define GL_BLEND_ADVANCED_COHERENT_NV 0x9285
#define GL_BLEND_PREMULTIPLIED_SRC_NV 0x9280
#define GL_BLEND_OVERLAP_NV 0x9281
#define GL_UNCORRELATED_NV 0x9282
#define GL_DISJOINT_NV 0x9283
#define GL_CONJOINT_NV 0x9284
#define GL_SRC_NV 0x9286
#define GL_DST_NV 0x9287
#define GL_SRC_OVER_NV 0x9288
#define GL_DST_OVER_NV 0x9289
#define GL_SRC_IN_NV 0x928A
#define GL_DST_IN_NV 0x928B
#define GL_SRC_OUT_NV 0x928C
#define GL_DST_OUT_NV 0x928D
#define GL_SRC_ATOP_NV 0x928E
#define GL_DST_ATOP_NV 0x928F
#define GL_MULTIPLY_NV 0x9294
#define GL_SCREEN_NV 0x9295
#define GL_OVERLAY_NV 0x9296
#define GL_DARKEN_NV 0x9297
#define GL_LIGHTEN_NV 0x9298
#define GL_COLORDODGE_NV 0x9299
#define GL_COLORBURN_NV 0x929A
#define GL_HARDLIGHT_NV 0x929B
#define GL_SOFTLIGHT_NV 0x929C
#define GL_DIFFERENCE_NV 0x929E
#define GL_EXCLUSION_NV 0x92A0
#define GL_INVERT_RGB_NV 0x92A3
#define GL_LINEARDODGE_NV 0x92A4
#define GL_LINEARBURN_NV 0x92A5
#define GL_VIVIDLIGHT_NV 0x92A6
#define GL_LINEARLIGHT_NV 0x92A7
#define GL_PINLIGHT_NV 0x92A8
#define GL_HARDMIX_NV 0x92A9
#define GL_HSL_HUE_NV 0x92AD
#define GL_HSL_SATURATION_NV 0x92AE
#define GL_HSL_COLOR_NV 0x92AF
#define GL_HSL_LUMINOSITY_NV 0x92B0
#define GL_PLUS_NV 0x9291
#define GL_PLUS_CLAMPED_NV 0x92B1
#define GL_PLUS_CLAMPED_ALPHA_NV 0x92B2
#define GL_PLUS_DARKER_NV 0x9292
#define GL_MINUS_NV 0x929F
#define GL_MINUS_CLAMPED_NV 0x92B3
#define GL_CONTRAST_NV 0x92A1
#define GL_INVERT_OVG_NV 0x92B4

typedef void (GLAPIENTRY * GLBlendParameteriNV) (GLenum pname, GLint value);
typedef void (GLAPIENTRY * GLBlendBarrierNV) (void);

#undef GLAPIENTRY

#endif /* MOZILLA_GFX_NVPR_GLDEFS_H_ */
