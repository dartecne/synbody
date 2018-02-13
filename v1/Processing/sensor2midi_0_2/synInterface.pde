
import processing.serial.*;

int S_1 = 0x0001; // 1110
int S_2 = 0x0002; // 1101
int S_3 = 0x0004; // 1011
int S_4 = 0x0008; // 0111

class SynInterface {
  Acc acc1, acc2;
  IR ir;  // Infrared sensor
  int sensors[]; // raw values read from serial port
  int sw, sw_up, sw_down;  // switches in a byte
  boolean connected = false;
  int range_min = 0;   // range that arduino gives
//  int range_max = 512; //
  int range_max = 1024; // 
  int cc_min = 0;  // range for CC MIDI message attribute
  int cc_max = 127;
  
  Serial port;
  int com_port, baud;
  
  SynInterface() {
    acc1 = new Acc();
    acc2 = new Acc();
    ir = new IR();
  }
    
  int connect( PApplet parent, int cp, int baud ) {
      com_port = cp;
      if( connected) {
        println( "Port already connected" );
        return 0;
      }
      if( Serial.list().length == 0 ) {
        println( "No COM port abailable!!!" );
        println( "Exiting" );
        return -1;      
      }
      print( "Serial ports: \n" );
      String[] ports = Serial.list();
      println( ports );
//      println( "puerto 1 = " + ports[0] );
      println( "Connecting to port " + Serial.list()[com_port] + " at " + baud );
      port = new Serial( parent, Serial.list()[com_port], baud );    // Com port specified here 
      port.bufferUntil( 10 );
      connected = true;
      return 0;
  }
 
  /** Lee valores de todos los sensores en el siguiente formato:
  xa   ya   Dxa   Dya  xb   yb   Dxb   Dyb  IR  SW  SW_up  SW_down*/
  void read_sensors() {
    if( get_serial_data() < 12) return;
    acc1.setXY( sensors[0], sensors[1] ); 
    acc1.setDXY( sensors[2], sensors[3] ); 
    acc2.setXY( sensors[4], sensors[5] ); 
    acc2.setDXY( sensors[6], sensors[7] ); 
    ir.setValue( sensors[8] );
    sw = sensors[9];
    sw_up = sensors[10];  
    sw_down = sensors[11];
  }  
  
  void callibrate() {
    acc1.callibrate();
    acc2.callibrate();
    ir.callibrate();
  }
  
/*  void normalize( int maxi, int mini ) {
    acc1.normalize( maxi, mini );
    acc2.normalize( maxi, mini );
    ir.normalize( maxi, mini );
  }
  */ 
  int get_serial_data() {
    if( !connected ) return -2;
    String my_string = "null";
    if( port.available() > 0 ) {
      my_string = port.readStringUntil( 10 );
//      println( my_string );
      if( my_string != null ) {
        my_string = trim( my_string );
//        println( split(my_string, ',') );
        sensors = int( split( my_string, ',' ));
        return( sensors.length ); 
      }      
    }
    return -1;
  }
}


