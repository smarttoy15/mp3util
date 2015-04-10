/*
 * This file is part of the id3v2lib library
 *
 * Copyright (c) 2013, Lorenzo Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef id3v2lib_header_h
#define id3v2lib_header_h

#include "types.h"
#include "constants.h"
#include "utils.h"

int has_id3v2tag(ID3v2_header* tag_header);
int _has_id3v2tag(char* raw_header);
// 以下函数获取得ID3v2_header由调用者来管理，如果被add入ID3v2_tag中时，则由ID3v2_tag来统一管理
ID3v2_header* get_tag_header(const char* file_name);
ID3v2_header* get_tag_header_with_buffer(char* buffer, int length);
int get_tag_version(ID3v2_header* tag_header);
void edit_tag_size(ID3v2_tag* tag);

#endif
