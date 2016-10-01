/***
 * EddieCtrl handles all the running(/controll) of the device's functions
 */

#ifndef EDDIECTRL_H_
#define EDDIECTRL_H_

#include "hsv.h"

/**
 * Enum/IDs for available properties
 */
enum EddieDevProperty
{
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

/**
 * The controlling (ruling?) class
 */
class EddieCtrl
{
  public:
    EddieCtrl();

    void IncomingCommand( EddieDevProperty prop, char* cmd );
    void IncomingCommand( EddieDevProperty prop, EddieDevCommand cmd );
    void IncomingCommand( EddieDevProperty prop, EddieDevCommand cmd, int num );
    void IncomingCommand( EddieDevProperty prop, EddieDevCommand cmd, Color col );
    //void IncomingCommand( EddieDevProperty prop, EddieDevCommand cmd, RgbColor col );

    void loop();

    Color currentColor;
};

#endif
