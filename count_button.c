#include <msp430.h>

#define pos1 4                                                 // Digit A1 - L4
#define pos2 6                                                 // Digit A2 - L6
#define pos3 8                                                 // Digit A3 - L8
#define pos4 10                                                // Digit A4 - L10
#define pos5 2                                                 // Digit A5 - L2
#define pos6 18                                                // Digit A6 - L18

const char digit[10] =
{
    0xFC,                                                      // "0"
    0x60,                                                      // "1"
    0xDB,                                                      // "2"
    0xF3,                                                      // "3"
    0x67,                                                      // "4"
    0xB7,                                                      // "5"
    0xBF,                                                      // "6"
    0xE4,                                                      // "7"
    0xFF,                                                      // "8"
    0xF7                                                       // "9"
};


int i = 0;

int main(void){
  WDTCTL = WDTPW | WDTHOLD;
 
  P4SEL0 |= BIT1 | BIT2 ;                                     // P4.2~P4.1: crystal pins
 
  //입출력 설정
  P1DIR &= ~BIT2;
  P1DIR |= BIT2;
  P1OUT |= BIT2;
  
  P1IES |= BIT2;
  P1IFG &= ~BIT2;
  P1IE |= BIT2;

  //입력만 설정
  P2DIR &= ~BIT6;
  P2IES |= BIT6;
  P2IFG &= ~BIT6;
  P2IE |= BIT6;
   
  do
  {
    CSCTL7 &= ~(XT1OFFG | DCOFFG);                         // Clear XT1 and DCO fault flag  (interupt flag = 0)
    SFRIFG1 &= ~OFIFG;
  }while (SFRIFG1 & OFIFG);                                  // Test oscillator fault flag ( interupt flag =  1 될 경우 do while 탈출)
  
  
  PM5CTL0 &= ~ LOCKLPM5;
    
  SYSCFG2 |= LCDPCTL;                                        // Devices Without Smart Analog Combo SYSCFG2 Register의 R13/R23/R33/LCDCAP0/LCDCAP1 pins selected

  LCDPCTL0 = 0xFFFF;  //L0 ~ L15
  LCDPCTL1 = 0x07FF;  //L16 ~ L26
  LCDPCTL2 = 0x00F0;   //LL36 ~ L39                                      // L0~L26 & L36~L39 pins selected


  LCDCTL0 = LCDSSEL_0 | LCDDIV_7;          
  LCDVCTL = LCDCPEN | LCDREFEN | VLCD_6 | (LCDCPFSEL0 | LCDCPFSEL1 | LCDCPFSEL2 | LCDCPFSEL3);
  
      
  LCDMEMCTL |= LCDCLRM;
  LCDCSSEL0 = 0x000F;  
  LCDCSSEL1 = 0x0000;                                       
  LCDCSSEL2 = 0x0000;
  LCDM0 = 0x21;
  LCDM1 = 0x84; 
  
  LCDMEM[pos6] = digit[i];
  //__delay_cycles(1000);
  
  LCDCTL0 |= LCD4MUX | LCDON;   
  
  PMMCTL0_H = PMMPW_H;
  PMMCTL0_L |= PMMREGOFF_L;
  
  //LCDMEMCTL |= LCDCLRM; 
  
  __bis_SR_register(LPM0_bits | GIE);     
  
  __no_operation();
  
  /*
  while(1){
  }
*/
}
  
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void){
  if(P1IFG & BIT2){
   i++;
   
   if(i>9)      i = 0;
   
   LCDMEM[pos6] = digit[i];
   P1IFG &= ~BIT2;
  }
}

#pragma vector = PORT2_VECTOR
__interrupt void Port_2 (void){
  if(P2IFG & BIT6){
    i = i+1;
    if(i >9)      i = 0;
    
    LCDMEM[pos6] = digit[i];
    P2IFG &= ~BIT6;
  } 
}
 