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

    RgbColor rgb;
    HsvColor hsv;
};

#endif /* HSV_H_ */
