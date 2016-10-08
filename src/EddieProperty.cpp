#include <Arduino.h>

#include "conf.h"
#include "extra.h"

#include "EddieProperty.h"
#include "EddieCtrl.h"


RgbStripProperty::RgbStripProperty()
{
/*  Serial.println("RgbStrip Init");

  // Initialize the pins as outputs
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);
#ifdef RGB_W_PIN
  pinMode(RGB_W_PIN, OUTPUT);
#endif

  _curr_color = RGB_DEF;
  _dest_color = Color((RgbColor){0,0,0});*/
}

void
RgbStripProperty::Command( EddieDevCommand cmd )
{
/*  Serial.print("RGB Command: ");
  Serial.println(cmd);

  if ( cmd == ON ) {
     _curr_dim = PWM_MAX;
  } else if ( cmd == OFF ) {
    _curr_dim = 0;
  } else if ( cmd == DIM ) {
    if ( _curr_dim == 0 )
      return;

    _dim_to = 0;
    _curr_dir = -1;
  } else if ( cmd == STOP ) {
    cmd = ON;
  } else if ( ( cmd == FLASH ) || ( cmd == PULSE ) ) {
    if ( _curr_dim == 0 )
      _curr_dim = PWM_MAX;

    _dim_to = _curr_dim;
  }

  _command = cmd;*/
}

void
RgbStripProperty::Command( EddieDevCommand cmd, int num )
{
/*  if ( cmd == DIM_TO ) {
    _dim_to = num;
    if ( _dim_to < _curr_dim )
      _curr_dir = -1;
    else
      _curr_dir = 1;

    Serial.print("RGB_DIM_TO:");
    Serial.println(_dim_to);
  } else if ( cmd == SET_DIM ) {
    //_setDim( num );
    _dim_to   = num;
    _curr_dim = num;
    Serial.print("RGB_SET_DIM:");
    Serial.println(_curr_dim);
    return;
  } else if ( cmd == SPEED ) {
    _speed = num;
    Serial.print("RGB Speed set to:");
    Serial.println(_speed);
    return;

  } else if ( cmd == SET_HUE ) {
    _curr_color.hsv.h = num;
    _curr_color.HsvUpdated();
    return;
  } else if ( cmd == SET_SAT ) {
    _curr_color.hsv.s = num;
    _curr_color.HsvUpdated();
    return;
  } else if ( cmd == SET_VAL ) {
    _curr_color.hsv.v = num;
    _curr_color.HsvUpdated();
    return;

  } else if ( cmd == FADE_HUE_TO ) {
    _dest_color.hsv = _curr_color.hsv;
    _dest_color.hsv.h = num;
  } else if ( cmd == FADE_SAT_TO ) {
    _dest_color.hsv = _curr_color.hsv;
    _dest_color.hsv.s = num;
  } else if ( cmd == FADE_VAL_TO ) {
    _dest_color.hsv = _curr_color.hsv;
    _dest_color.hsv.v = num;
  }

  _command = cmd;*/
}

void
RgbStripProperty::Command( EddieDevCommand cmd, Color col )
{
/*  if ( cmd == FADE_TO) {
    _dest_color = col;
  } else {
    _curr_color = col;
    _setColor( _curr_color );
  }

  _command = cmd;*/
}

void
RgbStripProperty::_setColor( Color &color )
{
/*  uint16_t r, g, b;
  r = color.rgb.r;
  g = color.rgb.g;
  b = color.rgb.b;

  float dim = _curr_dim / PWM_MAX;

  #if RGB_MAX < PWM_MAX
    r = MAP(r,RGB_MAX,PWM_MAX);
    g = MAP(g,RGB_MAX,PWM_MAX);
    b = MAP(b,RGB_MAX,PWM_MAX);
  #endif

  r = r * dim;
  g = g * dim;
  b = b * dim;

  analogWrite(RGB_R_PIN, r);
  analogWrite(RGB_G_PIN, g);
  analogWrite(RGB_B_PIN, b);

#ifdef RGB_W_PIN
  uint16_t w = color.rgb.w;
  #if RGB_MAX < PWM_MAX
    w = MAP(w,RGB_MAX,PWM_MAX);
  #endif

  analogWrite(RGB_W_PIN, w * dim);
#endif*/
}

void
RgbStripProperty::Loop()
{
  if ( ( _command == ON ) || ( _command == OFF ) )
    return;

  if ( (millis() - _last_ms) < _speed )
    return;

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
    else if ( ( _curr_dim >= PWM_MAX ) || ( _curr_dim == _dim_to ) )
      _command = ON;
  } else if ( _command == FLASH ) {
    if ( _curr_dim == 0 )
      _curr_dim = _dim_to;
    else
      _curr_dim = 0;
  }

  _setColor( _curr_color );

  _last_ms = millis();
}



LampProperty::LampProperty()
{
  Serial.println("Lamp Init");

  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  _setDim( LAMP_DEF );
  _command = LAMP_DEF_CMD;
}

void
LampProperty::Command( EddieDevCommand cmd )
{
  Serial.print("Lamp Command: ");
  Serial.println(cmd);

  if ( cmd == ON ) {
    _setDim( PWM_MAX );
    _curr_dim = PWM_MAX;
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
      _curr_dim = PWM_MAX;

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
  analogWrite(BUILTIN_LED, PWM_MAX - dim);
}

void
LampProperty::Loop()
{
  if ( ( _command == ON ) || ( _command == OFF ) )
    return;

  if ( (millis() - _last_ms) < _speed )
    return;

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
    else if ( ( _curr_dim >= PWM_MAX ) || ( _curr_dim == _dim_to ) )
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
