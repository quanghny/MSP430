#include <18F4431.h>
#fuses  NOWDT,PUT,HS,NOPROTECT,NOLVP
#use  delay(clock=20000000)
int1 START_STT=0,END_STT=0,mode_tay=0;
int1 en_Start=0,en_End=0;
//mode=1 ep chan; mode=0 ep dau;
//autoRun=1 tu dong;

//ngo ra dong co thuy luc
#define X_Thuan   PIN_D6   //Xuong, Vao, Qua la thuan
#define X_Nghich  PIN_D5   //Len, Ra, Lai la nghich
//!#define Z_Thuan   PIN_D4
//!#define Z_Nghich  PIN_D5
//!#define Y_Thuan   PIN_D6
//!#define Y_Nghich  PIN_D7
#define bom       PIN_D7
//ngo vao
#define X_Cuoi PIN_A4
#define X_Dau1 PIN_A5   
//!#define X_Dau2 PIN_A2   //dap chan
//!#define Y_Dau  PIN_A3
//!#define Y_Cuoi PIN_A4
//!#define Z_Cuoi PIN_A5
//!#define Z_Dau  PIN_B0


#define AUTO_BTN  PIN_A0
//!#define MODE_BTN  PIN_B6
#define END_BTN   PIN_E1
#define START_BTN PIN_E0           
//!#define XUONG_BTN PIN_D0     
//!#define LEN_BTN   PIN_B4      
//!#define QUA_BTN   PIN_B3  //toi vi tri 2      
//!#define LAI_BTN   PIN_B2  //ve vi tri 1    
//!#define VAO_BTN   PIN_B7      
//!#define RA_BTN    PIN_B4      
void ep_Vao();
void nha_Ra();
void main()
{
   set_tris_D(0x00);
   START_STT=input(START_BTN);
   END_STT=input(END_BTN);
   en_Start=0;
   en_End=0;
   output_d(0xFF);
   while(TRUE)
   {
      WHILE ((input(AUTO_BTN))==0) //Che do tu dong chay bom
      {  
         OUTPUT_LOW(bom); 
         if (input(START_BTN)!=START_STT)
            {  
               START_STT=input(START_BTN);
               if (input(START_BTN)==0) en_Start=1;
            }   
         if (input(END_BTN)!=END_STT)
            {  
               END_STT=input(END_BTN);
               if (input(END_BTN)==0) en_End=1;
            }   
         if (en_Start) //dap chan
         {            
            en_Start=0;
            ep_Vao();
         }
         if (en_End)
         {
            en_End=0;
            nha_Ra();
         }
      }
      WHILE ((input(AUTO_BTN))==1) //Che do tu dong chay bom
      {  
         if (input(START_BTN)!=START_STT)
            {  
               START_STT=input(START_BTN);
               if (input(START_BTN)==0) en_Start=1;
            }   
         if (input(END_BTN)!=END_STT)
            {  
               END_STT=input(END_BTN);
               if (input(END_BTN)==0) en_End=1;
            }   
         if (en_Start) //dap chan
         {            
            en_Start=0;
            ep_Vao();
         }
         if (en_End)
         {
            en_End=0;
            nha_Ra();
         }
      }
   }
}
void ep_Vao()
{
   //Ep vao
   if (input(X_Cuoi)) output_low(X_Thuan);
   while (input(X_Cuoi));
   output_high(X_Thuan);
   delay_ms(200);   
   output_low(bom);
}

void nha_Ra()
{
   //Nha ra
   if (input(X_Dau1)) output_low(X_Nghich);
   while (input(X_Dau1));
   output_high(X_Nghich);
   delay_ms(200);
}
