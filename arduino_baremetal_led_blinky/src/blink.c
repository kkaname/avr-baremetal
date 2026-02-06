typedef unsigned short uint16_t;

#define DDRB *((volatile unsigned char *)0x24)      //controlls whether the pins of Port B are input or output
#define PORTB *((volatile unsigned char *)0x25)     //controlls the output state of the pins of Port B
//we'll be setting the DDRB register to set the pin13 or PORTB5 as output to control the built-in LED

/* we'll be using timer_1 for this project, which has 3 control, 4 data registers and 2 interrupt registers.
 * control registers: TCCR1A, TCCR1B, TCCR1C
 * data registers: OCR1A, OCR1B, TCNT1, ICR1
 * interrupt registers: TIMSK1, TIFR1
 */

#define TCNT1 *((volatile uint16_t *)0x84) //Timer1 16-bit control register: counts 0-65535
//split into two 8-bit registers: TCNT1H(higher byte) and TCNT1L(lower byte)

//define timer_1 control register, A and B
#define TCCR1A *((volatile unsigned char *)0x80)
#define TCCR1B *((volatile unsigned char *)0x81)

//setting the OCR1A(Output Compare Register) High and Low byte
#define OCR1AH *((volatile unsigned char *)0x89)    //High byte     | compare value
#define OCR1AL *((volatile unsigned char *)0x88)    //Low byte      | for channel A

#define TIMSK1 *((volatile unsigned char *)0x6F)
#define TIFR1 *((volatile unsigned char *)0x36)

#define TIMER1_COUNT 62499
#define PORTB5 5
#define DDRB5 5
#define CS12 2 //to set the prescaler to 256, CS12 = 1, CS11 = 0, CS10 = 0 in TCCR1B
#define WGM12 3 //to set the mode to CTC, WGM12 = 1, WGM11 = 0, WGM10 = 0 in TCCR1B
#define TOV1 0  //Timer1 Overflow Flag in TIFR1
#define OCIE1A 1 //Timer1 Output Compare A Match Interrupt Enable in TIMSK1
#define OCF1A 1     //Output Compare A match flag in TIFR1

//defining a Interupt Service Routine(ISR) for timer1 interrupt
void __vector_11(void) __attribute__((signal, used, externally_visible));

void __vector_11(void)
{
    PORTB ^= (1 << PORTB5); // Toggle PORTB5
}

int main(void) {
    TCNT1 = 0;
    TCCR1A = 0;
    TCCR1B = 0;
    
    // set timer1 compare register high byte and lower byte
    // by slitting the value
    OCR1AH = TIMER1_COUNT >> 8;
    OCR1AL = TIMER1_COUNT & 0xFF;

    TCCR1B |= ((1 << CS12) | (1 << WGM12)); // Set Timer1 Clock Prescaler to 256
    TIFR1 ^= (1 << OCF1A); // Clear Timer1 Compare Match A Flag
    DDRB |= (1 << DDRB5); // Set PORTB5 as output (Pin 13 on Arduino Uno)

    // Setting Interrupts
    __asm__ __volatile__("cli" ::: "memory");
    //this line is to add assembly code to disable interrupts

    TIMSK1 = (1 << OCIE1A); // Enable Timer1 Overflow Interrupt

    __asm__ __volatile__("sei" ::: "memory");
    //this line is to add assembly code to disable interrupts

    PORTB |= (1 << PORTB5); // Set PORTB5 to High

    while (1) {
        __asm__ __volatile__("sleep" ::: "memory");
    }
}

