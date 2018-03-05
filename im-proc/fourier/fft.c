#include <float.h>
#include <stdlib.h>
#include <math.h>

#include <fft.h>

fftw_complex *
forward(int rows, int cols, unsigned short* g_img)
{
  (void)rows;
  (void)cols;
  (void)g_img;
  return NULL;
}

unsigned short *
backward(int rows, int cols, fftw_complex* freq_repr)
{
  (void)rows;
  (void)cols;
  (void)freq_repr;
  return NULL;
}

void
freq2spectra(int rows, int cols, fftw_complex* freq_repr, float* as, float* ps) 
{
  (void)rows;
  (void)cols;
  (void)freq_repr;
  (void)as;
  (void)ps;
}

void 
spectra2freq(int rows, int cols, float* as, float* ps, fftw_complex* freq_repr)
{
  (void)rows;
  (void)cols;
  (void)as;
  (void)ps;
  (void)freq_repr;
}
