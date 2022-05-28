#include "stringbuffer.h"
#include <stdio.h>

static int nbtests = 0;
static int nbfails = 0;

void run_test(const char* name,void (*test)()) {
	nbtests++;
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
#define ASSERT(expr) if(!(expr))  { assert_fail(__FILE__,__LINE__,#expr); return; }

void test_sb_str() {
	stringbuffer_t sb = SB("Hello");
	ASSERT(sb_capacity(&sb)==0);
	ASSERT(strcmp(sb_str(&sb),"Hello")==0);
	ASSERT(sb_capacity(&sb)==0);
	sb_release(&sb);
	sb = SB("");
	ASSERT(strcmp(sb_str(&sb),"")==0);
	sb_release(&sb);
	sb = SB_EMPTY;
	ASSERT(strcmp(sb_str(&sb),"")==0);
	sb_release(&sb);
}

void test_sb_append_str() {
	stringbuffer_t sb = SB("Hello");
	sb_append_str(&sb,", World");
	sb_append_str(&sb," !");
	ASSERT(strcmp(sb_str(&sb),"Hello, World !")==0);
	sb_release(&sb);
}

void test_sb_append_char() {
	stringbuffer_t sb = SB("Wo");
	sb_append_char(&sb,'r');
	sb_append_char(&sb,'l');
	sb_append_char(&sb,'d');
	ASSERT(strcmp(sb_str(&sb),"World")==0);
	sb_release(&sb);
}

void test_sb_printf() {
	stringbuffer_t sb = SB("The response");
	sb_printf(&sb," %s %d !","is",42);
	ASSERT(strcmp(sb_str(&sb),"The response is 42 !")==0);
	sb_release(&sb);
}

void test_sb_insert() {
	stringbuffer_t sb = SB("Roll");
	sb_insert(&sb,0,&SB("Rock"));
	sb_insert(&sb,4,&SB("'n'"));
	ASSERT(strcmp(sb_str(&sb),"Rock'n'Roll")==0);
	sb_release(&sb);
}

void test_sb_insert_char() {
	stringbuffer_t sb = SB("Hell here !");
	sb_insert_char(&sb,4,'o');
	sb_insert_char(&sb,6,'t');
	ASSERT(strcmp(sb_str(&sb),"Hello there !")==0);
	sb_release(&sb);
}

void test_sb_replace() {
	stringbuffer_t sb = SB("Hello, toto!");
	sb_replace(&sb,7,4,&SB("World "));
	ASSERT(strcmp(sb_str(&sb),"Hello, World !")==0);
	sb_release(&sb);
}

void test_sb_cmp() {
	ASSERT(sb_cmp(&SB_EMPTY,&SB(""))==0);
	ASSERT(sb_cmp(&SB_EMPTY,&SB("ABC"))==-1);
	ASSERT(sb_cmp(&SB("ABC"),&SB_EMPTY)==1);
	ASSERT(sb_cmp(&SB("ABC"),&SB("ABC"))==0);
	ASSERT(sb_cmp(&SB("ABC"),&SB("DEF"))==-1);
	ASSERT(sb_cmp(&SB("DEF"),&SB("ABC"))==1);
	ASSERT(sb_cmp(&SB("AB"),&SB("ABC"))==-1);
	ASSERT(sb_cmp(&SB("ABC"),&SB("AB"))==1);
}

void test_sb_upper() {
	stringbuffer_t sb = SB("speak loud !");
	sb_upper(&sb);
	ASSERT(strcmp(sb_str(&sb),"SPEAK LOUD !")==0);
	sb_release(&sb);
}

void test_sb_lower() {
	stringbuffer_t sb = SB("BE QUIET !");
	sb_lower(&sb);
	ASSERT(strcmp(sb_str(&sb),"be quiet !")==0);
	sb_release(&sb);
}

int main(int argc,char** argv) {
	RUN_TEST(test_sb_str);
	RUN_TEST(test_sb_append_str);
	RUN_TEST(test_sb_append_char);
	RUN_TEST(test_sb_printf);
	RUN_TEST(test_sb_insert);
	RUN_TEST(test_sb_insert_char);
	RUN_TEST(test_sb_replace);
	RUN_TEST(test_sb_cmp);
	RUN_TEST(test_sb_upper);
	RUN_TEST(test_sb_lower);
	printf("%d tests passed, %d tests failed.\n",nbtests,nbfails);
	return nbfails;
}
