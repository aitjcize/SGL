/*
 * utils.c
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Image* image_new_from_raw(const char* filename)
{
  FILE *fp = NULL;
  int current = 0;
  Image* img = NULL;

  /* Read data */
  if (!(fp = fopen(filename, "rb"))) {
    fprintf(stderr, "Failed to open `%s' for reading!\n", filename);
    exit(1);
  }

  img = (Image*)malloc(sizeof(Image));
  current = ftell(fp);
  fseek(fp, 0, SEEK_END);
  img->len = ftell(fp) - current;
  rewind(fp);

  /* Extra byte allocated for convenience */
  img->buf = (unsigned char*)malloc(img->len + 1);
  fread(img->buf, img->len, 1, fp);
  fclose(fp);
  return img;
}

Image* image_new_with_size(int len)
{
  Image* img = NULL;

  img = (Image*)malloc(sizeof(Image));
  img->len = len;
  img->buf = (unsigned char*)malloc(img->len + 1);
  memset(img->buf, 0, img->len);
  return img;
}

void image_destroy(Image* img)
{
  if (img)
    free(img->buf);
  free(img);
}

void image_dump_raw(Image* img, char* name, char* suffix)
{
  char outname[BUFSIZ];
  FILE* out_fp = NULL;

  strncpy(outname, name, BUFSIZ);
  *strstr(outname, ".") = 0;
  strncat(outname, suffix, BUFSIZ - strlen(outname));
  out_fp = fopen(outname, "wb");

  fwrite(img->buf, img->len, 1, out_fp);
  fclose(out_fp);
}

#endif /* __UTILS_H__ */
