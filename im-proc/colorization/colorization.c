#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <bcl.h>

#define D 3
#define NB_SAMPLES 200
#define SIZE_NEIGHBOORHOOD 5

typedef struct Sample Sample;
struct Sample{
  float i;
  float j;
  float l;
  float a;
  float b;
  float standard_dev;
};

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
void fill_part_sample_from_buf(float *buf_src, Sample *sample, int size_sample, int w, int h);
float get_standard_deviation_luminance_pixel(float *buf_src, int i, int j, int w, int h);
void change_color_samples(Sample *sample_src, int size_src, Sample *sample_dst, int w, int h);
int search_matching_pixel(Sample *sample_src, int size_src, Sample pixel, float max_dev, float max_lum);
float compute_distance_samples(Sample s1, Sample s2, float max_dev, float max_lum);

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

int rand_a_b(int a, int b){
  return rand()%(b-a) + a;
}

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

float center_float_value(float min, float max, float value){
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

float get_standard_deviation_luminance_pixel(float *buf_src, int i, int j, int w, int h){//mean of standard deviation of its neighboors
  float sum = 0;
  float sum_dev = 0;
  float mean = 0;
  int nb_neighboors = 0;
  float tab[SIZE_NEIGHBOORHOOD * SIZE_NEIGHBOORHOOD];

  for(int x = i - SIZE_NEIGHBOORHOOD / 2; x <= i + SIZE_NEIGHBOORHOOD / 2; x++){
    for(int y = j - SIZE_NEIGHBOORHOOD / 2; y <= j + SIZE_NEIGHBOORHOOD / 2; y++){
      if(i >= 0 && i < w &&
          j >= 0 && j < h){
        int index = get_offset_buffer(i, j, w);
        tab[nb_neighboors] = buf_src[index];
        sum += tab[nb_neighboors];
        nb_neighboors++;
      }
    }
  }

  mean = sum / nb_neighboors;

  for(int k = 0; k < nb_neighboors; k++){
    sum_dev += pow(mean - tab[k], 2);
  }

  return sqrt(sum_dev / nb_neighboors);
}

void fill_sample_from_buf(float *buf_src, Sample *sample, int w, int h){
  for(int i = 0; i < w; i++){
    for(int j = 0; j < h; j++){
      int index_sample = j * w + i;
      int index_buf = get_offset_buffer(i, j, w);
      sample[index_sample].i = i;
      sample[index_sample].j = j;
      sample[index_sample].l = buf_src[index_buf];
      sample[index_sample].a = buf_src[index_buf + 1];
      sample[index_sample].b = buf_src[index_buf + 2];
      sample[index_sample].standard_dev = get_standard_deviation_luminance_pixel(buf_src, i, j, w, h);
    }
  }
}

void fill_part_sample_from_buf(float *buf_src, Sample *sample, int size_sample, int w, int h){//fill the sample tabular respecting the ratio of the initial image. Every left space in the tabular is set randomly.
  float ratio = (float)w / h;

  int w_n = sqrt(ratio * size_sample);
  int h_n = sqrt(size_sample / ratio);

  for(int i = 0; i < w_n; i++){
    for(int j = 0; j < h_n; j++){
      int x = (i / w_n) * w;
      int y = (j / h_n) * h;
      int index_sample = j * w_n + i;
      int index_buf = get_offset_buffer(x, y, w);
      sample[index_sample].i = x;
      sample[index_sample].j = y;
      sample[index_sample].l = buf_src[index_buf];
      sample[index_sample].a = buf_src[index_buf + 1];
      sample[index_sample].b = buf_src[index_buf + 2];
      sample[index_sample].standard_dev = get_standard_deviation_luminance_pixel(buf_src, x, y, w, h);
    }
  }

  for(int i = w_n * h_n; i < size_sample; i++){//fill every other samples randomly
    int x = rand_a_b(0, w);
    int y = rand_a_b(0, h);
    int index_buf = get_offset_buffer(x, y, w);
    sample[i].i = x;
    sample[i].j = y;
    sample[i].l = buf_src[index_buf];
    sample[i].a = buf_src[index_buf + 1];
    sample[i].b = buf_src[index_buf + 2];
    sample[i].standard_dev = get_standard_deviation_luminance_pixel(buf_src, x, y, w, h);
  }
}

void fill_buf_from_sample(float *buf, Sample *sample, int w, int h){
  for(int i = 0; i < w * h; i++){
    int index = get_offset_buffer(sample[i].i, sample[i].j, w);
    buf[index] = sample[i].l;
    buf[index + 1] = sample[i].a;
    buf[index + 2] = sample[i].b;
  }
}

float compute_distance_samples(Sample s1, Sample s2, float max_dev, float max_lum){
  float dist_dev = (s1.standard_dev - s2.standard_dev) / max_dev;
  float dist_lum = (s1.l - s2.l) / max_lum;

  return pow(dist_dev, 2) + pow(dist_lum, 2);
}

int search_matching_pixel(Sample *sample_src, int size_src, Sample pixel, float max_dev, float max_lum){
  float min_dist = -1;
  int current_index = 0;

  for(int i = 0; i < size_src; i++){
    float current_dist = compute_distance_samples(sample_src[i], pixel, max_dev, max_lum);
    if(min_dist == -1 || current_dist < min_dist){
      min_dist = current_dist;
      current_index = i;
    }
  }

  return current_index;
}

void change_color_samples(Sample *sample_src, int size_src, Sample *sample_dst, int w, int h){
  float max_dev = 0;
  float max_lum = 0;

  for(int i = 0; i < size_src; i++){
    if(sample_src[i].l > max_lum){
      max_lum = sample_src[i].l;
    }
    if(sample_src[i].standard_dev > max_dev){
      max_dev = sample_src[i].standard_dev;
    }
  }

  for(int i = 0; i < w * h; i++){
    if(sample_dst[i].l > max_lum){
      max_lum = sample_dst[i].l;
    }
    if(sample_dst[i].standard_dev > max_dev){
      max_dev = sample_dst[i].standard_dev;
    }
  }

  for(int i = 0; i < w * h; i++){
    int index = search_matching_pixel(sample_src, size_src, sample_dst[i], max_dev, max_lum);
    sample_dst[i].a = sample_src[index].a;
    sample_dst[i].b = sample_src[index].b;
  }
}

void
process(char *ims, char *imt, char* imd){
  pnm img_t = pnm_load(imt);
  pnm img_src = pnm_load(ims);

  int w_src = pnm_get_width(img_src);
  int h_src = pnm_get_height(img_src);

  int w_t = pnm_get_width(img_t);
  int h_t = pnm_get_height(img_t);

  pnm img_dst = pnm_new(w_t, h_t, PnmRawPpm);
  float *buf_src = malloc(w_src * h_src * D * sizeof(float));
  float *buf_t = malloc(w_t * h_t  * D * sizeof(float));
  Sample *sample_dst = malloc(w_t * h_t * sizeof(Sample));
  Sample sample_src[NB_SAMPLES];

  if(!buf_src || !buf_t || !sample_dst){
    fprintf(stderr, "Error : cannot allocate buffer memory\n");
    exit(EXIT_FAILURE);
  }

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

  fill_part_sample_from_buf(buf_src, sample_src, NB_SAMPLES, w_src, h_src);
  fill_sample_from_buf(buf_t, sample_dst, w_t, h_t);

  change_color_samples(sample_src, NB_SAMPLES, sample_dst, w_t, h_t);

  fill_buf_from_sample(buf_t, sample_dst, w_t, h_t);


  /*Algorithme :
  -Transformer les deux images en lab
  -Générer un echantillonnage de l'image colorée (calcul de l'écart type puis compression)
  -calculer l'écart-type du facteur l de chaque pixel des images en fonction de leur voisinage
  -Pour chaque pixel de l'image grise, trouver le pixel de l'image coloré adapté
  -Remplacer les composantes alpha et beta du pixel de l'image en niveau de gris par
    celles de son pixel coloré correspondant
  -Transformer l'image vers rgb, puis sauvegarder
  */


  transform_buf(img_t, buf_t, LAB2LMS);
  apply_exp_buffer(buf_t, w_t * h_t);
  transform_buf(img_t, buf_t, LMS2RGB);

  fill_img_from_buffer(img_dst, buf_t);

  pnm_save(img_dst, PnmRawPpm, imd);

  free(buf_src);
  free(buf_t);
  free(sample_dst);
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
  srand(time(NULL));
  process(argv[1], argv[2], argv[3]);
  return EXIT_SUCCESS;
}
