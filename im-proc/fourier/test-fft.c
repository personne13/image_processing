/**
 * @file test-fft.c
 * @brief test the behaviors of functions in fft module
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <fft.h>
#include <bcl.h>

#define SIZE_BUFFER 256
#define PI 3.14159

char * get_name_src(char *name){
  int n = strlen(name);
  char *res = malloc((n + 1) * sizeof(char));

  for(int i = n - 1; name[i] > 0; i--){
    if(name[i] == '/'){
      strncpy(res, &name[i + 1], n - i);
      return res;
    }
  }

  return name;
}

/**
 * @brief test the forward and backward functions
 * @param char* name, the input image file name
 */
void
test_forward_backward(char* name)
{
  fprintf(stderr, "test_forward_backward: ");
  char extension[4] = "FB-";

  if(strlen(name) > SIZE_BUFFER - strlen(extension))
    fprintf(stderr, "%s : name too long (%d max)\n", name, SIZE_BUFFER);

  char *name_file = get_name_src(name);
  char path_dst[SIZE_BUFFER];

  sprintf(path_dst, "%s%s", extension, name_file);

  pnm img = pnm_load(name);
  int rows = pnm_get_width(img);
  int cols = pnm_get_height(img);
  pnm dest = pnm_new(rows, cols, PnmRawPpm);

  unsigned short *g_img = pnm_get_channel(img, NULL, PnmRed);//already gray image

  fftw_complex *img_forw = forward(rows, cols, g_img);
  unsigned short *img_back = backward(rows, cols, img_forw);

  pnm_set_channel(dest, img_back, PnmRed);
  pnm_set_channel(dest, img_back, PnmGreen);
  pnm_set_channel(dest, img_back, PnmBlue);

  pnm_save(dest, PnmRawPpm, path_dst);

  free(img_forw);
  free(img_back);
  free(g_img);
  free(name_file);
  pnm_free(img);
  pnm_free(dest);
  fprintf(stderr, "OK\n");
}

/**
 * @brief test image reconstruction from of magnitude and phase spectrum
 * @param char *name: the input image file name
 */
void
test_reconstruction(char* name)
{
  fprintf(stderr, "test_reconstruction: ");

  char extension[9] = "FB-ASPS-";

  if(strlen(name) > SIZE_BUFFER - strlen(extension))
    fprintf(stderr, "%s : name too long (%d max)\n", name, SIZE_BUFFER);

  char *name_file = get_name_src(name);
  char path_dst[SIZE_BUFFER];


  sprintf(path_dst, "%s%s", extension, name_file);

  pnm img_src = pnm_load(name);
  int rows = pnm_get_width(img_src);
  int cols = pnm_get_height(img_src);
  pnm img_dest = pnm_new(rows, cols, PnmRawPpm);

  unsigned short *g_img = pnm_get_channel(img_src, NULL, PnmRed);//already gray image
  float *as = malloc(rows * cols * sizeof(float));
  float *ps = malloc(rows * cols * sizeof(float));

  if(!as || !ps){
    fprintf(stderr, "Error malloc : test_reconstruction (%dx%d)\n", rows, cols);
    exit(EXIT_FAILURE);
  }

  fftw_complex *img_forw = forward(rows, cols, g_img);
  freq2spectra(rows, cols, img_forw, as, ps);
  spectra2freq(rows, cols, as, ps, img_forw);
  unsigned short *img_back = backward(rows, cols, img_forw);

  pnm_set_channel(img_dest, img_back, PnmRed);
  pnm_set_channel(img_dest, img_back, PnmGreen);
  pnm_set_channel(img_dest, img_back, PnmBlue);

  pnm_save(img_dest, PnmRawPpm, path_dst);

  free(img_back);
  pnm_free(img_src);
  pnm_free(img_dest);
  free(name_file);
  free(img_forw);
  free(g_img);
  free(as);
  free(ps);
  fprintf(stderr, "OK\n");
}

void transform_buffer(float *buf, int size){//non linear normalization
  float m = 0;
  float p = 0.1;

  for(int i = 0; i < size; i++){
    if(m < buf[i]){
      m = buf[i];
    }
  }

  for(int i = 0; i < size; i++){
    buf[i] = 255 * pow((buf[i] / m), p);
  }
}

void copy_buffer(float *buf_src, unsigned short *buf_dst, int rows, int cols){
  for(int i = 0; i < rows * cols; i++){
    buf_dst[i] = buf_src[i];
  }
}

/**
 * @brief test construction of magnitude and phase images in ppm files
 * @param char* name, the input image file name
 */
void
test_display(char* name)
{
  fprintf(stderr, "test_display: ");

  char extension_p[4] = "PS-";
  char extension_a[4] = "AS-";

  if(strlen(name) > SIZE_BUFFER - strlen(extension_p))
    fprintf(stderr, "%s : name too long (%d max)\n", name, SIZE_BUFFER);

  char *name_file = get_name_src(name);
  char path_dst_p[SIZE_BUFFER];
  char path_dst_a[SIZE_BUFFER];


  sprintf(path_dst_p, "%s%s", extension_p, name_file);
  sprintf(path_dst_a, "%s%s", extension_a, name_file);

  pnm img_src = pnm_load(name);
  int rows = pnm_get_width(img_src);
  int cols = pnm_get_height(img_src);
  pnm img_dest_p = pnm_new(rows, cols, PnmRawPpm);
  pnm img_dest_a = pnm_new(rows, cols, PnmRawPpm);

  unsigned short *g_img = pnm_get_channel(img_src, NULL, PnmRed);//already gray image
  float *as = malloc(rows * cols * sizeof(float));
  float *ps = malloc(rows * cols * sizeof(float));
  unsigned short *as_short = malloc(rows * cols * sizeof(float));
  unsigned short *ps_short = malloc(rows * cols * sizeof(float));

  if(!as || !ps){
    fprintf(stderr, "Error malloc : test_reconstruction (%dx%d)\n", rows, cols);
    exit(EXIT_FAILURE);
  }

  fftw_complex *img_forw = forward(rows, cols, g_img);
  freq2spectra(rows, cols, img_forw, as, ps);

  transform_buffer(as, rows * cols);

  copy_buffer(as, as_short, rows, cols);
  copy_buffer(ps, ps_short, rows, cols);

  pnm_set_channel(img_dest_p, ps_short, PnmRed);
  pnm_set_channel(img_dest_p, ps_short, PnmGreen);
  pnm_set_channel(img_dest_p, ps_short, PnmBlue);

  pnm_set_channel(img_dest_a, as_short, PnmRed);
  pnm_set_channel(img_dest_a, as_short, PnmGreen);
  pnm_set_channel(img_dest_a, as_short, PnmBlue);

  pnm_save(img_dest_p, PnmRawPpm, path_dst_p);
  pnm_save(img_dest_a, PnmRawPpm, path_dst_a);

  pnm_free(img_src);
  pnm_free(img_dest_p);
  pnm_free(img_dest_a);
  free(name_file);
  free(img_forw);
  free(g_img);
  free(as);
  free(ps);
  fprintf(stderr, "OK\n");
}

int get_index_buffer(int i, int j, int cols){
  return i * (cols - 1) + j;
}

void add_freq(float *as, int rows, int cols){
  float m = 0;
  int freq = 8;

  for(int i = 0; i < rows * cols; i++){
    if(m < as[i])
      m = as[i];
  }

  float factor_add = 0.25 * m;

  as[get_index_buffer(rows / 2 + 1 + freq, freq + 1, cols)] += factor_add;
  as[get_index_buffer(rows / 2 + 1, -freq + 1, cols)] += factor_add;
  as[get_index_buffer(rows / 2 + 1 - freq, -freq + 1, cols)] += factor_add;
  as[get_index_buffer(rows / 2 + 1, freq + 1, cols)] += factor_add;
}

/**
 * @brief test the modification of magnitude by adding a periodic functions
          on both vertical and horizontal axis, and
 *        construct output images
 * @param char* name, the input image file name
 */
void

test_add_frequencies(char* name)
{
  fprintf(stderr, "test_add_frequencies: ");
  char extension_freq[6] = "FREQ-";
  char extension_fas[6] = "FAS-";

  if(strlen(name) > SIZE_BUFFER - strlen(extension_freq))
    fprintf(stderr, "%s : name too long (%d max)\n", name, SIZE_BUFFER);

  char *name_file = get_name_src(name);
  char path_dst_freq[SIZE_BUFFER];
  char path_dst_fas[SIZE_BUFFER];

  sprintf(path_dst_freq, "%s%s", extension_freq, name_file);
  sprintf(path_dst_fas, "%s%s", extension_fas, name_file);

  pnm img_src = pnm_load(name);
  int rows = pnm_get_width(img_src);
  int cols = pnm_get_height(img_src);
  pnm img_dest_freq = pnm_new(rows, cols, PnmRawPpm);
  pnm img_dest_fas = pnm_new(rows, cols, PnmRawPpm);

  unsigned short *g_img = pnm_get_channel(img_src, NULL, PnmRed);//already gray image
  float *as = malloc(rows * cols * sizeof(float));
  float *ps = malloc(rows * cols * sizeof(float));
  unsigned short *as_short = malloc(rows * cols * sizeof(float));

  if(!as || !ps || !as_short){
    fprintf(stderr, "Error malloc : test_reconstruction (%dx%d)\n", rows, cols);
    exit(EXIT_FAILURE);
  }

  fftw_complex *img_forw = forward(rows, cols, g_img);
  freq2spectra(rows, cols, img_forw, as, ps);
  add_freq(as, rows, cols);
  spectra2freq(rows, cols, as, ps, img_forw);
  transform_buffer(as, rows * cols);
  copy_buffer(as, as_short, rows, cols);
  unsigned short *img_back = backward(rows, cols, img_forw);

  pnm_set_channel(img_dest_freq, img_back, PnmRed);
  pnm_set_channel(img_dest_freq, img_back, PnmGreen);
  pnm_set_channel(img_dest_freq, img_back, PnmBlue);

  pnm_set_channel(img_dest_fas, as_short, PnmRed);
  pnm_set_channel(img_dest_fas, as_short, PnmGreen);
  pnm_set_channel(img_dest_fas, as_short, PnmBlue);

  pnm_save(img_dest_freq, PnmRawPpm, path_dst_freq);
  pnm_save(img_dest_fas, PnmRawPpm, path_dst_fas);

  pnm_free(img_src);
  pnm_free(img_dest_freq);
  pnm_free(img_dest_fas);
  free(name_file);
  free(img_forw);
  free(g_img);
  free(as);
  free(ps);
  fprintf(stderr, "OK\n");
}

void
run(char* name)
{
  test_forward_backward(name);
  test_reconstruction(name);
  test_display(name);
  test_add_frequencies(name);
}

void
usage(const char *s)
{
  fprintf(stderr, "Usage: %s <ims> \n", s);
  exit(EXIT_FAILURE);
}

#define PARAM 1
int
main(int argc, char *argv[])
{
  if (argc != PARAM+1) usage(argv[0]);
  run(argv[1]);
  return EXIT_SUCCESS;
}
