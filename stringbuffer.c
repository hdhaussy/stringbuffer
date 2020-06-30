#include "stringbuffer.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

void sb_alloc(stringbuffer_t* sb,size_t size) {
	char* buffer = NULL;
	if(sb->capacity==0) {
		buffer = (char*) malloc(size);
		if(buffer != NULL) memcpy(buffer,sb->buffer,sb->length);
	}
	else {
		buffer = (char*) realloc(sb->buffer,size);
	}
	if(buffer != NULL) {
		sb->buffer = buffer;
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
	if(sb->capacity==0) return sb->buffer;
	else if(sb_ensure_capacity(sb,sb->length+1))
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

void sb_append_str(stringbuffer_t* dest,const char* src) {
	size_t len = strlen(src);
	if(sb_ensure_capacity(dest,dest->length + len)) {
		memcpy(dest->buffer + dest->length,src,len);
		dest->length += len;
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
		memmove(dest->buffer + pos + src->length, dest->buffer + pos, dest->length - pos);
		memcpy(dest->buffer + pos,src->buffer,src->length);
		dest->length += src->length;
	}
}

void sb_insert_char(stringbuffer_t* dest,size_t pos,const char c) {
	if(pos >= dest->length) return sb_append_char(dest,c);
	if(sb_ensure_capacity(dest,dest->length + 1)) {
		memmove(dest->buffer + pos + 1, dest->buffer + pos, dest->length - pos);
		dest->buffer[pos] = c;
		dest->length++;
	}
}

void sb_replace(stringbuffer_t* dest,size_t pos,size_t len,const stringbuffer_t* src) {
	if(sb_ensure_capacity(dest,dest->length - len + src->length)) {
		memmove(dest->buffer + pos + src->length, dest->buffer + pos + len, dest->length - pos - len);
		memcpy(dest->buffer + pos,src->buffer,src->length);
		dest->length += src->length - len;
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
