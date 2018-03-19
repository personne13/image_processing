#include <float.h>
#include <stdlib.h>
#include <math.h>

#include <fft.h>

fftw_complex *
forward(int rows, int cols, unsigned short* g_img)
{
  fftw_complex *in = malloc(rows * cols * sizeof(fftw_complex));
  fftw_complex *out = malloc(rows * cols * sizeof(fftw_complex));

  if(!in || !out){
    fprintf(stderr, "forward : Error allocating buffer (%dx%d)\n", cols, rows);
    exit(EXIT_FAILURE);
  }

  for(int i = 0; i < rows * cols; i++){
    in[i] = g_img[i];
    int x = i % cols;
    int y = (i - x) / rows;
    if((x + y) % 2 != 0)
      in[i] *= -1;
  }

  fftw_plan plan = fftw_plan_dft_2d(rows, cols, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(plan);
  fftw_destroy_plan(plan);

  free(in);

  return out;
}

unsigned short *
backward(int rows, int cols, fftw_complex* freq_repr, int norm_factor)
{
  unsigned short *res = malloc(rows * cols * sizeof(short));
  fftw_complex *out = malloc(rows * cols * sizeof(fftw_complex));

  if(!out){
    fprintf(stderr, "forward : Error allocating buffer (%dx%d)\n", cols, rows);
    exit(EXIT_FAILURE);
  }

  fftw_plan plan = fftw_plan_dft_2d(rows, cols, freq_repr, out, FFTW_BACKWARD, FFTW_ESTIMATE);
  fftw_execute(plan);
  fftw_destroy_plan(plan);

  for(int i = 0; i < rows * cols; i++){
    float tmp = 1.0 / norm_factor * creal(out[i]);

    int x = i % cols;
    int y = (i - x) / rows;
    if((x + y) % 2 != 0)
      tmp *= -1;

    if(tmp > 255)
      res[i] = 255;
    else if(tmp < 0)
      res[i] = 0;
    else
      res[i] = tmp;
  }

  free(out);

  return res;
}

void
freq2spectra(int rows, int cols, fftw_complex* freq_repr, float* as, float* ps)
{
  for(int i = 0; i < rows * cols; i++){
    as[i] = sqrt(pow(creal(freq_repr[i]), 2) + pow(cimag(freq_repr[i]), 2));
    ps[i] = atan2(cimag(freq_repr[i]), creal(freq_repr[i]));
  }
}

void
spectra2freq(int rows, int cols, float* as, float* ps, fftw_complex* freq_repr)
{
  for(int i = 0; i < rows * cols; i++){
    freq_repr[i] = as[i] * cos(ps[i]) + I * as[i] * sin(ps[i]);
  }
}
