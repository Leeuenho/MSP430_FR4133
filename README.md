# MSP430_FR4133

datasheet : https://www.ti.com/lit/ds/symlink/msp430fr4133.pdf?ts=1639626293449&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FMSP430FR4133

user's guid : https://www.ti.com/lit/ug/slau445i/slau445i.pdf?ts=1639626210168&ref_url=https%253A%252F%252Fwww.google.com%252F


변수의 default 값은 1이다 (0으로 바꾸기위해 not(~) 이용)

*watchdog the control(= WDTCL) = system 이상 감지시 reset/ data load 할 때 정지시켜줘야함
WDTCTL = WDTPW | WDTHOLD;  // efault 된 password로 접속 하여 HOLD(정지) 시켜준다

