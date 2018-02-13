/**
 * read_sensors_and_print_v1
 * Lee los sensores siguientes:
 * - 2 acc de 2 ejes
 * - 4 switches
 * - 1 InfraRojo
 * 
 * * La aplicación escribe los datos por el puerto serie
 * 
 */
#include "display.h"

#define TAU 20 // tiempo de bulce en ms

//Acelerometro2Axis
const int xapin = A5;   
const int yapin = A4;
const int xbpin = A3;  
const int ybpin = A2;

int xa = 0, ya = 0, xa_old = 0, ya_old = 0, Dxa = 0, Dya = 0; // acc values and its derivates
int xb = 0, yb = 0, xb_old = 0, yb_old = 0, Dxb = 0, Dyb = 0;  // acc values and its derivates

// values taken empiricaly 
int xa_min = 288, ya_min = 294;
int xa_max = 383, ya_max = 395;
int xb_min = 293, yb_min = 300;
int xb_max = 443, yb_max = 398;

//IR
int IR_min = 60, IR_max = 400;
const int IR_PIN = A0;
int IR = 0;

//Switches
#define RESET_PIN 3
int S1 = 0, S2 = 0, S3 = 0, S4 = 0;
int S1_up = 0, S1_down = 0, S1_old = 0;
int S2_up = 0, S2_down = 0, S2_old = 0;
int S3_up = 0, S3_down = 0, S3_old = 0;
int S4_up = 0, S4_down = 0, S4_old = 0;
int SW, SW_up, SW_down;

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
//  callibrate();
//  print_header();
  digitalWrite( RESET_PIN, LOW);
}

int feedback = 0;

///////////////////////////////////////////////////////////////////////////////////
void loop() {
  read_acc_a(); // read xa, ya, Dxa, Dya
  read_acc_b();
  IR = analogRead( IR_PIN );
  read_switches();
//  normalize();
  derivate();
//  print_sensor_values();
  send_sensor_values();

  delay( TAU );
}
///////////////////////////////////////////////////////////////////////////////////

void calc_sw_old_new( int &S, int &S_old, int &S_up, int &S_down  ) {
  if( S & !S_old ) { S_up = 1; S_down = 0;}
  else if( !S & S_old ) { S_up = 0; S_down = 1;}
  else{ S_up = 0; S_down = 0;} 
}

void read_switches() {
  //Switches
  //read the pushbutton value into a variable
  S1_old = S1;
  S2_old = S2;
  S3_old = S3;
  S4_old = S4;
  
  S1 = digitalRead( 4 );
  S2 = digitalRead( 5 );
  S3 = digitalRead( 6 );
  S4 = digitalRead( 7 );
  
//  reset_switches();

  S1 = !S1;
  calc_sw_old_new( S1, S1_old, S1_up, S1_down );   
  S2 = !S2;
  calc_sw_old_new( S2, S2_old, S2_up, S2_down );   
  S3 = !S3;
  calc_sw_old_new( S3, S3_old, S3_up, S3_down );   
  S4 = !S4;
  calc_sw_old_new( S4, S4_old, S4_up, S4_down );   

  SW = S1 | S2<<1 | S3<<2 | S4<<3;
  SW_up = S1_up | S2_up<<1 | S3_up<<2 | S4_up<<3;
  SW_down = S1_down | S2_down<<1 | S3_down<<2 | S4_down<<3;
}

/**
 * Normaliza cada valor de un sensor entre su maximo y minimo despues de calibrarse
 * para que este en el rando 0-127 aceptado por la funcion MIDI ControlChange
 */
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
  
  IR = constrain( IR, 0, 127 );  
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

    print_sensor_limits_values();

    delay(1);
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

void read_acc_b() {
  //Acelerometro 2Axis
  xb = analogRead( xbpin );
  yb = analogRead( ybpin );

}

void derivate() {
  Dxa = abs( xa_old - xa );
  xa_old = xa;
  Dya = abs( ya_old - ya );
  ya_old = ya;
}

/* Nunca llegue a entender esta funcion y creo que es el resultado 
de una confusion de la alumna que hizo la primera version del traje */
void reset_switches() {
  
  if (S1 == HIGH | S2 == HIGH | S3 == HIGH | S4 == HIGH ) {
    digitalWrite( RESET_PIN, LOW);
  } else {
    digitalWrite( RESET_PIN, HIGH);
  }
}

