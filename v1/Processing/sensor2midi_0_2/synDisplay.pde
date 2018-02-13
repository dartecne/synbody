
import controlP5.*;

class SynDisplay implements ControlListener {
  ControlP5 cp5;
  DropdownList list_serial_ports; 
  SynInterface sensor;
  int init_x = 30; 
  int init_y = 30; 
  int sep = 60;
    
  SynDisplay() {    
  }
  PApplet parent;

  void update_serial_ports() {
    String[] ports = Serial.list();
    list_serial_ports.clear();
    for( int i = 0; i < ports.length; i++ ) {
      list_serial_ports.addItem( ports[i], i );
    }       
  }
  
  void create_GUI( PApplet parent, SynInterface s ) {
    this.parent = parent;
    this.sensor = s;
    int num_sliders = 7;

    cp5 = new ControlP5( parent );

//    ControlP5.printPublicMethodsFor(ControllerProperties.class);

    list_serial_ports = cp5.addDropdownList("serial_port")
     .setPosition(init_x + 6 * sep, init_y + 400 )
     .setSize(200, 200)
     .addListener( this )  
     ;
     list_serial_ports.captionLabel().set( "Serial Ports" );
     list_serial_ports.addItem("none", 0);
 
    create_acc_GUI( 1, "x", init_x, init_y );
    create_acc_GUI( 1, "y", init_x + 6 * sep, init_y );
    create_acc_GUI( 2, "x", init_x, init_y + 200 );
    create_acc_GUI( 2, "y", init_x + 6 * sep, init_y + 200 );

    create_IR_GUI( init_x, init_y + 400 );

    create_sw_GUI( init_x, init_y + 620 );
    cp5.addButton("b3")
       .setValue( 0 )
       .setPosition( init_x + 4 * sep, init_y + 620 )
       .setSize(80, 12)
       .setCaptionLabel("save default")
       .addListener( this );
    cp5.addButton("b4")
       .setValue( 0 )
       .setPosition( init_x + 4 * sep + 81, init_y + 620 )
       .setSize(80, 12)
       .setCaptionLabel("load default")
       .addListener( this )
       .setColorBackground(color(0, 100, 50));

    cp5.loadProperties("default.ser");
  }
  
  void create_IR_GUI( int init_x, int init_y ) {
    cp5.addSlider( "s_ir" )
       .setPosition( init_x, init_y )
       .setSize( 20, 100 )
       .setRange( sensor.range_min, sensor.range_max )
       .setCaptionLabel( "IR" )
       ;
    cp5.addSlider( "s_ir_min" )
       .setPosition( init_x+2*sep, init_y )
       .setSize( 20, 100 )
       .setRange( sensor.range_min, sensor.range_max )
       .setCaptionLabel( "set min" )
       ;
    cp5.addSlider( "s_ir_max" )
       .setPosition( init_x+3*sep, init_y )
       .setSize( 20, 100 )
       .setRange( sensor.range_min, sensor.range_max )
       .setCaptionLabel( "set max" )
       ;
    cp5.addNumberbox( "min_ir" )
       .setPosition( init_x+2*sep, init_y + 120 )
       .setSize( 30, 14 )
       .setCaptionLabel( "min" )
       ;
    cp5.addNumberbox( "max_ir"  )
       .setPosition( init_x+3*sep, init_y + 120 )
       .setSize( 30, 14 )
       .setCaptionLabel( "max" )
       ;
    cp5.addBang( "reset_min_ir" )
       .setPosition( init_x+2*sep, init_y + 150 )
       .setSize( 15, 15 )
       .setCaptionLabel( "reset" ) 
       .addListener( this )  
       ;
    cp5.addBang( "reset_max_ir" )
       .setPosition( init_x+3*sep, init_y + 150 )
       .setSize( 15, 15 )
       .setCaptionLabel( "reset" ) 
       .addListener( this )  
       ;
    cp5.addSlider( "s_ir_n" )
       .setPosition( init_x+4*sep, init_y )
       .setSize( 20, 100 )
       .setRange( sensor.cc_min, sensor.cc_max )
       .setCaptionLabel( "norm" )
       ;
  }
  
  void create_sw_GUI( int init_x, int init_y ){
    for( int i = 0; i < 4; i++ ) {
      cp5.addToggle( "SW_" + (i+1) )
       .setPosition( init_x+i*sep, init_y )
       .setSize( 30, 30 )
       .setValue( false )
       ;
    }     
  }
  
  void create_acc_GUI( int id, String dim, int init_x, int init_y ) {
    cp5.addSlider( "s_" + id + "_" + dim )
       .setPosition( init_x, init_y )
       .setSize( 20, 100 )
       .setRange( sensor.range_min, sensor.range_max )
       .setCaptionLabel( dim )
       ;
    //cp5.getController("s_1_x").getValueLabel().align(ControlP5.LEFT, ControlP5.BOTTOM_OUTSIDE).setPaddingX(0);
    cp5.addSlider( "s_" + id + "_" + dim + "_mean" )
       .setPosition( init_x+sep, init_y )
       .setSize( 20, 100 )
       .setRange( sensor.range_min, sensor.range_max )
       .setCaptionLabel( "mean " + dim )
       ;       
    cp5.addSlider( "s_" + id + "_" + dim + "_min" )
       .setPosition( init_x+2*sep, init_y )
       .setSize( 20, 100 )
       .setRange( sensor.range_min, sensor.range_max )
       .setCaptionLabel( "set min" )
       ;
    cp5.addSlider( "s_" + id + "_" + dim + "_max" )
       .setPosition( init_x+3*sep, init_y )
       .setSize( 20, 100 )
       .setRange( sensor.range_min, sensor.range_max )
       .setCaptionLabel( "set max" )
       ;

    cp5.addNumberbox( "min_" + id + "_" + dim )
       .setPosition( init_x+2*sep, init_y + 120 )
       .setSize( 30, 14 )
       .setCaptionLabel( "min" )
       ;
    cp5.addNumberbox( "max_" + id + "_" + dim )
       .setPosition( init_x+3*sep, init_y + 120 )
       .setSize( 30, 14 )
       .setCaptionLabel( "max" )
       ;
    cp5.addBang( "reset_min" + id + "_" + dim )
       .setPosition( init_x+2*sep, init_y + 150 )
       .setSize( 15, 15 )
       .setCaptionLabel( "reset" ) 
       .addListener( this )  
       ;
    cp5.addBang( "reset_max" + id + "_" + dim )
       .setPosition( init_x+3*sep, init_y + 150 )
       .setSize( 15, 15 )
       .setCaptionLabel( "reset" ) 
       .addListener( this )  
       ;
    cp5.addSlider( "s_" + id + "_" + dim + "_n" )
       .setPosition( init_x+4*sep, init_y )
       .setSize( 20, 100 )
       .setRange( sensor.cc_min, sensor.cc_max )
       .setCaptionLabel( dim + " norm" )
       ;
    }
    
  public void controlEvent( ControlEvent e ) {
    if(e.isController()) {
      println( "GUI from controller: " + e.getController().getName() );
      if( e.getController().getName().equals( "reset_min_1_x" ) ) {
          sensor.acc1.reset_min_x();
      }
      else if( e.getController().getName().equals( "reset_max_1_x" ) ) {
        sensor.acc1.reset_max_x();
      }
      else if( e.getController().getName().equals( "reset_min_2_x" ) ) {
        sensor.acc2.reset_min_x();
      }
      else if( e.getController().getName().equals( "reset_max_2_x" ) ) {
        sensor.acc2.reset_max_x();
      }
      else if( e.getController().getName().equals( "reset_max_1_y" ) ) {
        sensor.acc1.reset_max_y();
      }
      else if( e.getController().getName().equals( "reset_min_2_y" ) ) {
        sensor.acc2.reset_min_y();
      }
      else if( e.getController().getName().equals( "reset_max_2_y" ) ) {
        sensor.acc2.reset_max_y();
      }
      else if( e.getController().getName().equals( "reset_max_2_y" ) ) {
        sensor.acc2.reset_max_y();
      }
      else if( e.getController().getName().equals( "reset_min_ir" ) ) {
        sensor.ir.reset_min();
      }
      else if( e.getController().getName().equals( "reset_max_ir" ) ) {
        sensor.ir.reset_max();
      }
      else if( e.getController().getName().equals( "b3" ) ) {
        println( "save" );
        cp5.saveProperties("default.ser");
      }
      else if( e.getController().getName().equals( "b4" ) ) {
        println( "load" );
        cp5.loadProperties("default.ser");
      }
    } else if( e.isGroup() ) {
      println( "GUI group value: " + e.getGroup().getValue() );
      if( e.getGroup().getName().equals( "serial_port" ) ) {
        update_serial_ports();
        int port_num = int(e.getGroup().getValue());
        println( "User wants to connect to port: " + port_num );
        if( syn_interface.connect( parent, port_num, 9600 ) < 0 ) {
          println( "ERROR trying to connect to port" );
        } else {
          println( "CONNECTED" );
        }
      }
    }
    else {
      println( "WARNING: an unknown GUI event happened" );
    }   
  }
    
  void draw_graphics() {
    pg.beginDraw();
    pg.background( 0 );
    pg.noStroke();
    pg.fill(204, 20);
    
    //draw first rectangle
    pg.pushMatrix();
    pg.translate(200, 200);
    // 280, 450; maximos y minimos del sensor
    float x1 = sensor.acc1.x_mean;
    x1 = 2*PI*(sensor.acc1.x_max - x1)/sensor.acc1.x_max;
    pg.rotateX( x1);
//    rotateY();
    pg.rect( -100, -100, 100, 100 );  
    pg.popMatrix();
    pg.endDraw();
    image(pg, 0, 0);
  }
  
  void update_acc() {
    cp5.getController( "s_1_x" ).setValue( sensor.acc1.getX() );
    cp5.getController( "s_1_x_mean" ).setValue( sensor.acc1.x_mean );
    cp5.getController( "min_1_x" ).setValue( sensor.acc1.x_min );
    cp5.getController( "max_1_x" ).setValue( sensor.acc1.x_max );
    sensor.acc1.normalize_x( int(cp5.getController( "s_1_x_min" ).getValue()),
                             int(cp5.getController( "s_1_x_max" ).getValue()),
                              sensor.cc_min,
                              sensor.cc_max );
    cp5.getController( "s_1_x_n" ).setValue( sensor.acc1.x_n );

    cp5.getController( "s_1_y" ).setValue( sensor.acc1.getY() );
    cp5.getController( "s_1_y_mean" ).setValue( sensor.acc1.y_mean );
    cp5.getController( "min_1_y" ).setValue( sensor.acc1.y_min );
    cp5.getController( "max_1_y" ).setValue( sensor.acc1.y_max );
    sensor.acc1.normalize_y( int(cp5.getController( "s_1_y_min" ).getValue()),
                             int(cp5.getController( "s_1_y_max" ).getValue()),
                              sensor.cc_min,
                              sensor.cc_max );
    cp5.getController( "s_1_y_n" ).setValue( sensor.acc1.y_n );

    cp5.getController( "s_2_x" ).setValue( sensor.acc2.getX() );
    cp5.getController( "s_2_x_mean" ).setValue( sensor.acc2.x_mean );
    cp5.getController( "min_2_x" ).setValue( sensor.acc2.x_min );
    cp5.getController( "max_2_x" ).setValue( sensor.acc2.x_max );
    sensor.acc2.normalize_x( int(cp5.getController( "s_2_x_min" ).getValue()),
                             int(cp5.getController( "s_2_x_max" ).getValue()),
                              sensor.cc_min,
                              sensor.cc_max );
    cp5.getController( "s_2_x_n" ).setValue( sensor.acc2.x_n );

    cp5.getController( "s_2_y" ).setValue( sensor.acc2.getY() );
    cp5.getController( "s_2_y_mean" ).setValue( sensor.acc2.y_mean );
    cp5.getController( "min_2_y" ).setValue( sensor.acc2.y_min );
    cp5.getController( "max_2_y" ).setValue( sensor.acc2.y_max );
    sensor.acc2.normalize_y( int(cp5.getController( "s_2_y_min" ).getValue()),
                             int(cp5.getController( "s_2_y_max" ).getValue()),
                              sensor.cc_min,
                              sensor.cc_max );
    cp5.getController( "s_2_y_n" ).setValue( sensor.acc2.y_n );
  }
  
  void update_sw() {
    cp5.getController( "SW_1" ).setValue(sensor.sw & S_1);
    cp5.getController( "SW_2" ).setValue(sensor.sw & S_2);
    cp5.getController( "SW_3" ).setValue(sensor.sw & S_3);
    cp5.getController( "SW_4" ).setValue(sensor.sw & S_4);
  }
  
  void update_ir() {
    cp5.getController( "s_ir" ).setValue( sensor.ir.getValue() );
    cp5.getController( "min_ir" ).setValue( sensor.ir.d_min );
    cp5.getController( "max_ir" ).setValue( sensor.ir.d_max );
    sensor.ir.d_min = int(cp5.getController( "s_ir_min" ).getValue());
    sensor.ir.d_max = int(cp5.getController( "s_ir_max" ).getValue());
    sensor.ir.normalize( int(cp5.getController( "s_ir_min" ).getValue()),
                             int(cp5.getController( "s_ir_max" ).getValue()),
                              sensor.cc_min,
                              sensor.cc_max );
    cp5.getController( "s_ir_n" ).setValue( sensor.ir.d_n );
  }
  
  void update() {
    //draw_graphics();
    update_acc();
    update_sw();
    update_ir();
  }
  
}
