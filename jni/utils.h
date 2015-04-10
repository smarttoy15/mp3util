/*
 * This file is part of the id3v2lib library
 *
 * Copyright (c) 2013, Lorenzo Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef id3v2lib_utils_h
#define id3v2lib_utils_h

#include <inttypes.h>

#include "types.h"

unsigned int btoi(char* bytes, int size, int offset);
char* itob(int integer);
int syncint_encode(int value);
int syncint_decode(int value);
void add_to_list(ID3v2_frame_list* list, ID3v2_frame* frame);
ID3v2_frame* get_from_list(ID3v2_frame_list* list, char* frame_id);
void free_tag(ID3v2_tag* tag);
char* get_mime_type_from_filename(const char* filename);

// String functions
int has_bom(uint16_t* string);
char* parse_bom_utf16_to_utf8(uint16_t* string, int length);	// 这里需要调用者对返回的utf8字符串进行内存管理
int getEncodeEndianTypeFromBom(uint16_t* bomString);
void println_utf16(uint16_t* string, int size);
char* get_path_to_file(const char* file);

// utf16 / utf8
void utf16_to_utf8(uint16_t* utf16Start, uint16_t* utf16End, uint8_t* utf8Start, uint8_t* utf8End);
void utf8_to_utf16(uint8_t* utf8Start, uint8_t* utf8End, uint16_t* utf16Start, uint16_t* utf16End);

// 注意utf16Out的大小应该与utf16In的一致或大于
void utf16_big_endian_to_little_endian(uint16_t* utf16In, int inLen, uint16_t* utf16Out);

#endif
