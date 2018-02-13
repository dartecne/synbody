/**
 * Lee los datos de los siguientes sensores:
 * - 2x acelerometros ADXL3xx de 2 ejes conectados a 
 *    A1 (x1) y A0 (y1) ( brazo )
 *    A4 (x2) y A3 (y2) ( abdomen )
 * - SHARP ID conectado a A2
 * - 4 Switches conectados a D4, D5, D6, y D7
 * 
 * Calibrando se obtienen los siguientes valores min y max:

xa_min = 288   ya_min = 294
xa_max = 383   ya_max = 395
xb_min = 293   yb_min = 300
xb_max = 443   yb_max = 398

IR:
60 -- 40cm
160 -- 20cm
350 -- 5 cm
600 -- 0cm
 */


#define TAU 250 // tiempo de bulce en ms

//Acelerometro2Axis
const int xapin = A1;   
const int yapin = A0;
const int xbpin = A4;  
const int ybpin = A3;

int xa = 0, ya = 0;
int xb = 0, yb = 0;

int xa_min = 288, ya_min = 294;
int xa_max = 383, ya_max = 395;
int xb_min = 293, yb_min = 300;
int xb_max = 443, yb_max = 398;

//IR
const int IR_PIN = A2;
int IR = 0;

//Switches
#define RESET_PIN 3
int S1 = 0, S2 = 0, S3 = 0, S4 = 0;

/*******************************************/

void setup() {
  Serial.begin(9600);
  Serial.println( "Lectura de datos en el siguiente formato" );
  Serial.println( "x1 y1 x1 y1 S1 S2 S3 S4 IR" );

 //Switches
 //configure pin2 as an input and enable the internal pull-up resistor
  pinMode( 4, INPUT_PULLUP );
  pinMode( 5, INPUT_PULLUP );
  pinMode( 6, INPUT_PULLUP );
  pinMode( 7, INPUT_PULLUP );
  pinMode( RESET_PIN, OUTPUT );

 //IR
 pinMode( IR_PIN, INPUT);

 //callibrate();
}

/*******************************************/

void loop() {
  read_acc_a();
  read_acc_b();
  read_switches();
  IR = analogRead( IR_PIN );
  normalize();  
  print_sensor_values();
  delay( TAU );
}

/*******************************************/

void normalize() {
  xa = map( xa, xa_min, xa_max, 0, 127 );
  ya = map( ya, ya_min, ya_max, 0, 127 );

  xb = map( xb, xb_min, xb_max, 0, 127 );
  yb = map( yb, yb_min, yb_max, 0, 127 );
}

void callibrate() {
  xa_max = -1024; xb_max = -1024;
  ya_max = -1024; yb_max = -1024;
  xa_min = 1024;  xb_min = 1024;
  ya_min = 1024;  yb_min = 1024;

  int tau = 1;
  Serial.println( "Callibrating..." );

  for ( int i = 0; i < 400 / tau; i++ ) {
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

    delay( tau );
  }
  Serial.println( "Callibration END:" );
  Serial.println( "***********************************" );
  Serial.print( "xa_min = " ); Serial.print( xa_min ); Serial.print( "   ya_min = " ); Serial.println(ya_min);
  Serial.print( "xa_max = " ); Serial.print( xa_max ); Serial.print( "   ya_max = " ); Serial.println(ya_max);
  Serial.print( "xb_min = " ); Serial.print( xb_min ); Serial.print( "   yb_min = " ); Serial.println(yb_min);
  Serial.print( "xb_max = " ); Serial.print( xb_max ); Serial.print( "   yb_max = " ); Serial.println(yb_max);
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


void read_switches() {
  //Switches
  //read the pushbutton value into a variable
  S1 = digitalRead( 4 );
  S2 = digitalRead( 5 );
  S3 = digitalRead( 6 );
  S4 = digitalRead( 7 );

  reset_switches();
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

void print_sensor_values() {
//  Serial.println( "xa   ya   xb   yb   S1 S2 S3 S4   IR" );
  Serial.print( xa );  Serial.print( "   " );
  Serial.print( ya );  Serial.print( "   " );
  Serial.print( xb );  Serial.print( "   " );
  Serial.print( yb );  Serial.print( "   " );
  Serial.print( S1 ); Serial.print( " " );
  Serial.print( S2 ); Serial.print( " " );
  Serial.print( S3 ); Serial.print( " " );
  Serial.print( S4 ); Serial.print( " " );
  Serial.println( IR );
}

void print_sensor_limits_values() {
//  Serial.println( "xa  xa_min xa_max ya  ya_min ya_max xb  xb_min xb_max yb  yb_min yb_max" );
  Serial.print( xa );  Serial.print( "   " );
  Serial.print( xa_min );  Serial.print( "   " );
  Serial.print( xa_max );  Serial.print( "   " );
  Serial.print( ya );  Serial.print( "   " );
  Serial.print( ya_min );  Serial.print( "   " );
  Serial.print( ya_max );  Serial.print( "   " );

  Serial.print( xb );  Serial.print( "   " );
  Serial.print( xb_min );  Serial.print( "   " );
  Serial.print( xb_max );  Serial.print( "   " );
  Serial.print( yb );  Serial.print( "   " );
  Serial.print( yb_min );  Serial.print( "   " );
  Serial.print( yb_max );  Serial.print( "   " );
  Serial.println();
}


