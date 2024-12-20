// refs: https://www.electronicwings.com/8051/8051-uart
// refs: https://embetronicx.com/tutorials/microcontrollers/8051/8051-uart-tutorial-serial-communication/

#ifndef _BLUETOOTH_UART_H_
#define _BLUETOOTH_UART_H_
#include "LCD_1602.h"
#include "Utilities.h"
#include "String_Ultils.h"

/*
Valid msg from Bluetooth module:
NOTE: flip dev-state (1->0, 0->1) if the dev is controlled by USER
	inv1
	inv2
	inv3
	inv4
	inv5
*/

#define _max_buffer_size 8 
#define _bounded_y(x, y) ((x)%(y))

uint8 ble_rcv_size;
char  ble_rcv_data[_max_buffer_size];

void UART_Byte_Transmit(char transmit_data){
    // Copy data to BUFFER
    SBUF = transmit_data;
    // while for another Transmission until it end
    while (TI==0);
    // Reset TI flag to start this Transmission
    TI = 0;
}

void UART_Bytes_Transmit(char transmit_data[], int32 transmit_data_size){
	int32 i;
	if(transmit_data_size == 0){
		while(*transmit_data){
			UART_Byte_Transmit(*transmit_data);
			transmit_data++;
		}
	}else{
		REP(i, 1, transmit_data_size){
			UART_Byte_Transmit(transmit_data[i-1]);
		}
	}

}

// uint8 UART_Read_Data(char uart_data[], char read_size){
// 	if(read_size == 0 || ble_rcv_size == 0) return false; // for read failed
// 	read_size = min_val(read_size, ble_rcv_size);
// 	_string_copy(uart_data, ble_rcv_data, read_size, 0);
// 	//reset buffer size (aka ble_rcv_data)
// 	ble_rcv_size = 0;
// 	return true; // for read successful
// }

uint8 ble_has_contained(char pattern[], uint8 pat_size){
	if(ble_rcv_size<1) return 0;
    return _string_find_pattern(pattern, pat_size, ble_rcv_data, ble_rcv_size, 0) < ble_rcv_size;
}

void Bluetooth_UART_Initial(){
    //UART initial
	GLOBAL_INT(ENABLE);
    ES = 1; //Serial interrupt
    TMOD |= 0x20; //Set timer 2 mode 8bit
    TH1 = 0xFD; //load value for baud rate = 9600
    TL1 = 0xFD; //load value for baud rate = 9600
    SCON = 0x50;
    TR1 = 1; // start timer 1
	
    //Bluetooth initial
	ble_rcv_size = 0;

}

void UART_Received() interrupt 4 {
	char temp_char;
	if(RI == 1){
		temp_char = SBUF;
		if( 
			_is_carriage_return(temp_char)
			|| _is_new_line(temp_char)
		){
				RI = 0;
				return;
		}
		if(_is_upper_case(temp_char)) temp_char -= 'a'-'A';
		ble_rcv_size = _bounded_y(ble_rcv_size, _max_buffer_size)+1;
		ble_rcv_data[ble_rcv_size-1] = temp_char;
		RI = 0; 
	}else{
		// TI = 0;
	}
}

#endif