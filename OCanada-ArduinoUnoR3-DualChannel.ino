/*

    Ryerson University
    G. Raymond Chang School of Continuing Education

    CCPS 310 (1J0) – Computer Organization II – Winter 2020
    Instructor: Alexander Ufkes (https://www.linkedin.com/in/alex-ufkes-37154844/)
    Lab 6 - Lab 6 – AVR III: Program a piece of music for the Arduino Uno Board to play on the passive buzzer

    Version 2: O'Canada, double-line melody;
    one buzzer on pin 9 of the Arduino Uno Board, and another on its pin 11.
       - Main melody on pin 9      (buzzer 1).
       - Accompaniment on pin 11   (buzzer 2).

    Melody lines is encoded by me, Rob (Sohrab) Soltani, in April 2020 (https://rob.soltani.io).
    Melody lines are based on the score downloaded from: http://cantorion.org/music/3873/O-Canada-French-lyrics.

*/

// pitches.h provided by Alexander Ufkes
# include "pitches.h"

// Processor frequency divided by 8 (because using an 8-bit counter?)
#define BUZZER_CLOCK 16000000/8

// timer_init initializes time/counter control registers TCCR1A, TCCR1B, TCCR2A and TCCR2B
// Function sets appropriate bits in the registers
// Function was originally provided by Alexander Ufkes
void timer_init(void)
{

    // Using ARC Assembly language, set the bit for pin 9
    // on the DDRB register which is port B's data direction register (0x04).
    // This sets the pin as output.
   asm(" sbi 0x04, 1 ");

   TCCR1A = (1 << COM1A1) | (1 << WGM11);
   TCCR1B = (1 << WGM13)  | (1 << WGM12)  | (1 << CS11);

   // The rest of the code in this function by me.

   // Using ARC Assembly language, set the bit for pin 11
   // on the DDRB register which is port B's data direction register (0x04).
   // This sets the pin as output.
   asm(" sbi 0x04, 3 "); // Use PWM pin 11 Port b3 for accompaniment

   // Registers TCCR2A and TCCR2B are used to achive the secondary PWM,
   // for the second buzzer to be able to play a different frequency.
   // Second PWM implementation inspired by: https://www.arduino.cc/en/Tutorial/SecretsOfArduinoPWM
   TCCR2A =  (1 << COM2A0) | (1 << WGM20);
   TCCR2B =  (1 << WGM22)  | (1 << CS22);

}

// PlayTones accepts two frequency parameters; each for one buzzer.

// Based on freq, it sets the values of ICR1 and OCR1A registers,
// which in turn change the tone produced by buzzer 1 on pin 9.

// Based on freq2, the functionsets the value of OCR2A,
// which in turn changes the tone produced by buzzer 2 on pin 11.

// The PlayTones function is written by me
// based on the tone function originally provided by Alexander Ufkes;
// the tone function can be seen in: https://github.com/rob-soltani/OCanada-ArduinoUnoR3-SingleChannel.

// Unlike the SingleChannel (the GitHub project link above),
// in this project I do not use muteDurations
// as a separate array for silence between the notes.
// Instead my PlayTones function treats zeroes in the notes
// as silent notes with their own note duration in the
// noteDurations array.
void PlayTones(uint16_t freq, uint16_t freq2)
{

   // Part I: Handle buzzer 1 on pin 9
   // Check if we have silent note
   if (freq != 0 )
   {
     // We don't have silent note if we get here.

     // Again, set the bit for pin 9 on port B's data direction register (0x04)
     // because for some notes, and silence at the end, both the mute function
     // and the this (PlayTones) function clear the bit, so we need it set
     // again to play tones.
     asm(" sbi 0x04, 1 ");

     // Set the value of the Input Capture Register based on the input frequency.
     ICR1 = BUZZER_CLOCK/freq;

     // Set the value of the OCR1A register which is connected to pin 9.
     OCR1A = ICR1*3/4;
   }
   else
   {
     // We have a silent note if we get here.

     // Clear the LSB bit (pin 9) on port B's data direction register (0x04),
     // to stop output tone.
      asm(" cbi 0x04, 1 ");

      // Also, clear the LSB bit (pin 9) on port B's data register (0x05),
      // to stop output tone.
      asm(" cbi 0x05, 1 ");
   }



   // Part II: Handle buzzer 2 on pin 11
   // Check if we have silent note
   if (freq2 != 0 )
   {
     // We don't have silent note if we get here.

     // Again, set the bit for pin 11 on port B's data direction register (0x04)
     // because for some notes, and silence at the end, both the mute function
     // and the this (PlayTones) function clear the bit, so we need it set
     // again to play tones.
     asm(" sbi 0x04, 3 ");

     // Set the value of the OCR2A register which is connected to pin 11.
     // This way of setting PWM on register OCR2A (pin 11)is inspired by: https://www.arduino.cc/en/Tutorial/SecretsOfArduinoPWM
     OCR2A = BUZZER_CLOCK/64/(freq2)/2/2;
   }
   else
   {
     // We have a silent note if we get here.

     // Clear the 3rd bit (pin 11) on port B's data direction register (0x04),
     // to stop output tone.
      asm(" cbi 0x04, 3 ");

      // Also, clear the 3rd bit (pin 11) on port B's data register (0x05),
      // to stop output tone.
      asm(" cbi 0x05, 3 ");
   }

}

// The mute function achieves silence and silent notes when needed
// by clearing the pin 9 and pin 11 outputs and clearing the data direction bits.
// Function was originally provided by Alexander Ufkes and was modified by me to
// accomodate the second buzzer
void mute(void)
{
  // Clear the LSB bit on port B's data direction register (0x04),
  // to stop output tone.
  asm(" cbi 0x04, 1 ");

  // Also, clear the LSB bit on port B's data register (0x05),
  // to stop output tone.
  asm(" cbi 0x05, 1 ");

  asm(" cbi 0x04, 1 ");
  asm(" cbi 0x05, 1 ");

  asm(" cbi 0x04, 3 ");
  asm(" cbi 0x05, 3 ");
}

// The ususal Arduino setup function
// Function was provided by Alexander Ufkes
void setup()
{
  timer_init();
}


// The general template of the following three arrays and the loop Function
// was provided by Alexander Ufkes

/*
   An array to hold the notes for the main melody line of O Canada
   These are encoded by me.
   Zeroes represent silent notes (when the buzzer 1 on pin 9 is silenced).
*/
/*
   I divided the melody into what I call sentences (is that a valid music term?),
   which to me are musical statements that have musical meaning with a start,
   and an end. This helped me be able to listen and debug the music better.

   Each of these musical sentences are roughly represented as a group of four bars,
   which correspond to the bars in the original sheet music cited above.
   Each bar is on a separate line.

   This note groupings or buckets used in melody_line1, melody_line2, and noteDurations arrays.

   To synchronize melody lines, I decided to use one note duration in the
   noteDurations for both melody lines. That required me to repeat any note in one line
   which started at the same time as another note in the other line but itself
   was a longer note; e.g. if in a bar, the main melody began the bar with a whole note,
   but the accompaniment (second melody line) began with an eight note, I used durations in
   muteDurations so that it fit the smallest note which meant that the whole note had to
   be repeated as many times as there were notes in the second melody line.
   I hope I have made at least some sense here.
*/
uint16_t melody_line1[] =
{
    NOTE_G5,   NOTE_AS5,          0,   NOTE_AS5,                                                             // 1st Sentence - 1st bar
   NOTE_DS5,   NOTE_DS5,   NOTE_DS5,    NOTE_F5,                                                             // 1st Sentence - 2nd bar
    NOTE_G5,   NOTE_GS5,   NOTE_AS5,    NOTE_C6,                                                             // 1st Sentence - 3rd bar
    NOTE_F5,    NOTE_F5,                                                                                     // 1st Sentence - 4th bar

   // 14 note buckets in this sentence
   // 14 note buckets So far


    NOTE_G5,    NOTE_G5,    NOTE_A5,   NOTE_A5,   0,   NOTE_A5,                                              // 2nd Sentence - 1st bar
   NOTE_AS5,   NOTE_AS5,   NOTE_AS5,   NOTE_C6,                                                              // 2nd Sentence - 2nd bar
    NOTE_D6,          0,    NOTE_D6,         0,        NOTE_C6,   0,   NOTE_C6,   0,                         // 2nd Sentence - 3rd bar
   NOTE_AS5,   NOTE_AS5,   NOTE_AS5,   NOTE_F5,        NOTE_G5,                                              // 2nd - 3rd Sentence - 4th bar (1st bar)

   // 23 note buckets in this sentence*
   // 37 note buckets So far


   NOTE_GS5,   NOTE_GS5,   NOTE_GS5,    NOTE_G5,    NOTE_F5,    NOTE_G5,   NOTE_GS5,                         // 3rd Sentence - 1st bar*
   NOTE_AS5,   NOTE_AS5,   NOTE_AS5,   NOTE_GS5,    NOTE_G5,   NOTE_GS5,   NOTE_AS5,                         // 3rd Sentence - 2nd bar*
    NOTE_C6,          0,   NOTE_AS5,          0,   NOTE_GS5,   NOTE_GS5,          0,   NOTE_G5,   0,         // 3rd Sentence - 3rd bar *
    NOTE_F5,          0,    NOTE_F5,    NOTE_G5,                                                             // 3rd Sentence - 4th bar *

   // 27 note buckets in this sentence*
   // 64 note buckets So far


   NOTE_GS5,    NOTE_G5,    NOTE_F5,    NOTE_G5,   NOTE_GS5,                                                 // 4th Sentence - 1st bar *
   NOTE_AS5,   NOTE_AS5,   NOTE_GS5,    NOTE_G5,          0,   NOTE_G5,                                      // 4th Sentence - 2nd bar *
    NOTE_F5,   NOTE_AS5,          0,   NOTE_AS5,    NOTE_A5,   NOTE_G5,   NOTE_A5,                           // 4th Sentence - 3rd bar *
   NOTE_AS5,   NOTE_AS5,   NOTE_AS5,   NOTE_AS5,                                                             // 4th Sentence - 4th bar *


   // 22 note buckets in this sentence*
   // 86 note buckets So far


    NOTE_G5,   NOTE_AS5,    0,   NOTE_AS5,                                                                   // 5th Sentence - 1st bar
   NOTE_DS5,          0,    0,                                                                               // 5th Sentence - 2nd bar
   NOTE_GS5,    NOTE_C6,    0,    NOTE_C6,                                                                   // 5th Sentence - 3rd bar
    NOTE_F5,    NOTE_F5,    0,                                                                               // 5th Sentence - 4th bar

   // 14 note buckets in this sentence*
   // 100 note buckets So far


   NOTE_AS5,      NOTE_B5,           0,         NOTE_B5,                                                     // 6th Sentence - 1st bar
   NOTE_C6,      NOTE_GS5,     NOTE_G5,         NOTE_F5,                                                     // 6th Sentence - 2nd bar
   NOTE_DS5,     NOTE_DS5,     NOTE_F5,                                                                      // 6th Sentence - 3rd bar
   NOTE_G5,       NOTE_G5,           0,                                                                      // 6th Sentence - 4th bar

   // 14 note buckets in this sentence
   // 114 note buckets So far


   NOTE_AS5,   NOTE_DS6,         0,   NOTE_DS6,                                                              // 7th Sentence - 1st bar
    NOTE_C6,   NOTE_GS4,   NOTE_G4,    NOTE_F4,                                                              // 7th Sentence - 2nd bar
   NOTE_AS5,    NOTE_D5,                                                                                     // 7th Sentence - 3rd bar
   NOTE_DS6                                                                                                  // 7th Sentence - 4th bar

   // 11 note buckets in this sentence
   // 125 note buckets So far

};


/*
   An array to hold the notes for the accompaniment (second melody line) of O Canada
   These are encoded by me.
   Zeroes represent silent notes (when the buzzer 2 on pin 11 is silenced).
*/
/*
   The main ideas were expressed in the comments above the melody_line1 array.
   But one point to made here is that I had to selectively choose what notes
   from the sheet music (cited above) to include in the second line of music.
   I had to do this because the sheet music dictated that three or more notes
   be played at the same time which we could not do with two passive buzzer.

   Kudos to Calixa Lavallée for this beautiful piece of music and to musicians
   who can play all of its notes.
*/
uint16_t melody_line2[] =
{
   NOTE_DS2,   NOTE_D2,   NOTE_D2,    NOTE_D2,                                                               // 1st Sentence - 1st bar
    NOTE_C2,   NOTE_G1,   NOTE_C2,    NOTE_D2,                                                               // 1st Sentence - 2nd bar
   NOTE_DS2,   NOTE_C2,   NOTE_G1,   NOTE_GS1,                                                               // 1st Sentence - 3rd bar
   NOTE_AS1,         0,                                                                                      // 1st Sentence - 4th bar

   // 14 note buckets in this sentence
   // 14 note buckets So far


   NOTE_DS2,   NOTE_D2,    NOTE_C2,    NOTE_D2,    NOTE_D2,    NOTE_D2,                                      // 2nd Sentence - 1st bar
    NOTE_G1,   NOTE_D2,    NOTE_G2,   NOTE_FS2,                                                              // 2nd Sentence - 2nd bar
    NOTE_F2,   NOTE_F2,    NOTE_F2,    NOTE_F2,    NOTE_F1,    NOTE_F1,   NOTE_F1,   NOTE_F1,                // 2nd Sentence - 3rd bar
   NOTE_AS1,   NOTE_F1,   NOTE_AS1,   NOTE_AS1,   NOTE_AS1,                                                  // 2nd - 3rd Sentence - 4th bar (1st bar)

   // 23 note buckets in this sentence*
   // 37 note buckets So far


   NOTE_AS1,          0,   NOTE_AS1,   NOTE_AS1,   NOTE_AS1,   NOTE_GS1,   NOTE_GS1,                         // 3rd Sentence - 1st bar*
    NOTE_G1,    NOTE_G1,    NOTE_F1,    NOTE_F1,   NOTE_DS1,   NOTE_DS2,   NOTE_DS2,                         // 3rd Sentence - 2nd bar *
   NOTE_GS1,   NOTE_GS1,   NOTE_GS1,   NOTE_GS1,   NOTE_GS1,   NOTE_GS1,   NOTE_GS1,   NOTE_A1,   NOTE_A1,   // 3rd Sentence - 3rd bar *
   NOTE_AS1,   NOTE_AS1,   NOTE_AS1,   NOTE_AS1,                                                             // 3rd Sentence - 4th bar *

   // 27 note buckets in this sentence*
   // 64 note buckets So far


   NOTE_AS1,   NOTE_AS1,   NOTE_AS1,    NOTE_AS1,   NOTE_AS1,                                                // 4th Sentence - 1st bar *
   NOTE_DS2,    NOTE_D2,    NOTE_D2,    NOTE_DS2,   NOTE_DS2,    NOTE_E2,                                    // 4th Sentence - 2nd bar *
    NOTE_F2,    NOTE_F2,    NOTE_F2,     NOTE_F2,    NOTE_F2,    NOTE_F2,   NOTE_F2,                         // 4th Sentence - 3rd bar *
    NOTE_G1,    NOTE_F1,   NOTE_DS1,     NOTE_D1,                                                            // 4th Sentence - 4th bar *

   // 22 note buckets in this sentence*
   // 86 note buckets So far

   NOTE_DS1,    NOTE_D1,    NOTE_D1,    NOTE_D1,                                                             // 5th Sentence - 1st bar.
    NOTE_C1,   NOTE_GS1,    NOTE_G1,                                                                         // 5th Sentence - 2nd bar
    NOTE_F1,   NOTE_GS1,   NOTE_GS1,   NOTE_GS1,                                                             // 5th Sentence - 3rd bar
   NOTE_AS1,   NOTE_AS1,   NOTE_GS1,                                                                         // 5th Sentence - 4th bar

   // 14 note buckets in this sentence*
   // 100 note buckets So far


    NOTE_G1,   NOTE_DS1,   NOTE_DS1,   NOTE_DS1,                                                             // 6th Sentence - 1st bar
   NOTE_GS1,    NOTE_F1,    NOTE_E1,    NOTE_F1,                                                             // 6th Sentence - 2nd bar
    NOTE_C2,   NOTE_AS1,   NOTE_GS1,                                                                         // 6th Sentence - 3rd bar
    NOTE_G1,    NOTE_G2,    NOTE_F1,                                                                         // 6th Sentence - 4th bar

   // 14 note buckets in this sentence
   // 114 note buckets So far


   NOTE_DS1,   NOTE_G1,   NOTE_G1,    NOTE_G1,                                                              // 7th Sentence - 1st bar
   NOTE_DS1,   NOTE_F1,   NOTE_G1,   NOTE_GS1,                                                              // 7th Sentence - 2nd bar
    NOTE_G1,   NOTE_F1,                                                                                     // 7th Sentence - 3rd bar
   NOTE_DS1                                                                                                 // 7th Sentence - 4th bar

  // 11 note buckets in this sentence
  // 125 note buckets So far

};


/*
   An array to hold the duration of notes for both melody lines of O Canada
   These are encoded by me.
*/
/*
   Again, the main ideas were expressed in the comments above the melody_line1 array.

   One point about this function is that compared to the single-line melody version
   of the current project (https://github.com/rob-soltani/OCanada-ArduinoUnoR3-SingleChannel),
   in this double-line version I do not use a separate mechanism for silence after notes.

   So note durations in this array both apply to the non-silent notes in the two melody lines and
   the silent notes represented by zeroes in the corresponding arrays.
*/
uint8_t noteDurations[] =
{
    8,   5,   1,   2,                            // 1st Sentence - 1st bar
    4,   4,   4,   4,                            // 1st Sentence - 2nd bar
    4,   4,   4,   4,                            // 1st Sentence - 3rd bar
   10,   6,                                      // 1st Sentence - 4th bar

   // 14 note buckets in this sentence
   // 14 note buckets So far


   4,   4,   4,   1,   1,   2,                   // 2nd Sentence - 1st bar
   4,   4,   4,   4,                             // 2nd Sentence - 2nd bar
   3,   1,   3,   1,   3,   1,   3,   1,         // 2nd Sentence - 3rd bar
   4,   4,   4,   3,   1,                        // 2nd - 3rd Sentence - 4th bar (1st bar)

   // 23 note buckets in this sentence*
   // 37 note buckets So far


    3,   1,   2,   2,   4,   3,   1,             // 3rd Sentence - 1st bar *
    3,   1,   2,   2,   4,   3,   1,             // 3rd Sentence - 2nd bar *
    3,   1,   3,   1,   2,   1,   1,   3,   1,   // 3rd Sentence - 3rd bar *
   11,   1,   3,   1,                            // 3rd - 4th Sentence - 4th bar (1st bar)*

   // 27 note buckets in this sentence*
   // 64 note buckets So far


   6,   2,   4,   3,   1,                        // 4th Sentence - 1st bar *
   4,   2,   2,   3,   1,   4,                   // 4th Sentence - 2nd bar *
   4,   3,   1,   2,   2,   2,   2,              // 4th Sentence - 3rd bar *
   4,   4,   4,   4,                             // 4th Sentence - 4th bar *

   // 22 note buckets in this sentence*
   // 86 note buckets So far


   8,   5,   1,   2,                             // 5th Sentence - 1st bar
   8,   4,   4,                                  // 5th Sentence - 2nd bar
   8,   5,   1,   2,                             // 5th Sentence - 3rd bar
   8,   4,   4,                                  // 5th Sentence - 4th bar

   // 14 note buckets in this sentence
   // 100 note buckets So far


   8,   5,   1,   2,                             // 6th Sentence - 1st bar
   4,   4,   4,   4,                             // 6th Sentence - 2nd bar
   4,   4,   8,                                  // 6th Sentence - 3rd bar
   8,   4,   4,                                  // 6th Sentence - 4th bar

   // 14 note buckets in this sentence
   // 114 note buckets So far

    8,   5,   1,   2,                            // 7th Sentence - 1st bar
    4,   4,   4,   4,                            // 7th Sentence - 2nd bar
    8,   8,                                      // 7th Sentence - 3rd bar
   16,                                           // 7th Sentence - 4th bar

   // 11 note buckets in this sentence
   // 125 note buckets So far

};


// The usual Arduino loop.
// This was provided by Alexander Ufkes,
// But I modified it more than a bit to make the played music
// more pleasant to my ears and to support a double-line melody
// with two buzzers.
void loop()
{

   // Iterate through the notes of the melody
   for (int noteID = 0; noteID < sizeof(melody_line1)/sizeof(uint16_t *) ; noteID++)
   {
     // Read the frequency of the current note from the main melody array.
     int freq = melody_line1[noteID];

     // Read the frequency of the current note from the accompaniment melody array.
     int freq2 = melody_line2[noteID];

     // Send the frequencies to the PlayTones function to play them on
     // buzzer 1 and buzzer 2 on pin 9 and 11
     PlayTones(freq, freq2);

     // Read the duration of the current note from the noteDurations array.
     uint16_t noteDuration = noteDurations[noteID]*100;

     // Delay (keep playing the note) for the duration of the note
     delay(noteDuration);
   }

   // Silence the two buzzerz (this is after the whole song is played)
   mute();

   // Wait one second before playing the melody again and again and again ...
   delay(1000);
}
