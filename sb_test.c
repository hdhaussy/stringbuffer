#include "stringbuffer.h"
#include <stdio.h>
#include <assert.h>

static int nbfails = 0;

void run_test(const char* name,void (*test)()) {
	printf("Running %s...",name);
	int failbefore = nbfails;
	test();
	if(nbfails == failbefore) printf("PASSED\n");
}

void assert_fail(const char* file,int line,const char* test) {
	printf("Assertion %s failed in %s at line %d\n",test,file,line);
	nbfails++;
}

#define RUN_TEST(test) run_test(#test,test)
#define ASSERT(expr) if(!(expr)) assert_fail(__FILE__,__LINE__,#expr);

void test_sb_str() {
	stringbuffer_t sb = SB("abcdefgh");
	ASSERT(sb_capacity(&sb)==0);
	ASSERT(strcmp(sb_str(&sb),"abcdefgh")==0);
	ASSERT(sb_capacity(&sb)==0);
	sb_release(&sb);
	sb = SB("");
	ASSERT(strcmp(sb_str(&sb),"")==0);
	sb_release(&sb);
}

void test_sb_append_char() {
	stringbuffer_t sb = SB("ab");
	sb_append_char(&sb,'c');
	sb_append_char(&sb,'d');
	ASSERT(strcmp(sb_str(&sb),"abcd")==0);
	sb_release(&sb);
}

void test_sb_printf() {
	stringbuffer_t sb = SB("H");
	sb_printf(&sb,"ello, %s !","World");
	ASSERT(strcmp(sb_str(&sb),"Hello, World !")==0);
	sb_release(&sb);
}

void test_sb_insert() {
	stringbuffer_t sb = SB("Hello, !");
	sb_insert(&sb,6,&SB(" World"));
	ASSERT(strcmp(sb_str(&sb),"Hello, World !")==0);
	sb_release(&sb);
}

void test_sb_replace() {
	stringbuffer_t sb = SB("Hello, toto!");
	sb_replace(&sb,7,4,&SB("World "));
	ASSERT(strcmp(sb_str(&sb),"Hello, World !")==0);
	sb_release(&sb);
}


int main(int argc,char** argv) {
	RUN_TEST(test_sb_str);
	RUN_TEST(test_sb_append_char);
	RUN_TEST(test_sb_printf);
	RUN_TEST(test_sb_insert);
	RUN_TEST(test_sb_replace);
	return 0;
}
