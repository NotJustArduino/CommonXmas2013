/* Modified Tone Sketch from Tom Igoe - Taken from the Arduino SDK examples */
/* Modded by NJA Hackers to play Carol of the Bells*/
/*circuit:
 * 8-ohm speaker on digital pin 8 */
#include "pitches.h"
#include "tunes.h"
#include <avr/pgmspace.h>

//Variables for the proximity sensor
const int  pinTrig = 9;
const int  pinEcho = 10;

int speedSoundMicrosPerCm = 29;
int lastRange = 0;
int timeout = 0;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  initRange();
  
}

void  initRange(){
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);
}

int handleRange(){
  int rangeCm = 0;
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite( pinTrig , HIGH);
  delayMicroseconds(10);
  digitalWrite( pinTrig , LOW);
  delayMicroseconds(2);  // sound only travels 0.06cm in this time 

  int duration = pulseIn(pinEcho, HIGH, 15000); //~5m timeout
  int curRangeCm = duration / speedSoundMicrosPerCm / 2; 
  if (curRangeCm<=500 && curRangeCm>0) {
    rangeCm = curRangeCm;
  }
  Serial.println(rangeCm);
  return rangeCm;
}

void loop() {
  int range = handleRange();
  if((range<45)&&(range!=0)){
    playTune();
    lastRange =  range;
  }
}

void playTune(){
  int index = random(8);
  int numberOfNotes = sizeof(aTune[index].notes)/sizeof(int);
  for(int i=0;i<numberOfNotes;i++){
    int currentNote = pgm_read_word_near(aTune[index].notes+i);
    if(currentNote==-1)
      break;
    int tempo = 200;
    int currentDuration = tempo*(pgm_read_word_near(aTune[index].durations+i));
    tone(8, currentNote,currentDuration);
    delay(currentDuration*0.8);
    noTone(8);
  }
}
