/*
 * I2C.h
 *
 * Created: 07/06/2020 9:47:16 PM
 *  Author: M
 */ 


#ifndef I2C_H_
#define I2C_H_
#include "header.h"

// Master
void I2C_master_init(void);
void I2C_master_start();
void I2C_master_addr(uint8_t addr);
void I2C_master_write(uint8_t data);
uint8_t I2C_master_read();	// new
void I2C_master_stop(void);

// Slave
void I2C_slave_Init(uint8_t My_Add);
uint8_t I2C_slave_avialable();
uint8_t I2C_slave_read(void);


#endif /* I2C_H_ */