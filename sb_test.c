#include "sb.h"
#include <stdio.h>

int main(int argc,char** argv) {
	sb_t sb1 = sb_empty(),sb2 = SB("SB");
	sb_append(&sb1,&SB("HWorld"));
	sb_insert(&sb1,1,&SB("ello, "));
	sb_append_char(&sb1,'!');
	//sb_copy(&sb2,&sb1);
	sb_printf(&sb2," is c%02dl ;-)",0);
	printf("%s\n",sb_str(&sb1));
	printf("%s\n",sb_str(&sb2));
	sb_release(&sb1);
	sb_release(&sb2);
	return 0;
}
