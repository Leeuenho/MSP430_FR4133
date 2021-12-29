
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

int main( void )
{
  WDTCTL = WDTPW | WDTHOLD;                                  // Stop watchdog timer 

    // Configure XT1 oscillator
    P4SEL0 |= BIT1 | BIT2 ;                                     // P4.2~P4.1: crystal pins
    
    do
    {
      CSCTL7 &= ~(XT1OFFG | DCOFFG);                         // Clear XT1 and DCO fault flag  (interupt flag = 0)
        SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);                                  // Test oscillator fault flag ( interupt flag =  1 될 경우 do while 탈출)
    
    CSCTL6 = (CSCTL6 & ~(XT1DRIVE_3)) | XT1DRIVE_2;            // Higher drive strength and current consumption for XT1 oscillator (crystal oscillator pulse 크기 조정)


    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
   
    PM5CTL0 &= ~LOCKLPM5;  //Power Management Module Control Register 활성화 = oscillator 활성화

    // Configure LCD pins
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
    
    

    for(int i = 0;  i<(sizeof(digit)/sizeof(char));  i++){
      
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

      }
     LCDMEMCTL |= LCDCLRM; 
    __bis_SR_register(LPM3_bits | GIE);                        // Enter LPM3.5
    __no_operation();                 // For debugger
   
}


