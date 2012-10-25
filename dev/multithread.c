#include <avr/io.h>
#include <avr/interrupt.h>
#include "multithread.h"

#define TRUE 1
#define FALSE 0
static int swapped_sph_A;
static int swapped_spl_A;

static int swapped_sph_B;
static int swapped_spl_B;

static void (*thread_A)(void);
static void (*thread_B)(void);

static uint8_t thread_A_active;
static uint8_t first_switch;

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
    
    //Initialise the bottom half of the stack for our B thread
    uint16_t thread_B_SP = RAMEND/2;
    swapped_sph_B = (uint8_t) (thread_B_SP & 0x00ff );
    swapped_spl_B = (uint8_t) (thread_B_SP & 0xff00 ) >> 8;
    
    //Initialise the tracking boolean
    thread_A_active = TRUE;
    first_switch = TRUE;
    
}

void execute_parallel(void (*function1)(void),void (*function2)(void))
{
    thread_A = function1;
    thread_B = function2;
    thread_A();
}

#define SAVE_CONTEXT()\
    asm volatile ( \
        "push r0\n\t" \
        "in r0, __SREG__\n\t" \
        "push r0\n\t" \
        "push r1\n\t" \
        "clr r1\n\t" \
        "push r2\n\t" \
        "push r3\n\t" \
        "push r4\n\t" \
        "push r5\n\t" \
        "push r6\n\t" \
        "push r7\n\t" \
        "push r8\n\t" \
        "push r9\n\t" \
        "push r10\n\t" \
        "push r11\n\t" \
        "push r12\n\t" \
        "push r13\n\t" \
        "push r14\n\t" \
        "push r15\n\t" \
        "push r16\n\t" \
        "push r17\n\t" \
        "push r18\n\t" \
        "push r19\n\t" \
        "push r20\n\t" \
        "push r21\n\t" \
        "push r22\n\t" \
        "push r23\n\t" \
        "push r24\n\t" \
        "push r25\n\t" \
        "push r26\n\t" \
        "push r27\n\t" \
        "push r28\n\t" \
        "push r29\n\t" \
        "push r30\n\t" \
        "push r31\n\t" \
    );

#define RESTORE_CONTEXT()\
    asm volatile ( \
        "pop r31\n\t" \
        "pop r30\n\t" \
        "pop r29\n\t" \
        "pop r28\n\t" \
        "pop r27\n\t" \
        "pop r26\n\t" \
        "pop r25\n\t" \
        "pop r24\n\t" \
        "pop r23\n\t" \
        "pop r22\n\t" \
        "pop r21\n\t" \
        "pop r20\n\t" \
        "pop r19\n\t" \
        "pop r18\n\t" \
        "pop r17\n\t" \
        "pop r16\n\t" \
        "pop r15\n\t" \
        "pop r14\n\t" \
        "pop r13\n\t" \
        "pop r12\n\t" \
        "pop r11\n\t" \
        "pop r10\n\t" \
        "pop r9\n\t" \
        "pop r8\n\t" \
        "pop r7\n\t" \
        "pop r6\n\t" \
        "pop r5\n\t" \
        "pop r4\n\t" \
        "pop r3\n\t" \
        "pop r2\n\t" \
        "pop r1\n\t" \
        "pop r0\n\t" \
        "out __SREG__, r0\n\t" \
        "pop r0" \
    );

//naked: tells gcc not to modify the stack while entering this
ISR(TIMER0_COMPA_vect,ISR_NAKED) {
    //Save the registers to the stack at the start to ensure we don't mess up the stack
    SAVE_CONTEXT();
    
    //----------Context switch----------
    //Keep track of which thread is active
    thread_A_active = !thread_A_active;
    
    if(!first_switch){
        if(!thread_A_active){
            //--- A goes out, B comes in ---//
            //Save the stack pointers to kernel space
            swapped_sph_A = SPH; 
            swapped_spl_A = SPL;
            
            //Make the current stack pointers B's (initially, half way down the stack)
            SPH = swapped_sph_B;
            SPL = swapped_spl_B;
            
        }else{
            //--- B goes out, A comes in ---//
            //Save the stack pointers to kernel space
            swapped_sph_B = SPH;
            swapped_spl_B = SPL;
            
            //Make the current stack pointers A's
            SPH = swapped_sph_A;
            SPL = swapped_spl_A;
        }
    }else{
        first_switch = FALSE;
        swapped_spl_A = SPL;
        swapped_sph_A = SPH;
        
        SPH = swapped_sph_B;
        SPL = swapped_spl_B;
        
        asm volatile (
            "lds r24,thread_B\n\t"
            "lds r25,thread_B+1\n\t"
            "push r24\n\t"
            "push r25\n\t"
        );
        reti();
    }
        
    //We've just changed the SP to the correct place
    RESTORE_CONTEXT();
    reti();
}

