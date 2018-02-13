
class IR {
  int sample_size = 10; // number of readings for setting the mean value
  int[] d;
  int d_mean = 0;
  int d_min, d_max;
  int d_n = 0;
  int i = 0;
  boolean is_inside, is_inside_old; // they specify if the obstacle is inside d_min, d_max or outside
                            // true - inside; false - outside;
  int zone_change = 0; // specifies if obstacle went from inside to outside or vice-versa
  IR() {
    d = new int[sample_size]; 
    reset();
  }
  
  int getValue() {
    return d[i];
  }
  
  void setValue( int v ) {
    d[i] = v;
  }
  
  int mean( int[] v ) {
    long m = 0;
    for( int j = 0; j < sample_size; j++ ) {
      m += v[j];
    }
    return (int)(m / sample_size);
  }

  void callibrate() {    
    d_mean = mean( d ); 
    if( d_max < d_mean ) d_max = d_mean;
    if( d_min > d_mean ) d_min = d_mean;
  }
  
  void reset_min() {
    d_min = 1023;
  }

  void reset_max() {
    d_max = 0;
  }
  
  void reset() {
    d_max = d_max = 0;
    d_min = d_min = 1023;
  }
  
  int update() {
    is_inside_old = is_inside;
    zone_change = 0;
    if( getValue() < d_max ) {
      is_inside = true;        // the obstacle is inside the range d_max  
      if( is_inside_old == false ) zone_change = -1; // obstacle has gone from outside to inside
    } else if( getValue() > d_max ) {
      is_inside = false;  //
      if( is_inside_old == true ) zone_change = 1;
    }
    return zone_change;  
  }
  
  void normalize( int input_min, int input_max, int mini, int maxi ) {
    d_n = (int)constrain( map( d[i], input_min, input_max, mini, maxi ), mini, maxi );
  } 
}
