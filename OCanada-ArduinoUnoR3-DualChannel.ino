/*

    CCPS 310 - Winter 2020 - Lab 6 - Version 2
    Sohrab Soltani, Student #500801172

    Version Two: O'Canada, two melody lines;
    Main melody on pin 9      (buzzer 1)
    Accompaniment on pin 11   (buzzer 2)

    Melody is encoded by Sohrab Soltani
    Based on the score downloaded from: http://cantorion.org/music/3873/O-Canada-French-lyrics

*/

# include "pitches.h"
#define BUZZER_CLOCK 16000000/8

void timer_init(void) {

   asm(" sbi 0x04, 1 "); // Use PWM pin 9 for main melody

   TCCR1A = (1 << COM1A1) | (1 << WGM11);
   TCCR1B = (1 << WGM13)  | (1 << WGM12)  | (1 << CS11);


   asm(" sbi 0x04, 3 "); // Use PWM pin 11 Port b3 for accompaniment

   // Second PWM implementation inspired by https://www.arduino.cc/en/Tutorial/SecretsOfArduinoPWM
   TCCR2A =  (1 << COM2A0) | (1 << WGM20); //_BV(COM2A0) | _BV(COM2B1) | _BV(WGM20);
   TCCR2B =  (1 << WGM22)  | (1 << CS22); //_BV(WGM22) | _BV(CS22);

}

void PlayTones(uint16_t freq, uint16_t freq2)
{

  if (freq != 0 )
  {
   asm(" sbi 0x04, 1 ");

   ICR1 = BUZZER_CLOCK/freq;
   OCR1A = ICR1*3/4;
  }
  else
  {
     asm(" cbi 0x04, 1 ");
     asm(" cbi 0x05, 1 ");
  }

  if (freq2 != 0 )
  {
     asm(" sbi 0x04, 3 ");

    OCR2A = BUZZER_CLOCK/64/(freq2)/2/2;
  }
  else
  {
     asm(" cbi 0x04, 3 ");
     asm(" cbi 0x05, 3 ");
  }

}

void setup() {
  timer_init();
}

void mute()
{
   asm(" cbi 0x04, 1 ");
   asm(" cbi 0x05, 1 ");

   asm(" cbi 0x04, 3 ");
   asm(" cbi 0x05, 3 ");
}

void loop()
{

  uint16_t melody_line1[] = {
     NOTE_G5,  NOTE_AS5,        0,   NOTE_AS5,  // 1st Sentence - 1st bar
    NOTE_DS5,  NOTE_DS5,  NOTE_DS5,   NOTE_F5,  // 1st Sentence - 2nd bar
     NOTE_G5,  NOTE_GS5,  NOTE_AS5,   NOTE_C6,  // 1st Sentence - 3rd bar
     NOTE_F5,   NOTE_F5,                        // 1st Sentence - 4th bar


    // 14 note buckets in this sentence
    // 14 note buckets So far



    NOTE_G5,    NOTE_G5,   NOTE_A5,   NOTE_A5,  0,   NOTE_A5,                                   // 2nd Sentence - 1st bar
    NOTE_AS5,  NOTE_AS5,  NOTE_AS5,   NOTE_C6,                                                  // 2nd Sentence - 2nd bar
    NOTE_D6,          0,   NOTE_D6,         0,       NOTE_C6,          0,    NOTE_C6,  0,       // 2nd Sentence - 3rd bar
    NOTE_AS5,  NOTE_AS5,  NOTE_AS5,   NOTE_F5,       NOTE_G5,                                   // 2nd - 3rd Sentence - 4th bar (1st bar)

    // 23 note buckets in this sentence*
    // 37 note buckets So far


    NOTE_GS5, NOTE_GS5,  NOTE_GS5,   NOTE_G5,   NOTE_F5,       NOTE_G5,   NOTE_GS5,                   // 3rd Sentence - 1st bar*
    NOTE_AS5, NOTE_AS5,  NOTE_AS5,  NOTE_GS5,   NOTE_G5,      NOTE_GS5,   NOTE_AS5,                   // 3rd Sentence - 2nd bar*
     NOTE_C6,        0,  NOTE_AS5,         0,  NOTE_GS5,      NOTE_GS5,          0,    NOTE_G5,    0, // 3rd Sentence - 3rd bar *
     NOTE_F5,        0,   NOTE_F5,   NOTE_G5,                                                         // 3rd Sentence - 4th bar *

    // 27 note buckets in this sentence*
    // 64 note buckets So far

    NOTE_GS5,  NOTE_G5,   NOTE_F5,   NOTE_G5,  NOTE_GS5,                         // 4th Sentence - 1st bar *
    NOTE_AS5, NOTE_AS5,  NOTE_GS5,   NOTE_G5,         0,   NOTE_G5,              // 4th Sentence - 2nd bar *
     NOTE_F5, NOTE_AS5,         0,  NOTE_AS5,   NOTE_A5,   NOTE_G5,    NOTE_A5,  // 4th Sentence - 3rd bar *
    NOTE_AS5, NOTE_AS5,  NOTE_AS5,  NOTE_AS5,                                    // 4th Sentence - 4th bar *


    // 22 note buckets in this sentence*
    // 86 note buckets So far


     NOTE_G5,      NOTE_AS5,          0,       NOTE_AS5,                          // 5th Sentence - 1st bar
    NOTE_DS5,            0,           0,                                          // 5th Sentence - 2nd bar
    NOTE_GS5,      NOTE_C6,           0,        NOTE_C6,                          // 5th Sentence - 3rd bar
     NOTE_F5,      NOTE_F5,           0,                                          // 5th Sentence - 4th bar


    // 14 note buckets in this sentence*
    // 100 note buckets So far

    NOTE_AS5,      NOTE_B5,           0,         NOTE_B5,                         // 6th Sentence - 1st bar
    NOTE_C6,      NOTE_GS5,     NOTE_G5,         NOTE_F5,                         // 6th Sentence - 2nd bar
    NOTE_DS5,     NOTE_DS5,     NOTE_F5,                                          // 6th Sentence - 3rd bar
    NOTE_G5,       NOTE_G5,           0,                                          // 6th Sentence - 4th bar


   // 14 note buckets in this sentence
   // 114 note buckets So far

   NOTE_AS5,      NOTE_DS6,           0,         NOTE_DS6,                        // 7th Sentence - 1st bar
    NOTE_C6,      NOTE_GS4,     NOTE_G4,          NOTE_F4,                        // 7th Sentence - 2nd bar
   NOTE_AS5,       NOTE_D5,                                                       // 7th Sentence - 3rd bar
   NOTE_DS6                                                                       // 7th Sentence - 4th bar

   // 11 note buckets in this sentence
   // 125 note buckets So far

  };





  uint16_t melody_line2[] = {
    NOTE_DS2, NOTE_D2, NOTE_D2,  NOTE_D2,  // 1st Sentence - 1st bar
     NOTE_C2, NOTE_G1, NOTE_C2,  NOTE_D2,  // 1st Sentence - 2nd bar
    NOTE_DS2, NOTE_C2, NOTE_G1, NOTE_GS1,  // 1st Sentence - 3rd bar
    NOTE_AS1, 0,                           // 1st Sentence - 4th bar


    // 14 note buckets in this sentence
    // 14 note buckets So far


    NOTE_DS2, NOTE_D2,  NOTE_C2,   NOTE_D2,  NOTE_D2,  NOTE_D2,                               // 2nd Sentence - 1st bar
     NOTE_G1, NOTE_D2,  NOTE_G2,  NOTE_FS2,                                                   // 2nd Sentence - 2nd bar
     NOTE_F2, NOTE_F2,  NOTE_F2,   NOTE_F2,  NOTE_F1,   NOTE_F1,   NOTE_F1,   NOTE_F1,        // 2nd Sentence - 3rd bar
    NOTE_AS1, NOTE_F1, NOTE_AS1,  NOTE_AS1, NOTE_AS1,                                         // 2nd - 3rd Sentence - 4th bar (1st bar)


    // 23 note buckets in this sentence*
    // 37 note buckets So far

    NOTE_AS1,        0, NOTE_AS1,  NOTE_AS1, NOTE_AS1,   NOTE_GS1, NOTE_GS1,                       // 3rd Sentence - 1st bar*
    NOTE_G1,   NOTE_G1,  NOTE_F1,   NOTE_F1, NOTE_DS1,   NOTE_DS2, NOTE_DS2,                       // 3rd Sentence - 2nd bar *
    NOTE_GS1, NOTE_GS1, NOTE_GS1,  NOTE_GS1, NOTE_GS1,   NOTE_GS1, NOTE_GS1,   NOTE_A1,   NOTE_A1, // 3rd Sentence - 3rd bar *
    NOTE_AS1, NOTE_AS1, NOTE_AS1,   NOTE_AS1,                                                      // 3rd Sentence - 4th bar *

    // 27 note buckets in this sentence*
    // 64 note buckets So far

     NOTE_AS1, NOTE_AS1, NOTE_AS1,   NOTE_AS1,    NOTE_AS1,                                        // 4th Sentence - 1st bar *
     NOTE_DS2,  NOTE_D2,  NOTE_D2,   NOTE_DS2,    NOTE_DS2,  NOTE_E2,                              // 4th Sentence - 2nd bar *
      NOTE_F2,  NOTE_F2,  NOTE_F2,    NOTE_F2,     NOTE_F2,  NOTE_F2,  NOTE_F2,                    // 4th Sentence - 3rd bar *
      NOTE_G1,  NOTE_F1, NOTE_DS1,    NOTE_D1,                                                     // 4th Sentence - 4th bar *

     // 22 note buckets in this sentence*
     // 86 note buckets So far

     NOTE_DS1,     NOTE_D1,     NOTE_D1,     NOTE_D1,                                                    // 5th Sentence - 1st bar.
      NOTE_C1,    NOTE_GS1,     NOTE_G1,                                                                 // 5th Sentence - 2nd bar
      NOTE_F1,    NOTE_GS1,    NOTE_GS1,    NOTE_GS1,                                                    // 5th Sentence - 3rd bar
      NOTE_AS1,   NOTE_AS1,    NOTE_GS1,                                                                 // 5th Sentence - 4th bar

      // 14 note buckets in this sentence*
      // 100 note buckets So far

     NOTE_G1,     NOTE_DS1,    NOTE_DS1,    NOTE_DS1,                                                   // 6th Sentence - 1st bar
     NOTE_GS1,     NOTE_F1,     NOTE_E1,     NOTE_F1,                                                   // 6th Sentence - 2nd bar
     NOTE_C2,     NOTE_AS1,    NOTE_GS1,                                                                // 6th Sentence - 3rd bar
     NOTE_G1,      NOTE_G2,     NOTE_F1,                                                                // 6th Sentence - 4th bar

     // 14 note buckets in this sentence
     // 114 note buckets So far

    NOTE_DS1,      NOTE_G1,      NOTE_G1,    NOTE_G1,                                                   // 7th Sentence - 1st bar
    NOTE_DS1,      NOTE_F1,      NOTE_G1,   NOTE_GS1,                                                   // 7th Sentence - 2nd bar
    NOTE_G1,        NOTE_F1,                                                                            // 7th Sentence - 3rd bar
    NOTE_DS1                                                                                            // 7th Sentence - 4th bar

    // 11 note buckets in this sentence
    // 125 note buckets So far


  };





  uint8_t noteDurations[] = { 8, 5, 1, 2, // 1st Sentence - 1st bar
                              4, 4, 4, 4, // 1st Sentence - 2nd bar
                              4, 4, 4, 4, // 1st Sentence - 3rd bar
                              10, 6,      // 1st Sentence - 4th bar

                              // 14 note buckets in this sentence
                              // 14 note buckets So far


                              4, 4, 4, 1, 1, 2,        // 2nd Sentence - 1st bar
                              4, 4, 4, 4,              // 2nd Sentence - 2nd bar
                              3, 1, 3, 1, 3, 1, 3, 1,  // 2nd Sentence - 3rd bar
                              4, 4, 4, 3, 1,           // 2nd - 3rd Sentence - 4th bar (1st bar)

                              // 23 note buckets in this sentence*
                              // 37 note buckets So far



                              3, 1, 2,  2, 4, 3, 1,        // 3rd Sentence - 1st bar *
                              3, 1, 2,  2, 4, 3, 1,        // 3rd Sentence - 2nd bar *
                              3, 1, 3,  1, 2, 1, 1, 3, 1,  // 3rd Sentence - 3rd bar *
                             11, 1, 3, 1,                  // 3rd - 4th Sentence - 4th bar (1st bar)*

                              // 27 note buckets in this sentence*
                              // 64 note buckets So far



                              6, 2, 4, 3, 1,                // 4th Sentence - 1st bar *
                              4, 2, 2, 3, 1, 4,             // 4th Sentence - 2nd bar *
                              4, 3, 1, 2, 2, 2, 2,          // 4th Sentence - 3rd bar *
                              4, 4, 4, 4,                   // 4th Sentence - 4th bar *


                              // 22 note buckets in this sentence*
                              // 86 note buckets So far

                              8, 5, 1, 2,                       // 5th Sentence - 1st bar
                              8, 4, 4,                          // 5th Sentence - 2nd bar
                              8, 5, 1, 2,                       // 5th Sentence - 3rd bar
                              8, 4, 4,                          // 5th Sentence - 4th bar


                              // 14 note buckets in this sentence
                              // 100 note buckets So far

                              8, 5, 1, 2,                       // 6th Sentence - 1st bar
                              4, 4, 4, 4,                       // 6th Sentence - 2nd bar
                              4, 4, 8,                          // 6th Sentence - 3rd bar
                              8, 4, 4,                          // 6th Sentence - 4th bar

                              // 14 note buckets in this sentence
                              // 114 note buckets So far

                              8, 5, 1, 2,                       // 7th Sentence - 1st bar
                              4, 4, 4, 4,                       // 7th Sentence - 2nd bar
                              8, 8,                             // 7th Sentence - 3rd bar
                              16,                               // 7th Sentence - 4th bar

                              // 11 note buckets in this sentence
                              // 125 note buckets So far

  };



  for (int noteID = 0; noteID < 125 ; noteID++)
  {
    int freq = melody_line1[noteID];
    int freq2 = melody_line2[noteID];

    PlayTones(freq, freq2);

    uint16_t noteDuration = noteDurations[noteID]*100;
    delay(noteDuration);
  }

  mute();
  delay(1000);
}
