#include <Wire.h>
//#include <SharpIR.h>

#define TAU 250
//Acelerometros 3Axis
#define DEVICE (0x53)    //second ADXL345 device address muñequera
#define TO_READ (6)        //num of bytes we are going to read each time (two bytes for each axis)

byte buff[TO_READ] ;      //6 bytes buffer for saving data read from the device
char str[512];            //string buffer to transform data before sending it to the serial port

int regAddress = 0x32;
int xa = 0, ya = 0, za = 0;
int xa_max = -1024, ya_max = -1024, za_max = -1024;
int xa_min = 1024, ya_min = 1024, za_min = 1024;

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
  Serial.begin(9600);
//  Serial.println( "Lectura de datos en el siguiente formato" );
  Serial.println( "xa ya za xc yc S1 S2 S3 S4 IR" );

  writeTo(DEVICE, 0x2D, 24);   
  
 //Switches
 //configure pin2 as an input and enable the internal pull-up resistor
  pinMode( 4, INPUT_PULLUP );
  pinMode( 5, INPUT_PULLUP );
  pinMode( 6, INPUT_PULLUP );
  pinMode( 7, INPUT_PULLUP );
  pinMode( RESET_PIN, OUTPUT );
  callibrate();
}

void loop() {
  read_acc_a();  
  read_acc_c();
  normalize();

  read_switches();

 // IR
 IR = analogRead( IR_PIN );

 print_sensor_values();
 
 delay( TAU );
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
  xa = map( xa, xa_min, xa_max, -1000, 1000 );
  ya = map( ya, ya_min, ya_max, -1000, 1000 );
  za = map( za, za_min, za_max, -1000, 1000 );

  xc = map( xc, xc_min, xc_max, -1000, 1000 );
  yc = map( yc, yc_min, yc_max, -1000, 1000 );
}

void callibrate() {
  Serial.println( "Callibration..." );
  Serial.println( "xa, xa_min, xa_max, ya, ya_min, ya_max, za, za_min, za_max, xc, xc_min, xc_max, yc, yc_min, yc_max" );
  
  for( int i = 0; i < 150; i++ ) {
    read_acc_mean_value( 10 );
    if( xa_max < xa ) xa_max = xa;
    if( ya_max < ya ) ya_max = ya;
    if( za_max < za ) za_max = za;

    if( xa_min > xa ) xa_min = xa;
    if( ya_min > ya ) ya_min = ya;
    if( za_min > za ) za_min = za;

    if( xc_max < xc ) xc_max = xc;
    if( yc_max < yc ) yc_max = yc;

    if( xc_min > xc ) xc_min = xc;
    if( yc_min > yc ) yc_min = yc;

    print_sensor_limits_values();
    
    delay(100);    
  }

  Serial.println( "Callibration OK!" );
  Serial.println( "xa_min, xa_max, ya_min, ya_max, za_min, za_max, xc_min, xc_max" );
  // -305, 300, -387, 251, 511, 511
  
  Serial.print( xa_min ); Serial.print( "\t" ); 
  Serial.print( xa_max ); Serial.print( "\t" ); 
  Serial.print( ya_min ); Serial.print( "\t" ); 
  Serial.print( ya_max ); Serial.print( "\t" ); 
  Serial.print( za_min ); Serial.print( "\t" ); 
  Serial.print( za_max ); Serial.print( "\t" );
  Serial.print( xc_min ); Serial.print( "\t" ); 
  Serial.print( xc_max ); Serial.println( "\t" ); 
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

  xa = xa_mean/sample_size;
  ya = ya_mean/sample_size;
  za = za_mean/sample_size;
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
  ya = (((int)buff[3])<< 8) | buff[2];
  za = (((int)buff[5]) << 8) | buff[4];
  
/*  sprintf( xa, "%d", xa );  
  sprintf( ya, "%d", ya );  
  sprintf( za, "%d", za );  
//  sprintf(str, "%d\t %d\t %d\t", xa, ya, za);  
*/
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
