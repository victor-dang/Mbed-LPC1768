#include "mbed.h"
#include "MAX4466.h"
//DigitalOut myled(p18);
Serial pc(USBTX, USBRX); //enable serial port which links to USB 
DigitalOut led(LED1); 
DigitalIn DigitIn(p5);
MAX4466 mic(p20);
InterruptIn button(p10);
int flag = 0;
void button_pressing(){
    char c = pc.getc();
    if(c=='a')
        {
            mic.volume_indicator();
            pc.printf("\n\r %f",mic._value);    
        }
}
void ISR1()
{
    led = !led;
    flag = !flag;
    pc.printf("\n\r button pressed!, flag: %d",flag);
}
int main() {
    button.rise(&ISR1); // attach the address of the ISR function to the
    // interrupt rising edge  
 
    while(1) {
        //myled = DigitIn;
        //pc.printf("\n\r Digital in: %d, AnalogIN: %d", DigitIn.read(), AIn.read());
        //wait(5);
        if(flag)
        {
            mic.volume_indicator();
            pc.printf("\n\r %f",mic._value);
        }
        //pc.printf("\n\r Level is %f", mic.sound_level());
        //pc.printf("\n\r Digital In: %f",DigitIn.read());
    }
}
