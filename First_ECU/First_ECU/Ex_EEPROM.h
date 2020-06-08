/*
 * Ex_EEPROM.h
 *
 * Created: 08/06/2020 11:55:34 AM
 *  Author: M
 */ 


#ifndef EX_EEPROM_H_
#define EX_EEPROM_H_
#include "I2C.h"

void Ex_EEPROM_init();
uint8_t Ex_EEPROM_read(uint8_t address);
void Ex_EEPROM_Write (uint8_t address, uint8_t data);


#endif /* EX_EEPROM_H_ */