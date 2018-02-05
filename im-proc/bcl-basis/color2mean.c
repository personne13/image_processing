#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <bcl.h>

void process(char *ims, char *imd);

void
usage (char *s)
{
  fprintf(stderr,"Usage: %s %s", s, "<ims> <imd>\n");
  exit(EXIT_FAILURE);
}

#define PARAM 2
int
main(int argc, char *argv[])
{
  if (argc != PARAM+1)
    usage(argv[0]);
  else
    process(argv[1], argv[2]);
  return EXIT_SUCCESS;
}

void process(char *ims, char *imd){
  pnm img_src = pnm_load(ims);

  int w = pnm_get_width(img_src);
  int h = pnm_get_height(img_src);

  pnm img_dest = pnm_new(w, h, PnmRawPpm);

  unsigned short *buf_red = pnm_get_channel(img_src, NULL, PnmRed);
  unsigned short *buf_green = pnm_get_channel(img_src, NULL, PnmGreen);
  unsigned short *buf_blue = pnm_get_channel(img_src, NULL, PnmBlue);
  unsigned short *buf_res = malloc(sizeof(unsigned short) * w * h);

  if(!buf_res){
    printf("Error allocating memory\n");
    return;
  }

  for(int i = 0; i < w; i++){
    for(int j = 0; j < h; j++){
      int index = i + j * w;
      buf_res[index] = (unsigned short)((int)buf_red[index] + (int)buf_green[index] + (int)buf_blue[index]) / 3;
    }
  }

  pnm_set_channel(img_dest, buf_res, PnmRed);
  pnm_set_channel(img_dest, buf_res, PnmGreen);
  pnm_set_channel(img_dest, buf_res, PnmBlue);

  pnm_save(img_dest, PnmRawPpm, imd);

  free(buf_red);
  free(buf_green);
  free(buf_blue);
  free(buf_res);

  pnm_free(img_src);
  pnm_free(img_dest);
}
