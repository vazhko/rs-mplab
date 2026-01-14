#include <xc.h>
#define _XTAL_FREQ 4000000UL
#pragma config OSC = IntRC
#pragma config WDTE = OFF
#pragma config MCLRE = OFF
#pragma config CP = OFF

void main(void){
    OPTION=0x05;GPIO=0;TRIS=0b1011;
    for(;;){
        if(TMR0&0x40)STATUSbits.GPWUF=1;else STATUSbits.GPWUF=0;
        if(STATUSbits.GPWUF)__delay_ms(500);
        GPIObits.GP2=1;__delay_ms(1000);GPIObits.GP2=0;
        if(STATUSbits.GPWUF)__delay_ms(500);else __delay_ms(1000);
        __delay_ms(500);
    }
}
