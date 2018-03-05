#include "mbed.h"
#include "MAX4466.h"
 
MAX4466::MAX4466(PinName pin): _pin(pin), _led1(p15), _led2(p16), _led3(p17), _led4(p18) {
    calibration();
}
 
//CALIBRATION FUNCTION:
//Find average sound level at the intialization of the microphone
//Use this in the calculation of the LED indication array values
float MAX4466::calibration() {
    
    _t.start();
    _t1.start();
   // float peakToPeak=0, signalMax=0, signalMin=1024;
    float peakToPeak=0, signalMax=0, signalMin=1024;
    while (_t1.read()<1) {
        while (_t.read_ms()<50) {
            _sample=_pin.read();
 
            if (_sample< signalMin) {
                if (_sample>signalMax)
                    signalMax=_sample;
 
                else if (_sample<signalMin)
                    signalMin=_sample;
            }
        }
     _t.reset();
     peakToPeak=signalMax-signalMin;
     _value= (peakToPeak*3.3);
     _value = floor(_value * 100) / 100;
     _sum+=_value;
     _count++;
    }
    _average=_sum/_count;
    _t1.reset();
 
    return _average;
}
 
//LED ARRAY FUNCTION:
//Setup array of 4 LEDs 
//The input to this function is the average from the calibration function
//The variable _value which indicates the current value from the microphone is a global variable updated in the sound_level function
//As different threshholds of volume are met more LEDs will light up
void MAX4466::led_array(float x ) {
float a,b,c,d,e;
//a = 0.05; b = 0.5; c = 1; d = 1.2; e = 2.8;
a = 0.0005; b = 0.005; c = 0.01; d = 0.012; e = 0.028;
    if (_value<x+a) {
            _led1=0;
            _led2=0;
            _led3=0;
            _led4=0;
    }
    if (_value>x+a&&_value<b+x) {
            _led1=1;
            _led2=0;
            _led3=0;
            _led4=0;
    }
    if (_value>b+x&&_value<c+x) {
            _led1=1;
            _led2=1;
            _led3=0;
            _led4=0;
    }
    if (_value>c+x&&_value<d+x) {
            _led1=1;
            _led2=1;
            _led3=1;
            _led4=0;
    }
    if (_value>d+x&&_value<e+x) {
            _led1=1;
            _led2=1;
            _led3=1;
            _led4=1;
    }
}
 
 
//SOUND LEVEL FUNCTIOM:
//Read in current sound level from the microphone
//Update the global variable _value which will in turn be read by the LED array function
float MAX4466::sound_level() {
 
    _t.start();
    //float peakToPeak=0, signalMax=0, signalMin=1024;
    float peakToPeak=0, signalMax=0, signalMin=1024;
    while (_t.read_ms()<50) {
        _sample=_pin.read();
 
        if (_sample<1024) {
            if (_sample>signalMax)
                signalMax=_sample;
 
            else if (_sample<signalMin)
                signalMin=_sample;
        }
    }
    _t.reset();
    peakToPeak=signalMax-signalMin;
    _value= (peakToPeak*3.3);
    _value = floor(_value * 100) / 100;
    _sum+=_value;
    _count++;
 
    return _value;
}
 
//VOLUME INDICATOR FUNCTION:
//To be called in main function
//Updates the led array
//outputs current numerical value
void MAX4466::volume_indicator() {
        led_array(_average);
        sound_level();
}
void MAX4466::stop(){
    _t.reset();
    _t1.reset(); 
    _led1=0;
    _led2=0;
    _led3=0;
    _led4=0;  
}