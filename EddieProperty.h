#ifndef EDDIEPROP_H_
#define EDDIEPROP_H_

#include "conf.h"
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
  FADE = 2,
  FADE_HUE = 3,
  DIM      = 4,
  PULSE    = 5,
  FLASH    = 6, BLINK = 6,
  WANDER   = 7,
  JUMP     = 8,

  SET_DIM   = 11,
  SET_COLOR = 12,

  DIM_TO  = 21,
  FADE_TO = 22,

  SPEED = 98,
  STOP  = 99
};


class EddieProperty
{
  public:
    EddieProperty() {}

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
    int8  _curr_dir = 1;
    
};

#endif
