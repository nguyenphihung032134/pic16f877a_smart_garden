     
#define DHT11_PIN PIN_B4    

int DATA[5];
int check=0 ;

void start_signal(){

  output_drive(DHT11_PIN);       // dat che do output
  output_low(DHT11_PIN);        // xuat muc thap
  delay_ms(30);
  
  output_high(DHT11_PIN);      // lem muc cao cho tin hieu phan hoi
  output_float(DHT11_PIN);     // dat che do input
  delay_us(40);
  
  if(!input(DHT11_PIN)){       // neu nhan duoc tin hieu phan hoi muc thap 80us
    delay_us(80); 
    
    if(input(DHT11_PIN)){      // tin hieu phan hoi len muc cao 80 us
      check=1;                //  co nghia cam bien hoat dong va cho bien check =1
      delay_us(80);
    }
  }
}

void Read_Data(){           // doc data

 for(int i = 0; i<5; i++){        //// 40bit chia ra lam 5 phan moi phan 8bit 
       for(int ii = 0; ii<8; ii++) {
        while(input(DHT11_PIN) == 0);       /// neu chan tin hieu xuong muc thap                     
        delay_us(50);                         
        if(input(DHT11_PIN) == 1)                               //neu >28us ma data =1 thi bit do se la bit 1
        {
            DATA[i] =DATA[i]<<1| 1;             /// ghi vao mang DATA dich trai 1 vi tri =1
            while(input(DHT11_PIN) == 1){}
        }
        else  DATA[i] =DATA[i]<<1| 0;                        // neu sau 28us ma data =0 thi bit do la bit 0
       }                                                     // ghi vao mang DATA dich trai 1 vi tri =0
 }
 }

 
    
//!    Start_signal();          /// khoi chay ham khoi dong cam bien
//!    
//!    if(check==1)           /// neu cam bien phan hoi tin hieu thi tien hanh doc du lieu
//!    {                    
//!       Read_Data();
//!     
//!        if(DATA[4] == ( DATA[3]+ DATA[2]+DATA[1]+ DATA[0]))   /// kiem tra chinh xac cua du lieu 
//!       {                                                      /// neu tong 4 mang dau = mang cuoi thi du lieiu chinh xac tien hanh xuat du lieu
//!       output_low(pin_b2);
//!       delay_ms(200);
//!       output_high(pin_b2);
//!       delay_ms(200);
//!
//!      lcd_gotoxy(1,1);
//!      lcd_putc("NHIET DO : ");
//!      printf(lcd_putc,"%02d.",DATA[2]);
//!      printf(lcd_putc,"%02d C",DATA[3]);
//!      lcd_gotoxy(1,2);
//!      lcd_putc("DO AM : ");
//!      printf(lcd_putc,"%02d. ",DATA[0]);
//!      printf(lcd_putc,"%02d ",DATA[1]);lcd_putc("%");
//!      
//!       }
//!       else{
//!        lcd_gotoxy(1,1);                     
//!        lcd_putc(" DU LIEU KHONG CHINH XAC");
//!       }
//!     }
//!     else{
//!     lcd_gotoxy(1,1);
//!        lcd_putc(" cam bien hong");
//!     }
//!   }
//!    
//!  
//! }
//!
