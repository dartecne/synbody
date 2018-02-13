extern int xa, ya, Dxa, Dya, xb, yb;
extern int xa_min, xa_max, ya_min, ya_max, Dxa, Dya, xb_min, xb_max, yb_min, yb_max;

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

