#include <quatthongminh.h>

#include "lcd.h" 
//Dinh nghia cac chan ket noi nut nhan
#define K1 input(pin_b1)
#define K2 input(pin_b2)
#define K3 input(pin_b3)
#define K5 input(pin_b5)
#define K6 input(pin_b6)
#define K7 input(pin_b7)
#define K4 input(pin_b4)

// pin_c0 so 1
// pin_c1 so 2
// pin_c2 so 3
// pin_c3  bao che do tu dong
// pin_c4  bao phun suong 
// pin_c5  bao quay 
//Khai bao bien cuc bo
int idx_m = 0, idx_s = 0;

unsigned int16 value;
float voltage,nhietdo;

// ham auto 
void Auto_mode()
{ 
         
   value = read_adc(); // doc adc 
   voltage = 5000.0f*value/1023.0f;// chuyen doi ra muc dien ap
   nhietdo= voltage/10;
   
   if(nhietdo <20)
   {
         output_bit(pin_c1, 0);
         output_bit(pin_c2, 0);
         output_bit(pin_c3, 0);
         output_bit(pin_c6, 0);
         output_bit(pin_c7, 1);
   }
      
   if(nhietdo <= 23 && nhietdo >= 20) //Muc nong 1
   {
         output_bit(pin_c1,1);
         output_bit(pin_c2,0);
         output_bit(pin_c3,0);
         output_bit(pin_c6, 0);
         output_bit(pin_c7,1);
          
   }
    if(nhietdo >= 23 && nhietdo <= 28 )// Muc nong 2
   {
         output_bit(pin_c1,0);
         output_bit(pin_c2,1);
         output_bit(pin_c3,0);
         output_bit(pin_c6, 0);
         output_bit(pin_c7,1);
      //   delay_ms(2000);
            
   }
    if(nhietdo >= 28 && nhietdo <= 35) //Muc nong 3
   { 
         output_bit(pin_c1,0);
         output_bit(pin_c2,0);
         output_bit(pin_c3,1);
         output_bit(pin_c6, 0);
         output_bit(pin_c7,1);
       //  delay_ms(2000);
             
   }
   if(nhietdo >= 35) //Muc nong 3 bat phun suong
   { 
         output_bit(pin_c1,0);
         output_bit(pin_c2,0);
         output_bit(pin_c3,1);
         output_bit(pin_c6,1);//phun suwong ko ra tín hieu
         output_bit(pin_c7,1);
                    
   }
    
}
//ham chinh tay
#INT_RB
void Manual_mode()
{
   //Dieu khien so 1
   if(K1 == 1)
   {
      delay_ms(20);
      if(K1 == 1)
      {
         output_bit(pin_b1,1);
         output_bit(pin_b2,0);
         output_bit(pin_b3,0);
         
//!         output_bit(pin_c1,1);
//!         output_bit(pin_c2,0);
//!         output_bit(pin_c3,0);
      }
   }while(K1 == 1);
}  


/// Ham dieu khien qua va phun suong
void phu()
{
// dieu khien quay
if(K5 == 1)
   {
      delay_ms(20);
      if(K5 == 1)
         idx_s++;
      if(idx_s == 2) idx_s = 0;
   }while(K5 == 1);
   if(idx_s == 1) 
   { 
      output_bit(pin_c5,1);
    
   }
   else 
   { 
      output_bit(pin_c5,0);
  
   }
   
   //Dieu khien che do phun suong
   if(K6 == 1)
   {
      delay_ms(20);
      if(K6 == 1)
         idx_m++;
      if(idx_m == 2) idx_m = 0;
    } while(K6 == 1);
   if(idx_m == 1) 
   {
      output_bit(pin_c6,1);
    
      }
   else
   {
      output_bit(pin_c6,0);
   
      }
}

//!// hien thi nhiet do 
void hienthinhietdo()
{
   value = read_adc(); // doc adc 
   voltage = 5000.0f*value/1023.0f;// chuyen doi ra muc dien ap
   nhietdo= voltage/10;
   
    lcd_gotoxy(1,1);  
    lcd_putc("NHIET DO:");
    lcd_gotoxy(10,1);    
    printf(lcd_putc,"%2.1f C",nhietdo); // print value on LCD 
       
  
   
}
//!

void  Manual_mode1(){
   int i = 0;
   if(K1 == 1)
      i = 1;
   if(K2 == 1)
      i = 2;
   if(K3 == 1)
      i = 3;
   if(i == 1){
      output_bit(pin_c1, 1);
      output_bit(pin_c2, 0);
      output_bit(pin_c3, 0);
   }
   if(i == 2){
      output_bit(pin_c1, 0);
      output_bit(pin_c2, 1);
      output_bit(pin_c3, 0);
   }
   if(i == 3){
      output_bit(pin_c1, 0);
      output_bit(pin_c2, 0);
      output_bit(pin_c3, 1);
   }
}
int flag,j;
void main()
{
   // setup ngat
   enable_interrupts(global);
   enable_interrupts(int_RB);
   ext_int_edge(H_to_L);
   
   // setup adc
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_adc_ports(ALL_ANALOG);
   set_adc_channel(0);//Doc gia tri adc o chan A0
   
  // setup cac chan dau vao dau ra 
   set_tris_b(0xff);// set tris b la input
//!   set_tris_d(0x00);// set tris d la output
   set_tris_c(0x00);// set tris d la output
//!   set_tris_a(0xff);
   //khoi tao lcd 
   lcd_init();
   // dat gia tri cac co ban dau=0
   
   flag = 0; // mac dinh che do Manual_mode()
   j = 0;
      lcd_putc(" NPH SMART FAN  ");
      output_bit(pin_c1, 0);
      output_bit(pin_c2, 0);
      output_bit(pin_c3, 0);
      output_bit(pin_c4, 0);
      output_bit(pin_c5, 0);
      output_bit(pin_c6, 0);
      output_bit(pin_c7, 0);
      delay_ms(1000);
      lcd_putc("                ");
   
   
//!  
   while(TRUE)
   { 
   if(K4 == 1){
        lcd_gotoxy(1,2);
        lcd_putc("BAT QUAT");
        
      if(K7 == 1) // bat au to
         j = 1;
      if(K1 == 1 || K2 == 1 || K3 == 1) // tat che do auto
         j = 0;
      
      if(j == 0){
         Manual_mode1();
         output_bit(pin_c7, 0); //  den c5 tat, che do bang tay
      }
      if(j == 1){
         Auto_mode();
         output_bit(pin_c7, 1); //  den c5 bat, che do auto
      }
   }
   
   else{
        lcd_gotoxy(1,2);
        lcd_putc("TAT QUAT ");
        output_bit(pin_c1, 0);
        output_bit(pin_c2, 0);
        output_bit(pin_c3, 0);
   }
   
   
   //chedo(); // chay ham chon che do
 
      phu();   // chay ham bat tat quay va phun suong
 
      hienthinhietdo();
   }
}


