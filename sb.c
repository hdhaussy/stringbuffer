#include "sb.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

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

void sb_append(sb_t* dest,const sb_t src) {
	if(sb_ensure_size(dest,dest->size + src.size)) {
		memcpy(dest->data + dest->size,src.data,src.size);
		dest->size += src.size;
	}
}

void sb_insert(sb_t* dest,size_t pos,const sb_t src) {
	if(pos >= dest->size) return sb_append(dest,src);
	if(sb_ensure_size(dest,dest->size + src.size)) {
		if(pos>0) memmove(dest->data + pos + src.size, dest->data + pos, dest->size - pos);
		memcpy(dest->data + pos,src.data,src.size);
		dest->size += src.size;
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
