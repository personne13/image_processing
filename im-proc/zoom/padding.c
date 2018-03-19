#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <bcl.h>
#include "fft.h"

#define PARAM 3

int get_index_buffer(int i, int j, int cols){
  return i * cols + j;
}

fftw_complex * reshape_buffer(fftw_complex *buf_src, int old_rows, int old_cols, int new_rows, int new_cols){
  fftw_complex *res = malloc(new_rows * new_cols * sizeof(fftw_complex));

  if(!res){
    fprintf(stderr, "Error malloc (img %dx%d)\n", new_rows, new_cols);
    exit(EXIT_FAILURE);
  }

  int i_min = new_rows / 2 - old_rows / 2;
  int j_min = new_cols / 2 - old_cols / 2;

  int i_max = new_rows / 2 + old_rows / 2;
  int j_max = new_cols / 2 + old_cols / 2;

  for(int i = 0; i < new_rows; i++){
    for(int j = 0; j < new_cols; j++){
      int index = get_index_buffer(i, j, new_cols);
      if(i >= i_min && i < i_max &&
         j >= j_min && j < j_max){
        int index_src = get_index_buffer(i - i_min, j - j_min, old_cols);
        res[index] = buf_src[index_src];
      }
      else{
        res[index] = 0;
      }
    }
  }

  return res;
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

  pnm img_dst = pnm_new(w_dst, h_dst, PnmRawPpm);

  for(int i = 0; i < 3; i++){
    unsigned short *buf_img = pnm_get_channel(img_src, NULL, i);
    fftw_complex *img_forw = forward(h_src, w_src, buf_img);
    fftw_complex *img_forw_resized = reshape_buffer(img_forw, h_src, w_src, h_dst, w_dst);
    unsigned short *img_back = backward(h_dst, w_dst, img_forw_resized, w_src * h_src);
    pnm_set_channel(img_dst, img_back, i);
    free(buf_img);
    free(img_forw);
    free(img_forw_resized);
    free(img_back);
  }

  pnm_save(img_dst, PnmRawPpm, imd);
  pnm_free(img_src);
  pnm_free(img_dst);
}

void
usage (char *s){
  fprintf(stderr, "Usage: %s <factor> <ims> <imd> \n", s);
  exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[]){
  if (argc != PARAM+1) usage(argv[0]);
  process(atoi(argv[1]), argv[2], argv[3]);
  return EXIT_SUCCESS;
}
