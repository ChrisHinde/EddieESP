/*
 * HSV.c
 *
 * Created: 2014-10-27 22:38:44
 *  Code from http://stackoverflow.com/a/22120275/4204058
 */


#include <Arduino.h>
#include <stdlib.h>

#include "HSV.h"

RgbColor::RgbColor( uint16_t _r, uint16_t _g, uint16_t _b, uint16_t _w )
{
  r = _r;
  g = _g;
  b = _b;
  w = _w;
}

uint16_t&
RgbColor::operator[](RGBChannel chann)
{
  switch (chann) {
    case R:
      return r;
    case G:
      return g;
    case B:
      return b;
    case W:
      return w;
  }
}

HsvColor::HsvColor( uint16_t _h, uint16_t _s, uint16_t _v )
{
  h = _h;
  s = _s;
  v = _v;
}

/**
 *  HsvToRgb - Convert HSV (Hue, Saturation, Value) to RGB (Red, Green, Blue)
 *   Bosed on code from http://stackoverflow.com/a/22120275/4204058
 *    adapted to 10 bit (0-1023 instead of 0-255)
 *
 *  Read more:
 *   http://www.rapidtables.com/convert/color/hsv-to-rgb.htm
 *   https://en.wikipedia.org/wiki/HSL_and_HSV#Converting_to_RGB
 */
RgbColor HsvToRgb(HsvColor hsv)
{
  RgbColor rgb;
  unsigned int region, p, q, t;
  unsigned int h, s, v, remainder;

  // If the Saturation is 0, then use the Value for each RGB channel
  if (hsv.s == 0)
  {
    rgb.r = hsv.v;
    rgb.g = hsv.v;
    rgb.b = hsv.v;
    return rgb;
  }

  h = hsv.h;
  s = hsv.s;
  v = hsv.v;

  // There's 6 regions (60dgrs apart), so each region is ~171 for 0-1023
  region = h / 171;
  remainder = (h - (region * 171)) * 6;

  // Calculate values
  p = (v * (1023 - s)) >> 10;
  q = (v * (1023 - ((s * remainder) >> 10))) >> 10;
  t = (v * (1023 - ((s * (1023 - remainder)) >> 10))) >> 10;

  // Place values, depending on region
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

/**
 * RgbToHsv - Convert RGB (Red, Green, Blue) to HSV (Hue, Saturation, Value)
 *   Bosed on code from http://stackoverflow.com/a/22120275/4204058
 *    adapted to 10 bit (0-1023 instead of 0-255)
 *
 *  Read more here:
 *    http://www.rapidtables.com/convert/color/rgb-to-hsv.htm
 */
HsvColor RgbToHsv(RgbColor rgb)
{
  HsvColor hsv = HsvColor();
  uint16 rgbMin, rgbMax;

  // Find min and max values in the RGB channels
  rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
  rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

  // Set the Value to the max RGB value
  hsv.v = rgbMax;

  // If the Value is 0 (black), set Saturation to 1023 (max)
  //   This is perhaps unorthodox, but helps when we work with the HSV data later
  if ( hsv.v == 0 )
    hsv.s = 1023;
  // Otherwise.. Calculate Saturation
  else
    hsv.s = 1023 * ((long)(rgbMax - rgbMin)) / hsv.v;

  // Calculate Hue
  if ( (rgbMax - rgbMin) == 0 ) {
    hsv.h = 0;
  } else {
    if (rgbMax == rgb.r)
      hsv.h = 0 + 171 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
      hsv.h = 341 + 171 * (rgb.b - rgb.r) / (rgbMax - rgbMin); // 338??
    else
      hsv.h = 682 + 171 * (rgb.r - rgb.g) / (rgbMax - rgbMin); // 684??
  }

  return hsv;
}

/**
  * Convert a hex value (single character: 0-F ) to integer
  */
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
  rgb = RgbColor(0,0,0);
  hsv = HsvColor(0,1023,0);
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

    rgb = RgbColor( r * 4, g * 4, b *4 );
    hsv = RgbToHsv( rgb );
  } else if ( color_string[3] == '(' ) { // if color_string is something like 'rgb(123,321,42)
    uint16_t a, b, c;
    char* token;
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
      rgb = RgbColor( a, b, c );
      hsv = RgbToHsv( rgb );
    } else if ( strncmp(color_string, "hsv", 3) == 0 ) {
      hsv = HsvColor( a, b, c );
      rgb = HsvToRgb( hsv );
    }
  }
}

Color::Color( EddieColorType type, unsigned char a, unsigned char b, unsigned char c )
{
  if ( type == RGB ) {
    rgb = RgbColor( a, b, c );
    hsv = RgbToHsv( rgb );
  } else if ( type == HSV ) {
    hsv = HsvColor( a, b, c );
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


void
Color::RgbUpdated()
{
  hsv = RgbToHsv( rgb );
}
void
Color::HsvUpdated()
{
  rgb = HsvToRgb( hsv );
}
