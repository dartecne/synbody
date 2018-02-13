/**
 * main_control_v0 incluye un acelerometro ADXL345, que se deja en la siguiente versión por dar muchos problemas de comunicación y robustez.
 * La aplicación incluye:
 * - lectura de los sensores: ADXL345, IR_SHARP y ADXL3xx, 4 Switches
 * - calibración de los acelerometros
 * - escritura MIDI ad-hoc para configurar Ableton LIVE
 * - escritura MIDI en el control 
 * 
 * TODO: mejorar la calibración. Incluir estructuras complejas de control entre los sensores y mensajes MIDI
 */


#include <Wire.h>

#define NOTE_ON 0x90
#define NOTE_OFF 0x80
#define AFTER_POLY 0xA0
#define CONTROL_CHANGE 0xB0
#define PROGRAM_CHANGE 0xC0
#define CLOCK 0xF8
#define START 0xFA
#define CONTINUE 0xFB
#define STOP 0xFC

#define TAU 250
//Acelerometros 3Axis
#define DEVICE (0x53)    //second ADXL345 device address muñequera
#define TO_READ (6)        //num of bytes we are going to read each time (two bytes for each axis)

byte buff[TO_READ] ;      //6 bytes buffer for saving data read from the device
char str[512];            //string buffer to transform data before sending it to the serial port

int regAddress = 0x32;
int xa = 0, ya = 0, za = 0;
  // -305, 300, -387, 251, 511, 511
/*
int xa_max = -1024, ya_max = -1024, za_max = -1024;
int xa_min = 1024, ya_min = 1024, za_min = 1024;
*/
int xa_max = 300, ya_max = 251, za_max = -1024;
int xa_min = 305, ya_min = -384, za_min = 1024;

//Acelerometro2Axis
const int xpin = A1;   // Cada coordenada va referida a un pin de la placa
const int ypin = A0;
int xc = 0, yc = 0;
int xc_max = -1024;
int xc_min = 1024;
int yc_max = -1024;
int yc_min = 1024;

//IR
const int IR_PIN = A1;    //Mirar si es la entrada analógica adecuada!!!!!
int IR = 0;

//Switches
#define RESET_PIN 3
int S1 = 0, S2 = 0, S3 = 0, S4 = 0;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin( 115200 ); // MIDI communication

  writeTo(DEVICE, 0x2D, 24);  // turns-on Acc A

  //Switches
  //configure pin2 as an input and enable the internal pull-up resistor
  pinMode( 4, INPUT_PULLUP );
  pinMode( 5, INPUT_PULLUP );
  pinMode( 6, INPUT_PULLUP );
  pinMode( 7, INPUT_PULLUP );
  pinMode( RESET_PIN, OUTPUT );
//  ableton_config();
  callibrate();
}

///////////////////////////////////////////////////////////////////////////////////
void loop() {
  read_acc_a();
  read_acc_c();
  normalize();

  read_switches();
  IR = analogRead( IR_PIN );

  if( S1 == LOW ) noteOn( 69, 127, 1 );
//  test_cc();
  /**/   
  controlChange( 0, xa, 1 );
  controlChange( 10, ya, 1 );
  controlChange( 0, xc, 1 );
  controlChange( 10, yc, 1 );
/**/

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
  S1 = digitalRead( 4 );
  S2 = digitalRead( 5 );
  S3 = digitalRead( 6 );
  S4 = digitalRead( 7 );

  reset_switches();
}

void normalize() {
  xa = map( xa, xa_min, xa_max, 0, 127 );
  ya = map( ya, ya_min, ya_max, 0, 127 );
  za = map( za, za_min, za_max, 0, 127 );

  xc = map( xc, xc_min, xc_max, 0, 127 );
  yc = map( yc, yc_min, yc_max, 0, 127 );
}

void callibrate() {

  for ( int i = 0; i < 150; i++ ) {
    read_acc_mean_value( 10 );
    if ( xa_max < xa ) xa_max = xa;
    if ( ya_max < ya ) ya_max = ya;
    if ( za_max < za ) za_max = za;

    if ( xa_min > xa ) xa_min = xa;
    if ( ya_min > ya ) ya_min = ya;
    if ( za_min > za ) za_min = za;

    if ( xc_max < xc ) xc_max = xc;
    if ( yc_max < yc ) yc_max = yc;

    if ( xc_min > xc ) xc_min = xc;
    if ( yc_min > yc ) yc_min = yc;

//    print_sensor_limits_values();

    delay(100);
  }

  // -305, 300, -387, 251, 511, 511
}

void print_sensor_values() {
  Serial.print( xa );  Serial.print( "\t" );
  Serial.print( ya );  Serial.print( "\t" );
  // Serial.print( za );  Serial.print( "\t" );
  Serial.print( xc );  Serial.print( "\t" );
  Serial.print( yc );  Serial.print( "\t" );
  Serial.println();
}

void print_sensor_limits_values() {
  Serial.print( xa );  Serial.print( "\t" );
  Serial.print( xa_min );  Serial.print( "\t" );
  Serial.print( xa_max );  Serial.print( "\t" );
  Serial.print( ya );  Serial.print( "\t" );
  Serial.print( ya_min );  Serial.print( "\t" );
  Serial.print( ya_max );  Serial.print( "\t" );
  /* Serial.print( za );  Serial.print( "\t" );
    Serial.print( za_min );  Serial.print( "\t" );
    Serial.print( za_max );  Serial.print( "\t" );
  */
  Serial.print( xc );  Serial.print( "\t" );
  Serial.print( xc_min );  Serial.print( "\t" );
  Serial.print( xc_max );  Serial.print( "\t" );
  Serial.print( yc );  Serial.print( "\t" );
  Serial.print( yc_min );  Serial.print( "\t" );
  Serial.print( yc_max );  Serial.print( "\t" );
  Serial.println();
}

void read_acc_mean_value( int sample_size ) {
  long xa_mean = 0, ya_mean = 0, za_mean = 0;
  long xc_mean = 0, yc_mean = 0;
  read_acc_a();
  read_acc_c();
  delay(1);
  for (int i = 0; i < sample_size; i++) {
    xa_mean += xa;
    ya_mean += ya;
    za_mean += za;
    read_acc_a();
  }

  xa = xa_mean / sample_size;
  ya = ya_mean / sample_size;
  za = za_mean / sample_size;
}

void read_acc_c() {
  //Acelerometro 2Axis
  xc = analogRead( xpin );
  yc = analogRead( ypin );

}

void read_acc_a() {
  //Acelerometros 3Axis
  readFrom(DEVICE, regAddress, TO_READ, buff); //read the acceleration data from the ADXL345
  //each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
  //thus we are converting both bytes in to one int
  xa = (((int)buff[1]) << 8) | buff[0];
  ya = (((int)buff[3]) << 8) | buff[2];
  za = (((int)buff[5]) << 8) | buff[4];

}

void reset_switches() {
  // Keep in mind the pullup means the pushbutton's
  // logic is inverted. It goes HIGH when it's open,
  // and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:

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

//Funciones de Acelerometros 3Axis
void writeTo(int device, byte address, byte val) {
  Wire.beginTransmission(device); //start transmission to device
  Wire.write(address);        // send register address
  Wire.write(val);        // send value to write
  Wire.endTransmission(); //end transmission
}

void readFrom(int device, byte address, int num, byte buff[]) {
  Wire.beginTransmission(device); //start transmission to device
  Wire.write(address);        //sends address to read from
  Wire.endTransmission(); //end transmission

  Wire.beginTransmission(device); //start transmission to device
  Wire.requestFrom(device, num);    // request 6 bytes from device

  int i = 0;
  while (Wire.available())   //device may send less than requested (abnormal)
  {
    buff[i] = Wire.read(); // receive a byte
    i++;
  }
  Wire.endTransmission(); //end transmission
}

