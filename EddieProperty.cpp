#include <Arduino.h>

#include "conf.h"
#include "EddieProperty.h"
#include "EddieCtrl.h"


RgbStripProperty::RgbStripProperty() 
{
  Serial.println("RgbStrip Init");
}

void
RgbStripProperty::Command( EddieDevCommand cmd )
{
  
  _command = cmd;
}

void
RgbStripProperty::Command( EddieDevCommand cmd, int num )
{

  _command = cmd;  
}

void
RgbStripProperty::Command( EddieDevCommand cmd, Color col )
{

  _command = cmd;
}

void
RgbStripProperty::Loop()
{
  
}



LampProperty::LampProperty()
{
  Serial.println("Lamp Init");
  _setDim( LAMP_DEF );
    _command = LAMP_DEF_CMD;
}

void
LampProperty::Command( EddieDevCommand cmd )
{
  Serial.print("Lamp Command: ");
  Serial.println(cmd);

  if ( cmd == ON ) {
    _setDim( 1024 );
    _curr_dim = 1024;
    digitalWrite(BUILTIN_LED, LOW);
  } else if ( cmd == OFF ) {
    _setDim( 0 );
    _curr_dim = 0;
    digitalWrite(BUILTIN_LED, HIGH);
  } else if ( cmd == DIM ) {
    if ( _curr_dim == 0 )
      return;

    _dim_to = 0;
    _curr_dir = -1;
  } else if ( cmd == STOP ) {
    cmd = ON;
  } else if ( ( cmd == FLASH ) || ( cmd == PULSE ) ) {
    if ( _curr_dim == 0 )
      _curr_dim = 1024;

    _dim_to = _curr_dim;
  }

  _command = cmd;
}

void
LampProperty::Command( EddieDevCommand cmd, int num )
{
  
  if ( cmd == DIM ) {
    /*_dim_to = 0;
    Serial.println("DIM!!!!!!!");*/
    return;
  } else if ( cmd == DIM_TO ) {
    _dim_to = num;
    if ( _dim_to < _curr_dim )
      _curr_dir = -1;
    else
      _curr_dir = 1;

    Serial.print("DIM_TO:");
    Serial.println(_dim_to);
  } else if ( cmd == SET_DIM ) {
    _setDim( num );
    _dim_to   = num;
    _curr_dim = num;
    Serial.print("SET_DIM:");
    Serial.println(_curr_dim);
    return;
  } else if ( cmd == SPEED ) {
    _speed = num;
    Serial.print("Speed set to:");
    Serial.println(_speed);
    return;
  }

  _command = cmd;
}

void
LampProperty::Command( EddieDevCommand cmd, Color col )
{
  // The lamp doesn't have color, so just redirect to "default"
  Command( cmd );
}

void
LampProperty::_setDim( uint16 dim )
{
  analogWrite(BUILTIN_LED, 1024 - dim);
}

void
LampProperty::Loop()
{
  if ( ( _command == ON ) || ( _command == OFF ) )
    return;

  if ( (millis() - _last_ms) < _speed )
    return;

  Serial.print(".");

  if ( _command == PULSE ) {
    if ( _curr_dim == 0 ) {
      _curr_dir = 1;
    } else if ( _curr_dim == _dim_to ) {
      _curr_dir = -1;
    }
    _curr_dim += _curr_dir;
  } else if ( ( _command == DIM ) || ( _command == DIM_TO ) ) {
    _curr_dim += _curr_dir;

    if ( _curr_dim <= 0 )
      _command = OFF;
    else if ( ( _curr_dim >= 1024 ) || ( _curr_dim == _dim_to ) )
      _command = ON;
  } else if ( _command == FLASH ) {
    if ( _curr_dim == 0 )
      _curr_dim = _dim_to;
    else
      _curr_dim = 0;
  }

  _setDim( _curr_dim );

  _last_ms = millis();
}

