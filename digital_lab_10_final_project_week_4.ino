#include "BetterButton.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
// GUItool
AudioSynthWaveform waveform3; //xy=212,369
//^did not end up using this
AudioSynthWaveform waveform2; //xy=214,307
AudioSynthWaveform waveform1; //xy=216,241
AudioMixer4 mixer1; //xy=415,313
AudioEffectEnvelope envelope1; //xy=569,315
AudioOutputAnalog dac1; //xy=778,325
AudioConnection patchCord1(waveform3, 0, mixer1, 2);
AudioConnection patchCord2(waveform2, 0, mixer1, 1);
AudioConnection patchCord3(waveform1, 0, mixer1, 0);
AudioConnection patchCord4(mixer1, envelope1);
AudioConnection patchCord5(envelope1, dac1);
//GLOBAL VARIABLES
int button1Pin = 6;
int button2Pin = 7;
int button3Pin = 8;
int button4Pin = 9;
int button5Pin = 10;
int button6Pin = 11;
int button7Pin = 12;
int button8Pin = 24;
int button9Pin = 25;
int button10Pin = 26;
int button11Pin = 27;
int button12Pin = 28;
int button13Pin = 29;
float volume1 = 0.5;
float volume2 = 0.5;
//EACH WAVEFORM’s FREQUENCIES
int notes[13] = {32.703, 34.648, 36.708, 38.891, 41.203, 43.654, 46.249, 48.999, 51.913, 55,
58.27, 61.735, 65.406};
int overtones[13] = {32.703, 34.648, 36.708, 38.891, 41.203, 43.654, 46.249, 48.999, 51.913, 55,
58.27, 61.735, 65.406};
//ADSR TIME
int attackTime = 0;
int decayTime = 0;
float sustainTime = 0;
int releaseTime = 0;
//OCTAVE BUTTON VARIABLEs
bool lastNextOctaveState = LOW;
bool nextOctaveState = LOW;
bool lastPrevOctaveState = LOW;
bool prevOctaveState = LOW;
int nextOctaveButtonPin = 38;
int prevOctaveButtonPin = 37;
int octave = 2;
//DETUNE
int detune1 = 0;
int detune2 = 0;
//BETTERBUTTON PIN CORRESPONDED TO ELEMENT NUMBERS FOR WAVERFORM
FREQUENCIES
BetterButton button1(button1Pin, 0);
BetterButton button2(button2Pin, 1);
BetterButton button3(button3Pin, 2);
BetterButton button4(button4Pin, 3);
BetterButton button5(button5Pin, 4);
BetterButton button6(button6Pin, 5);
BetterButton button7(button7Pin, 6);
BetterButton button8(button8Pin, 7);
BetterButton button9(button9Pin, 8);
BetterButton button10(button10Pin, 9);
BetterButton button11(button11Pin, 10);
BetterButton button12(button12Pin, 11);
BetterButton button13(button13Pin, 12);
//SETUP
void setup() {
// put your setup code here, to run once:
Serial.begin(9600);
AudioMemory(12);
pinMode(nextOctaveButtonPin, INPUT);
pinMode(prevOctaveButtonPin, INPUT);
waveform1.begin(WAVEFORM_TRIANGLE);
waveform2.begin(WAVEFORM_SAWTOOTH);
waveform1.amplitude(0.65);
waveform2.amplitude(0.01);
button1.pressHandler(onPress);
button1.releaseHandler(onRelease);
button2.pressHandler(onPress);
button2.releaseHandler(onRelease);
button3.pressHandler(onPress);
button3.releaseHandler(onRelease);
button4.pressHandler(onPress);
button4.releaseHandler(onRelease);
button5.pressHandler(onPress);
button5.releaseHandler(onRelease);
button6.pressHandler(onPress);
button6.releaseHandler(onRelease);
button7.pressHandler(onPress);
button7.releaseHandler(onRelease);
button8.pressHandler(onPress);
button8.releaseHandler(onRelease);
button9.pressHandler(onPress);
button9.releaseHandler(onRelease);
button10.pressHandler(onPress);
button10.releaseHandler(onRelease);
button11.pressHandler(onPress);
button11.releaseHandler(onRelease);
button12.pressHandler(onPress);
button12.releaseHandler(onRelease);
button13.pressHandler(onPress);
button13.releaseHandler(onRelease);
}
//MAIN CODE w Functions
void loop() {
// put your main code here, to run repeatedly:
amplitudeReadWaveform1();
amplitudeReadWaveform2();
changeOctave();
waveformFrequencyKnobs();
aDSR();
button1.process();
button2.process();
button3.process();
button4.process();
button5.process();
button6.process();
button7.process();
button8.process();
button9.process();
button10.process();
button11.process();
button12.process();
button13.process();
}
//Function for when button is pressed, includes adsr
void onPress(int val) {
Serial.print(val);
envelope1.noteOn();
envelope1.attack(attackTime);
waveform1.frequency(notes[val] * octave + detune1);
waveform2.frequency(overtones[val] * octave);
envelope1.decay(decayTime);
envelope1.sustain(sustainTime/2.5);
delay(5);
//usbMIDI.sendNoteOn(val, 127, 1);
Serial.println("on");
}
//Function for when button is released
void onRelease(int val) {
Serial.print(val);
envelope1.release(releaseTime);
envelope1.noteOff();
//NOTE OFF when button is released
delay(5);
//usbMIDI.sendNoteOff(val, 0, 1);
Serial.println("off");
}
//FUNCTIONS for mapping amplitude knobs
void amplitudeReadWaveform1(){
volume1 = map(analogRead(A12), 0, 1023, 0, 500);
waveform1.amplitude(volume1 / 1000.0);
}
void amplitudeReadWaveform2(){
volume2 = map(analogRead(A13), 0, 1023, 0, 500);
waveform2.amplitude(volume2 / 2000.0);
//^^ cause saws are loud
}
//FUNCTION FOR ANALOG READ ADSR
void aDSR(){
attackTime = map(analogRead(A17), 0, 1023, 11, 2500);
decayTime = map(analogRead(A16), 0, 1023, 11, 2500);
sustainTime = map(analogRead(A15), 0, 1023, 0, 1);
releaseTime = map(analogRead(A14), 0, 1023, 0, 5000);
}
//OCTAVE BUTTONS prevOctave will decrease octave number, nextOctave will increase the
octave
void changeOctave(){
prevOctaveState = digitalRead (prevOctaveButtonPin);
if (prevOctaveState == HIGH and lastPrevOctaveState == LOW){
octave /= 2;
delay(5);
if (octave < 2 ) {
octave= 2;
delay(5);
}
}
lastPrevOctaveState = prevOctaveState;
nextOctaveState = digitalRead (nextOctaveButtonPin);
if (nextOctaveState == HIGH and lastNextOctaveState == LOW){
octave *= 2;
delay(5);
if (octave > 32) {
octave = 32;
delay(5);
}
}
lastNextOctaveState = nextOctaveState;
Serial.println (octave);
}
//DETUNE KNOB
void waveformFrequencyKnobs(){
detune1 = map(analogRead(A20), 0, 1023, 0, 25);
detune2 = map(analogRead(A21), 0, 1023, 0, 25);
}
