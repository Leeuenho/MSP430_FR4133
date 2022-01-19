
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

int i =0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                         // Stop WDT

    // Configure GPIO
    //P1DIR |= BIT6 | BIT7;                             // P1.6 and P1.7 output
    //P1SEL0 |= BIT6 | BIT7;                            // P1.6 and P1.7 options select
    //P4SEL0 |= BIT1 | BIT2;                            // P4.2~P4.1: crystal pins
    
    //P1DIR |= BIT0;
    //P4DIR |= BIT0;
    P1SEL0 |= BIT0;
    P4SEL0 |= BIT0;
    
    P4SEL0 |= BIT1 | BIT2;
    
    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);                // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);                         // Test oscillator fault flag
    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Configure clock
    __bis_SR_register(SCG0);                          // disable FLL
    CSCTL3 |= SELREF__REFOCLK;                        // Set REFOCLK as FLL reference source
    CSCTL0 = 0;                                       // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);                           // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_3;                              // Set DCOCLK = 8MHz
    CSCTL2 = FLLD_1 + 121;                            // FLLD = 1, DCODIV = 4MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                          // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));        // Poll until FLL is locked
    CSCTL4 = SELMS__DCOCLKDIV | SELA__XT1CLK;         // set ACLK = XT1 = 32768Hz, DCOCLK as MCLK and SMCLK source
    CSCTL5 |= DIVM0 | DIVS0;                          // SMCLK = MCLK = DCODIV = 1MHz, by default

    // Setup Timer0_A
    TA0CCR0 = 100-1;                                  // PWM Period
    TA0CCTL1 = OUTMOD_7;                              // CCR1 reset/set
    TA0CCR1 = 75;                                     // CCR1 PWM duty cycle
    TA0CCTL2 = OUTMOD_7;                              // CCR2 reset/set
    TA0CCR2 = 25;                                     // CCR2 PWM duty cycle
    TA0CTL = TASSEL_1 | MC_1 | TACLR | TAIE;                 // ACLK, up mode, clear TAR

    
    SYSCFG2 |= LCDPCTL;                                        // Devices Without Smart Analog Combo SYSCFG2 Register의 R13/R23/R33/LCDCAP0/LCDCAP1 pins selected

    LCDPCTL0 = 0xFFFF;  //L0 ~ L15
    LCDPCTL1 = 0x07FF;  //L16 ~ L26
    LCDPCTL2 = 0x00F0;   //LL36 ~ L39                                      // L0~L26 & L36~L39 pins selected

    //LCDSSEL : 클럭, LCDON = 0일 때만 변겅 가능 
    //LCDDIV : LCD 주파수 분배기, LCDON = 0일 때만 변겅 가능 
    
    LCDCTL0 = LCDSSEL_0 | LCDDIV_7;                            // flcd ref freq is xtclk (flcd 참조 주파수는 xtclk)

    // LCD Operation - Mode 3, internal 3.08v, charge pump 256Hz
    LCDVCTL = LCDCPEN | LCDREFEN | VLCD_6 | (LCDCPFSEL0 | LCDCPFSEL1 | LCDCPFSEL2 | LCDCPFSEL3); //charge pump |R13내부기준전압 활성화| 내부기준 전압선택(VLCD = 2.96V)|증폭할 주파수(1111 = 256Hz)



    // LCD Memory for Static to 4-Mux Mode ? Example for 4-Mux Mode With 240 Segments

                                             // L2 = COM2, L3 = COM3
    
    

      
        LCDMEMCTL |= LCDCLRM;                                      // Clear LCD memory
        
        LCDCSSEL0 = 0x000F;                                        // Configure COMs and SEGs  // L0, L1, L2, L3: COM pins
        LCDCSSEL1 = 0x0000;                                       
        LCDCSSEL2 = 0x0000;
        
        LCDM0 = 0x21;                                              // L0 = COM0, L1 = COM1
        LCDM1 = 0x84; 
        
        LCDMEM[pos6] = digit[i];
        
        LCDCTL0 |= LCD4MUX | LCDON;                                // Turn on LCD, 4-mux selected
        
         
    //PMMCTL  제어를 위한 PMM암호
    // PMM = Power Management Module
      PMMCTL0_H = PMMPW_H;                                       // Open PMM Registers for write
      PMMCTL0_L |= PMMREGOFF_L;                                  // and set PMMREGOFF
      
      
       __delay_cycles(1000000);

    
    __bis_SR_register(LPM3_bits | GIE);                     // Enter LPM3
    __no_operation();                                 // For debugger
}

int cnt=0;

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{
  cnt++;
  if(cnt>30000)
  {
    cnt = 0;
    i++;
    LCDMEM[pos6] = digit[i];
    
    if(i>=9){
      i=-1;
    }
  }
  
}

