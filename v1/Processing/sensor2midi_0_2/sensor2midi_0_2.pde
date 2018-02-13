/**

*/
SynInterface syn_interface = new SynInterface();
SynDisplay syn_display = new SynDisplay();
SynControl syn_control = new SynControl();

PGraphics pg;

void setup() {
  size( 800, 700 );
  pg = createGraphics( 90, 90, P3D );

  if( syn_control.init_midi_bus( this ) < 0 ) {
    exit();
  }
  /*
while(true){
//  syn_control.midi_bus.sendNoteOn( 0, 69, 127 );
  syn_control.midi_bus.sendControllerChange( 0, 2, 10 );
  delay( 50 );
//  syn_control.midi_bus.sendNoteOff( 0, 69, 127 );
  syn_control.midi_bus.sendControllerChange( 0, 2, 120 );
  delay( 50 );
}  */
  syn_display.create_GUI( this, syn_interface );
  syn_control.assign_interface( syn_interface );
  syn_control.init();
  
  smooth();
}

void draw() {
  background( 0 );
  stroke( 255 );
  syn_interface.read_sensors();
  
  syn_interface.callibrate();

  syn_display.update();
  
  syn_control.sensor2midi(); 
}
