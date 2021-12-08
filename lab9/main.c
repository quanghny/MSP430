#include <msp430.h>
#include <stdbool.h>
unsigned char set_P2=0;
unsigned char H1=0,H2=0,P1=0,P2=0,G1=0,G2=0,K1=0,K2=0,K3=0;
unsigned char *Tx_data;
unsigned char Tx_byte;
unsigned char count=0;
_Bool KT = false;
unsigned char buffer[]={0xD6,0xFC,0xF4,0xE0,0x16,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00};

unsigned char set_giatri[] ={
 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F // 0 1 2 3 4 5 6 7 8 9
};
struct bitdata {
      unsigned char B0:1 ; //cac bien B0...B7 duoc hieu la BIT bang cach ":1"
      unsigned char B1:1 ; //su dung bit cuoi cung
      unsigned char B2:1 ;
      unsigned char B3:1;
      unsigned char B4:1 ;
      unsigned char B5:1 ;
      unsigned char B6:1 ;
      unsigned char B7:1 ;
};
union kethop{
	unsigned char byte;
	struct bitdata bit;
};
union kethop Mang[41]={0xD6,0xFC,0xF4,0xE0,0x16,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00};
union kethop transmit_bit;

void display (unsigned char VT,unsigned char Num){
	transmit_bit.byte = set_giatri[Num];
	switch (VT){
          case 1: // dơn vi cua giay
          Mang[7].bit.B3 = transmit_bit.bit.B3;
          Mang[9].bit.B3 = transmit_bit.bit.B2;
          Mang[9].bit.B4 = transmit_bit.bit.B4;
          Mang[11].bit.B3 =transmit_bit.bit.B1;
          Mang[11].bit.B4 = transmit_bit.bit.B6;
          Mang[13].bit.B3 = transmit_bit.bit.B0;
          Mang[13].bit.B4 = transmit_bit.bit.B5;
          break;

          case 2:// hang chuc giay
          Mang[7].bit.B6=1;
          Mang[7].bit.B5= transmit_bit.bit.B3;
          Mang[9].bit.B5 = transmit_bit.bit.B2;
          Mang[9].bit.B6 = transmit_bit.bit.B4;
          Mang[11].bit.B5 = transmit_bit.bit.B1;
          Mang[11].bit.B6 = transmit_bit.bit.B6;
          Mang[13].bit.B5 = transmit_bit.bit.B0;
          Mang[13].bit.B6 = transmit_bit.bit.B5;
          break;

          case 3:// don vi phut
          Mang[8].bit.B7 = transmit_bit.bit.B3;
          Mang[10].bit.B7= transmit_bit.bit.B2;
          Mang[10].bit.B6= transmit_bit.bit.B4;
          Mang[12].bit.B7= transmit_bit.bit.B1;
          Mang[12].bit.B6= transmit_bit.bit.B6;
          Mang[14].bit.B7= transmit_bit.bit.B0;
          Mang[14].bit.B6= transmit_bit.bit.B5;
          break;

          case 4: // hang chuc phut
          Mang[8].bit.B4 = 1;
          Mang[8].bit.B5 = transmit_bit.bit.B3;
          Mang[10].bit.B5= transmit_bit.bit.B2;
          Mang[10].bit.B4= transmit_bit.bit.B4;
          Mang[12].bit.B5= transmit_bit.bit.B1;
          Mang[12].bit.B4= transmit_bit.bit.B6;
          Mang[14].bit.B5= transmit_bit.bit.B0;
          Mang[14].bit.B4= transmit_bit.bit.B5;
         break;

          case 5:// hang don vi gio
           Mang[8].bit.B3 = transmit_bit.bit.B3;
           Mang[10].bit.B3= transmit_bit.bit.B2;
           Mang[10].bit.B2= transmit_bit.bit.B4;
           Mang[12].bit.B3= transmit_bit.bit.B1;
           Mang[12].bit.B2= transmit_bit.bit.B6;
           Mang[14].bit.B3= transmit_bit.bit.B0;
           Mang[14].bit.B2= transmit_bit.bit.B5;
         break;

          case 6://hang chuc gio
           Mang[7].bit.B7= transmit_bit.bit.B3;
           Mang[9].bit.B7= transmit_bit.bit.B2;
           Mang[10].bit.B0= transmit_bit.bit.B4;
           Mang[11].bit.B7= transmit_bit.bit.B1;
           Mang[12].bit.B0 = transmit_bit.bit.B6;
           Mang[13].bit.B7 = transmit_bit.bit.B0;
           Mang[14].bit.B0 = transmit_bit.bit.B5;
         break;

	}
}

void setup_SMCLK(){   // cau hinh chon bo giao dong ngoại smclk
	FLL_CTL0 |= XCAP11PF;
	FLL_CTL1 &=~XT2OFF;
	FLL_CTL1 |= SELS;
	FLL_CTL2 |= XT2S_2;
}
void setup_I2CB1(){ // thiet lap ucsi chon chuc nang giao tip i2c
	P2SEL |= BIT1+BIT2;
	UCB1CTL1 |= UCSWRST;
	UCB1CTL0 = UCMST + UCMODE_3 + UCSYNC;
	UCB1CTL1 = UCSSEL_2 + UCSWRST;
	UCB1BR0 = 200;
	UCB1BR1 = 0;
	UCB1I2CSA = 0x3C;
	UCB1CTL1 &=~ UCSWRST;
	UC1IE |= UCB1TXIE;
}
#pragma vector = USCIAB1TX_VECTOR  // ham ngat co chuc nang gui du lieu
__interrupt void USCIAB1TX_ISR(void)
{
  if (Tx_byte)
  {
    UCB1TXBUF = *Tx_data++;
    Tx_byte--;
  }
  else
  {
    UCB1CTL1 |= UCTXSTP;
    UC1IFG &=~ UCB1TXIFG;
  }
}
// khoi tao vao bat dau gui du lieu tu vdk sai pcf 8578
int transmit_i2c( unsigned char dataSend[], unsigned char count){
	Tx_data = (unsigned char *)dataSend;
	Tx_byte= count;
	UCB1CTL1 |= UCTR + UCTXSTT;
	__bis_SR_register(GIE);
	while (UCB1CTL1 & UCTXSTP);
	return 0;
}
void convert_data(){ // chuyen tu kieu du du lieu union sang unsigned char
	int i;
	for (i=0;i<41;i++){
     buffer[i]=Mang[i].byte;
	 }
}


void setup_timer (unsigned char T){    // timer đinh thoi gian vao khoang 1s
	TA0CTL=TASSEL_2 + MC_1 + ID_3;
	TA0CCR0 = 57600;
	TA0CCTL0 = CCIE;
	_enable_interrupt();

}
#pragma vector = TIMER0_A0_VECTOR     // vector ngat CCR0 cua Timer
__interrupt void TimerInterruptCCR0(void){
	count ++;
	if (count== 31){
		KT= true;
		count=0;
	}
}
void hour(){ // ham hien thi gio và cho gio hoat dong
	if (K3==24){
		K3=0;
	}
	if (K3<24){
		H2=K3/10;
		H1=K3%10;
	}
	display(5,H1);
    display(6,H2);
}
void minutes(){  // ham hien thi phut và cho phut hoat dong
	if(K2==60){
		K2=0;
		K3++;
	}
	if (K2<60){
		P1=K2%10;
		P2=K2/10;
	}
		display(3,P1);
		display(4,P2);
}
void second (){ //ham hien thi giây cho giay hoạt dong
	if (K1==60){
		K1=0;
		K2++;
	}
	if(K1<60){
		G1=K1%10;
		G2=K1/10;
	}
	display(1,G1);
    display(2,G2);
    K1++;
}
void Time(){ // ham cho thoi gian chay tu dong
    second();
    minutes();
    hour();
	convert_data();
	transmit_i2c(buffer,sizeof (buffer));
}
// ham su dung de clear cac bit tại cac vị tri de thưc hiện việc nhap nhay
void no_display(unsigned char VT){
	switch (VT){
	case 1:
	Mang[7].bit.B3 = 0;
    Mang[9].bit.B3 = 0;
	Mang[9].bit.B4 = 0;
    Mang[11].bit.B3 =0;
	Mang[11].bit.B4 = 0;
	Mang[13].bit.B3 = 0;
	Mang[13].bit.B4 =0;

    Mang[7].bit.B6=1;
    Mang[7].bit.B5= 0;
    Mang[9].bit.B5 = 0;
    Mang[9].bit.B6 = 0;
    Mang[11].bit.B5 = 0;
    Mang[11].bit.B6 = 0;
    Mang[13].bit.B5 = 0;
    Mang[13].bit.B6 = 0;

    break;

    case 2:// don vi phut
    Mang[8].bit.B7 = 0;
    Mang[10].bit.B7= 0;
    Mang[10].bit.B6= 0;
    Mang[12].bit.B7= 0;
    Mang[12].bit.B6= 0;
    Mang[14].bit.B7= 0;
    Mang[14].bit.B6= 0;

    Mang[8].bit.B4 = 1;
    Mang[8].bit.B5 = 0;
    Mang[10].bit.B5= 0;
    Mang[10].bit.B4= 0;
    Mang[12].bit.B5= 0;
    Mang[12].bit.B4= 0;
    Mang[14].bit.B5= 0;
    Mang[14].bit.B4= 0;
   break;

    case 3:// hang don vi gio
        Mang[8].bit.B3 = 0;
        Mang[10].bit.B3= 0;
        Mang[10].bit.B2= 0;
        Mang[12].bit.B3= 0;
        Mang[12].bit.B2= 0;
        Mang[14].bit.B3= 0;
        Mang[14].bit.B2= 0;

  	    Mang[7].bit.B7= 0;
  	    Mang[9].bit.B7= 0;
  	    Mang[10].bit.B0= 0;
  	    Mang[11].bit.B7= 0;
        Mang[12].bit.B0 = 0;
  	    Mang[13].bit.B7 = 0;
  	    Mang[14].bit.B0 = 0;
  	    break;

	}
}
// ham nay dung de nhap nhay cac vi tri khi vao che do thiet lap .
void blink_number(unsigned char set_time, unsigned char TD){
	unsigned char A1,A2;
	A1=TD%10;
	A2=TD/10;
	  if (set_time==2){
		    no_display(1);
		    convert_data();
		    transmit_i2c(buffer,sizeof (buffer));
		  	_delay_cycles(150000);
		     display(1,A1);
		     display(2,A2);
		  	 convert_data();
		  	 transmit_i2c(buffer,sizeof (buffer));
		  	_delay_cycles(150000);
	  }else if (set_time==3){
		     no_display(2);
		     convert_data();
		     transmit_i2c(buffer,sizeof (buffer));
		     _delay_cycles(150000);
		     display(3,A1);
	         display(4,A2);
		     convert_data();
		     transmit_i2c(buffer,sizeof (buffer));
		     _delay_cycles(150000);

	  }else if(set_time==4){
		    no_display(3);
		    convert_data();
		    transmit_i2c(buffer,sizeof (buffer));
		    _delay_cycles(150000);
		    display(5,A1);
		    display(6,A2);
		    convert_data();
		    transmit_i2c(buffer,sizeof (buffer));
		    _delay_cycles(150000);

	  }
}
 void setup_button(){
	 int x=0,y=0,z=0;
	    x=K1; y=K2; z=K3;                                     // gan cac gia trị ở thời trươc khi dừng chay cua ham Time cho viec set thoi gian .
	     		 while(set_P2<5){
	     			 if ((P2IN & BIT7)!= BIT7){                        // kiem tra xem set_P2 co duoc nhan 1 lần nữa
	     			 while ((P2IN & BIT7)!= BIT7);
	     			                                          // tiếp tục tăng giá trị set_P2 . neu set_P2 = 2,3,4 ung vơi cac vi tri giay phut gio.
	     			 }
	     			while (set_P2==2){                                 // Ung vs vị tri giay
	     				if ((P2IN & BIT7)!= BIT7){                      // kiem tra xem Set_P2 co duoc nhan
	     				   while ((P2IN & BIT7)!= BIT7) ;
	     				     set_P2++;
	     				     }

	     				if((P3IN & BIT4)!= BIT4){                        // kiem tra xem P3.4 co duoc nhan đe tang giá trị giay thiet lap len 1
	     					while ((P3IN & BIT4)!= BIT4) ;
	     						x++;
	     				}
	     				if ((P3IN & BIT3)!= BIT3){                        // kiem tra xem P3.3 co duoc nhan de giam giá trị giay thiet lap xuong 1
	     				    	  while ((P3IN & BIT3)!= BIT3) ;
	     				    	  x--;
	     				}
	     					if(x>59){                                    // kiem tra giá trị thiep lap neu lơn hon 59 thi reset ve 0
	     						x=0;
	     					}

	     				blink_number(2,x);                                // ham thuc hien viec nhap nhay
	     			}
	     			while (set_P2==3){                                     // ung voi vi tri phut set nút nhu vị tri giay

	     				if ((P2IN & BIT7)!= BIT7){
	     				  while ((P2IN & BIT7)!= BIT7) ;
	     				    set_P2++;
	     	              }
	     				    if((P3IN & BIT4)!= BIT4){
	     				    while ((P3IN & BIT4)!= BIT4) ;
	     				     	y++;}
	     				    if ((P3IN & BIT3)!= BIT3){
	     				         while ((P3IN & BIT3)!= BIT3) ;
	     				          	  y--;
	     				             }
	     				    if(y>59){
	     				   y=0;
	     				  }
	     				blink_number(3,y);
	     			}
	     			while (set_P2==4){                                      // ung voi vị tri gio cung xet nut như vị tri giay
	     				  if ((P2IN & BIT7)!= BIT7){
	     			      while ((P2IN & BIT7)!= BIT7) ;
	     				     set_P2++;
	     				      }
	     				    if((P3IN & BIT4)!= BIT4){
	     				    while ((P3IN & BIT4)!= BIT4) ;
	     				    	z++;
	     				    }
	     				    if ((P3IN & BIT3)!= BIT3){
	     				         while ((P3IN & BIT3)!= BIT3) ;
	     				         z--;
	     				     		}
	     				    	if(z>24){
	     				    	z=0;
	     				  	}
	     				blink_number(4,z);
	     			}
	     		 }
	     		 K1=x;K2=y;K3=z;                          // gan cac gia tri da thiep lạp lại cho cac gia tri cua ham chay tu dong
	     	     set_P2=0;                                    // reset gia tri set_P2 lai bang 0.
}

int main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
  P2DIR &=~ BIT7;  // khoi tạo nut nhan
  P3DIR &=~ BIT3;  // khoi tao nut nhan
  P3DIR &=~ BIT4;  // khoi tao nut nhan
  P5DIR |= BIT0;
  setup_SMCLK();
  setup_I2CB1();
  setup_timer(32);
     while (1) {
    	 if (KT==true){
    		 Time();
    		 KT=false;
    	 }
    	 if ((P2IN & BIT7)!= BIT7){                                // kiem tra xem nut set P2.7 co duoc nhan
    		 while ((P2IN & BIT7)!= BIT7) ;      //   chông rung nut nhấn cho toi khi duc nha ra
    		 set_P2++;
    		 setup_button();
     }
   }
}

