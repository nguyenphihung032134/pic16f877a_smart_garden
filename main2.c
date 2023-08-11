
#include <main.h>
#include <dht11_22.h>
#include <lcd.h>

/////////////////////////////////  chan nut nhan
#define BTMode input(PIN_B0)
#define BTUp   input(PIN_B1)
#define BTDown input(PIN_B2)
////////////////////////////////// chan nut nhan 
#define BTmotor  input(PIN_B3)
#define BTquat   input(PIN_B5)

///////////////////////////////// chan xuat tin hieu ra
#define Relay_bom   PIN_C0
#define Relay_quat  PIN_C1

//////////////////////////////////

int8 Mode;
//////////////////////////////////////
unsigned int8 TempS, HumiS,HumiS1;//Nhiet Do, Do Am dat, Do Am Khong Khi , tu cam bien 
///////////////////////////////////////
unsigned int8 HumiLv;// Do Am cai dat ON bom
unsigned int8 HumiD; // Do Am caii dat OFF bom 
////////////////////////////////////
unsigned int8 TempLv ; // Nhiet do cai dat ON quat
unsigned int8 TempD  ; // Nhiet do cai dat OFF quat
///////////////////////////////////
unsigned int16  ADC1;
float doam;
//////////////////////////////////
int1 Status  = 1;  // co trang thai may bom 
int1 Status1 = 1;  // co trang thai quat

//////////////////////////////////
#int_ext
_NgatRB()  // NGAT PIN B0
{  
   Mode++;
   lcd_putc('\f');  // xoa man hinh 
   if (Mode== 6)
      Mode = 0;
   return Mode;
}

//////////////////////////////////KHAII BAO CHUONG TRINH CON 

void _BOM ( unsigned int8 HumiLv,  unsigned int8 HumiS, unsigned int8 HumiD  );  // CHUONG TRINH XU LY MAY BOM
///////////////////////
void _QUAT( unsigned int8 TempS, unsigned int8 TempLv, unsigned int8 TempD );  // CHUONG TRINH SU LY QUAT
//////////////////////
void _BtMode(int8 Mode);  // TRUYEN GIA TRI MODE VAO CHUONG TRINH NUT NHAN 

/////////////////////////////////

///////////////////////////////
void dochumi()
{
  ADC1=0;
  set_adc_channel(1);//Doc gia tri adc o chan A1
  delay_ms(20);
  ADC1=read_adc();
  
  doam = 100-(((ADC1*90.0f)/1023.0f));
  delay_ms(100);
}
/////////////////////////////
void doc_dht11()
{
    Start_signal();          /// khoi chay ham khoi dong cam bien dht11
    
    if(check==1)           /// neu cam bien phan hoi tin hieu thi tien hanh doc du lieu
    {                    
       Read_Data();
     
        if(DATA[4] == ( DATA[3]+ DATA[2]+DATA[1]+ DATA[0]))   /// kiem tra chinh xac cua du lieu 
       {                                                      /// neu tong 4 mang dau = mang cuoi thi du lieiu chinh xac tien hanh xuat du lieu
       
       TempS  = DATA[2];
       HumiS1  = DATA[0];
       
      }
      else
      {
       lcd_gotoxy(1,2);
       printf(lcd_putc,"      ");  
       delay_ms(1000);
       lcd_gotoxy(1,2);
       printf(lcd_putc,"SAI!! ");  
       delay_ms(1000);
      }
     delay_ms(300);
     }
     else
     {
      
       lcd_gotoxy(1,2);
       printf(lcd_putc,"      ");  
       delay_ms(1000);
       lcd_gotoxy(1,2);
       printf(lcd_putc,"HONG: ");  
       delay_ms(1000);
      
      
     }
     delay_ms(300);
}
void main()
{
  
  
   SET_TRIS_A(0xff);  //// set tris a input
   SET_TRIS_D(0x00);  //// set tris d output
   SET_TRIS_B(0xff);  //// set tris b input
   SET_TRIS_C(0X00);  //// set tris c output
  
   output_low(Relay_bom);
   output_low(Relay_quat);
   // set ngat 
   enable_interrupts(int_ext);
   ext_int_edge(h_to_l);
   enable_interrupts(global);
   Mode = 0;  
   
   // DOC GIAI TRI eeprom
   TempLv = read_eeprom(0x01); // doc gia tri tu epprom
   TempD  = read_eeprom(0x02); 
   
   HumiLv = read_eeprom(0x03); 
   HumiD  = read_eeprom(0x04);
  
   
   //  SET UP ADC  XUNG NOI
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_adc_ports(RA0_RA1_RA3_ANALOG);
   
   
   // KHOI TGAO LCD 
   lcd_init();
   delay_ms(200);
   lcd_putc('\f');
   lcd_gotoxy(1,1);
   printf(lcd_putc,"   AUTO WATERING    ");
   lcd_gotoxy(1,2);
   printf(lcd_putc,"  NGUYEN PHI HUNG   ");
   lcd_gotoxy(1,4);
   printf(lcd_putc,"    MSV:10120649    ");
   lcd_gotoxy(1,3);
   printf(lcd_putc,"     IOT:101203     ");
   delay_ms(10000);
   lcd_putc('\f');
   
   
   while(TRUE)
   {
      
      //Nhiet do, Do am tu cam bien
      
      
       
       
      switch(Mode)  // NUT NHAN RB0
      {
      /////////////////////////////////////
         case 0:  
            lcd_gotoxy(1,1);
            printf(lcd_putc,"SETUP ON OFF SENSOR ");
            lcd_gotoxy(1,2);
            printf(lcd_putc,"TEMP: ");
            lcd_gotoxy(1,3);
            printf(lcd_putc,"HUMI: ");
             doc_dht11();
            
             dochumi();
             
             HumiS  = doam;
           
      
            
            lcd_gotoxy(7,2);
            printf(lcd_putc,"%02d",TempLv) ; lcd_putc("-");
            printf(lcd_putc,"%02d",TempD)  ;  lcd_putc("C");
           
            lcd_gotoxy(7,3);
            printf(lcd_putc,"%02d",HumiLv);   lcd_putc("-");
             printf(lcd_putc,"%02d",HumiD);  lcd_putc("% ");
          
            
            lcd_gotoxy(15,2);
            printf(lcd_putc,"%02d",TempS); lcd_putc("C  ");
            lcd_gotoxy(15,3);
            printf(lcd_putc,"%02d",HumiS);lcd_putc("%  ");   
            
            _BOM ( HumiLv,   HumiS,  HumiD  );
            delay_ms(50);
            
           
            _QUAT(  TempS,  TempLv,  TempD );  
             delay_ms(50);
            
           
           
            break;
        //////////////////////////////////////////////////////////
        
/// BAT TAT THIET BI BANG NUT NHAN 
         case 1:
            
             doc_dht11();
            
             dochumi();
           
             HumiS  = doam;
          
            lcd_gotoxy(1,2);
            printf(lcd_putc,"TEMP:");
            lcd_gotoxy(12,2);
            printf(lcd_putc,"RH:");
            
            lcd_gotoxy(7,2);
            printf(lcd_putc,"%02d",TempS); lcd_putc("C  ");
            lcd_gotoxy(15,2);
            printf(lcd_putc,"%02d",HumiS1); lcd_putc("%  ");
            
            lcd_gotoxy(4,1);
            printf(lcd_putc,"DO AM DAT: ");
            printf(lcd_putc,"%02d",HumiS); lcd_putc("% ");
             
            lcd_gotoxy(1,3);
            printf(lcd_putc," NHAN NUT DE BAT TAT");
            
           
            
            if(BTmotor == 0)   // NUT NHAN BAT TAT MAY BOM BANG TAY 
            {
               delay_ms(20);
               if ( BTmotor == 0 )
               {
               while(BTmotor == 0);
               if(Status == 1)
               {
                  Status = 0;              
                  output_high(Relay_bom);
                  lcd_gotoxy(1,4);
                  printf(lcd_putc,"BOM ON ");
               }  
               else
               {
                  Status = 1;
                  output_low(Relay_bom);
                  lcd_gotoxy(1,4);
                  printf(lcd_putc,"BOM OFF");
               }               
            }  }
            //////////////////////////////////////
            if(BTquat == 0)   // NUT NHAN BAT TAT QUAT BANG TAY 
            {
               delay_ms(20);
               if ( BTquat == 0 )
               {
               while(BTquat == 0);
               if(Status1 == 1)
               {
                  Status1 = 0;              
                  output_high(Relay_quat);
                  lcd_gotoxy(12,4);
                  printf(lcd_putc,"QUAT ON ");
               }  
               else
               {
                  Status1 = 1;
                  output_low(Relay_quat);
                  lcd_gotoxy(12,4);
                  printf(lcd_putc,"QUAT OFF");
               }               
            }  }
            /////////////////////////////////////////////////////
            break;  
            /////////////////////////////////////////////////////
            
 //  TANG GIAM GIA TRI NHIET DO CAI DAT ON QUAT LAM MAT
             
         case 2:
            if(BTUp == 0)  
            {
               delay_ms(20);   ///nut tang 
               if(BTUp == 0)
               {
               while(BTUp == 0);
               TempLv = TempLv + 1;
               if(TempLv >= 50)
               TempLv = 50;
               }  }
            else if(BTDown == 0)  /// nut giam 
            {
               delay_ms(20);
               if(BTDown ==0 )
               {
               while(BTDown == 0);
               TempLv = TempLv - 1;
               if(TempLv <= 1)
               TempLv = 1;
            } 
            }
            
            write_eeprom(0x01, TempLv);   // LUU IGIA TRI vao eprom
            
            lcd_gotoxy(1,1);
            printf(lcd_putc,"Nhiet Do ON QUAT   ");
            lcd_gotoxy(1,2);
            printf(lcd_putc,"Temp: %02d",TempLv);
            lcd_putc(223);
            lcd_putc("C   ");
            lcd_gotoxy(14,2);
            printf(lcd_putc,"      ");
            lcd_gotoxy(1,3);
            printf(lcd_putc,"                    ");
            lcd_gotoxy(1,4);
            printf(lcd_putc,"                    ");
            ////////////////////////////////////////////
            break;
            ///////////////////////////////////////////
            
 // TANG GIAM GIA TRI CAI DAT OFF QUAT LAM MAT
            case 3:
            if(BTUp == 0)  
            {
               delay_ms(20);   ///nut tang 
               if(BTUp == 0)
               {
               while(BTUp == 0);
               TempD = TempD + 1;
               if(TempD >= 50)
               TempD = 50;
               }  }
            else if(BTDown == 0)  /// nut giam 
            {
               delay_ms(20);
               if(BTDown ==0 )
               {
               while(BTDown == 0);
               TempD = TempD - 1;
               if(TempD <= 1)
               TempD = 1;
            } 
            }
            
            write_eeprom(0x02, TempD);   // LUU IGIA TRI vao eprom
            
            lcd_gotoxy(1,1);
            printf(lcd_putc,"Nhiet Do OFF QUAT  ");
            lcd_gotoxy(1,2);
            printf(lcd_putc,"Temp: %02d",TempD);
            lcd_putc(223);
            lcd_putc("C   ");
            lcd_gotoxy(14,2);
            printf(lcd_putc,"      ");
            lcd_gotoxy(1,3);
            printf(lcd_putc,"                    ");
            lcd_gotoxy(1,4);
            printf(lcd_putc,"                    ");
            ///////////////////////////////////////
            break;
            //////////////////////////////////////
            
// TANG GIAM GIAI TRI DO AM CAI DAT ON BOM  
         case 4:   
            if(BTUp == 0)// nut tang giai tri nhiet do cai dat
            {
               delay_ms(20);
               if(BTUp == 0)
               {
               while(BTUp == 0);
               HumiLv = HumiLv + 1;
               if(HumiLv >= 99)
               HumiLv = 99;
               }
            }
            else if(BTDown == 0)// nut giam gia tri nhiiet do cai dat
            {
               delay_ms(20);
               if(BTDown == 0)
               {
               while(BTDown == 0);
               HumiLv = HumiLv - 1;
               if(HumiLv <= 10)
               HumiLv = 10;
               }
            }  
            
            write_eeprom(0x03, HumiLv);  // LUU GIAI TRI VAO eprom  
            
            lcd_gotoxy(1,1);
            printf(lcd_putc,"Muc Do Am ON BOM   ");
            lcd_gotoxy(1,2);
            printf(lcd_putc,"Humi: %02d",HumiLv);
            lcd_putc("% ");
            ////////////////////////////////////////////////////
            break;
            ///////////////////////////////////////////////////
            
 ////// TANG GIAM GIAI TRI DO AM OFF MAY BOM 
 
            case 5:   
            if(BTUp == 0)// nut tang 
            {
               delay_ms(20);
               if( BTUp == 0)
               {
               while(BTUp == 0);
               HumiD = HumiD + 1;
               if(HumiD >= 99)
               HumiD = 99;
               }
            }
            else if(BTDown == 0)// nut giam 
            {
               delay_ms(20);
               if(BTDown == 0)
               {
               while(BTDown == 0);
               HumiD = HumiD - 1;
               if(HumiD <= 10)
               HumiD = 10;
               }
            }  
            
            write_eeprom(0x04, HumiD);  // LUU GIAI TRI VAO eprom  
            
            lcd_gotoxy(1,1);
            printf(lcd_putc,"Muc Do Am OFF BOM  ");
            lcd_gotoxy(1,2);
            printf(lcd_putc,"Humi: %02d",HumiD);
            lcd_putc("% ");
            //////////////////////////////////////////
            break;
            /////////////////////////////////////////   
      }
   }
}
 ////////////////////////////////
void _BOM( unsigned int8 HumiLv, unsigned int8 HumiS ,unsigned int8 HumiD ) //// su lý so sanh nhiet do do am 
{
   if ( HumiS <= HumiLv )    // neu nhiet do lon hon nhiet do cai dat va do am nho hon do am cai dat thì bat may bom 
                            //TempS >= TempLv && HumiS <= HumiLv
   {
      output_high(Relay_bom);
      lcd_gotoxy(1,4);
      printf(lcd_putc,"BOM ON ");
    
   }
   else if (HumiS >= HUmiD)   /// bom nuoc den khi do am >=70 thif dung lai 
   {
      output_low(Relay_bom);
      lcd_gotoxy(1,4);
      printf(lcd_putc,"BOM OFF");
   }
}
//////////////////////////////
void _QUAT( unsigned int8 TempS, unsigned int8 TempLv, unsigned int8 TempD )
{
if ( TempS >= TempLv )    //  neu nhiet do cam bien lon hon nhiet do cai dat on 
                           
   {
      output_high(Relay_quat);
      lcd_gotoxy(12,4);
      printf(lcd_putc,"QUAT ON ");
    
   }
   else if (TempS <= TempD)   /// bom nuoc den khi do am >=70 thi dung lai 
   {
      output_low(Relay_quat);
      lcd_gotoxy(12,4);
      printf(lcd_putc,"QUAT OFF");
   }
}
//////////////////////////////////////////////////////////////////////////////
