// Matt Rienzo
// EE354 Assignment 10
// Oct. 10, 2018
// 
// Reads tilt data from accelerometer and lights up one of four LEDs corresponding to tilt 
// 
#include <at89c51cc03.h>      //  CC03 library file 
// 

// 
// Function prototypes
void main(void);	// Entry point
void initializeBoard(void);	// Sets up default state of pins and registers
void setupADC(unsigned char);	// Sets up ADC for in-loop action
void initADC(unsigned char); // Set up initial ADC state
void clearInts(void); // Turn off interrupts
void clearLEDs(void);	// I can clear the LEDs, but I can't seem to clear the crushing haze of anxiety from my mind
// 
// 

void main(){	// entry point; TO-CODY: Apparently Keil decided that this must be named main(), and will always go at 0x0000 in the 8051 memory.
	//TO-CODY: When you define variables in Keil's C51, they have to either be global (outside of the function) or at the top of the function.
	//	   This is because Keil's compiler doesn't scan and check for variable declarations, and instead just compiles line-by-line.
	//	   When the compiler converts this into assembly and then machine code, variables have to be at the beginning because in assembly for 8051
	//	   the variables have to be in the .data (.idata or something) section, which goes before code in flash memory.  Keil's compiler *could* be
	//	   programmed to deal with variables in a layered way instead of line-by-line, like GCC (a different C compiler), but they opted to not do that
	//	   optimization, probably for compile speed and size.
	unsigned char tmp; //TO-CODY: This variable helps us check that the ADC is done reading data for each sample.
	unsigned int nondescriptVariableName; //TO-CODY: This variable will hold the data from the ADC.
	
	initializeBoard();
	while(1){
		setupADC(0x04); // Ch4 -- turn on the news!
		
		tmp = (ADCON & 0x10); //TO-CODY: This puts the `end of data' bit from ADCON into tmp.
		while(tmp != 0x10){	// Loop until we have completed reading from ADC
			tmp = (ADCON & 0x10);
		}
		
		// Get our 10bit ADC value
		nondescriptVariableName = ((unsigned int)ADDH << 2) + ADDL;
		

	//TO-CODY: You'll see that I call `clearLEDs()' each time before turning the ones I want back on.  This eliminates any bugs related to 
	//	   accidentally leaving them on
		// Light up respective LED
		if((nondescriptVariableName > 430) && (nondescriptVariableName <= 510)){ //TO-CODY: Full tilt right on my board, or....
			// Turn on LED 1
			clearLEDs();
			P0_7 = 0;
			if(nondescriptVariableName > 493) //TO-CODY: ....almost full tilt right.  This gives the LEDs the effect of being `fluid'
				P0_6 = 0;
		}else if((nondescriptVariableName > 510) && (nondescriptVariableName <= 620)){ //TO-CODY: Kinda middle
			clearLEDs();
			P0_6 = 0;
			if(nondescriptVariableName > 550) //TO-CODY: Definitely middle, and so we light up the two middle LEDs
				P0_5 = 0;
		}else if((nondescriptVariableName > 620) && (nondescriptVariableName <= 675)){ //TO-CODY: Kinda middle the other way
			// Turn on LED 3
		  clearLEDs();
			P0_5 = 0;
			if(nondescriptVariableName > 650) //TO-CODY: Almost full tilt left on my board
				P0_4 = 0;
		}else if((nondescriptVariableName > 675) && (nondescriptVariableName <= 750)){ //TO-CODY: Full tilt left on my board
			// Turn on LED 4
			clearLEDs();
			P0_4 = 0;
		}
		
		//TO-CODY: Reset the ADC channel and read state
		ADCON &= 0xEF;
	}
}

// initializeBoard(void) - Sets up the board's initiale state
//-- args: none
//rr rets: none
void initializeBoard(){
	// Raise LED pins to 1 so that LEDS are initially off
	clearLEDs();
	initADC(0x10); // P1.4 for ADC channel 4 (AN4)
	clearInts();
}

// setupADC(unsigned char channel) -- resets the ADC and starts reading from supplied channel
//-- args: byte `channel' where the 5 MSB are zero, for ADC channel
//rr rets: none
void setupADC(unsigned char channel){
		ADCON &= 0xF8;
		ADCON |= channel;
		ADCON |= 0x20;
		ADCON |= 0x08;
}

// initADC(unsigned char channel) -- sets the ADC's initial state at the start of the program
//-- args: byte `channel' where the bits set `P1.channel' are zero, for ADC channel
//rr rets: none
void initADC(unsigned char channel){
	ADCF = channel;
	ADCON = 0x20;
	ADCLK = 0x00;
}

// clearInts(void) -- turns off interrupts
//-- args: none
//rr rets: none
void clearInts(){
	EA = 0;
}

// clearLEDs(void) -- sets all LED ports to 1, turning off the LEDs
//-- args: none
//rr rets: none
void clearLEDs(){
	P0 |= 0xF0;
}
