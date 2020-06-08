/*
 * Second_ecu.c
 *
 * Created: 07/06/2020 12:17:43 PM
 *  Author: Mohamed Elsayed
 */ 
/******************************************************** Motor Time ************************************************************/
/*Time needed for the motor to open the door in ms */
#define Motor_time 500
/******************************************************** Motor Speed ***********************************************************/
/*Motor speed in percentage from 0 to 100% */
#define Motor_speed 100
/*******************************************************************************************************************************/

#include "PWM.h"
#include "UART.h"
uint8_t y=0;
int main(void)
{
   sei();
   SETBIT(DDRC,1);
   SETBIT(DDRC,2);
   Uart_init();
   PWM_init_timer1_oc1b();
   set_duty_oc1b(0);
   while(1)
    {
         
    }
}
ISR(USART_RXC_vect){
	y=UDR;
	if(y=='1'){
	set_duty_oc1b(Motor_speed);
	SETBIT(PORTC,1);
	CLRBIT(PORTC,2);
	_delay_ms(Motor_time);
	SETBIT(PORTC,2);
	CLRBIT(PORTC,1);
	set_duty_oc1b(Motor_speed);
	_delay_ms(Motor_time);
	set_duty_oc1b(0);
	CLRBIT(PORTC,2);
	}
		
}
