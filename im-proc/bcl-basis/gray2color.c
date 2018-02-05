#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <bcl.h>

void process(char *ims0, char *ims1, char *ims2, char *imd);

void
usage (char *s)
{
  fprintf(stderr,"Usage: %s %s", s, "<ims0> <ims1> <ims2> <imd>\n");
  exit(EXIT_FAILURE);
}

#define PARAM 4
int
main(int argc, char *argv[])
{
  if (argc != PARAM+1)
    usage(argv[0]);
  else
    process(argv[1], argv[2], argv[3], argv[4]);
  return EXIT_SUCCESS;
}

void process(char *ims0, char *ims1, char *ims2, char *imd){
  pnm img_src_r = pnm_load(ims0);
  pnm img_src_g = pnm_load(ims1);
  pnm img_src_b = pnm_load(ims2);

  int w = pnm_get_width(img_src_r);
  int h = pnm_get_height(img_src_r);

  if(w != pnm_get_width(img_src_g) || h != pnm_get_height(img_src_g) ||
     w != pnm_get_width(img_src_b) || h != pnm_get_height(img_src_b)){
    fprintf(stderr, "Error : Input img don't have the same dimensions\n");    
    pnm_free(img_src_r);
    pnm_free(img_src_g);
    pnm_free(img_src_b);
    return;
  }

  pnm img_dest = pnm_new(w, h, PnmRawPpm);

  unsigned short *buf_red = pnm_get_channel(img_src_r, NULL, 0);
  unsigned short *buf_green = pnm_get_channel(img_src_g, NULL, 0);
  unsigned short *buf_blue = pnm_get_channel(img_src_b, NULL, 0);

  pnm_set_channel(img_dest, buf_red, PnmRed);
  pnm_set_channel(img_dest, buf_green, PnmGreen);
  pnm_set_channel(img_dest, buf_blue, PnmBlue);

  pnm_save(img_dest, PnmRawPpm, imd);

  free(buf_red);
  free(buf_green);
  free(buf_blue);

  pnm_free(img_src_r);
  pnm_free(img_src_g);
  pnm_free(img_src_b);
  pnm_free(img_dest);
}
