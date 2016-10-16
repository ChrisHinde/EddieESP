#ifndef EDDIEPROP_H_
#define EDDIEPROP_H_

#include "conf.h"
#include "extra.h"
#include "hsv.h"

/**
 * Enum/IDs for available properties
 */
enum EddieDevProperty
{
  ALL = 0,
  RGB_STRIP = 1,
  LAMP = 2,

  TEMP = 11,
  HUMID = 12
};


/**
 * Enum/IDs for available commands
 */
enum EddieDevCommand
{
  OFF  = 0,
  ON   = 1,

  // Dim/Brightness commands
  DIM       = 2,
  PULSE     = 3,
  FLASH     = 4, BLINK = 4,
  // Color commands
  FADE      = 5,
  FADE_HUE  = 6,
  WANDER    = 7,
  JUMP      = 8,
  LIGHTNING = 9,
  FLICKER   = 10,
  PATTERN   = 11,

  SET_DIM   = 21,
  SET_COLOR = 22,
  SET_HUE   = 23,
  SET_SAT   = 24,
  SET_VAL   = 25,

  DIM_TO  = 31,
  FADE_TO = 32,
  FADE_HUE_TO = 33,
  FADE_SAT_TO = 34,
  FADE_VAL_TO = 35,

  SPEED = 98,
  STOP  = 99
};


class EddieProperty
{
  public:
    EddieProperty() {};

    virtual void Command( EddieDevCommand cmd ) {};
    virtual void Command( EddieDevCommand cmd, int num ) {};
    virtual void Command( EddieDevCommand cmd, Color col ) {};

    virtual void Loop() {};

    virtual EddieDevProperty GetID() = 0;

  protected:
    EddieDevCommand _command;
};
class RgbStripProperty : public EddieProperty
{
  public:
    RgbStripProperty();

    void Command( EddieDevCommand cmd );
    void Command( EddieDevCommand cmd, int num );
    void Command( EddieDevCommand cmd, Color col );

    void Loop();

    EddieDevProperty GetID() { return RGB_STRIP; }

  private:
    void _setColor( Color color );

    unsigned long _last_ms  = 0;
    unsigned int _curr_dim = RGB_DIM_DEF;
    unsigned int _speed    = RGB_SPEED;
    unsigned int _dim_to   = RGB_DIM_DEF;
    int  _curr_dir = 1;

    Color _curr_color;
    Color _dest_color;

    RGBChannel _curr_channel;
    HsvColor _hsv_temp;
    IntTrip _hsv_diff;
};

class LampProperty : public EddieProperty
{
  public:
    LampProperty();

    void Command( EddieDevCommand cmd );
    void Command( EddieDevCommand cmd, int num );
    void Command( EddieDevCommand cmd, Color col );

    void Loop();

    EddieDevProperty GetID() { return LAMP; }
  private:
    void _setDim( uint16 dim );

    uint32 _last_ms  = 0;
    uint16 _curr_dim = LAMP_DEF;
    uint16 _speed    = LAMP_SPEED;
    uint16 _dim_to  = 0;
    int   _curr_dir = 1;

};

#endif
