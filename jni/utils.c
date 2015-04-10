/*
 * This file is part of the id3v2lib library
 *
 * Copyright (c) 2013, Lorenzo Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

unsigned int btoi(char* bytes, int size, int offset)
{
    unsigned int result = 0x00;
    int i = 0;
    for(i = 0; i < size; i++)
    {
        result = result << 8;
        result = result | (unsigned char) bytes[offset + i];
    }
    
    return result;
}

char* itob(int integer)
{
    int size = 4;
    char* result = (char*) malloc(sizeof(char) * size);
    
    // We need to reverse the bytes because Intel uses little endian.
    char* aux = (char*) &integer;
    int i = size - 1;
    for(; i >= 0; i--)
    {
        result[size - 1 - i] = aux[i];
    }
    
    return result;
}

int syncint_encode(int value)
{
    int out, mask = 0x7F;
    
    while (mask ^ 0x7FFFFFFF) {
        out = value & ~mask;
        out <<= 1;
        out |= value & mask;
        mask = ((mask + 1) << 8) - 1;
        value = out;
    }
    
    return out;
}

int syncint_decode(int value)
{
    unsigned int a, b, c, d, result = 0x0;
    a = value & 0xFF;
    b = (value >> 8) & 0xFF;
    c = (value >> 16) & 0xFF;
    d = (value >> 24) & 0xFF;
    
    result = result | a;
    result = result | (b << 7);
    result = result | (c << 14);
    result = result | (d << 21);
    
    return result;
}

void add_to_list(ID3v2_frame_list* main, ID3v2_frame* frame)
{
    // if empty list
    if(main->start == NULL)
    {
        main->start = main;
        main->last = main;
        main->frame = frame;
    }
    else
    {
        ID3v2_frame_list* current = new_frame_list();
        current->frame = frame;
        current->start = main->start;
        main->last->next = current;
        main->last = current;
    }
}

ID3v2_frame* get_from_list(ID3v2_frame_list* list, char* frame_id)
{
    while(list != NULL && list->frame != NULL)
    {
        if(strncmp(list->frame->frame_id, frame_id, 4) == 0) {
            return list->frame;
        }
        list = list->next;
    }
    return NULL;
}

char* get_mime_type_from_filename(const char* filename)
{
    if(strcmp(strrchr(filename, '.') + 1, "png") == 0)
    {
        return PNG_MIME_TYPE;
    }
    else
    {
        return JPG_MIME_TYPE;
    }
}

// String functions
int has_bom(uint16_t* string)
{
	if (memcmp("\xFF\xFE", string, 2) == 0) {
		return LITTLE_ENDIAN_BOM;
	}
	if (memcmp("\xFE\xFF", string, 2) == 0) {   
        return BIG_ENDIAN_BOM;
    }
    
    return NO_BOM;
}

char* parse_bom_utf16_to_utf8(uint16_t* raw, int length) {
	int bom = has_bom(raw);
	if (bom == NO_BOM) {
		// error, unicode always need a bom header to identify big endian or little endian
		return NULL;
	}

	raw++;
	length--;

	uint16_t* wstring = (uint16_t*)malloc(sizeof(uint16_t) * length);
	// 大端 要统一改为小端
	if (BIG_ENDIAN_BOM == bom) {
		utf16_big_endian_to_little_endian(raw, length, wstring);
	} else {
		memcpy(wstring, raw, sizeof(uint16_t) * length);
	}

	int utf8Len = 3 * length + 1; // 1个unicode有可能需要占用到3个字节，最后再加一位'\0'
	char* string = (char*)malloc(sizeof(char) * utf8Len);
	memset(string, 0, utf8Len);

	utf16_to_utf8(wstring, wstring + length, string, string + utf8Len);

	free(wstring);
	return string;
}

void println_utf16(uint16_t* string, int size)
{
    int i = 1; // Skip the BOM
    while(1)
    {
        if(size > 0 && i > size)
        {
            break;
        }
        
        if(string[i] == 0x0000)
        {
            break;
        }
        
        printf("%lc", string[i]);
        i++;
    }
    printf("\n");
}

char* get_path_to_file(const char* file)
{
    char* file_name = strrchr(file, '/');
    unsigned long size = strlen(file) - strlen(file_name) + 1; // 1 = trailing '/'
    
    char* file_path = (char*) malloc(size * sizeof(char));
    strncpy(file_path, file, size);
    
    return file_path;
}

/*
UCS-2编码    UTF-8 字节流(二进制)
0000 - 007F  0xxxxxxx
0080 - 07FF 110xxxxx 10xxxxxx
0800 - FFFF 1110xxxx 10xxxxxx 10xxxxxx
*/

#define UTF8_ONE_START      (0xOOO1)
#define UTF8_ONE_END        (0x007F)
#define UTF8_TWO_START      (0x0080)
#define UTF8_TWO_END        (0x07FF)
#define UTF8_THREE_START    (0x0800)
#define UTF8_THREE_END      (0xFFFF)

void utf16_to_utf8(uint16_t* utf16Start, uint16_t* utf16End, uint8_t* utf8Start, uint8_t* utf8End) {
	uint16_t* pTempUTF16 = utf16Start;
	uint8_t* pTempUTF8 = utf8Start;

	while (pTempUTF16 < utf16End)
	{
		if (*pTempUTF16 <= UTF8_ONE_END
			&& pTempUTF8 + 1 < utf8End)
		{
			//0000 - 007F  0xxxxxxx
			*pTempUTF8++ = (uint8_t)*pTempUTF16;
		}
		else if (*pTempUTF16 >= UTF8_TWO_START && *pTempUTF16 <= UTF8_TWO_END
			&& pTempUTF8 + 2 < utf8End)
		{
			//0080 - 07FF 110xxxxx 10xxxxxx
			*pTempUTF8++ = (*pTempUTF16 >> 6) | 0xC0;
			*pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
		}
		else if (*pTempUTF16 >= UTF8_THREE_START && *pTempUTF16 <= UTF8_THREE_END
			&& pTempUTF8 + 3 < utf8End)
		{
			//0800 - FFFF 1110xxxx 10xxxxxx 10xxxxxx
			*pTempUTF8++ = (*pTempUTF16 >> 12) | 0xE0;
			*pTempUTF8++ = ((*pTempUTF16 >> 6) & 0x3F) | 0x80;
			*pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
		}
		else
		{
			break;
		}
		pTempUTF16++;
	}
	*pTempUTF8 = 0;
}

void utf8_to_utf16(uint8_t* utf8Start, uint8_t* utf8End, uint16_t* utf16Start, uint16_t* utf16End) {
	uint16_t* pTempUTF16 = utf16Start;
	uint8_t* pTempUTF8 = utf8Start;

	while (pTempUTF8 < utf8End && pTempUTF16 + 1 < utf16End)
	{
		if (*pTempUTF8 >= 0xE0 && *pTempUTF8 <= 0xEF)//是3个字节的格式
		{
			//0800 - FFFF 1110xxxx 10xxxxxx 10xxxxxx
			*pTempUTF16 |= ((*pTempUTF8++ & 0xEF) << 12);
			*pTempUTF16 |= ((*pTempUTF8++ & 0x3F) << 6);
			*pTempUTF16 |= (*pTempUTF8++ & 0x3F);

		}
		else if (*pTempUTF8 >= 0xC0 && *pTempUTF8 <= 0xDF)//是2个字节的格式
		{
			//0080 - 07FF 110xxxxx 10xxxxxx
			*pTempUTF16 |= ((*pTempUTF8++ & 0x1F) << 6);
			*pTempUTF16 |= (*pTempUTF8++ & 0x3F);
		}
		else if (*pTempUTF8 >= 0 && *pTempUTF8 <= 0x7F)//是1个字节的格式
		{
			//0000 - 007F  0xxxxxxx
			*pTempUTF16 = *pTempUTF8++;
		}
		else
		{
			break;
		}
		pTempUTF16++;
	}
	*pTempUTF16 = 0;
}

uint16_t swap(uint16_t val) {
	uint8_t h = (val >> 8) & 0x00FF;
	uint8_t l = (val) & 0x00FF;

	return ((l << 8) & 0xFF00) | h;
}

void utf16_big_endian_to_little_endian(uint16_t* utf16In, int inLen, uint16_t* utf16Out) {
	int i = 0;
	for (; i < inLen; i++) {
		utf16Out[i] = swap(utf16In[i]);
	}
}
