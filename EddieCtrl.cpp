#include <Arduino.h>
#include <stdlib.h>

#include "EddieCtrl.h"
#include "hsv.h"

/**
 * Initialize
 */
EddieCtrl::EddieCtrl()
{
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
}

/**
 * Handle an incoming command as string for a certain property
 */
void
EddieCtrl::IncomingCommand( EddieDevProperty prop, char* cmd )
{
  char *c;

  // If the first byte is '!', then treat the rest as "binary form"
  if ( cmd[0] == '!' ) {
    // Just a command, no extra data
    if ( cmd[2] == 0 ) {
      IncomingCommand( prop, (EddieDevCommand)cmd[1] );
    // Data  indicated to be RGB
    } else if ( cmd[2] == 'r' ) {
      Color col = Color( RGB, cmd[3], cmd[4], cmd[5] );
      IncomingCommand( prop, (EddieDevCommand)cmd[1], col );
    // Data  indicated to be HSV
    } else if ( cmd[2] == 'h' ) {
      Color col = Color( HSV, cmd[3], cmd[4], cmd[5] );
      IncomingCommand( prop, (EddieDevCommand)cmd[1], col );
    }

// Recognize command and "translate" to enums
  } else if ( stricmp(cmd, "off") == 0 ) {
    IncomingCommand( prop, OFF );
  } else if ( stricmp(cmd, "stop") == 0 ) {
    IncomingCommand( prop, STOP );
  } else if ( ( stricmp(cmd, "fade") == 0 ) || ( stricmp(cmd, "fade_rgb") == 0 ) ) {
    IncomingCommand( prop, FADE );
  } else if ( ( stricmp(cmd, "fade_hue") == 0 ) || ( stricmp(cmd, "fade_hsv") == 0 ) ) {
    IncomingCommand( prop, FADE_HUE );
  } else if ( ( stricmp(cmd, "flash") == 0 ) || ( stricmp(cmd, "blink") == 0 ) ) {
    IncomingCommand( prop, FLASH );
  } else if ( stricmp(cmd, "pulse") == 0 ) {
    IncomingCommand( prop, PULSE );
  } else if ( stricmp(cmd, "wander") == 0 ) {
    IncomingCommand( prop, WANDER );
  } else if ( stricmp(cmd, "jump") == 0 ) {
    IncomingCommand( prop, JUMP );
  } else if ( stricmp(cmd, "dim") == 0 ) {
    IncomingCommand( prop, DIM );

  // The command contains data/arguments (most likely color)
  } else if ( ( c = strchr(cmd,':') ) != NULL ) {
    char p2[20];
    int p = c - cmd;
    strcpy(p2, c + 1);

  // Recognize command and "translate" to enums and include the data
    if ( strncmp(cmd, "dim_to", p) == 0 ) {
      IncomingCommand( prop, DIM_TO, Color(p2) );
    } else if ( strncmp(cmd, "set_dim", p) == 0 ) {
      IncomingCommand( prop, SET_DIM, Color(p2) );
    } else if ( strncmp(cmd, "fade_to", p) == 0 ) {
      IncomingCommand( prop, FADE_TO, Color(p2) );
    } else if ( ( strncmp(cmd, "color", p) == 0 ) || ( strncmp(cmd, "set_color", p) == 0 ) ) {
      IncomingCommand( prop, FADE_TO, Color(p2) );
    } else if ( ( strncmp(cmd, "flash", p) == 0 ) || ( strncmp(cmd, "blink", p) == 0 ) ) {
      IncomingCommand( prop, FADE_TO, Color(p2) );
    } else if ( strncmp(cmd, "pulse",p) == 0 ) {
      IncomingCommand( prop, PULSE, Color(p2) );
    } else if ( strncmp(cmd, "speed",p) == 0 ) {
      IncomingCommand( prop, SPEED, atoi(p2) );
    }
  }
}


void
EddieCtrl::IncomingCommand( EddieDevProperty prop, EddieDevCommand cmd )
{
  
}

void
EddieCtrl::IncomingCommand( EddieDevProperty prop, EddieDevCommand cmd, Color col )
{
  
}

void
EddieCtrl::IncomingCommand( EddieDevProperty prop, EddieDevCommand cmd, int num )
{
  
}


void
EddieCtrl::loop()
{
  
}

