/*
 * hsv.h
 *
 * Created: 2014-10-27 22:40:48
 *  Author: Christopher
 */


#ifndef HSV_H_
#define HSV_H_

enum RGBChannel
{
  R = 0,
  G,
  B,
  W
};

class RgbColor
{
  public:
    RgbColor() { RgbColor(0,0,0); }
    RgbColor( uint16_t r, uint16_t g, uint16_t b, uint16_t w = 0 );

    uint16_t& operator[] ( RGBChannel chann );

    uint16_t r;
    uint16_t g;
    uint16_t b;
    uint16_t w;
  };

class HsvColor
{
  public:
    HsvColor() { HsvColor(0,0,0); }
    HsvColor( uint16_t h, uint16_t s, uint16_t v );

    uint16_t h;
    uint16_t s;
    uint16_t v;
};

RgbColor HsvToRgb(HsvColor hsv);
HsvColor RgbToHsv(RgbColor rgb);

unsigned char hex_to_uchar( char c );
unsigned char hex_to_uchar( const char *c );

enum EddieColorType
{
  RGB,
  HSV
};

class Color
{
  public:
    Color();
    Color( const char* color );
    Color( EddieColorType type, unsigned char a, unsigned char b, unsigned char c );
    Color( RgbColor color );
    Color( HsvColor color );

    void RgbUpdated();
    void HsvUpdated();

    RgbColor rgb;
    HsvColor hsv;
};

#endif /* HSV_H_ */
