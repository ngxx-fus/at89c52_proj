/*
    Note:
        Base_Lib.h is a lib that include all define, typedef,
        base function, ... It can be reused in many following project.
    Autor:
        Nguyen Thanh Phu
    Version:
        0.0.2
*/
#ifndef _BASE_LIB_H_
#define _BASE_LIB_H_

#ifndef elif
#define elif else if
#endif
#ifndef DECREASE_ONE
#define DECREASE_ONE(VAR) VAR = (VAR>0?(VAR-1):VAR)
#endif
#ifndef REP
#define REP(i, a, b) for(i = (a); i <= (b); ++i)
#endif
#ifndef REV
#define REV(i, a, b) for(i = (a); i >= (b); --i)
#endif


typedef unsigned char uint8;
typedef unsigned int uint32;
typedef char int8;
typedef int int32;

enum enum_STATE{ LOW  = 0, HIGH = 1 };
enum enum_ENABLE{ DISABLE=0, ENABLE, START, 
    STOP, MODE_16BIT, RESET 
};

static void delay_us(uint32 us){
    uint32 i = 0; 
    for(i = 0; i < us; i = i + 1){
        // do nothin'
    }
}

void delay_ms(uint32 ms){
    uint32 i = 0; 
    for(i = 0; i < ms*12; i = i + 1){
        // do nothin'
    }
}

void eINT0_CTL(uint8 CONFIG){
    if( CONFIG == ENABLE){
        EX0 = 1;
        IT0 = 1;
    }
    if( CONFIG == DISABLE){
        EX0 = 0;
        IT0 = 1;
    }
}

void eINT1_CTL(uint8 CONFIG){
    if( CONFIG == ENABLE){
        // Configure INT1 falling edge interrupt
        IT1 = 1;   
        // Enable the INT1 External Interrupt    
        EX1 = 1;
    }
    if( CONFIG == DISABLE){
        // Configure INT1 falling edge interrupt
        IT1 = 0;   
        // Enable the INT1 External Interrupt    
        EX1 = 0;
    }
}

#define RESET_TH 0xFC 
#define RESET_TL 0x67
void TIMER0_CTL(uint8 CONFIG){
    switch (CONFIG) {
        case ENABLE:
            ET0 = 1;            return;
        case DISABLE:
            ET0 = 0;            return;
        case RESET:
            TL0 = RESET_TL;
            TH0 = RESET_TH;     return;
        case START:
            TR0 = 1;            return;
        case STOP:
            TR0 = 0;            return;
        case MODE_16BIT:
            TMOD = TMOD|0x01;   return;
    }
}

#define GLOBAL_INT(CONFIG) EA=(CONFIG==ENABLE)?(1):(0)

#endif