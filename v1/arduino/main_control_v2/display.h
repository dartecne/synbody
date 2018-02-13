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

