#ifndef __include_sb_h
#define __include_sb_h

#include <string.h>

typedef struct {
	size_t allocated;
	size_t size;
	char* data;
} sb_t;

#define SB(str) sb(str,sizeof(str)-1)

void sb_init(sb_t* sb);
void sb_alloc(sb_t* sb,size_t size);
void sb_release(sb_t* sb);
void sb_copy(sb_t* dest,sb_t* src);
void sb_move(sb_t* dest,sb_t* src);
const char* sb_str(sb_t* sb);

void sb_append(sb_t* dest,const sb_t src);
void sb_insert(sb_t* dest,size_t pos,const sb_t src);
void sb_printf(sb_t* sb,const char* fmt,...);

static inline sb_t sb(const char* data,size_t size) {
	sb_t sb;
	sb.allocated = 0;
	sb.size = size;
	sb.data = (char*) data;
	return sb;
}
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

#endif // #ifndef __include_sb_h
