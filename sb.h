#ifndef __include_sb_h
#define __include_sb_h

#include <string.h>

typedef struct sb {
	size_t allocated;
	size_t size;
	char* data;
} sb_t;

static inline char* sb_data(sb_t* sb);
static inline size_t sb_size(sb_t* sb);
static inline size_t sb_allocated(sb_t* sb);
static inline void sb_resize(sb_t* sb,size_t size);

void sb_init(sb_t* sb);
void sb_alloc(sb_t* sb,size_t size);
void sb_release(sb_t* sb);
void sb_copy(sb_t* dest,sb_t* src);
void sb_move(sb_t* dest,sb_t* src);
const char* sb_str(sb_t* sb);
void sb_append_char(sb_t* sb,char c);
void sb_append_str(sb_t* sb,const char* str);
void sb_insert_str(sb_t* sb,size_t pos,const char* str);
void sb_printf(sb_t* sb,const char* fmt,...);
#endif // #ifndef __include_sb_h
