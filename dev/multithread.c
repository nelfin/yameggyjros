#include <avr/io.h>
#include <avr/interrupt.h>
#include "multithread.h"

#define TIME_SLICE 200 //Arbritrary time units at the moment
#define TRUE 0
#define FALSE 1
static int timer_ticks = 0;
static int swapped_sph_A;
static int swapped_spl_A;

//static int swapped_sph_B;
//static int swapped_spl_B;

static void (*thread_A)(void);
static void (*thread_B)(void);

static int thread_A_active;

void init_multithread(void)
{
    //serial_putdebug("SPH!",SPH);
    //serial_putdebug("SPL!",SPL);
    /* Prepping Timer 0 (8-bit resolution) for context switching */

    // Set the Timer Mode to CTC
    TCCR0A |= (1 << WGM01);
    // Set the value that you want to count to
    OCR0A = 0xF9;
    TIMSK0 |= (1 << OCIE0A);    //Set the ISR COMPA vect
    sei();         //enable interrupts
    TCCR0B |= (1 << CS02);
    // set prescaler to 256 and start the timer

}

void execute_parallel(void (*function1)(void),void (*function2)(void))
{
    thread_A = function1;
    thread_B = function2;
    thread_A_active = TRUE;
    thread_A();
}

#define SAVE_CONTEXT()\
    asm("push r0");\
    asm("in r0, __SREG__");\
    asm("push r0");\
    asm("push r1");\
    asm("push r2");\
    asm("push r3");\
    asm("push r4");\
    asm("push r5");\
    asm("push r6");\
    asm("push r7");\
    asm("push r8");\
    asm("push r9");\
    asm("push r10");\
    asm("push r11");\
    asm("push r12");\
    asm("push r13");\
    asm("push r14");\
    asm("push r15");\
    asm("push r16");\
    asm("push r17");\
    asm("push r18");\
    asm("push r19");\
    asm("push r20");\
    asm("push r21");\
    asm("push r22");\
    asm("push r23");\
    asm("push r24");\
    asm("push r25");\
    asm("push r26");\
    asm("push r27");\
    asm("push r28");\
    asm("push r29");\
    asm("push r30");\
    asm("push r31");\
    
#define RESTORE_CONTEXT()\
    asm("pop r31");\
    asm("pop r30");\
    asm("pop r29");\
    asm("pop r28");\
    asm("pop r27");\
    asm("pop r26");\
    asm("pop r25");\
    asm("pop r24");\
    asm("pop r23");\
    asm("pop r22");\
    asm("pop r21");\
    asm("pop r20");\
    asm("pop r19");\
    asm("pop r18");\
    asm("pop r17");\
    asm("pop r16");\
    asm("pop r15");\
    asm("pop r14");\
    asm("pop r13");\
    asm("pop r12");\
    asm("pop r11");\
    asm("pop r10");\
    asm("pop r9");\
    asm("pop r8");\
    asm("pop r7");\
    asm("pop r6");\
    asm("pop r5");\
    asm("pop r4");\
    asm("pop r3");\
    asm("pop r2");\
    asm("pop r1");\
    asm("pop r0");\
    asm("out __SREG__, r0");\
    asm("pop r0");

//naked: tells gcc not to modify the stack while entering this
ISR(TIMER0_COMPA_vect,ISR_NAKED) {

    timer_ticks++;
    timer_ticks = timer_ticks % TIME_SLICE;
    if(timer_ticks==0){
        //----------Context switch----------
        //Keep track of which thread is active
        thread_A_active = !thread_A_active;
        
        //Save the stack pointers to kernel space
        swapped_sph_A = SPH;
        swapped_spl_A = SPL;
        
        //Save the registers to the stack
        SAVE_CONTEXT();
        
        //serial_putdebug("RAMEND",RAMEND);
        
        //Switch to Thread B by pushing the address to the stack so that reti(); returns to a different place.
        //Addresses are 2 bytes (14 bits I believe in reality), use unsigned short
        /*uint16_t thread_B_address = (uint16_t) thread_B;
        serial_putdebug("B address",thread_B_address);
        uint8_t thread_B_address_byte_low = (uint8_t) (thread_B_address & 0x00ff );
        uint8_t thread_B_address_byte_high = (uint8_t) (thread_B_address & 0xff00 ) >> 8;	
        serial_putdebug("B address high",thread_B_address_byte_high);
        serial_putdebug("B address low",thread_B_address_byte_low);
        asm("lds 16, thread_B_address_byte_low");
        asm("lds 17, thread_B_address_byte_high");
        asm("push 16");
        asm("push 17");*/
        
        //RESTORE_CONTEXT();
        //uint8_t *stack_pointer = (uint8_t *) ((SPH << 8 ) | (SPL & 0xff));
        //*stack_pointer = thread_B_address_byte_low;
        //*(stack_pointer+1) = thread_B_address_byte_high;
        //serial_putdebug("stack pointer",stack_pointer);
    }
    reti();
}
