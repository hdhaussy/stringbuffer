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

#ifdef __cplusplus
	#define SB(str) { 0, sizeof(str)-1, (char*) (str) }
#else
	#define SB(str) ((stringbuffer_t) { 0, sizeof(str)-1, (char*) (str) })
#endif

/**
 * Allocate or Re-allocate the internal buffer of the stringbuffer
 * @param sb a pointer to the stringbuffer
 * @param desired the desired buffer size
 * The string is truncated to the desired size is less than the current length.
 */
void sb_alloc(stringbuffer_t* sb,size_t desired);

/**
 * Frees the memory allocated for the stringbuffer's internal buffer
 * @param sb a pointer to the stringbuffer
 */
void sb_release(stringbuffer_t* sb);

/**
 * Copies the content of a source stringbuffer to a destination stringbuffer
 * @param dest the destination stringbuffer
 * @param src the source stringbuffer
 */
void sb_copy(stringbuffer_t* dest,const stringbuffer_t* src);

/**
 * Returns a null-terminated C string representing the content of the stringbuffer
 * @param sb a pointer to the stringbuffer
 * @return the C string
 */
const char* sb_str(stringbuffer_t* sb);

/**
 * Appends the content of a source stringbuffer to the end of the destination stringbuffer
 * @param dest the destination stringbuffer
 * @param src the source stringbuffer
 */
void sb_append(stringbuffer_t* dest,const stringbuffer_t* src);

/**
 * Appends a C string to the end of the stringbuffer
 * @param dest the destination stringbuffer
 * @param src the C string to append
 */
void sb_append_str(stringbuffer_t* dest,const char* src);

/**
 * Appends a character to the end of the stringbuffer
 * @param dest the destination stringbuffer
 * @param c the character to append
 */
void sb_append_char(stringbuffer_t* dest,const char c);

/**
 * Inserts the content of a source stringbuffer at a given position in the destination stringbuffer
 * @param dest the destination stringbuffer
 * @param pos the insertion position
 * @param src the source stringbuffer
 */
void sb_insert(stringbuffer_t* dest,size_t pos,const stringbuffer_t* src);

/**
 * Inserts a character at a given position in the stringbuffer
 * @param dest the destination stringbuffer
 * @param pos the insertion position
 * @param c the character to insert
 */
void sb_insert_char(stringbuffer_t* dest,size_t pos,const char c);

/**
 * Replaces a portion of the stringbuffer with the content of another stringbuffer
 * @param dest the destination stringbuffer
 * @param pos the starting position
 * @param len the length to replace
 * @param src the source stringbuffer
 */
void sb_replace(stringbuffer_t* dest,size_t pos,size_t len,const stringbuffer_t* src);

/**
 * Appends a formatted string to the stringbuffer using printf-style formatting
 * @param sb the stringbuffer
 * @param fmt the format string
 * @param ... the variable arguments
 */
void sb_printf(stringbuffer_t* sb,const char* fmt,...);

/**
 * Compares two stringbuffers
 * @param sb1 the first stringbuffer
 * @param sb2 the second stringbuffer
 * @return a negative integer if sb1 < sb2, 0 if equal, positive if sb1 > sb2
 */
int sb_cmp(stringbuffer_t* sb1,stringbuffer_t* sb2);

/**
 * Converts all characters in the stringbuffer to uppercase
 * @param sb the stringbuffer
 */
void sb_upper(stringbuffer_t* sb);

/**
 * Converts all characters in the stringbuffer to lowercase
 * @param sb the stringbuffer
 */
void sb_lower(stringbuffer_t* sb);

/** Returns a pointer to the stringbuffer's internal buffer */
static inline char* sb_buffer(stringbuffer_t* sb) { return sb->buffer; }

/** Returns the current length of the stringbuffer's content */
static inline size_t sb_length(stringbuffer_t* sb) { return sb->length; }

/** Returns the current capacity of the stringbuffer's buffer */
static inline size_t sb_capacity(stringbuffer_t* sb) { return sb->capacity; }

/** Ensures the buffer capacity is at least the desired size, reallocating if necessary */
static inline char sb_ensure_capacity(stringbuffer_t* sb,size_t desired) {
	size_t alloc = sb->capacity;
	if(alloc < desired) {
		if(alloc==0) alloc = sizeof(int);
		while(alloc < desired) alloc *= 2;
		sb_alloc(sb,alloc);
	}
	return sb->capacity >= desired;
}

/** Returns the available space in the buffer (capacity - length) */
static inline size_t sb_available(stringbuffer_t* sb) { return sb->capacity>0?sb->capacity-sb->length:0; }

/**
 * Extract a substring from the stringbuffer
 * @param sb the source stringbuffer
 * @param start the starting index
 * @param len the length of the substring
 * @return a new stringbuffer containing the substring
 */
stringbuffer_t sb_substr(stringbuffer_t* sb, size_t start, size_t len);

/**
 * Find the first occurrence of a substring in the stringbuffer
 * @param sb the stringbuffer to search in
 * @param substr the stringbuffer containing the substring to find
 * @return the index of the first occurrence, or -1 if not found
 */
size_t sb_find(stringbuffer_t* sb, stringbuffer_t* substr);

#endif // #ifndef __include_stringbuffer_h
