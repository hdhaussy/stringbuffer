#ifndef __include_stringbuffer_h
#define __include_stringbuffer_h

#include <string.h>

typedef struct stringbuffer stringbuffer_t;

struct stringbuffer {
	size_t capacity;
	size_t length;
	char* buffer;
};

#define SB(str) ((stringbuffer_t) { .capacity=0, .length=sizeof(str)-1, .buffer=(char*) (str) })
#define SB_EMPTY ((stringbuffer_t) { 0 })

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
static inline size_t sb_available(stringbuffer_t* sb) { if(sb->capacity>0) return sb->capacity - sb->length; else return 0; }

#endif // #ifndef __include_stringbuffer_h
