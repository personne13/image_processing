#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bcl.h"

static void
pstr(char *s)
{
    printf("%s:%d\n", s, strlen(s));
}

int 
main(int argc, char *argv[])
{
    char *s1 = str_dup("abcdef|");
    char *s2 = str_catn(s1, " <<", argv[0], " >>", NULL);
    char *s3 = str_cat(s1, s2);

    char *prefix = str_prefix("prefix.suffix", '.');
    char *suffix = str_suffix("prefix.suffix", '.');

    char *m = str_formated_static_storage(Int, 345,
					  String, " is in ",
					  String, argv[0],
					  String, " for an example ",
					  String, "(and what about ",
					  Double, -12345.6789E-20,
					  String, " ?)", 
					  Eop);


    message_set_header(argv[0]); 

    pstr(s1);
    pstr(s2);
    pstr(s3);

    pstr(prefix);
    pstr(suffix);

    pstr(m);
    str_formated_static_storage(String, 
				"Gulp! (or Sob!, or Gasp! as you want)\n",
				String, 
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
 				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! "
				"too long! too long! too long! too long! ",
				Eop);
    pstr(m);

    return EXIT_SUCCESS;
}
