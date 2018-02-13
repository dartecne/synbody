extern int xa, ya, Dxa, Dya, Dxb, Dyb, xb, yb;
extern int xa_min, xa_max, ya_min, ya_max, xb_min, xb_max, yb_min, yb_max;
extern int IR;
extern int S1, S2, S3, S4, SW, SW_up, SW_down;

void print_header() {
  Serial.println( "Read Sensors and Print..." );
  Serial.println( "xa   ya   Dxa   Dya   xb   yb   Dxb   Dyb  IR  SW1  SW2  SW3  SW4  SW1_up SW2_up  SW3_up  SW4_up  SW1_down  SW2_down  SW3_down  SW4_down" );
  Serial.println( "----------------------------------------------------------------------------------------------------------------------------------------" );
}

void send_int( int val ) {
  Serial.write( 0xff );
  Serial.write( (val >> 8) & 0xff );
  Serial.write( val & 0xff );
}

void send_string( int val ) {
  Serial.print( val, DEC );
  Serial.print(",");
//  Serial.write( '\n0' );
}

void test_send_values() {
  for( int i = 0; i < 12; i++ ) {
    send_string( i );  
  }
}

void send_sensor_values() {
  send_string( xa );
  send_string( ya );
  send_string( Dxa );
  send_string( Dya );
  send_string( xb );
  send_string( yb );
  send_string( Dxb );
  send_string( Dyb );
  send_string( IR );
  send_string( SW ); // byte of switches
  send_string( SW_up ); // byte of switches up
  Serial.println( SW_down, DEC ); // byte of switches down
}

void print_sensor_values() {
/*  Serial.print( xa );  Serial.print( "   " );
  Serial.print( ya );  Serial.print( "   " );
  Serial.print( Dxa ); Serial.print( "   " );
  Serial.print( Dya ); Serial.print( "   " );
  Serial.print( xb );  Serial.print( "\t" );
  Serial.print( yb );  Serial.print( "\t" );
  Serial.print( Dxb ); Serial.print( "   " );
  Serial.print( Dyb ); Serial.print( "   " );
  */
  Serial.print( S1 ); Serial.print( "   " );
  Serial.print( S2 ); Serial.print( "   " );
  Serial.print( S3 ); Serial.print( "   " );
  Serial.print( S4 ); Serial.print( "   " );
  Serial.print( SW, BIN ); Serial.print( "   " );
  Serial.print( SW_up, BIN ); Serial.print( "   " );
  Serial.print( SW_down, BIN ); Serial.print( "   " );

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


