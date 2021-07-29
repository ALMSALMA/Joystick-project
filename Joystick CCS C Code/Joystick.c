#include   <16f877A.h>
#device    adc=10
#use       delay (CLOCK=20M, CRYSTAL=20M)

#fuses   BROWNOUT, HS, NOWDT, NOLVP


#define   lcd_d4   pin_b0
#define   lcd_d5   pin_b1
#define   lcd_d6   pin_b2
#define   lcd_d7   pin_b3

#define   lcd_rs   pin_b5
#define   lcd_en   pin_b4

#define   func_set       0b00101000
#define   disp_control   0b00001100
#define   disp_clear     0b00000001

/////////////////////////////////////////////////
///////// variables /////////////////////////////


unsigned int16   adc_value;
unsigned int16   adc_valueX;
unsigned int16   adc_valueY;
unsigned int16   adc_value_eskiX = 0;
unsigned int16   adc_value_eskiY = 0;
signed int16   digit1, digit2, digit3, digit4;

/////////////////////////////////////////////////
///////// functions /////////////////////////////

void   lcd_init       (void);
void   lcd_data       (unsigned char);
void   lcd_cmd        (unsigned char);
void   lcd_nibble     (unsigned char);
void   lcd_clear      (void);
void   lcd_line1      (unsigned char);
void   lcd_line2      (unsigned char);
void   bcd            (void);

/////////////////////////////////////////////////

void main()
{   
   lcd_init();
   
   SETUP_ADC_PORTS(AN0_AN1_AN3);
   
   setup_adc(ADC_CLOCK_DIV_2);
   
   set_adc_channel(1);
   adc_value_eskiX = read_adc();
   
   set_adc_channel(0);
   adc_value_eskiY = read_adc();

while(1)
   {
   
   set_adc_channel(1);
   delay_ms(10);

   adc_value = read_adc();
   adc_valueX = adc_value;
   bcd();

   lcd_line1(0);

   lcd_data("X= ");

   lcd_data(digit4+0x30);
   lcd_data(digit3+0x30);
   lcd_data(digit2+0x30);
   lcd_data(digit1+0x30);
  

 
   set_adc_channel(0);
   delay_ms(10);

   adc_value = read_adc();
   adc_valueY = adc_value;

   bcd();

   lcd_data(" Y= ");

   lcd_data(digit4+0x30);
   lcd_data(digit3+0x30);
   lcd_data(digit2+0x30);
   lcd_data(digit1+0x30);
   
   lcd_line2(0);
   
   if( adc_valueY == adc_value_eskiY && adc_valueX == adc_value_eskiX ){
         
         lcd_data("     SABIT    ");
         
   }else if( adc_valueX == adc_value_eskiX ){
      
      if( adc_valueY > adc_value_eskiY ){
         lcd_data("     ILERI    ");
      }else if( adc_valueY < adc_value_eskiY ){
         lcd_data("     GERI    ");
      }
      
   }else if( adc_valueY == adc_value_eskiY ){
      
      if( adc_valueX > adc_value_eskiX ){
         lcd_data("     SAG     ");
      }else if( adc_valueX < adc_value_eskiX ){
         lcd_data("     SOL     ");
      }
   }else if( adc_valueY > adc_value_eskiY && adc_valueX > adc_value_eskiX ){
      
         lcd_data("    ILERI SAG    ");
     
   }else if( adc_valueY < adc_value_eskiY && adc_valueX < adc_value_eskiX ){
      
         lcd_data("    GERI SOL    ");
     
   }else if( adc_valueY > adc_value_eskiY && adc_valueX < adc_value_eskiX ){
      
         lcd_data("    ILERI SOL    ");
     
   }else if( adc_valueY < adc_value_eskiY && adc_valueX > adc_value_eskiX ){
      
         lcd_data("    GERI SAG    ");
     
   }


}   
}

/////////////////////////////////////////////////

void bcd (void)
{   
   digit4 = (adc_value/1000);
   digit3 = (adc_value/100 ) % 10;
   digit2 = (adc_value/10  ) % 10;
   digit1 = (adc_value     ) % 10; 
}

/////////////////////////////////////////////////

void   lcd_init (void)            // its a lcd initilize function
{   
   output_low (lcd_rs);
   output_low (lcd_en);

   delay_ms(250);

   lcd_nibble(func_set);

   delay_ms(1);
   lcd_cmd(func_set);      // function set

   delay_ms(1);
   lcd_cmd(func_set);      // function set

   delay_ms(1);
   lcd_cmd(func_set);      // function set

   lcd_cmd(disp_control);   // display controll

   lcd_clear();

}

/////////////////////////////////////////////////

void lcd_line1 (unsigned char position)   //lcd line 1 position function
{   lcd_cmd(0x80 + position); }

void lcd_line2 (unsigned char position)   //lcd line 2 position function
{   lcd_cmd(0xc0 + position); }

/////////////////////////////////////////////////

void lcd_clear (void)                     // its a lcd clear function
{   
    lcd_cmd (disp_clear);
    delay_ms(50);
}

/////////////////////////////////////////////////

void   lcd_cmd (unsigned char i)          // its a command function
{   
   unsigned int8 temp;

   output_low (lcd_rs);      
   temp=i;
   lcd_nibble (temp);

   temp=temp<<4;
   lcd_nibble (temp);

}

/////////////////////////////////////////////////

void   lcd_data(unsigned char i)          // its a data function
{   
   unsigned int8 temp;

   output_high(lcd_rs);      
   temp=i;
   lcd_nibble (temp);

   temp=temp<<4;
   lcd_nibble (temp);
}

/////////////////////////////////////////////////

void lcd_nibble (unsigned char j)         // its a 4 bit mode function for lcd
{   
   if(bit_test(j,4))   output_high(lcd_d4);
   else                output_low (lcd_d4);

   if(bit_test(j,5))   output_high(lcd_d5);
   else                output_low (lcd_d5);

   if(bit_test(j,6))   output_high(lcd_d6);
   else                output_low (lcd_d6);

   if(bit_test(j,7))   output_high(lcd_d7);
   else                output_low (lcd_d7);

   output_high(lcd_en);
   delay_us (50);
   output_low (lcd_en);
}

