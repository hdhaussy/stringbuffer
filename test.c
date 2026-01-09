#include "stringbuffer.h"
#include <stdio.h>
#include <stdlib.h>

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
	sb = SB("");
	ASSERT(strcmp(sb_str(&sb),"")==0);
	sb = SB("");
	ASSERT(strcmp(sb_str(&sb),"")==0);
}

void test_sb_append_str() {
	stringbuffer_t sb = SB("Hello");
	sb_append_str(&sb,", World");
	sb_append_str(&sb," !");
	ASSERT(sb_cmp(&sb, &SB("Hello, World !"))==0);
	sb_release(&sb);
}

void test_sb_append_char() {
	stringbuffer_t sb = SB("Wo");
	sb_append_char(&sb,'r');
	sb_append_char(&sb,'l');
	sb_append_char(&sb,'d');
	ASSERT(sb_cmp(&sb, &SB("World"))==0);
	sb_release(&sb);
}

void test_sb_printf() {
	stringbuffer_t sb = SB("The response");
	sb_printf(&sb," %s %d !","is",42);
	ASSERT(sb_cmp(&sb, &SB("The response is 42 !"))==0);
	sb_release(&sb);
}

void test_sb_insert() {
	stringbuffer_t sb = SB("Roll");
	sb_insert(&sb,0,&SB("Rock"));
	sb_insert(&sb,4,&SB("'n'"));
	ASSERT(sb_cmp(&sb, &SB("Rock'n'Roll"))==0);
	sb_release(&sb);
}

void test_sb_insert_char() {
	stringbuffer_t sb = SB("Hell here !");
	sb_insert_char(&sb,4,'o');
	sb_insert_char(&sb,6,'t');
	ASSERT(sb_cmp(&sb, &SB("Hello there !"))==0);
	sb_release(&sb);
}

void test_sb_replace() {
	stringbuffer_t sb = SB("Hello, toto!");
	sb_replace(&sb,7,4,&SB("World "));
	ASSERT(sb_cmp(&sb, &SB("Hello, World !"))==0);
	sb_release(&sb);
}

void test_sb_cmp() {
	ASSERT(sb_cmp(&SB(""),&SB(""))==0);
	ASSERT(sb_cmp(&SB(""),&SB("ABC"))==-1);
	ASSERT(sb_cmp(&SB("ABC"),&SB(""))==1);
	ASSERT(sb_cmp(&SB("ABC"),&SB("ABC"))==0);
	ASSERT(sb_cmp(&SB("ABC"),&SB("DEF"))==-1);
	ASSERT(sb_cmp(&SB("DEF"),&SB("ABC"))==1);
	ASSERT(sb_cmp(&SB("AB"),&SB("ABC"))==-1);
	ASSERT(sb_cmp(&SB("ABC"),&SB("AB"))==1);
}

void test_sb_upper() {
	stringbuffer_t sb = SB("speak loud !");
	sb_upper(&sb);
	ASSERT(sb_cmp(&sb, &SB("SPEAK LOUD !"))==0);
	sb_release(&sb);
}

void test_sb_lower() {
	stringbuffer_t sb = SB("BE QUIET !");
	sb_lower(&sb);
	ASSERT(sb_cmp(&sb, &SB("be quiet !"))==0);
	sb_release(&sb);
}

void test_sb_alloc() {
	stringbuffer_t sb = SB("");
	sb_alloc(&sb, 10);
	ASSERT(sb_capacity(&sb) >= 10);
	sb_release(&sb);
}

void test_sb_release() {
	stringbuffer_t sb = SB("");
	sb_append_str(&sb, "test");
	sb_release(&sb);
	ASSERT(sb_capacity(&sb) == 0);
	ASSERT(sb_length(&sb) == 0);
}

void test_sb_copy() {
	stringbuffer_t src = SB("");
	sb_append_str(&src, "Hello World");
	stringbuffer_t dest = SB("");
	sb_copy(&dest, &src);
	ASSERT(sb_cmp(&dest, &SB("Hello World")) == 0);
	sb_release(&src);
	sb_release(&dest);
}

void test_sb_append() {
	stringbuffer_t sb1 = SB("");
	sb_append_str(&sb1, "Hello");
	stringbuffer_t sb2 = SB("");
	sb_append_str(&sb2, " World");
	sb_append(&sb1, &sb2);
	ASSERT(sb_cmp(&sb1, &SB("Hello World")) == 0);
	sb_release(&sb1);
	sb_release(&sb2);
}

void test_sb_buffer() {
	stringbuffer_t sb = SB("");
	sb_append_str(&sb, "test");
	ASSERT(sb_buffer(&sb) == sb.buffer);
	sb_release(&sb);
}

void test_sb_length() {
	stringbuffer_t sb = SB("hello");
	ASSERT(sb_length(&sb) == 5);
}

void test_sb_capacity() {
	stringbuffer_t sb = SB("");
	sb_alloc(&sb, 20);
	ASSERT(sb_capacity(&sb) >= 20);
	sb_release(&sb);
}

void test_sb_ensure_capacity() {
	stringbuffer_t sb = SB("");
	sb_ensure_capacity(&sb, 15);
	ASSERT(sb_capacity(&sb) >= 15);
	sb_release(&sb);
}

void test_sb_available() {
	stringbuffer_t sb = SB("");
	sb_append_str(&sb, "hi");
	size_t avail = sb_available(&sb);
	ASSERT(avail == 2);
	sb_release(&sb);
}

void test_sb_substr() {
	stringbuffer_t sb = SB("Hello World");
	stringbuffer_t substr = sb_substr(&sb, 6, 5);
	ASSERT(sb_cmp(&substr, &SB("World")) == 0);
	sb_release(&substr);
}

void test_sb_find() {
	stringbuffer_t sb = SB("Hello World");
	stringbuffer_t substr = SB("World");
	size_t pos = sb_find(&sb, &substr);
	ASSERT(pos == 6);
	substr = SB("NotFound");
	pos = sb_find(&sb, &substr);
	ASSERT(pos == -1);
	substr = SB("");
	pos = sb_find(&sb, &substr);
	ASSERT(pos == 0);
}

void test_sb_view() {
	stringbuffer_t sb = SB("Hello World");
	stringbuffer_t view = sb_view(&sb, 6, 5);
	ASSERT(sb_cmp(&view, &SB("World")) == 0);
	// View shares the buffer, no allocation
	ASSERT(sb_capacity(&view) == 0);
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
	RUN_TEST(test_sb_alloc);
	RUN_TEST(test_sb_release);
	RUN_TEST(test_sb_copy);
	RUN_TEST(test_sb_append);
	RUN_TEST(test_sb_buffer);
	RUN_TEST(test_sb_length);
	RUN_TEST(test_sb_capacity);
	RUN_TEST(test_sb_ensure_capacity);
	RUN_TEST(test_sb_available);
	RUN_TEST(test_sb_substr);
	RUN_TEST(test_sb_find);
	RUN_TEST(test_sb_view);
	printf("%d tests passed, %d tests failed.\n",nbtests,nbfails);
	return nbfails;
}
