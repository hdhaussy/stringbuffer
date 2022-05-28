#include <string>
#include <iostream>

extern "C" {
  #include "stringbuffer.h"
}

using namespace std;

void stdstring_append_char()
{
  string str = "";
  int i;
  size_t len = 1000000;
  for(i = 0; i < len; i++)
  {
    str.append(1,'*');
  }
}

void sb_append_char()
{
  stringbuffer_t sb = SB_EMPTY;
  int i;
  size_t len = 1000000;
  for(i = 0; i < len; i++)
  {
    sb_append_char(&sb,'*');
  }
  sb_release(&sb);
}

void stdstring_insert_char()
{
  string str = "";
  int i;
  size_t len = 100000;
  for(i = 0; i < len; i++)
  {
    size_t pos = i / 2;
    str.insert(pos,1,'*');
  }
}

void sb_insert_char()
{
  stringbuffer_t sb = SB_EMPTY;
  int i;
  size_t len = 100000;
  for(i = 0; i < len; i++)
  {
    size_t pos = i / 2;
    sb_insert_char(&sb,pos,'*');
  }
  sb_release(&sb);
}

void stdstring_insert_str()
{
  string str = "";
  string str2("hello");
  int i;
  size_t len = 10000;
  for(i = 0; i < len; i++)
  {
    size_t pos = i / 2;
    str.insert(pos,str2);
  }
}

void sb_insert_str()
{
  stringbuffer_t sb = SB_EMPTY;
  stringbuffer_t sb2 = SB("hello");
  int i;
  size_t len = 10000;
  for(i = 0; i < len; i++)
  {
    size_t pos = i / 2;
    sb_insert(&sb,pos,&sb2);
  }
  sb_release(&sb);
}

struct testdef_struct {
  const char* name;
  void (*test)();
};
typedef struct testdef_struct testdef;

double execute_test(testdef* def)
{
  clock_t begin, end;
  double t;
  begin = clock();
  def->test();
  end = clock();
  t = (double)(end - begin) / CLOCKS_PER_SEC * 1000;
  cout<<"executed "<<def->name<<" in "<<t<<" ms"<<endl;
  return t;
}

void benchmark(testdef* def1,testdef* def2)
{
  double t1 = execute_test(def1);
  double t2 = execute_test(def2);
  cout<<def2->name<<"/"<<def1->name<<" = "<<t1/t2*100<<"%"<<endl<<endl;
}

int main(int argc, char** argv) {
  testdef tests[] = {
    { "stdstring_append_char",&stdstring_append_char },
    { "sb_append_char",&sb_append_char },
    { "stdstring_insert_char",&stdstring_insert_char },
    { "sb_insert_char",&sb_insert_char },
    { "stdstring_insert_str",&stdstring_insert_str },
    { "sb_insert_str",&sb_insert_str },
  };
  size_t count = sizeof(tests)/sizeof(tests[0]);
  int i;
  for(i=0; i<count; i+=2)
  {
    benchmark(&tests[i],&tests[i+1]);
  }
  return 0;
}
