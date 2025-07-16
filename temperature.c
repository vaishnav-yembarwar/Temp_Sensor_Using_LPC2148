#include<lpc214x.h>
#define rs 1<<0  
#define en 1<<1 

void lcd_cmd(char);
void lcd_data(unsigned char);
void delay(int);
void display(void);
void initPLL(void);
void lcd_string( char *str);
int main()
{	
	IODIR0=0Xffff;
lcd_cmd(0x0e);
  lcd_cmd(0x01);
  lcd_cmd(0x06);
  lcd_cmd(0x38);
	lcd_cmd(0x85);
	lcd_string("WELCOME");
	delay(1800);
  lcd_cmd(0xc5);
	lcd_cmd(0x01);
	
	lcd_string("WE ARE USING");
	delay(1000);
	lcd_cmd(0xc0);
	lcd_string("TEMP SENSOR");
	delay(2000);
	 lcd_cmd(0x01);
	
while(1)
{
	
		AD0CR=0X01200302;	   //AD0.1, start conversion
	//AD0CR=0x01200202;
	
	while(!(AD0GDR & 1<<31));		//waiting A/D conversion
	
	lcd_cmd(0x80);
	lcd_string("TEMPRETURE IS");
	lcd_cmd(0xc0);
	display();
	delay(5000);
//	lcd_cmd(0x01);
 }
}

void lcd_cmd(char cmd)
{
	IOPIN0=cmd<<8;
	//IO0SET=cmd<<8
	IOCLR0=rs;	// 1<<0
	IOSET0=en; // 1<<1
	delay(100);
	IOCLR0=en;
	//IOCLR=cmd<<8
}

void lcd_data(unsigned char data)
{
	IOPIN0=data<<8;
	IOSET0=rs;
	IOSET0=en;
	delay(100);
	IOCLR0=en;
}

void delay(int x)
{
	int i,j;
	for(i=0;i<100;i++)
	for(j=0;j<x;j++);
}

void display()
{
	 int temperature;  // Integer temperature value

  
   unsigned int val;
	
   	val=AD0GDR;
	val=(val>>6)&0x3FF;
	//IOPIN0=val<<8;	// val =1023
	
   
	  temperature = (val *3.3 / 1024) *100;
        lcd_data((temperature / 100) + '0'); 			 // Display 100thplace of temperature
	 
        lcd_data(((temperature / 10) % 10) + '0');  // Display tenths place of temperature
        lcd_data((temperature% 10) + '0');  			// Display ones place of temperature
        lcd_data('c');
}

void initPLL(void)
{
// To generate 60MHz from 12MHz crystal 
	PLL0CFG=0X24;							// SET PSEL=2 AND MSEL=5  
	PLL0CON=0X01;							//PLL IS ACTIVE BUT NOT YET CONNECT  
	PLL0FEED=0XAA; 					//FEED SEQUENCE
	PLL0FEED=0X55;  
		while((PLL0STAT & 0X400)==0); 	//WAIT FOR FEED SEQUENCE TO BE INSERTED
	PLL0CON=0X03;											// PLL HAS BEEN ACTIVE AND BEING CONNECTRD 			
	PLL0FEED=0XAA;									 //FEED SEQUENCE
	PLL0FEED=0X55;				 					//FEED SEQUENCE
	
	VPBDIV=0x00;								// SET PCLK 15Mhz
}

void lcd_string( char *str)
{
	  while(*str!='\0')
    {
       lcd_data(*str);
       str++;
    }
}
