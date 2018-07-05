/* 
_____/\\\\\\\\\________/\\\\\\\\\\\______________________/\\\_        
 ___/\\\\\\\\\\\\\____/\\\/////////\\\________________/\\\\\\\_       
  __/\\\/////////\\\__\//\\\______\///________________\/////\\\_      
   _\/\\\_______\/\\\___\////\\\__________/\\\\\\\\\\\_____\/\\\_     
    _\/\\\\\\\\\\\\\\\______\////\\\______\///////////______\/\\\_    
     _\/\\\/////////\\\_________\////\\\_____________________\/\\\_   
      _\/\\\_______\/\\\__/\\\______\//\\\____________________\/\\\_  
       _\/\\\_______\/\\\_\///\\\\\\\\\\\/_____________________\/\\\_ 
        _\///________\///____\///////////_______________________\///_ 
        
5 Knob Sine Wave Phasor

POT 1 = OSCILLATOR 1 FREQUENCY
POT 2 = OSCILLATOR 2 FREQUENCY
POT 3 = OSCILLATOR 3 FREQUENCY
POT 4 = OSCILLATOR 4 FREQUENCY
POT 5 = OSCILLATOR 5 FREQUENCY
     ______________________________
   /     ____              ____     \
  |     /    \            /    \     |
  |    ( POT1 )          ( POT4 )    |
  |     \____/            \____/     |
  |               ____               |
  |              /    \              |
  |             ( POT3 )             |
  |              \____/              |
  |      ____              ____      |
  |     /    \            /    \     |
  |    ( POT2 )          ( POT5 )    |
  |     \____/            \____/     |
  |                                  |
   \ _______________________________/

Based on Mozzi example
Sine_Phasing
Tim Barrass 2013, CC by-nc-sa
http://sensorium.github.com/Mozzi/
  
Andrew Buckie 2018, CC by-nc-sa
*/

#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator
#include "SynthBoxConfig.h"

// Declare mapping variables
float Osc1Freq;
const int MinOsc1Freq = 0;
const int MaxOsc1Freq = 4400;
float Osc2Freq;
const int MinOsc2Freq = 0;
const int MaxOsc2Freq = 4400;
float Osc3Freq;
const int MinOsc3Freq = 0;
const int MaxOsc3Freq = 4400;
float Osc4Freq;
const int MinOsc4Freq = 0;
const int MaxOsc4Freq = 4400;
float Osc5Freq;
const int MinOsc5Freq = 0;
const int MaxOsc5Freq = 4400;

#define CONTROL_RATE 64 // Powers of 2

Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> Sine1(SIN2048_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> Sine2(SIN2048_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> Sine3(SIN2048_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> Sine4(SIN2048_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> Sine5(SIN2048_DATA);

void setup(){
  startMozzi(CONTROL_RATE);
  // Serial.begin(115200); // For Debugging of values
}

void updateControl(){

  // Read digital inputs
  ExpPot1 = digitalRead(DIP_1);   // DIP switch 1 - 1 = Off / 0 = On
  ExpPot2 = digitalRead(DIP_2);   // DIP switch 2 - 1 = Off / 0 = On
  ExpPot3 = digitalRead(DIP_3);   // DIP switch 3 - 1 = Off / 0 = On
  ExpPot4 = digitalRead(DIP_4);   // DIP switch 4 - 1 = Off / 0 = On
  ExpPot5 = digitalRead(DIP_5);   // DIP switch 5 - 1 = Off / 0 = On
  ExpDetct = digitalRead(ExpPlug); // 1 = Expression pedal plugged in / 0 = Expression pedal unplugged

  // Map to expression pedal if selected
  if (ExpPot1 == 0 & ExpDetct ==1){
    Pot1 = Exp;    
  }
  else{
    Pot1 = Pot1Default;
  }
  if (ExpPot2 == 0 & ExpDetct ==1){
    Pot2 = Exp;    
  }
  else{
    Pot2 = Pot2Default;
  }
  if (ExpPot3 == 0 & ExpDetct ==1){
    Pot3 = Exp;    
  }
  else{
    Pot3 = Pot3Default;
  }
  if (ExpPot4 == 0 & ExpDetct ==1){
    Pot4 = Exp;    
  }
  else{
    Pot4 = Pot4Default;
  }
  if (ExpPot5 == 0 & ExpDetct ==1){
    Pot5 = Exp;    
  }
  else{
    Pot5 = Pot5Default;
  }
  
  // Read all the analog inputs
  Pot1Val = mozziAnalogRead(Pot1); // value is 0-1023
  Pot2Val = mozziAnalogRead(Pot2); // value is 0-1023
  Pot3Val = mozziAnalogRead(Pot3); // value is 0-1023
  Pot4Val = mozziAnalogRead(Pot4); // value is 0-1023
  Pot5Val = mozziAnalogRead(Pot5); // value is 0-1023
  ExpVal  = mozziAnalogRead(Exp);  // value is 0-1023

  // Map Values
  Osc1Freq = (float)map(Pot1Val, 0, 1023, MinOsc1Freq, MaxOsc1Freq)/10;
  Osc2Freq = (float)map(Pot2Val, 0, 1023, MinOsc2Freq, MaxOsc2Freq)/10;
  Osc3Freq = (float)map(Pot3Val, 0, 1023, MinOsc3Freq, MaxOsc3Freq)/10;
  Osc4Freq = (float)map(Pot4Val, 0, 1023, MinOsc4Freq, MaxOsc4Freq)/10;
  Osc5Freq = (float)map(Pot5Val, 0, 1023, MinOsc5Freq, MaxOsc5Freq)/10;

  // Print Debug values
  /*
  Serial.print("Osc1Freq = ");
  Serial.print(Osc1Freq);
  Serial.print("   Osc2Freq = ");
  Serial.print(Osc2Freq);
  Serial.print("   Osc3Freq = ");
  Serial.print(Osc3Freq);
  Serial.print("   Osc4Freq = ");
  Serial.print(Osc4Freq);
  Serial.print("   Osc5Freq = ");
  Serial.print(Osc5Freq);
  Serial.println(); // Carriage return
  */

  // Set Oscillators
  Sine1.setFreq(Osc1Freq);
  Sine2.setFreq(Osc2Freq);
  Sine3.setFreq(Osc3Freq);
  Sine4.setFreq(Osc4Freq);
  Sine5.setFreq(Osc5Freq);
}

int updateAudio(){
  char asig1 = (char)(Sine1.next());
  char asig2 = (char)(Sine2.next());
  char asig3 = (char)(Sine3.next());
  char asig4 = (char)(Sine4.next());
  char asig5 = (char)(Sine5.next());
  return ((int)asig1-asig2-asig3-asig4-asig5)/5;
}

void loop(){
  audioHook(); // required here
}







