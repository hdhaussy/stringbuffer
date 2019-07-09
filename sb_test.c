#include "sb.h"
#include <stdio.h>

int main(int argc,char** argv) {
	sb_t sb1,sb2;
	sb_init(&sb1);
	sb_append_char(&sb1,'H');
	sb_append_str(&sb1,"World");
	sb_insert_str(&sb1,1,"ello, ");
	sb_append_char(&sb1,'!');
	sb_copy(&sb2,&sb1);
	sb_printf(&sb2," It's c%02dl ;-)",0);
	printf("%s\n",sb_str(&sb1));
	printf("%s\n",sb_str(&sb2));
	sb_release(&sb1);
	sb_release(&sb2);
	return 0;
}
