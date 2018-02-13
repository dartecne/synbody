void setup()
{
    Serial.begin( 115200 );          // Launch MIDI and listen to channel 4
}

void loop()
{
        noteOn(42,127);  // Send a Note (pitch 42, velo 127 on channel 1)
        delay(1000);		        // Wait for a second
        noteOn(42,0);   // Stop the note
        delay(1000);
}

//  plays a MIDI note.  Doesn't check to see that
//  cmd is greater than 127, or that data values are  less than 127:
void noteOn(int pitch, int velocity) {
  Serial.write( 0x90 );
  Serial.write(pitch);
  Serial.write(velocity);
}

