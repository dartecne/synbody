/**
 * main_control_v2 
 * Incluye estructuras de control mas complejas.
 * * La aplicación incluye:
 * 
 */

#include "MIDI.h"
#include "display.h"

#define ACC_DER_THRES 16
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

unsigned long timer = 0;
unsigned long timer_old = 0;

int next_step = 0;
void setup() {
 
  Serial.begin( 9600 ); // MIDI communication
  randomSeed( analogRead(0) );
  timer_old = millis();
  next_step = random( 300, 1300 );

  //Switches
  //configure pin2 as an input and enable the internal pull-up resistor
  pinMode( 4, INPUT_PULLUP );
  pinMode( 5, INPUT_PULLUP );
  pinMode( 6, INPUT_PULLUP );
  pinMode( 7, INPUT_PULLUP );
  pinMode( RESET_PIN, OUTPUT );
  ableton_config(); 
//  callibrate();
    noteOn( 60, 127, 1 ); // 

}

int feedback = 0;

///////////////////////////////////////////////////////////////////////////////////
void loop() {
  read_acc_a(); // read xa, ya, Dxa, Dya
  read_acc_b();
  IR = analogRead( IR_PIN );
  read_switches();
  normalize();
  derivate();
//  print_sensor_values();
  int randNumber = random(3);
  timer = millis();
  if( timer > timer_old + next_step ) {
//  if( timer > timer_old ) {
    play_note( randNumber );
    timer_old = timer;
    next_step = random( 300, 3000);
  }
/*
  if( S1 ) controlChange( 14, 0, 1 );  
  else controlChange( 14, 127, 1 );
 */ 
  feedback = Dxa;
  feedback = map( feedback, ACC_DER_THRES, 80, 20, 127 );
  feedback = constrain( feedback, 10, 127 );
  controlChange( 13, feedback, 1 ); // delay feedback

  delay( TAU );
}
///////////////////////////////////////////////////////////////////////////////////

void play_note( int n ) {
  switch( n ) {
    case 0:
      noteOn( 60, 127, 1 );
      break;
    case 1:
      noteOn( 62, 127, 1 );
      break;
    case 2:
      noteOn( 64, 127, 1 );
      break;
    }
}

void test_cc() {
  for( int i = 0; i < 127; i++ ) {
    controlChange( 0, i, 1 );  
    delay( 200 );
  }
}

void ableton_config(){
  delay( 3000 );
    noteOn( 60, 127, 1 ); // 
  delay( 3000 );
    noteOn( 62, 127, 1 ); // 
  delay( 3000 );
    noteOn( 64, 127, 1 ); // 
  delay( 3000 );
    controlChange( 13, 0, 1 );  // time delay
  delay( 3000 );
  
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


void read_acc_a() {
  //Acelerometro 2Axis
  xa = analogRead( xapin );
  ya = analogRead( yapin );
}

void derivate() {
  Dxa = abs( xa_old - xa );
  xa_old = xa;
  Dya = abs( ya_old - ya );
  ya_old = ya;
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

