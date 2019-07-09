#include "sb.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

static inline char* sb_data(sb_t* sb) { return sb->data; }
static inline size_t sb_size(sb_t* sb) { return sb->size; }
static inline size_t sb_allocated(sb_t* sb) { return sb->allocated; }
static inline char sb_ensure_size(sb_t* sb,size_t size) {
	if(sb->allocated < size) {
		size_t alloc = 8;
		while(alloc < size) alloc <<= 1;
		sb_alloc(sb,alloc);
	}
	return sb->allocated >= size;
}

void sb_init(sb_t* sb) {
	sb->allocated = 0;
	sb->size = 0;
	sb->data = 0;
}

void sb_alloc(sb_t* sb,size_t size) {
	char* data = realloc(sb->data,size);
	if(data != NULL) {
		sb->data = data;
		sb->allocated = size;
	}
}

void sb_release(sb_t* sb) {
	free(sb->data);
	sb_init(sb);
}

void sb_copy(sb_t* dest,sb_t* src) {
	sb_init(dest);
	sb_alloc(dest,src->allocated);
	memcpy(dest->data,src->data,src->size);
	dest->size = src->size;
}

void sb_move(sb_t* dest,sb_t* src) {
	*dest = *src;
	sb_init(src);
}

const char* sb_str(sb_t* sb) {
	if(sb_ensure_size(sb,sb->size+1))
		sb->data[sb->size] = 0;
	else
		sb->data[sb->size - 1] = 0;
	return sb->data;
}

void sb_append_char(sb_t* sb,char c) {
	if(sb_ensure_size(sb,sb->size+1)) {
		sb->data[sb->size++] = c;
	}
}

void sb_append_str(sb_t* sb,const char* str) {
	size_t len = strlen(str);
	if(sb_ensure_size(sb,sb->allocated + len)) {
		memcpy(sb->data + sb->size,str,len);
		sb->size += len;
	}
}

//  ##########
//  ##***########

void sb_insert_str(sb_t* sb,size_t pos,const char* str) {
	if(pos >= sb->size) return sb_append_str(sb,str);
	size_t len = strlen(str);
	if(sb_ensure_size(sb,sb->allocated + len)) {
		if(pos>0) memmove(sb->data + pos + len, sb->data + pos, sb->size - pos);
		memcpy(sb->data + pos,str,len);
		sb->size += len;
	}
}

void sb_printf(sb_t* sb,const char* fmt,...) {
	va_list args;
	va_start(args,fmt);
	int size = vsnprintf(sb->data,0,fmt,args);
	va_end(args);
	if(sb_ensure_size(sb, sb->size + size + 1)) {
		va_start(args,fmt);
		size = vsnprintf(sb->data + sb->size,sb->allocated - sb->size,fmt,args);
		va_end(args);
		sb->size += size;
	}
}
