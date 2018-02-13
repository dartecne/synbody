/**
 * Lee los datos de los siguientes sensores:
 * - acelerometro A ADXL345 conectado por I2C
 * - acelerometro B ADXL345 conectado por I2C
 * - acelerometro ADXL3xx de 2 ejes conectado a A1 (x) y A0 (y)
 * - SHARP ID conectado a A3
 * - 4 Switches conectados a D4, D5, D6, y D7
 */

#include <Wire.h>
//#include <SharpIR.h>

//Acelerometros 3Axis
#define DEVICE_A (0x1D)    //first ADXL345 device address tobillera
#define DEVICE_B (0x53)    //second ADXL345 device address muñequera
#define TO_READ (6)        //num of bytes we are going to read each time (two bytes for each axis)

byte buff[TO_READ] ;      //6 bytes buffer for saving data read from the device
char str[512];            //string buffer to transform data before sending it to the serial port

int regAddress = 0x32;
int xa = 0, ya = 0, za = 0;
int xb = 0, yb = 0, zb = 0;

//Acelerometro2Axis
const int xpin = A1;   // Cada coordenada va referida a un pin de la placa
const int ypin = A0;
int xc = 0, yc = 0;

//IR
const int IR_PIN = A2;    //Mirar si es la entrada analógica adecuada!!!!!
int IR = 0;

//Switches
#define RESET_PIN 3
int S1 = 0, S2 = 0, S3 = 0, S4 = 0;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);
//  Serial.println( "Lectura de datos en el siguiente formato" );
  Serial.println( "xa ya za xb yb zb xc yc S1 S2 S3 S4 IR" );

 //Turning on the both ADXL345s
  writeTo(DEVICE_A, 0x2D, 24);   
  writeTo(DEVICE_B, 0x2D, 24);
  
 // Acelerometro 2Axis
 //analogReference(EXTERNAL);
//  pinMode(xpin, INPUT);
//  pinMode(ypin, INPUT);

 //Switches
 //configure pin2 as an input and enable the internal pull-up resistor
  pinMode( 4, INPUT_PULLUP );
  pinMode( 5, INPUT_PULLUP );
  pinMode( 6, INPUT_PULLUP );
  pinMode( 7, INPUT_PULLUP );
  pinMode( RESET_PIN, OUTPUT );

 //IR
 pinMode( IR_PIN, INPUT);

}

void loop() {
//Acelerometros 3Axis
 readFrom(DEVICE_A, regAddress, TO_READ, buff); //read the acceleration data from the ADXL345  
   //each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
   //thus we are converting both bytes in to one int
  xa = (((int)buff[1]) << 8) | buff[0];   
  ya = (((int)buff[3])<< 8) | buff[2];
  za = (((int)buff[5]) << 8) | buff[4];
  
  readFrom(DEVICE_B, regAddress, TO_READ, buff); //read the acceleration data from the second ADXL345
  xb = (((int)buff[1]) << 8) | buff[0];   
  yb = (((int)buff[3])<< 8) | buff[2];
  zb = (((int)buff[5]) << 8) | buff[4];

  sprintf(str, "%d\t %d\t %d\t %d\t %d\t %d\t", xa, ya, za, xb, yb, zb);  

//Acelerometro 2Axis
  xc = analogRead( xpin );
  yc = analogRead( ypin );

  //Switches
  //read the pushbutton value into a variable
  S1 = digitalRead( 4 );
  S2 = digitalRead( 5 );
  S3 = digitalRead( 6 );
  S4 = digitalRead( 7 );
  
  reset_switches();

 // IR
 IR = analogRead( IR_PIN );
 int distance = 2076 / ( IR - 11 );
// Serial.println( "xa ya za xb yb zb xc yc S1 S2 S3 S4 IR" );

 Serial.print( str );
 Serial.print( xc );  Serial.print( "\t" );
 Serial.print( yc );  Serial.print( "\t" );
 Serial.print( S1 );  Serial.print( "\t" );
 Serial.print( S2 );  Serial.print( "\t" );
 Serial.print( S3 );  Serial.print( "\t" );
 Serial.print( S4 );  Serial.print( "\t" );
 Serial.println( IR );
 
 delay( 250 );
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
  while(Wire.available())    //device may send less than requested (abnormal)
  { 
    buff[i] = Wire.read(); // receive a byte
    i++;
  }
  Wire.endTransmission(); //end transmission
}
