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
	*sb = SB("");
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
	if(pos >= dest->length) sb_append(dest,src);
	if(sb_ensure_capacity(dest,dest->length + src->length)) {
		memmove(dest->buffer + pos + src->length, dest->buffer + pos, dest->length - pos);
		memcpy(dest->buffer + pos,src->buffer,src->length);
		dest->length += src->length;
	}
}

void sb_insert_char(stringbuffer_t* dest,size_t pos,const char c) {
	if(pos >= dest->length) sb_append_char(dest,c);
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

int sb_cmp(const stringbuffer_t* sb1,const stringbuffer_t* sb2) {
	size_t i = 0;
	size_t imax = sb1->length < sb2->length ? sb1->length : sb2->length;
	while(i < imax && sb1->buffer[i] == sb2->buffer[i]) i++;
	if(i == sb1->length && i == sb2->length)
		return 0;
	else if(i == sb1->length)
		return -1;
	else if(i == sb2->length)
		return 1;
	else
		return sb1->buffer[i] < sb2->buffer[i] ? -1 : 1;
}

void sb_upper(stringbuffer_t* sb) {
	if(sb->capacity == 0) sb_alloc(sb,sb->length);
	for(size_t i = 0; i < sb->length; i++) {
		if(sb->buffer[i] >= 'a' && sb->buffer[i] <= 'z') sb->buffer[i] += 'A' - 'a';
	}
}

void sb_lower(stringbuffer_t* sb) {
	if(sb->capacity == 0) sb_alloc(sb,sb->length);
	for(size_t i = 0; i < sb->length; i++) {
		if(sb->buffer[i] >= 'A' && sb->buffer[i] <= 'Z') sb->buffer[i] += 'a' - 'A';
	}
}

stringbuffer_t sb_substr(const stringbuffer_t* sb, size_t start, size_t len) {
	stringbuffer_t result = SB("");
	if (start >= sb->length || len == 0) return result;
	if (start + len > sb->length) len = sb->length - start;
	sb_ensure_capacity(&result, len);
	result.length = len;
	memcpy(result.buffer, sb->buffer + start, len);
	return result;
}

stringbuffer_t sb_view(const stringbuffer_t* sb, size_t start, size_t len) {
	if (start >= sb->length || len == 0) return SB("");
	if (start + len > sb->length) len = sb->length - start;
	return (stringbuffer_t) {0, len, sb->buffer + start};
}

size_t sb_find(const stringbuffer_t* sb, const stringbuffer_t* substr) {
	size_t substr_len = substr->length;
	if (substr_len == 0) return 0;
	if (substr_len > sb->length) return -1;
	for (size_t i = 0; i <= sb->length - substr_len; i++) {
		if (memcmp(sb->buffer + i, substr->buffer, substr_len) == 0) {
			return i;
		}
	}
	return -1;
}
