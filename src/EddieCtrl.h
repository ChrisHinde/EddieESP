/***
 * EddieCtrl handles all the running(/controll) of the device's functions
 */

#ifndef EDDIECTRL_H_
#define EDDIECTRL_H_

#include "hsv.h"
#include "EddieProperty.h"

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

    void Loop();

    void RegisterProperty( EddieProperty *prop );

  private:
    EddieProperty* _props[4];
    unsigned char _prop_count;
};

#endif
