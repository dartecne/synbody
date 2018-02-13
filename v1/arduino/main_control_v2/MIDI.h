#define NOTE_ON 0x90
#define NOTE_OFF 0x80
#define AFTER_POLY 0xA0
#define CONTROL_CHANGE 0xB0
#define PROGRAM_CHANGE 0xC0
#define CLOCK 0xF8
#define START 0xFA
#define CONTINUE 0xFB
#define STOP 0xFC

void noteOn( int pitch, int velocity, int channel ) {
//  Serial.write( NOTE_ON + channel );
  Serial.write( NOTE_ON );
  Serial.write( pitch );
  Serial.write( velocity );
}

void noteOff( int pitch, int channel ) {
  Serial.write( NOTE_OFF );
  Serial.write( pitch );
  Serial.write( 0 );
}



void controlChange( int controllerID, int value, int channel) {
  Serial.write( CONTROL_CHANGE );
  Serial.write( controllerID );
  Serial.write( value );
}


