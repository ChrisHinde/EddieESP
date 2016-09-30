/*
 * hsv.h
 *
 * Created: 2016-09-30 22:42:42
 *  Author: Christopher
 */ 

#ifndef EDDIECTRL_H_
#define EDDIECTRL_H_

#include "hsv.h"

enum EddieDevProperty
{
  RGB = 1,
  LAMP = 2,

  TEMP = 11,
  HUMID = 12
};
enum EddieDevCommand
{
  OFF = 0,
  ON  = 1,

  SET_DIM   = 11,
  SET_COLOR = 12,

  DIM_TO  = 21,
  FADE_TO = 22,
};

class EddieCtrl
{
  public:
    EddieCtrl();

    void IncomingCommand( EddieDevProperty prop, const char* cmd );
    void IncomingCommand( EddieDevProperty prop, EddieDevCommand cmd );
    void IncomingCommand( EddieDevProperty prop, EddieDevCommand cmd, HsvColor col );
    void IncomingCommand( EddieDevProperty prop, EddieDevCommand cmd, RgbColor col );
};

#endif /* EDDIECTRL_H_ */
