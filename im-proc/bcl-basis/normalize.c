#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <bcl.h>

void process(int min, int max, char *ims, char *imd);

void
usage (char *s)
{
  fprintf(stderr,"Usage: %s %s", s, "<min> <max> <ims> <imd>\n");
  exit(EXIT_FAILURE);
}

#define PARAM 4
int
main(int argc, char *argv[])
{
  if (argc != PARAM+1)
    usage(argv[0]);
  else
    process(atoi(argv[1]), atoi(argv[2]), argv[3], argv[4]);
  return EXIT_SUCCESS;
}

unsigned short get_max_value(unsigned short *buf, int l){
  unsigned short m = 0;
  for(int i = 0; i < l; i++){
    if(m < buf[i])
      m = buf[i];
  }

  return m;
}

unsigned short get_min_value(unsigned short *buf, int l){
  unsigned short m = 255;
  for(int i = 0; i < l; i++){
    if(m > buf[i])
      m = buf[i];
  }

  return m;
}

void normalize(int max, int min, unsigned short *buf, int l){
  unsigned short max_buf = get_max_value(buf, l);
  unsigned short min_buf = get_min_value(buf, l);

  if(max_buf == min_buf){
    printf("Error : from img : max = min\n");
    exit(EXIT_FAILURE);
  }

  for(int i = 0; i < l; i++){
    buf[i] = ((double)max - min) / (max_buf - min_buf) * buf[i] +
              ((double)min * max_buf - max * min_buf) / (max_buf - min_buf);
  }
}

void process(int min, int max, char *ims, char *imd){
  if(min > max){
    fprintf(stderr, "Error args\n");
  }

  pnm img_src = pnm_load(ims);

  int w = pnm_get_width(img_src);
  int h = pnm_get_height(img_src);

  pnm img_dest = pnm_new(w, h, PnmRawPpm);

  unsigned short *buf_red = pnm_get_channel(img_src, NULL, PnmRed);
  unsigned short *buf_green = pnm_get_channel(img_src, NULL, PnmGreen);
  unsigned short *buf_blue = pnm_get_channel(img_src, NULL, PnmBlue);

  normalize(max, min, buf_red, w * h);
  normalize(max, min, buf_green, w * h);
  normalize(max, min, buf_blue, w * h);

  pnm_set_channel(img_dest, buf_red, PnmRed);
  pnm_set_channel(img_dest, buf_green, PnmGreen);
  pnm_set_channel(img_dest, buf_blue, PnmBlue);

  pnm_save(img_dest, PnmRawPpm, imd);

  free(buf_red);
  free(buf_green);
  free(buf_blue);

  pnm_free(img_src);
  pnm_free(img_dest);
}
