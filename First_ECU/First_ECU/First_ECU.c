/*
 * First_ECU.c
 *
 * Created: 07/06/2020 8:25:06 AM
 *  Author: Mohamed Elsayed
 */ 
/**************************************  Timer Tick  ****************************************************/
// looks like Watchdog timer reset application and ask for the password every tick 
#define tick 245
/*
tick 31  --->> 0.5 sec  
tick 62  --->> 1   sec
tick 93  --->> 1.5 sec
tick 124 --->> 2   sec
	 |		   |
	 |		   |
	 V		   V
tick 245 --->> 4   sec  max value 
*/
/***************************************  Block Mode Timer  *********************************************/
#define block_timer 5000 
 /*1000 ---> 1 second*/
/********************************************************************************************************/
#include "LCD.h"
#include "keypad.h"
#include "Ex_EEPROM.h"
#include "UART.h"
#include "Timer0.h"
uint8_t d1,d2,d3,d4;						// password digits stored in the External EEPROM
uint8_t c1,c2,c3,c4;						// password digits entered by user
uint8_t n1,n2,n3,n4;						// New password in case you need to change the old one
uint8_t state=0;							// State 0 will open the Door & state 1 will change password
uint8_t count=0;							// That let you move betwwen cases of get_password function
uint8_t block_mode=0;						// If you forget password block_mode value will be updated
uint8_t start_msg=0;						// It must be 0 at the begining to display instructions
void get_password ();						// Main function --> get password
int main(void)
{
    LCD_init();								// Initializing LCD
	Uart_init();							// Initializing UART
	keypad_init();							// Initializing Keypad
	Ex_EEPROM_init();						// Initializing External EEPROM
	timer_normal_init();					// Initializing Timer0 in normal mode
	SETBIT(DDRC,4);							// LED FOR BLOCK MODE
	SETBIT(DDRC,5);							// LED FOR ACTION MODE
	block_mode=Ex_EEPROM_read(15);			// To check if you were in block mode when you restarted your application
	uint8_t first_password=Ex_EEPROM_read(14);// To check if you are entering for the first time
	while(1)
    {
        if (first_password==255)								//To set first value of Password
        {
			LCD_write_command(1);
			LCD_write_string("First Log In");
			_delay_ms(300);
			LCD_write_command(1);
			LCD_write_string("Enter Password");
			state=1;
			start_msg=1;
			first_password=15;
			Ex_EEPROM_Write(14,15);
        }
		if(block_mode==3){										//This will block you X sec if you are in block mode
		SETBIT(PORTC,4);
		LCD_write_command(1);
		LCD_write_string("you are blocked");
		LCD_write_command(0xc0);
		LCD_write_string("for 20 sec");
		_delay_ms(block_timer);
		CLRBIT(PORTC,4);
		LCD_write_command(1);
		LCD_write_string("Enter right pass");
		block_mode=0;
		Ex_EEPROM_Write(15,0);
	}
		if (start_msg==0)										//Start message to user before writing password
		{
			LCD_write_command(1);
			LCD_write_string(" Door Password");
			d1=Ex_EEPROM_read(10);								// Reading first digit of stored password
			d2=Ex_EEPROM_read(11);							    // Reading second digit of stored password
			d3=Ex_EEPROM_read(12);								// Reading third digit of stored password
			d4=Ex_EEPROM_read(13);								// Reading forth digit of stored password
			start_msg=1;
			CLRBIT(PORTC,5);
		}
		get_password();
		 
    }
}
void get_password ()
{
	int8_t key=keypad_read();
	if(key != -1 && key!='N'){
		switch (count)
		{
			case 0 :
			if (key=='O' || key=='C'){count=0;}			//to make password consists from numbers only
			else{	
			LCD_write_command(0xc3);
			LCD_write_char(key);
			_delay_ms(100);
			LCD_write_command(0xc3);
			LCD_write_char('*');
			_delay_ms(200);
			c1=key-48;									 // first digit of entered password from user
			count++;
			}
			break;
			case 1 :
			if (key=='O' || key=='C'){count=1;}			//to make password consists from numbers only
			else{
			LCD_write_char(key);
			_delay_ms(100);
			LCD_write_command(0xc4);
			LCD_write_char('*');
			c2=key-48;									// second digit of entered password from user
			_delay_ms(200);
			count++;
			}			
			break;
			case 2 :
			if (key=='O' || key=='C'){count=2;}			//to make password consists from numbers only
			else{
			LCD_write_char(key);
			_delay_ms(100);
			LCD_write_command(0xc5);
			LCD_write_char('*');
			c3=key-48;									// third digit of entered password from user
			_delay_ms(200);
			count++;
			}						
			break;
			case 3 :
			if (key=='O' || key=='C'){count=3;}			//to make password consists from numbers only
			else{
			LCD_write_char(key);
			_delay_ms(100);
			LCD_write_command(0xc6);
			LCD_write_char('*');
			c4=key-48;									// forth digit of entered password from user
			_delay_ms(200);
			if (state==0 && (c1==d1&&c2==d2&&c3==d3&&c4==d4))
			{	/*That mean you entered right Password*/
				LCD_write_command(0x1);
				LCD_write_string("o to open");
				LCD_write_command(0xc0);
				LCD_write_string("c to change");
				block_mode=0;
				Ex_EEPROM_Write(15,0);
				SETBIT(PORTC,5);
				SETBIT(TIMSK,TOIE0);
				count++;
			}
			else if (state==0 &&(c1!=d1||c2!=d2||c3!=d3||c4!=d4)){
				/*That mean you entered wrong Password*/
				block_mode++;
				LCD_write_command(1);
				LCD_write_string("Wrong password");
				_delay_ms(150);
				LCD_write_command(1);
				LCD_write_num(3-block_mode);
				LCD_write_string(" Tries left");
				Ex_EEPROM_Write(15,block_mode);
				_delay_ms(100);
				count=0;					
				if(block_mode==3){
				SETBIT(PORTC,4);
				LCD_write_command(1);
				LCD_write_string("you are blocked");
				LCD_write_command(0xc0);
				LCD_write_string("for 20 sec");
				_delay_ms(block_timer);
				CLRBIT(PORTC,4);
				LCD_write_command(1);
				LCD_write_string("Enter right pass");
				block_mode=0;
				Ex_EEPROM_Write(15,0);
				}
				
			}
			else if (state==1){
				/*That mean you need to change password*/
				n1=c1;
				n2=c2;
				n3=c3;
				n4=c4;
				LCD_write_command(0x1);
				LCD_write_string("plz confirm pass");
				state=2;
				count=0;
				
			}
			else if (state==2 && (c1==n1&&c2==n2&&c3==n3&&c4==n4))
			{    /* To change stored value of password with new one you confirmed*/
				Ex_EEPROM_Write(10,n1);  
				_delay_ms(100);
				Ex_EEPROM_Write(11,n2);
				_delay_ms(100);
				Ex_EEPROM_Write(12,n3);
				_delay_ms(100);
				Ex_EEPROM_Write(13,n4);
				LCD_write_command(0x1);
				LCD_write_string("password changed");
				_delay_ms(500);
				LCD_write_command(0x1);
				start_msg=0;
				state=0;
				count=0;
				
			}
			else if (state==2 && (c1!=n1||c2!=n2||c3!=n3||c4!=n4)){
				/*That for wrong confirmation to new Password*/
				LCD_write_command(0x1);
				LCD_write_string("wrong try again");
				state=1;
				count=0;
			}
			}			
			break;
			case 4 :
			if(key=='C'){
				/*will change your state to change Password*/
				LCD_write_command(0x1);
				LCD_write_string("write new pass");
				CLRBIT(TIMSK,TOIE0);
				state=1;
				_delay_ms(200);
			count=0;}
			else if (key=='O'){
				/*Will open your door*/
				CLRBIT(TIMSK,TOIE0);
				LCD_write_command(0x1);
				LCD_write_command(0x82);
				LCD_write_string("Door is open");
				Uart_Write('1');
				_delay_ms(1000);
				LCD_write_command(0x1);
				start_msg=0;
				count=0;}
			else{
				/*you wrote pressed wrong key*/
				LCD_write_command(0x1);
				LCD_write_string("Wrong Entery");
				_delay_ms(200);
				}
			break;
			
			
		}
	}
}
ISR(TIMER0_OVF_vect){					// To control HMI after writing Password and give user timeout 
	static uint8_t time_counter = 0;
	time_counter++;
	if (time_counter==tick)			
	{
		
		CLRBIT(PORTC,5);
		time_counter=0;
		count=0;
		start_msg=0;
		CLRBIT(TIMSK,TOIE0);
	}

}