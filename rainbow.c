/*
 * rainbow.c - Make the screen rainbow
 */
/*
avr-gcc -DF_CPU=16000000UL -mmcu=atmega328p -Wall -o rainbow.out rainbow.c
avr-objcopy -O ihex -R .eeprom rainbow.out rainbow.hex
avrdude -b57600 -patmega328p -cstk500v1 -P/dev/ttyUSB0 -U flash:w:rainbow.hex
*/


#include <avr/io.h>
#include "dev/serial.c"
#define RED 0
#define PURPLE 1
#define BLUE 2
#define GREEN 3
#define WHITE 4


#define SPEED 10
#define COLOR_COUNT 5
uint8_t fbRed[8];
uint8_t fbGreen[8];
uint8_t fbBlue[8];
uint8_t fbLights;

//I'm sure there are many better ways to represent the colours and draw them, but it's late, ok?
void setRow(uint8_t row, uint8_t color)
{
    if(color==RED)
    {
        fbRed[row]=0xFFU;
        fbGreen[row]=0x00U;
        fbBlue[row]=0x00U;
    }
    if(color==GREEN)
    {
        fbRed[row]=0x00U;
        fbGreen[row]=0xFFU;
        fbBlue[row]=0x00U;
    }
    if(color==BLUE)
    {
        fbRed[row]=0x00U;
        fbGreen[row]=0x00U;
        fbBlue[row]=0xFFU;
    }
    if(color==PURPLE)
    {
        fbRed[row]=0xFFU;
        fbGreen[row]=0x00U;
        fbBlue[row]=0xFFU;
    }
    if(color==WHITE)
    {
        fbRed[row]=0xFFU;
        fbGreen[row]=0xFFU;
        fbBlue[row]=0xFFU;
    }
}

int powerTwo(int n)
{
    if(n==0){
        return 1;
    }
    return 2*powerTwo(n-1);
}

void setRowHeight(uint8_t row, uint8_t height)
{
    if(fbRed[row] == 0xFFU)
    {
        fbRed[row] = powerTwo(height)-1;
    }
    if(fbGreen[row] == 0xFFU)
    {
        fbGreen[row] = powerTwo(height)-1;
    }
    if(fbBlue[row] == 0xFFU)
    {
        fbBlue[row] = powerTwo(height)-1;
    }
}

void delay(uint16_t ms) {
   uint16_t i,j;
   uint16_t loop = F_CPU / 17000;  // this is approximate and needs calabration.
   for (i=0;i<ms;i++) {
      for (j=0;j<loop;j++);
   }
}

void initFrameBuffer() {
   DDRB |=  (1<<PB2) | (1<<PB3); // make the serial pins output   
   PORTB |= (1<<PB2);  // set the led drivers to recieve input
 
   DDRD |= (1<<PD2); // make the pin for row 0 output
   PORTD |= (1<<PD2); // turn that row off.

   DDRD |= (1<<PD3); // row 2
   PORTD |= (1<<PD3); 

   DDRD |= (1<<PD4); // row 3
   PORTD |= (1<<PD4); 
  
   DDRD |= (1<<PD5); // row 4
   PORTD |= (1<<PD5); 

   DDRD |= (1<<PD6); // row 5
   PORTD |= (1<<PD6); 

   DDRD |= (1<<PD7); // row 6
   PORTD |= (1<<PD7); 

   DDRB |= (1<<PB0); // row 7
   PORTB |= (1<<PB0); 

   DDRB |= (1<<PB4); // row 8
   PORTB |= (1<<PB4); 
}

void drawFrameBuffer() {
     uint8_t i;
     
     for (i=1;i<=8;i++) {
        
       SPCR = (1 << SPE) | ( 1 << MSTR );    // enable SPI, master, and set clock rate

        SPDR = fbLights; // set the shift out register
        while(!(SPSR & (1<<SPIF))); // wait until complete

        SPDR = fbRed[i-1]; 
        while(!(SPSR & (1<<SPIF))); 

        SPDR = fbGreen[i-1]; 
        while(!(SPSR & (1<<SPIF))); 

        SPDR = fbBlue[i-1]; 
        while(!(SPSR & (1<<SPIF))); 
        SPCR = 0; //turn off spi 


        if (i==1) {
            PORTD &= ~(1<<PD2);
        } else if (i==2) {
            PORTD &= ~(1<<PD3);
        } else if (i==3) {
            PORTD &= ~(1<<PD4);
        } else if (i==4) {
            PORTD &= ~(1<<PD5);
        } else if (i==5) {
            PORTD &= ~(1<<PD6);
        } else if (i==6) {
            PORTD &= ~(1<<PD7);
        } else if (i==7) {
            PORTB &= ~(1<<PB0);
        } else if (i==8) {
            PORTB &= ~(1<<PB4);
        }

        delay(1);

        if (i==1) {
            PORTD |= (1<<PD2);
        } else if (i==2) {
            PORTD |= (1<<PD3);
        } else if (i==3) {
            PORTD |= (1<<PD4);
        } else if (i==4) {
            PORTD |= (1<<PD5);
        } else if (i==5) {
            PORTD |= (1<<PD6);
        } else if (i==6) {
            PORTD |= (1<<PD7);
        } else if (i==7) {
            PORTB |= (1<<PB0);
        } else if (i==8) {
            PORTB |= (1<<PB4);
        }
     }
}


int main() {
    initFrameBuffer();
    fbLights = 0xAAU;
   
    int iteration = 0;
    uart_init();
    uart_putstring("This is a test");
    while (1) {
        drawFrameBuffer();
        int i;
        for(i=0;i<8;i++)
        {
            setRow(i,(iteration+i)%COLOR_COUNT);
            int cycle = (iteration+i)%16;
            if(cycle > 8)
            {
                cycle = 16-cycle;
            }
            setRowHeight(i,cycle);
        }
        iteration++;
        int j;
        for(j=0;j<SPEED;j++) //delay here
        {
            delay(1);
            drawFrameBuffer();
        }
    }
    return 0;
} 
