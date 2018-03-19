#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bcl.h>
#include "fft.h"

#define PARAM 4

#define WF_BOX 1
#define WF_TENT 2
#define WF_BELL 3
#define WF_MITCH 4

float h_box(float x){
  if(x >= -0.5 && x < 0.5)
    return 1;

  else
    return 0;
}

float h_tent(float x){
  if(x >= -1 && x <= 1)
    return 1 - fabs(x);

  else
    return 0;
}

float h_bell(float x){
  if(fabs(x) <= 0.5)
    return -x * x + 0.75;
  else if(fabs(x) <= 1.5)
    return 0.5 * pow((fabs(x) - 1.5), 2);

  else
    return 0;
}

float h_mitch(float x){
  float fx = fabs(x);
  if(fx <= 1)
    return 7.0 / 6.0 * pow(fx, 3) -2 * pow(x, 2) + 8.0 / 9.0;

  else if(fx <= 2)
    return -7.0 / 18.0 * pow(fabs(x), 3) + 2 * x * x - 10.0 / 3.0 * fx + 16.0 / 9.0;

  else
    return 0;
}

void process(int factor, char *filter_name, char *ims, char *imd){
  if(factor < 1){
    fprintf(stderr, "Error : factor < 1\n");
    exit(EXIT_FAILURE);
  }

  pnm img_src = pnm_load(ims);

  int w_src = pnm_get_width(img_src);
  int h_src = pnm_get_height(img_src);

  int w_dst = factor * w_src;
  int h_dst = h_src;

  pnm img_dst = pnm_new(w_dst, h_dst, PnmRawPpm);

  float (*h)(float);
  float WF;

  if(!strcmp(filter_name, "box")){
    h = h_box;
    WF = WF_BOX;
  }
  else if(!strcmp(filter_name, "tent")){
    h = h_tent;
    WF = WF_TENT;
  }
  else if(!strcmp(filter_name, "bell")){
    h = h_bell;
    WF = WF_BELL;
  }
  else if(!strcmp(filter_name, "mitch")){
    h = h_mitch;
    WF = WF_MITCH;
  }
  else{
    fprintf(stderr, "Error : unknown filter : %s\n", filter_name);
    exit(EXIT_FAILURE);
  }

  for(int i = 0; i < h_dst; i++){
    for(int j_p = 0; j_p < w_dst; j_p++){
      float j = ((float)j_p) / factor;
      int left = j - WF;
      int right = j + WF;

      for(int c = 0; c < 3; c++){
        float s = 0;
        for(int k = left; k <= right; k++){
          if(k >= 0 && k < w_src){
            unsigned short value = pnm_get_component(img_src, i, k, c);
            s += value * h(k - j);
          }
        }
        pnm_set_component(img_dst, i, j_p, c, s);
      }
    }
  }

  pnm_save(img_dst, PnmRawPpm, imd);
  pnm_free(img_src);
  pnm_free(img_dst);
}

void
usage (char *s){
  fprintf(stderr, "Usage: %s <factor> <filter-name> <ims> <imd> \n", s);
  exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[]){
  if (argc != PARAM+1) usage(argv[0]);
  process(atoi(argv[1]), argv[2], argv[3], argv[4]);
  return EXIT_SUCCESS;
}
