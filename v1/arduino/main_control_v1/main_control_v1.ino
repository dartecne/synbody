/**
 * main_control_vX incluye un acelerometro ADXL345, que se deja en la siguiente versión por dar muchos problemas de comunicación y robustez.
 * La aplicación incluye:
 * - lectura de los sensores: IR_SHARP, 2 ADXL3xx, y 4 Switches
 * - calibración de los acelerometros
 * - escritura MIDI ad-hoc para configurar Ableton LIVE
 * - escritura MIDI en el control 
 * 
 * TODO: mejorar la calibración. Incluir estructuras complejas de control entre los sensores y mensajes MIDI
 */

#define NOTE_ON 0x90
#define NOTE_OFF 0x80
#define AFTER_POLY 0xA0
#define CONTROL_CHANGE 0xB0
#define PROGRAM_CHANGE 0xC0
#define CLOCK 0xF8
#define START 0xFA
#define CONTINUE 0xFB
#define STOP 0xFC

#define ACC_DER_THRES 30
#define TIME_FOR_PLAY 3000

#define TAU 20 // tiempo de bulce en ms

//Acelerometro2Axis
const int xapin = A1;   
const int yapin = A0;
const int xbpin = A4;  
const int ybpin = A3;

int xa = 0, ya = 0, xa_old = 0, ya_old = 0, Dxa = 0, Dya = 0; // acc values and its derivates
int xb = 0, yb = 0, xb_old = 0, yb_old = 0, Dxb = 0, Dyb = 0;  // acc values and its derivates

int xa_min = 288, ya_min = 294;
int xa_max = 383, ya_max = 395;
int xb_min = 293, yb_min = 300;
int xb_max = 443, yb_max = 398;

//IR
int IR_min = 60, IR_max = 400;
const int IR_PIN = A2;
int IR = 0;

//Switches
#define RESET_PIN 3
int S1 = 0, S2 = 0, S3 = 0, S4 = 0;
int S1_up = 0, S1_down = 0, S1_old = 0;

void setup() {
 
  Serial.begin( 9600 ); // MIDI communication

  //Switches
  //configure pin2 as an input and enable the internal pull-up resistor
  pinMode( 4, INPUT_PULLUP );
  pinMode( 5, INPUT_PULLUP );
  pinMode( 6, INPUT_PULLUP );
  pinMode( 7, INPUT_PULLUP );
  pinMode( RESET_PIN, OUTPUT );
  noteOn( 45, 127, 1 ); // Para iniciar los sonidos. Envia la nota A2
//  ableton_config();
//  callibrate();
}

unsigned long timer = 0;
bool flag_playing = 0;
///////////////////////////////////////////////////////////////////////////////////
void loop() {
  read_acc_a();
  read_acc_b();
  IR = analogRead( IR_PIN );
  read_switches();

  normalize();
  derivate();
  
  
//  print_sensor_values();

//  if( S1_up ) controlChange( 14, 0, 1 );  
//  else if( S1_down ) controlChange( 14, 127, 1 );
  if( S1 ) controlChange( 14, 0, 1 );  
  else controlChange( 14, 127, 1 );

  int f = xa, q = xa;
  controlChange( 12, f, 1 ); // Acc21 Frequency
  controlChange( 12, q, 1 );  // Acc21 Resonator
  controlChange( 13, IR, 1 );  // Acc21 IRI
  
  if( ( Dxa > ACC_DER_THRES ) | ( Dya > ACC_DER_THRES )   ) {
    controlChange( 15, 127, 1 ); // Sube volumen sonido Acc31
    timer = millis();
    flag_playing = 1;
  }
  if( flag_playing & ( millis() - timer ) > TIME_FOR_PLAY ) {
      controlChange(15, 0, 1 ); // Baja volumen sonido Acc31
      flag_playing = 0;
  }

  delay( TAU );
}
///////////////////////////////////////////////////////////////////////////////////

void test_cc() {
  for( int i = 0; i < 127; i++ ) {
    controlChange( 0, i, 1 );  
    delay( 200 );
  }
}

void ableton_config(){
  delay( 3000 );
  noteOn( 69, 127, 1 );
  delay( 3000 );
  controlChange( 0, 0, 1 );
  delay( 3000 );
  controlChange( 10, 0, 1 );
  delay( 3000 );
  
}

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

void read_switches() {
  //Switches
  //read the pushbutton value into a variable
  S1_old = S1;
  S1 = digitalRead( 4 );
  if( S1 & !S1_old ) { S1_up = 1; S1_down = 0;}
  else if( !S1 & S1_old ) { S1_up = 0; S1_down = 1;}
  else{ S1_up = 0; S1_down = 0;} 
   
  S2 = digitalRead( 5 );
  S3 = digitalRead( 6 );
  S4 = digitalRead( 7 );

  reset_switches();
}

void normalize() {
  xa = map( xa, xa_min, xa_max, 0, 127 );
  ya = map( ya, ya_min, ya_max, 0, 127 );

  xb = map( xb, xb_min, xb_max, 0, 127 );
  yb = map( yb, yb_min, yb_max, 0, 127 );

  IR = map( IR, IR_min, IR_max, 0, 127 );
  
  xa = constrain( xa, 0, 127 );
  ya = constrain( ya, 0, 127 );
  xb = constrain( xb, 0, 127 );
  yb = constrain( yb, 0, 127 );
  
  //IR = constrain( IR, 0, 127 );
  if( IR < 0 ) IR = 0;
  if( IR > 127 ) IR = 127;
  
}

void callibrate() {
  xa_max = -1024; xb_max = -1024;
  ya_max = -1024; yb_max = -1024;
  xa_min = 1024;  xb_min = 1024;
  ya_min = 1024;  yb_min = 1024;

  for ( int i = 0; i < 1500; i++ ) {
    read_acc_mean_value( 10 );
    if ( xa_max < xa ) xa_max = xa;
    if ( ya_max < ya ) ya_max = ya;

    if ( xa_min > xa ) xa_min = xa;
    if ( ya_min > ya ) ya_min = ya;

    if ( xb_max < xb ) xb_max = xb;
    if ( yb_max < yb ) yb_max = yb;

    if ( xb_min > xb ) xb_min = xb;
    if ( yb_min > yb ) yb_min = yb;

//    print_sensor_limits_values();

//    delay(1);
  }
}

void print_sensor_values() {
  Serial.print( xa );  Serial.print( "   " );
  Serial.print( ya );  Serial.print( "   " );
  Serial.print( Dxa ); Serial.print( "   " );
  Serial.print( Dya ); Serial.print( "   " );
  Serial.print( xb );  Serial.print( "\t" );
  Serial.print( yb );  Serial.print( "\t" );
  Serial.println();
}

void print_sensor_limits_values() {
  Serial.print( xa );  Serial.print( "\t" );
  Serial.print( xa_min );  Serial.print( "\t" );
  Serial.print( xa_max );  Serial.print( "\t" );
  Serial.print( ya );  Serial.print( "\t" );
  Serial.print( ya_min );  Serial.print( "\t" );
  Serial.print( ya_max );  Serial.print( "\t" );

  Serial.print( xb );  Serial.print( "\t" );
  Serial.print( xb_min );  Serial.print( "\t" );
  Serial.print( xb_max );  Serial.print( "\t" );
  Serial.print( yb );  Serial.print( "\t" );
  Serial.print( yb_min );  Serial.print( "\t" );
  Serial.print( yb_max );  Serial.print( "\t" );
  Serial.println();
}

void read_acc_mean_value( int sample_size ) {
  long xa_mean = 0, ya_mean = 0;
  long xb_mean = 0, yb_mean = 0;
  read_acc_a();
  read_acc_b();
  delay(1);
  for (int i = 0; i < sample_size; i++) {
    xa_mean += xa;
    ya_mean += ya;
    read_acc_a();
    xb_mean += xb;
    yb_mean += yb;
    read_acc_b();
  }

  xa = xa_mean / sample_size;
  ya = ya_mean / sample_size;
  xb = xb_mean / sample_size;
  yb = yb_mean / sample_size;
}

void derivate() {
  Dxa = abs( xa_old - xa );
  xa_old = xa;
}

void read_acc_a() {
  //Acelerometro 2Axis
  xa = analogRead( xapin );
  ya = analogRead( yapin );

}

void read_acc_b() {
  //Acelerometro 2Axis
  xb = analogRead( xbpin );
  yb = analogRead( ybpin );

}

void reset_switches() {
  if (S1 == HIGH) {
    digitalWrite( RESET_PIN, LOW);
  } else {
    digitalWrite( RESET_PIN, HIGH);
  }
  if (S2 == HIGH) {
    digitalWrite( RESET_PIN, LOW);
  } else {
    digitalWrite( RESET_PIN, HIGH);
  }
  if (S3 == HIGH) {
    digitalWrite( RESET_PIN, LOW);
  } else {
    digitalWrite( RESET_PIN, HIGH);
  }
  if (S4 == HIGH) {
    digitalWrite( RESET_PIN, LOW);
  } else {
    digitalWrite( RESET_PIN, HIGH);
  }
}

