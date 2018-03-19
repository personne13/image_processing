#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <bcl.h>

#define PARAM 3

void
usage (char *s){
  fprintf(stderr, "Usage: %s <factor> <ims> <imd> \n", s);
  exit(EXIT_FAILURE);
}

void process(int factor, char *ims, char *imd){
  if(factor < 1){
    fprintf(stderr, "Error : factor < 1\n");
    exit(EXIT_FAILURE);
  }

  pnm img_src = pnm_load(ims);

  int w_src = pnm_get_width(img_src);
  int h_src = pnm_get_height(img_src);

  int w_dst = factor * w_src;
  int h_dst = factor * h_src;

  unsigned short value;

  pnm img_dst = pnm_new(w_dst, h_dst, PnmRawPpm);

  for(int i = 0; i < w_dst; i++){
    for(int j = 0; j < h_dst; j++){
      for(int k = 0; k < 3; k++){
        value = pnm_get_component(img_src, i / factor, j / factor, k);
        pnm_set_component(img_dst, i, j, k, value);
      }
    }
  }

  pnm_save(img_dst, PnmRawPpm, imd);
  pnm_free(img_src);
  pnm_free(img_dst);
}


int
main(int argc, char *argv[]){
  if (argc != PARAM+1) usage(argv[0]);
  process(atoi(argv[1]), argv[2], argv[3]);
  return EXIT_SUCCESS;
}
