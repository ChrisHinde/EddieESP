#include <Arduino.h>

#include "conf.h"
#include "extra.h"

#include "EddieProperty.h"
#include "EddieCtrl.h"


RgbStripProperty::RgbStripProperty()
{
  Serial.println("RgbStrip Init");

  // Initialize the pins as outputs
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  analogWrite(RGB_R_PIN, 0);
  analogWrite(RGB_G_PIN, 0);
  analogWrite(RGB_B_PIN, 0);

#ifdef RGB_W_PIN
  pinMode(RGB_W_PIN, OUTPUT);
  analogWrite(RGB_W_PIN, 0);
#endif

  delay(100);
  _command = RGB_DEF_CMD;

  _curr_color = Color( RGB_DEF );

  _dest_color = Color();

  if ( _command != OFF )
    _setColor(_curr_color);
}


void
RgbStripProperty::Command( EddieDevCommand cmd )
{
  Serial.print("RGB Command: ");
  Serial.println(cmd);

  _curr_dir = 1;

  if ( cmd == ON ) {
     _curr_dim = PWM_MAX;
     _setColor(_curr_color);
  } else if ( cmd == OFF ) {
    _curr_dim = 0;
    _setColor(_curr_color);
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
  } else if ( cmd == FADE ) {
    _curr_channel = G;
    _curr_color.rgb = RgbColor(1023,0,0); // Start at red (this is the simplest way when fading RGB)
    _curr_color.RgbUpdated();
  } else if ( cmd == FADE_HUE ) {
//    _curr_dir = 1;
  } else if ( cmd == WANDER ) {
    _dest_color = _curr_color;
  } else if ( cmd == FLICKER ) {
    _dest_color = _curr_color;
    _curr_dir = 0;
  } else if ( cmd == LIGHTNING ) {
    _curr_color.rgb = RgbColor(960,960,1023); // Very slighly bluish white
    _curr_color.RgbUpdated();
  }

  _command = cmd;
}

void
RgbStripProperty::Command( EddieDevCommand cmd, int num )
{
  _curr_dir = 1;

  if ( cmd == DIM_TO ) {
    _dim_to = num;
    if ( _dim_to < _curr_dim )
      _curr_dir = -1;

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

  _command = cmd;
}

void
RgbStripProperty::Command( EddieDevCommand cmd, Color col )
{
  _curr_dir = 1;

  if ( cmd == FADE_TO) {
    _dest_color = col;
  } else {
    _curr_color = col;
    _setColor( _curr_color );
  }
  if ( cmd == SET_COLOR )
    if ( _command == OFF )
      _command = ON;
  else
    _command = cmd;
}

void
RgbStripProperty::_setColor( Color color )
{
  //Serial.print("SetColor(");
  uint16_t r, g, b;
  r = color.rgb.r;
  g = color.rgb.g;
  b = color.rgb.b;
  Serial.print(r);
  Serial.print(',');
  Serial.print(g);
  Serial.print(',');
  Serial.print(b);
  Serial.println(';');

  float dim = ((float)_curr_dim) / PWM_MAX;
/*  Serial.print(_curr_dim);
  Serial.println(')');
  /*Serial.print(PWM_MAX);
  Serial.print('=');
  Serial.println(dim);*/

  #if RGB_MAX < PWM_MAX
    r = MAP(r,RGB_MAX,PWM_MAX);
    g = MAP(g,RGB_MAX,PWM_MAX);
    b = MAP(b,RGB_MAX,PWM_MAX);
  #endif

  r = r * dim;
  g = g * dim;
  b = b * dim;
/*
    Serial.print(r);
    Serial.print(';');
    Serial.print(g);
    Serial.print(';');
    Serial.println(b);
      Serial.print(PWM_MAX - r);
      Serial.print(';');
      Serial.print(PWM_MAX - g);
      Serial.print(';');
      Serial.println(PWM_MAX - b);*/

  analogWrite(RGB_R_PIN, r);
  analogWrite(RGB_G_PIN, g);
  analogWrite(RGB_B_PIN, b);

#ifdef RGB_W_PIN
  uint16_t w = color.rgb.w;
  #if RGB_MAX < PWM_MAX
    w = MAP(w,RGB_MAX,PWM_MAX);
  #endif

  analogWrite(RGB_W_PIN, w * dim);
#endif
}

void
RgbStripProperty::Loop()
{
  /*if ( _command == OFF )
    digitalWrite(BUILTIN_LED,LOW);
  else
    digitalWrite(BUILTIN_LED,HIGH);*/

  // If the output is just ON (= static) or OFF, then we have nothing to do
  if ( ( _command == ON ) || ( _command == OFF ) )
    return;

  // Check the time gone by since last done action, if it's too soon, do nothing.
  if ( (millis() - _last_ms) < _speed )
    return;

  // Update the timestamp
  _last_ms = millis();

  // PULSE dims the RGB up and down (pulsing)
  if ( _command == PULSE ) {
    // Step up/down
    _curr_dim += _curr_dir;

    // If we reached the beginning, go up again
    if ( _curr_dim == 0 ) {
      _curr_dir = 1;
    // If we reached the end (or stop point), go down again
    } else if ( _curr_dim == _dim_to ) {
      _curr_dir = -1;
    }

  // DIM / DIM_TO changes the brightness of the output over time
  //  'til we reached no brightness or teh desired brightness
  } else if ( ( _command == DIM ) || ( _command == DIM_TO ) ) {
    // Step up/down
    _curr_dim += _curr_dir;

    // If we reached the beginning/no dim, turn off
    if ( _curr_dim <= 0 )
      _command = OFF;
    // If we reached the max or the position we were dimming to, stop and keep on
    else if ( ( _curr_dim >= PWM_MAX ) || ( _curr_dim == _dim_to ) )
      _command = ON;

  // FLASH turns the output on and off every "cycle"
  } else if ( _command == FLASH ) {
    // If it's off, turn on to desired dimming
    if ( _curr_dim == 0 )
      _curr_dim = _dim_to;
    // Otherwise, turn it off
    else
      _curr_dim = 0;

  // Fade through all the RGB colors (1024 values in each channel = 1024^3 = 1073741824 steps/hues)
  } else if ( _command == FADE ) {
    // Step up/down the current channel
    _curr_color.rgb[_curr_channel] += _curr_dir;

    // We reached the end of this channel
    if (_curr_color.rgb[_curr_channel] >= 1023) {
      // Cap it to the max
      _curr_color.rgb[_curr_channel] = 1023;
      // And switch direction
      _curr_dir = -1;
    // We reached the start of this channel
    } else if (_curr_color.rgb[_curr_channel] == 0) {
      // Switch direction
      _curr_dir = 1;
    }

    // If we are at a "turning point" (the start/end of current channel was reached)
    if ( (_curr_color.rgb[_curr_channel] == 0) || (_curr_color.rgb[_curr_channel] == 1023) ) {
      // Swap to the next channel
      switch (_curr_channel) {
        case R: // From Red to Blue
          _curr_channel = B;
          break;
        case G: // From Green to Red
          _curr_channel = R;
          break;
        case B: // From Blue to Green
          _curr_channel = G;
          break;
      }
    }

    _curr_color.RgbUpdated(); // We could, maybe, skip this, to speed it up.
                              //  But it's also good to make sure all data is updated!

  // Fade through all hues (This has only 1024 steps/hues [so faster],
  //  but has the advantage of being able to set Saturation and Value
  } else if ( _command == FADE_HUE ) {
    // Take a step around the "hue circle"
    _curr_color.hsv.h += _curr_dir;

    // If we reached the end of the hue values
    if (_curr_color.hsv.h >= 1024) {
      // Go back to the beginning
      _curr_color.hsv.h = 0;
    // If we reached the start of the hue values
    } else if ( _curr_color.hsv.h == 0 ) {
      // Go back to the end
      _curr_color.hsv.h = 1023;
    }

    // Indicate that the HSV data was updated (convert it to RGB)
    _curr_color.HsvUpdated(); // This is needed, since the output is RGB!

  // Wander through random colors (like JUMP, but with fading)
  } else if ( _command == WANDER ) {
    // If we reached the destination hue/color
    if ( (_curr_color.rgb.r == _dest_color.rgb.r) &&
          (_curr_color.rgb.g == _dest_color.rgb.g) &&
          (_curr_color.rgb.b == _dest_color.rgb.b) ) {
      // Select a random hue to fade to
      _dest_color.hsv.h = random(1023);
      _dest_color.HsvUpdated();
    }

    if ( _curr_color.rgb.r > _dest_color.rgb.r )
      _curr_color.rgb.r--;
    else if ( _curr_color.rgb.r < _dest_color.rgb.r )
      _curr_color.rgb.r++;

    if ( _curr_color.rgb.g > _dest_color.rgb.g )
      _curr_color.rgb.g--;
    else if ( _curr_color.rgb.g < _dest_color.rgb.g )
      _curr_color.rgb.g++;

    if ( _curr_color.rgb.b > _dest_color.rgb.b )
      _curr_color.rgb.b--;
    else if ( _curr_color.rgb.b < _dest_color.rgb.b )
      _curr_color.rgb.b++;


    // Indicate that the RGB data was updated (convert it to HSV)
    _curr_color.RgbUpdated();

  // Jump through random colors (or hues, because hues are eaiser to randomize)
  //  Besides, then you can adjust Saturation and Value/Brightness
  } else if ( _command == JUMP ) {
    // Select a random Hue
    _curr_color.hsv.h = random(1023);

    // Indicate that the HSV data was updated (convert it to RGB)
    _curr_color.HsvUpdated(); // This is needed, since the output is RGB!

  // Flicker the current color (could simulate fire or candle flame)
  } else if ( _command == FLICKER ) {
    // We use _curr_dir and _dim_to to not have to allocate another variable (and it's unused in this state)
    //  This might be subject to change in the future!!!

    // Calculate a change for hue (from "default") (can go +-4)
    _dim_to = (int)((double)random(4.0) * sin((double)_curr_dir / 255.0 * 2 * PI));
    _curr_color.hsv.h = _dest_color.hsv.h + _dim_to;

    // Calculate a change for Saturation (from "default") (can go +-2)
    _dim_to = (int)((double)random(2.0) * sin((double)_curr_dir / 255.0 * 2 * PI));
    _curr_color.hsv.s = _dest_color.hsv.s + _dim_to;

    // Calculate a change for value (from "default") (can go +-5)
    _dim_to = (int)((double)random(5.0) * sin((double)_curr_dir / 255.0 * 2 * PI));
    _curr_color.hsv.v = _dest_color.hsv.v + _dim_to;

    _curr_dir++;
    if ( _curr_dir == 1024 )
      _curr_dir = 0;

    _curr_color.HsvUpdated();

    _last_ms -= random(90);
  }

  // Change the RGB output
  _setColor( _curr_color );
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
    Serial.println("DIM!!!!!!!"); */
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
//  analogWrite(BUILTIN_LED, PWM_MAX - dim);
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
