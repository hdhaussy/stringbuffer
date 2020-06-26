#include "stringbuffer.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

void sb_alloc(stringbuffer_t* sb,size_t size) {
	char* data = NULL;
	if(sb->capacity==0) {
		data = malloc(size);
		if(data != NULL) memcpy(data,sb->buffer,sb->length);
	}
	else {
		data = realloc(sb->buffer,size);
	}
	if(data != NULL) {
		sb->buffer = data;
		sb->capacity = size;
	}
}

void sb_release(stringbuffer_t* sb) {
	if(sb->capacity) free(sb->buffer);
	*sb = SB_EMPTY;
}

void sb_copy(stringbuffer_t* dest,const stringbuffer_t* src) {
	sb_release(dest);
	sb_alloc(dest,src->capacity);
	memcpy(dest->buffer,src->buffer,src->length);
	dest->length = src->length;
}

const char* sb_str(stringbuffer_t* sb) {
	if(sb_ensure_capacity(sb,sb->length+1))
		sb->buffer[sb->length] = 0;
	else
		sb->buffer[sb->length - 1] = 0;
	return sb->buffer;
}

void sb_append(stringbuffer_t* dest,const stringbuffer_t* src) {
	if(sb_ensure_capacity(dest,dest->length + src->length)) {
		memcpy(dest->buffer + dest->length,src->buffer,src->length);
		dest->length += src->length;
	}
}

void sb_append_char(stringbuffer_t* dest,const char c) {
	if(sb_ensure_capacity(dest,dest->length + 1)) {
		dest->buffer[dest->length++] = c;
	}
}

void sb_insert(stringbuffer_t* dest,size_t pos,const stringbuffer_t* src) {
	if(pos >= dest->length) return sb_append(dest,src);
	if(sb_ensure_capacity(dest,dest->length + src->length)) {
		if(pos>0) memmove(dest->buffer + pos + src->length, dest->buffer + pos, dest->length - pos);
		memcpy(dest->buffer + pos,src->buffer,src->length);
		dest->length += src->length;
	}
}

void sb_printf(stringbuffer_t* sb,const char* fmt,...) {
	va_list args;
	va_start(args,fmt);
	size_t maxlen = sb_available(sb);
	int size = vsnprintf(sb->buffer + sb->length,maxlen,fmt,args);
	va_end(args);
	if(size>maxlen && sb_ensure_capacity(sb,sb->length + size)) {
		va_start(args,fmt);
		maxlen = sb_available(sb);
		size = vsnprintf(sb->buffer + sb->length,maxlen,fmt,args);
		va_end(args);
	}
	if(size>0) sb->length += size;
}
