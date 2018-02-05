#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <bcl.h>

void process(int num, char *ims, char *imd);

void
usage (char *s)
{
  fprintf(stderr,"Usage: %s %s", s, "<num> <ims> <imd>\n");
  exit(EXIT_FAILURE);
}

#define PARAM 3
int
main(int argc, char *argv[])
{
  if (argc != PARAM+1)
    usage(argv[0]);
  else
    process(atoi(argv[1]), argv[2], argv[3]);
  return EXIT_SUCCESS;
}

void process(int num, char *ims, char *imd){
  if(num < 0 || num > 2){
    fprintf(stderr, "Error : %d : unknown channel\n", num);
    return;
  }

  pnm img_src = pnm_load(ims);
  int w = pnm_get_width(img_src);
  int h = pnm_get_height(img_src);
  pnm img_dest = pnm_new(w, h, PnmRawPpm);

  unsigned short *buf = pnm_get_channel(img_src, NULL, num);
  pnm_set_channel(img_dest, buf, PnmRed);
  pnm_set_channel(img_dest, buf, PnmGreen);
  pnm_set_channel(img_dest, buf, PnmBlue);

  pnm_save(img_dest, PnmRawPpm, imd);

  free(buf);
  pnm_free(img_src);
  pnm_free(img_dest);
}
