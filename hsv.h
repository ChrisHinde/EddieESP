/*
 * hsv.h
 *
 * Created: 2014-10-27 22:40:48
 *  Author: Christopher
 */ 

#ifndef HSV_H_
#define HSV_H_

typedef struct RgbColor
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
} RgbColor;

typedef struct HsvColor
{
  unsigned char h;
  unsigned char s;
  unsigned char v;
} HsvColor;

RgbColor HsvToRgb(HsvColor hsv);
HsvColor RgbToHsv(RgbColor rgb);

#endif /* HSV_H_ */
