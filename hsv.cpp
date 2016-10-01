/*
 * HSV.c
 *
 * Created: 2014-10-27 22:38:44
 *  Code from http://stackoverflow.com/a/22120275/4204058
 */ 


#include <string.h>
#include <stdlib.h>

#include "HSV.h"

RgbColor HsvToRgb(HsvColor hsv)
{
  RgbColor rgb;
  unsigned char region, p, q, t;
  unsigned int h, s, v, remainder;

  if (hsv.s == 0)
  {
    rgb.r = hsv.v;
    rgb.g = hsv.v;
    rgb.b = hsv.v;
    return rgb;
  }

  // converting to 16 bit to prevent overflow
  h = hsv.h;
  s = hsv.s;
  v = hsv.v;

  region = h / 43;
  remainder = (h - (region * 43)) * 6;

  p = (v * (255 - s)) >> 8;
  q = (v * (255 - ((s * remainder) >> 8))) >> 8;
  t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

  switch (region)
  {
    case 0:
    rgb.r = v;
    rgb.g = t;
    rgb.b = p;
    break;
    case 1:
    rgb.r = q;
    rgb.g = v;
    rgb.b = p;
    break;
    case 2:
    rgb.r = p;
    rgb.g = v;
    rgb.b = t;
    break;
    case 3:
    rgb.r = p;
    rgb.g = q;
    rgb.b = v;
    break;
    case 4:
    rgb.r = t;
    rgb.g = p;
    rgb.b = v;
    break;
    default:
    rgb.r = v;
    rgb.g = p;
    rgb.b = q;
    break;
  }

  return rgb;
}

HsvColor RgbToHsv(RgbColor rgb)
{
  HsvColor hsv;
  unsigned char rgbMin, rgbMax;

  rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
  rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

  hsv.v = rgbMax;
  /*if (hsv.v == 0)
  {
    hsv.h = 0;
    hsv.s = 0;
    return hsv;
  }*/

  hsv.s = 255 * ((long)(rgbMax - rgbMin)) / hsv.v;
  /*if (hsv.s == 0)
  {
    hsv.h = 0;
    return hsv;
  }*/

  if (rgbMax == rgb.r)
    hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
  else if (rgbMax == rgb.g)
    hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
  else
    hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

  return hsv;
}


unsigned char
hex_to_uchar( char c )
{
  if ( ( c >= '0' ) && ( c <= '9' ) )
    return c - '0';
  else if ( ( c >= 'A' ) && ( c <= 'F' ) )
    return (c - 'A') + 10;
  else
    return 0;
}
unsigned char
hex_to_uchar( const char *c )
{
}

Color::Color()
{
  rgb = {0,0,0};
  hsv = {0,255,0};
}

Color::Color( const char *color_string )
{
  if ( color_string[0] == '#' ) {
    unsigned char r, g, b;
    if ( strlen(color_string) == 7 ) {
      r = hex_to_uchar(color_string[1]) * 16 + hex_to_uchar(color_string[2]);
      g = hex_to_uchar(color_string[3]) * 16 + hex_to_uchar(color_string[4]);
      b = hex_to_uchar(color_string[5]) * 16 + hex_to_uchar(color_string[6]);
    } else {
      r = hex_to_uchar(color_string[1]);
      g = hex_to_uchar(color_string[2]);
      b = hex_to_uchar(color_string[3]);
    }

    rgb = { r, g, b };
    hsv = RgbToHsv( rgb );
  } else if ( color_string[3] == '(' ) { // if color_string is something like 'rgb(123,321,42)
    unsigned char a, b, c;
    char* token = "\0\0\0\0";
    char tmp[16];

    a = strlen(color_string) - 5;
    strncpy(tmp, color_string + 4, a); // tmp should now be like '123,321,42'

    token = strtok(tmp,",");
    a = atoi(token);

    token = strtok(NULL,",");
    b = atoi(token);

    token = strtok(NULL,",");
    c = atoi(token);

    if ( strncmp(color_string, "rgb", 3) == 0 ) {
      rgb = { a, b, c };
      hsv = RgbToHsv( rgb );
    } else if ( strncmp(color_string, "hsv", 3) == 0 ) {
      hsv = { a, b, c };
      rgb = HsvToRgb( hsv );
    }
  }
}

Color::Color( EddieColorType type, unsigned char a, unsigned char b, unsigned char c )
{
  if ( type == RGB ) {
    rgb = { a, b, c };
    hsv = RgbToHsv( rgb );
  } else if ( type == HSV ) {
    hsv = { a, b, c };
    rgb = HsvToRgb( hsv );
  }
}

Color::Color( RgbColor col )
{
  rgb = col;
  hsv = RgbToHsv( col );
}

Color::Color( HsvColor col )
{
  hsv = col;
  rgb = HsvToRgb( col );
}

