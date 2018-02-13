
import themidibus.*;

class SynControl {
  int state = 0;
   MidiBus midi_bus;  
   SynInterface sensor;
   int channel = 0;
   int[] notes_audio1 = new int[2];
   int[] notes_audio2 = new int[3];
   int[] notes_roll = new int[10];
   int[] notes_roll_percu = new int[7];
   int note_index = 0;
   int note_index_percu = 0;
   int index_audio1 = 0;
   int index_audio2 = 0;
   float volumen = 127;
   SynControl() {
     MidiBus.list(); // List all available Midi devices on STDOUT. This will show each device's index and name.

     notes_roll[0] = 49;
     notes_roll[1] = 51;
     notes_roll[2] = 54;
     notes_roll[3] = 56;
     notes_roll[4] = 58;
     notes_roll[5] = 61;
     notes_roll[6] = 63;
     notes_roll[7] = 66;
     notes_roll[8] = 68;
     notes_roll[9] = 70;
     notes_audio1[0] = 69;
     notes_audio1[1] = 67;
     notes_audio2[0] = 60;
     notes_audio2[1] = 62;
     notes_audio2[2] = 71;

     notes_roll_percu[0] = 73;
     notes_roll_percu[1] = 75;
     notes_roll_percu[2] = 78;
     notes_roll_percu[3] = 80;
     notes_roll_percu[4] = 82;
     notes_roll_percu[5] = 85;
     notes_roll_percu[6] = 87;

   }

  int init_midi_bus(PApplet parent) {
//    midi_bus = new MidiBus( parent, -1, "Java Sound Synthesizer" );
//    midi_bus = new MidiBus( parent, -1, "loopMIDI Port" );
    midi_bus = new MidiBus( parent, -1, "Bus 1" );
    return 0;
  }   

  void assign_interface( SynInterface s ) {
    sensor = s;
  }  
  
  void init() {
    midi_bus.sendNoteOn( channel, 48, 127 );
  }
  
  void sensor2midi() {
    send_notes_from_switches();
    send_cc_from_acc();
    send_from_ir();
    volumen = volumen - 0.5;
  }
  
  void send_from_ir() {
    int zone_change = sensor.ir.update();
    if( zone_change == 1  )
      midi_bus.sendNoteOn( channel, 64, 127);
    else if( zone_change == -1  )
      midi_bus.sendNoteOff( channel, 64, 127);
//    if( sensor.ir.is_inside )
      midi_bus.sendControllerChange( channel, 5, sensor.ir.d_n );
  }

  void send_cc_from_acc() {
    int THRES = 120;
    midi_bus.sendControllerChange( channel, 1, sensor.acc1.x_n );
    midi_bus.sendControllerChange( channel, 2, sensor.acc1.y_n );
    midi_bus.sendControllerChange( channel, 3, int(volumen) );

    if( (sensor.acc1.getDx() > THRES) | (sensor.acc1.getDy() > THRES) ){
      println( "thres in acc1 X Y" );
      volumen = 127;
      midi_bus.sendNoteOn( channel, 72, 127);
      delay(200);
    }
    if(note_index > 9) note_index = 0;
    if(note_index_percu > 6) note_index_percu = 0;
    
    midi_bus.sendControllerChange( channel, 6, sensor.acc2.x_n );
    midi_bus.sendControllerChange( channel, 7, sensor.acc2.y_n );
    delay(10);
  }
  
  void send_notes_from_switches() {
//    bus.sendNoteOn(ch, pitch, vel);
    if( boolean(sensor.sw_up & S_1) ) {
//      println( "Sensor 1 !" );
      if( index_audio1 > 1 ) index_audio1 = 0; 
      midi_bus.sendNoteOn( channel, notes_audio1[index_audio1], 127);
      index_audio1++;
    } 
    if( boolean(sensor.sw_up & S_2) ) {
//      println( "Sensor 2 !" );
      if( index_audio2 > 2 ) index_audio2 = 0; 
      midi_bus.sendNoteOn( channel, notes_audio2[index_audio2], 127);
      index_audio2++;
    } 
    if( boolean(sensor.sw_up & S_3) ) {
//      println( "Sensor 3 !" );
      midi_bus.sendNoteOn( channel, notes_roll[note_index], 127);
      midi_bus.sendNoteOn( channel, 50, 127);
      note_index++;
    } 
    if( boolean(sensor.sw_up & S_4) ) {
//      println( "Sensor 4 !" );
      midi_bus.sendNoteOn( channel, notes_roll_percu[note_index_percu], 127);
      note_index_percu++;
    } 
  }
}


	// Either you can
	//                   Parent In Out
	//                     |    |  |
	//myBus = new MidiBus(this, 0, 1); // Create a new MidiBus using the device index to select the Midi input and output devices respectively.

	// or you can ...
	//                   Parent         In                   Out
	//                     |            |                     |
	//myBus = new MidiBus(this, "IncomingDeviceName", "OutgoingDeviceName"); // Create a new MidiBus using the device names to select the Midi input and output devices respectively.

	// or for testing you could ...
	//                 Parent  In        Out
	//                   |     |          |
