/*
* The MIT License (MIT)
*
* Copyright (c) 2020 Hubert d'Haussy
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef __include_stringbuffer_h
#define __include_stringbuffer_h

#include <string.h>

typedef struct stringbuffer stringbuffer_t;

struct stringbuffer {
	size_t capacity;
	size_t length;
	char* buffer;
};

#define SB(str) ((stringbuffer_t) { 0, sizeof(str)-1, (char*) (str) })
#define SB_EMPTY ((stringbuffer_t) { 0, 0, (char*) "" })

/**
 * Allocate or Re-allocate the internal buffer of the stringbuffer
 * @param sb a pointer to the stringbuffer
 * @param desired the desired buffer size
 * The string is truncated to the desired size is less than the current length.
 */
void sb_alloc(stringbuffer_t* sb,size_t desired);
void sb_release(stringbuffer_t* sb);
void sb_copy(stringbuffer_t* dest,const stringbuffer_t* src);
const char* sb_str(stringbuffer_t* sb);

void sb_append(stringbuffer_t* dest,const stringbuffer_t* src);
void sb_append_str(stringbuffer_t* dest,const char* src);
void sb_append_char(stringbuffer_t* dest,const char c);
void sb_insert(stringbuffer_t* dest,size_t pos,const stringbuffer_t* src);
void sb_insert_char(stringbuffer_t* dest,size_t pos,const char c);
void sb_replace(stringbuffer_t* dest,size_t pos,size_t len,const stringbuffer_t* src);
void sb_printf(stringbuffer_t* sb,const char* fmt,...);
int sb_cmp(stringbuffer_t* sb1,stringbuffer_t* sb2);

static inline char* sb_buffer(stringbuffer_t* sb) { return sb->buffer; }
static inline size_t sb_length(stringbuffer_t* sb) { return sb->length; }
static inline size_t sb_capacity(stringbuffer_t* sb) { return sb->capacity; }
static inline char sb_ensure_capacity(stringbuffer_t* sb,size_t desired) {
	size_t alloc = sb->capacity;
	if(alloc < desired) {
		if(alloc==0) alloc = sizeof(int);
		while(alloc < desired) alloc *= 2;
		sb_alloc(sb,alloc);
	}
	return sb->capacity >= desired;
}
static inline size_t sb_available(stringbuffer_t* sb) { return sb->capacity>0?sb->capacity-sb->length:0; }

#endif // #ifndef __include_stringbuffer_h
