/*
 * Ex_EEPROM.c
 *
 * Created: 08/06/2020 11:55:13 AM
 *  Author: Mohamed Elsayed
 __________________________________ 
 # Notes:-
 
 * A10		A9		A8		(M24C16 Data sheet)
 *  0		0		0	-->	Locations range from 0	   to	255
 *	0		0		1	--> Locations range from 256   to	511
 *	0		1		1	--> Locations range from 512   to	767
 *	|		|		|	
 *  V		V		V	--> Locations range from 1792  to	2047
 *	1		1		1
 _____________________________________
 We will send 0xA8 first which is (0b10101000) to select the external eeprom 
 as a slave and A10 set to one for locations range (1024 --> 1279 ) 
 */

#include "Ex_EEPROM.h"
void Ex_EEPROM_init(){
	I2C_master_init();
}
uint8_t Ex_EEPROM_read(uint8_t address){
	
	I2C_master_start();
	I2C_master_write(0xA8);
	I2C_master_write(address);
	I2C_master_start();
	I2C_master_write(0xA9);
	uint8_t data= I2C_master_read();
	I2C_master_stop();
	return data;
}
void Ex_EEPROM_Write (uint8_t address, uint8_t data){
	I2C_master_start();
	I2C_master_write(0xA8);
	I2C_master_write(address);
	I2C_master_write(data);
	I2C_master_stop();	
}