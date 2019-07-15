#ifndef __include_sb_h
#define __include_sb_h

#include <string.h>

typedef struct {
	size_t capacity;
	size_t length;
	char* buffer;
} sb_t;

#define SB(str) (sb_t) { .capacity=0, .length=sizeof(str)-1, .buffer=str }

void sb_alloc(sb_t* sb,size_t desired);
void sb_release(sb_t* sb);
void sb_copy(sb_t* dest,const sb_t* src);
const char* sb_str(sb_t* sb);

void sb_append(sb_t* dest,const sb_t* src);
void sb_append_char(sb_t* dest,const char c);
void sb_insert(sb_t* dest,size_t pos,const sb_t* src);
void sb_printf(sb_t* sb,const char* fmt,...);

static inline sb_t sb_empty() { return (sb_t) { .capacity=0, .length=0, .buffer=0 }; }
static inline char* sb_buffer(sb_t* sb) { return sb->buffer; }
static inline size_t sb_length(sb_t* sb) { return sb->length; }
static inline size_t sb_capacity(sb_t* sb) { return sb->capacity; }
static inline char sb_ensure_capacity(sb_t* sb,size_t desired) {
	if(sb->capacity < desired) {
		size_t alloc = 8;
		while(alloc < desired) alloc <<= 1;
		sb_alloc(sb,alloc);
	}
	return sb->capacity >= desired;
}
static inline size_t sb_available(sb_t* sb) { if(sb->capacity>0) return sb->capacity - sb->length; else return 0; }

#endif // #ifndef __include_sb_h
