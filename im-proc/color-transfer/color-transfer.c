/**
 * @file color-transfert
 * @brief transfert color from source image to target image.
 *        Method from Reinhard et al. :
 *        Erik Reinhard, Michael Ashikhmin, Bruce Gooch and Peter Shirley,
 *        'Color Transfer between Images', IEEE CGA special issue on
 *        Applied Perception, Vol 21, No 5, pp 34-41, September - October 2001
 */

 //source from wget http://www.labri.fr/perso/ta/tmp-files/color-transfer.tar.gz

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <bcl.h>

#define D 3

void product_matrix_vector(float matrix[D][D], float vector[D], float res[D]);
void print_matrix(float matrix[D][D]);
void print_vector(float vector[D]);
void transform_buf(pnm p, float *buf, float matrix[D][D]);
int get_offset_buffer(int i, int j, int w);
float center_float_value(float min, float max, float value);
void fill_buffer_from_img(pnm p, float *buf);
void fill_img_from_buffer(pnm p, float *buf);
void apply_log_buffer(float *buf, int size);//size should be w * h
void apply_exp_buffer(float *buf, int size);//size should be w * h
void apply_add_value_buffer(float *buf, float value, int canal, int size);
void apply_mul_value_buffer(float *buf, float value, int canal, int size);
float get_mean_buffer(float *buf, int size, int canal);
float get_standard_deviation_buffer(float *buf, int size, int canal, float mean);
float get_max_value(float *buf, int l);
float get_min_value(float *buf, int l);
void normalize(int max, int min, float *buf, int l);

float RGB2LMS[D][D] = {
  {0.3811, 0.5783, 0.0402},
  {0.1967, 0.7244, 0.0782},
  {0.0241, 0.1288, 0.8444}
};

float LMS2RGB[D][D] = {
  {4.4679, -3.5873, 0.1193},
  {-1.2186, 2.3809, -0.1624},
  {0.0497, -0.2439, 1.2045}
};

float LMS2LAB[D][D] = {
  {0.5773, 0.5773, 0.5773},
  {0.4082, 0.4082, -0.8164},
  {0.7071, -0.7071, 0}
};

float LAB2LMS[D][D] = {
  {0.5773, 0.4082, 0.7071},
  {0.5773, 0.4082, -0.7071},
  {0.5773, -0.8164, 0}
};

void product_matrix_vector(float matrix[D][D], float vector[D], float res[D]){
  for(int i = 0; i < D; i++){
    res[i] = 0;
    for(int k = 0; k < D; k++){
      res[i] += matrix[i][k] * vector[k];
    }
  }
}

void print_matrix(float matrix[D][D]){
  printf("(");
  for(int i = 0; i < D; i++){
    for(int j = 0; j < D; j++){
      printf("%f", matrix[i][j]);
      if(j < D - 1)
        printf(", ");
    }
    if(i < D - 1)
      printf("\n");
  }
  printf(")\n");
}

void print_vector(float vector[D]){
  printf("(");
  for(int i = 0; i < D; i++){
    printf("%f", vector[i]);
    if(i < 2)
      printf(", ");
  }
  printf(")\n");
}

float center_float_value(float min, float max, float value){//TODO : normalize
  if(value < min)
    value = min;
  else if(value > max)
    value = max;

  return value;
}

int get_offset_buffer(int i, int j, int w){
  return (j * w + i) * 3;
}

void fill_buffer_from_img(pnm p, float *buf){
  int w = pnm_get_width(p);
  int h = pnm_get_height(p);

  for(int i = 0; i < w; i++){
    for(int j = 0; j < h; j++){
      int index = get_offset_buffer(i, j, w);
      for(int c = 0; c < D; c++){
        buf[index + c] = pnm_get_component(p, j, i, c);
      }
    }
  }
}

void fill_img_from_buffer(pnm p, float *buf){
  int w = pnm_get_width(p);
  int h = pnm_get_height(p);

  normalize(255, 0, buf, w * h * 3);

  for(int i = 0; i < w; i++){
    for(int j = 0; j < h; j++){
      int index = get_offset_buffer(i, j, w);
      for(int c = 0; c < D; c++){
        pnm_set_component(p, j, i, c, center_float_value(0, 255, buf[index + c]));
      }
    }
  }
}

void apply_log_buffer(float *buf, int size){
  for(int i = 0; i < size * 3; i++){
      buf[i] = log10(buf[i] + 1);
  }
}

void apply_exp_buffer(float *buf, int size){
  for(int i = 0; i < size * 3; i++){
    buf[i] = powf(10, buf[i]) - 1;
  }
}

float get_mean_buffer(float *buf, int size, int canal){
  float m = 0;

  for(int i = 0; i < size; i++){
    m += buf[i * 3 + canal];
  }

  return m / size;
}

float get_standard_deviation_buffer(float *buf, int size, int canal, float mean){
  float gap = 0;

  for(int i = 0; i < size; i++){
    gap += powf(buf[i * 3 + canal] - mean, 2);
  }

  return sqrt(gap / size);
}

void apply_add_value_buffer(float *buf, float value, int canal, int size){
  for(int i = 0; i < size; i++){
    buf[i * 3 + canal] += value;
  }
}

void apply_mul_value_buffer(float *buf, float value, int canal, int size){
  for(int i = 0; i < size; i++){
    buf[i * 3 + canal] *= value;
  }
}

void transform_buf(pnm p, float *buf, float matrix[D][D]){
  int w = pnm_get_width(p);
  int h = pnm_get_height(p);
  float tmp_src[D];
  float tmp_dst[D];

  for(int i = 0; i < w; i++){
    for(int j = 0; j < h; j++){
      int index = get_offset_buffer(i, j, w);
      for(int c = 0; c < D; c++){
        tmp_src[c] = buf[index + c];
      }
      product_matrix_vector(matrix, tmp_src, tmp_dst);
      for(int c = 0; c < D; c++){
        buf[index + c] = tmp_dst[c];
      }
    }
  }
}

float get_max_value(float *buf, int l){
  float m = 0;
  for(int i = 0; i < l; i++){
    if(m < buf[i])
      m = buf[i];
  }

  return m;
}

float get_min_value(float *buf, int l){
  float m = 255;
  for(int i = 0; i < l; i++){
    if(m > buf[i])
      m = buf[i];
  }

  return m;
}

void normalize(int max, int min, float *buf, int l){
  float max_buf = get_max_value(buf, l);
  float min_buf = get_min_value(buf, l);

  if(max_buf == min_buf){
    printf("Error : from img : max = min\n");
    exit(EXIT_FAILURE);
  }

  for(int i = 0; i < l; i++){
    buf[i] = ((double)max - min) / (max_buf - min_buf) * buf[i] +
              ((double)min * max_buf - max * min_buf) / (max_buf - min_buf);
  }
}

void
process(char *ims, char *imt, char* imd){
  pnm img_src = pnm_load(imt);
  pnm img_t = pnm_load(ims);

  int w_src = pnm_get_width(img_src);
  int h_src = pnm_get_height(img_src);

  int w_t = pnm_get_width(img_t);
  int h_t = pnm_get_height(img_t);

  pnm img_dst = pnm_new(w_src, h_src, PnmRawPpm);
  float *buf_src = malloc(w_src * h_src * 3 * sizeof(float));
  float *buf_t = malloc(pnm_get_width(img_t) * pnm_get_height(img_t)  * 3 * sizeof(float));

  if(!buf_src || !buf_t){
    fprintf(stderr, "Error : cannot allocate buffer memory\n");
    exit(EXIT_FAILURE);
  }

  float mean_lab_ims[D];
  float mean_lab_imt[D];
  float sd_lab_ims[D];
  float sd_lab_imt[D];

  fill_buffer_from_img(img_src, buf_src);
  fill_buffer_from_img(img_t, buf_t);

  //transforming src rgb buf to lab buf
  transform_buf(img_src, buf_src, RGB2LMS);
  apply_log_buffer(buf_src, w_src * h_src);
  transform_buf(img_src, buf_src, LMS2LAB);

  //transforming t rgb buf to lab buf
  transform_buf(img_t, buf_t, RGB2LMS);
  apply_log_buffer(buf_t, w_t * h_t);
  transform_buf(img_t, buf_t, LMS2LAB);

  for(int i = 0; i < D; i++){
    mean_lab_ims[i] = get_mean_buffer(buf_src, w_src * h_src, i);
    mean_lab_imt[i] = get_mean_buffer(buf_t, w_t * h_t, i);
    sd_lab_ims[i] = get_standard_deviation_buffer(buf_src, w_src * h_src, i, mean_lab_ims[i]);
    sd_lab_imt[i] = get_standard_deviation_buffer(buf_t, w_t * h_t, i, mean_lab_imt[i]);
  }

  for(int i = 0; i < D; i++){
    apply_add_value_buffer(buf_src, -mean_lab_ims[i], i, w_src * h_src);
    //apply_add_value_buffer(buf_src, mean_lab_ims[i], i, w_src * h_src);
    apply_mul_value_buffer(buf_src, sd_lab_imt[i] / sd_lab_ims[i], i, w_src * h_src);
    apply_add_value_buffer(buf_src, mean_lab_imt[i], i, w_src * h_src);
  }

  transform_buf(img_src, buf_src, LAB2LMS);
  apply_exp_buffer(buf_src, w_src * h_src);
  transform_buf(img_src, buf_src, LMS2RGB);

  fill_img_from_buffer(img_dst, buf_src);

  pnm_save(img_dst, PnmRawPpm, imd);

  free(buf_src);
  free(buf_t);
  pnm_free(img_src);
  pnm_free(img_t);
  pnm_free(img_dst);
}

void
usage (char *s){
  fprintf(stderr, "Usage: %s <ims> <imt> <imd> \n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 3
int
main(int argc, char *argv[]){
  if (argc != PARAM+1) usage(argv[0]);
  process(argv[1], argv[2], argv[3]);
  return EXIT_SUCCESS;
}
