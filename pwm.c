
#include <msp430.h>


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

    // Configure GPIO
    P4DIR |= BIT0;                            // P4.0 and P8.3 output
   // P8DIR |= BIT3;
    P4SEL0 |= BIT0;                           // P4.0 and P8.3 options select
    //P8SEL0 &= ~BIT3; 
    //P8OUT &= ~BIT0;
    
    P2DIR &= ~BIT6;
   // P2DIR |= BIT6;
    //P2OUT |= BIT6;   
    P2IES |= BIT6;
    P2IFG &= ~BIT6;
    P2IE |= BIT6;
    
    P1DIR &= ~BIT2;
    //P1DIR |= BIT2;
    //P1OUT |= BIT2;   
    P1IES |= BIT2;
    P1IFG &= ~BIT2;
    P1IE |= BIT2;
    
    //P8DIR |= BIT0;
    //P8SEL0 |= BIT0;
    
    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings

    PM5CTL0 &= ~LOCKLPM5;

    
    TA1CCR0 = 1000-1;                         // PWM Period
    TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA1CCR1 = 0;                            // CCR1 PWM duty cycle
   // TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
   // TA1CCR2 = 500;                            // CCR2 PWM duty cycle
    TA1CTL = TASSEL__SMCLK | MC__UP | TACLR | TAIE;  // SMCLK, up mode, clear TAR

    __bis_SR_register(LPM0_bits |GIE);             // Enter LPM0

    
    __no_operation();                         // For debugger
    
}
int cnt=0;

#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void)
{
  cnt++;
  if(cnt>5000)
  {
    cnt=0;
    TA1CCR1 += 20;   
    if (TA1CCR1 >= 1000) TA1CCR1=10;// CCR1 PWM duty cycle
  }
  
}

#pragma vector = PORT1_VECTOR
__interrupt void Port_1 (void){
  if(P1IFG & BIT2){
   // TA1CCR0 = 1000-1;                         // PWM Period
    //TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA1CCR1 = 50;   
    if (TA1CCR1 >= 1000) TA1CCR1=0;// CCR1 PWM duty cycle
   // P8OUT ^= BIT3;
    //TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    //TA1CCR2 -= 200;                            // CCR2 PWM duty cycle
    //TA1CTL = TASSEL__SMCLK | MC__UP ;
    
    P1IFG &= ~BIT2;
  }
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT_2 (void){
  if(P2IFG & BIT6){
    //TA1CCR0 = 1000-1;                         // PWM Period
    //TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA1CCR1 -= 200;                            // CCR1 PWM duty cycle
    //TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    //TA1CCR2 += 200;                            // CCR2 PWM duty cycle
    //TA1CTL = TASSEL__SMCLK | MC__UP ;
    
    P2IFG &= ~BIT6;
  }
}

