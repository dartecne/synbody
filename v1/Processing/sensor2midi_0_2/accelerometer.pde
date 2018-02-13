/**
  ADXL 3xx - acc of two axis 
  Measurement values with no movement and movement
  x-axis (no movement)        x-axis (movement)
  0º - 506                      0º - 506
  90º - 443 (minimum value)    90º - 439
  180º - 506                  180º - 506
  270º - 571 (maximum value)  270º - 720
*/

class Acc {
  int sample_size = 10; // number of readings for setting the mean value
  int[] x, y; 
  int[] Dx, Dy; 
  int x_mean, y_mean = 0;
  int x_max, x_min, y_max, y_min;
  int x_n, y_n;
  int i = 0, j = 0;
  
  Acc() {
    x = new int[sample_size];
    y = new int[sample_size];
    Dx = new int[sample_size];
    Dy = new int[sample_size];
    reset();
  }
  
  int getX() { 
    return x[i];
  }
  
  int getY() { 
    return y[i];
  }

  int getDx() { 
    return Dx[j];
  }

  int getDy() { 
    return Dy[j];
  }
  
  void setXY( int lx, int ly ) { 
    x[i] = lx;
    y[i] = ly;
    i++; 
    if( i >= sample_size ) i = 0;
  }
  void setDXY( int lx, int ly ) { 
    Dx[j] = lx;
    Dy[j] = ly;
    j++; 
    if( j >= sample_size ) j = 0;
  }
  
  int mean( int[] v ) {
    long m = 0;
    for( int j = 0; j < sample_size; j++ ) {
      m += v[j];
    }
    return (int)(m / sample_size);
  }
    
  void callibrate() {    
    x_mean = mean( x ); 
    y_mean = mean( y ); 
    if( x_max < x_mean ) x_max = x_mean;
    if( y_max < y_mean ) y_max = y_mean;

    if( x_min > x_mean ) x_min = x_mean;
    if( y_min > y_mean ) y_min = y_mean;
  }

  void reset_min_x() {
    x_min = 1023;
  }

  void reset_min_y() {
    y_min = 1023;
  }

  void reset_min() {
    x_min = y_min = 1023;
  }

  void reset_max_x() {
    x_max = 0;
  }

  void reset_max_y() {
    y_max = 0;
  }
    
  void reset_max() {
    x_max = y_max = 0;
  }
  
  void reset() {
    reset_min();
    reset_max();
  }
  
  void normalize( int mini, int maxi ) {
    x_n = (int)constrain( map( x[i], x_min, x_max, mini, maxi ), mini, maxi );
    y_n = (int)constrain( map( y[i], y_min, y_max, mini, maxi ), mini, maxi );
  } 
  
  void normalize_x( int input_min, int input_max, int output_min, int output_max ) {
    x_n = (int)constrain( map( x_mean, input_min, input_max, output_min, output_max ), output_min, output_max );
  } 

  void normalize_y( int input_min, int input_max, int output_min, int output_max ) {
    y_n = (int)constrain( map( y_mean, input_min, input_max, output_min, output_max ), output_min, output_max );
  } 
  
}

