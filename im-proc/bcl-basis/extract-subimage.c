#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <bcl.h>

#define PARAM_I 1
#define PARAM_J 2
#define PARAM_ROW 3
#define PARAM_COLS 4
#define PARAM_IMS 5
#define PARAMS_IMD 6

void process(int i0, int j0, int w, int h, char *path_src, char *path_dst);

void
usage (char *s)
{
  fprintf(stderr,"Usage: %s %s", s, "<i> <j> <rows> <cols> <ims> <imd>\n");
  exit(EXIT_FAILURE);
}

#define PARAM 6
int
main(int argc, char *argv[])
{
  if (argc != PARAM+1)
    usage(argv[0]);
  else
    process(atoi(argv[PARAM_I]), atoi(argv[PARAM_J]), atoi(argv[PARAM_COLS]), atoi(argv[PARAM_ROW]), argv[PARAM_IMS], argv[PARAMS_IMD]);
  return EXIT_SUCCESS;
}

void process(int i0, int j0, int w, int h, char *path_src, char *path_dst){
  pnm img_src = pnm_load(path_src);
  pnm img_dest = pnm_new(h, w, PnmRawPpm);

  unsigned short value;

  if(pnm_get_width(img_src) - 1 < w + i0 || pnm_get_height(img_src) - 1 < h + j0){
    fprintf(stderr, "Error : img too short to extract\n");
    return;
  }

  if(w < 0 || i0 < 0 || h < 0 || j0 < 0){
    fprintf(stderr, "Error : invalid args\n");
  }

  for(int i = i0; i < w + i0; i++){
    for(int j = j0; j < h + j0; j++){
      value = pnm_get_component(img_src, i, j, PnmRed);
      pnm_set_component(img_dest, i - i0, j - j0, PnmRed, value);
      value = pnm_get_component(img_src, i, j, PnmRed);
      pnm_set_component(img_dest, i - i0, j - j0, PnmGreen, value);
      value = pnm_get_component(img_src, i, j, PnmRed);
      pnm_set_component(img_dest, i - i0, j - j0, PnmBlue, value);
    }
  }

  pnm_save(img_dest, PnmRawPpm, path_dst);

  pnm_free(img_src);
  pnm_free(img_dest);
}
