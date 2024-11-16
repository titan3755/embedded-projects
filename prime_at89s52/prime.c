#include <REGX52.h>
#include <math.h>

void delay() {
    int i;
    for (i = 0; i < 1000; i++) {
        TMOD = 0x01;  // Timer0 mode1
        TH0 = 0xFC;    // Initial value for 1ms
        TL0 = 0x66;
        TR0 = 1;       // Timer start
        while (TF0 == 0);  // Wait for overflow
        TR0 = 0;       // Stop Timer
        TF0 = 0;       // Clear flag
    }
}

unsigned int primeNumberFind() {
    unsigned int numberOfPrimes = 2;
    unsigned int currentNum = 4;
    unsigned int limit = 500;
    
    while (currentNum < limit) {
        unsigned int isPrime = 1;  // Assume number is prime
				int i;
        for (i = 2; i <= floor(sqrt(currentNum)); i++) {
            if (currentNum % i == 0) {
                isPrime = 0;  // Not a prime number
                break;
            }
        }
        if (isPrime) {
            numberOfPrimes++;
        }
        currentNum++;
    };
    return numberOfPrimes;
}

void main() {
    unsigned int pNums = primeNumberFind();
    P1_2 = 0x00;  // Initialize pin P1_2 as output
	P1_3 = 0x00;
    delay();
		if (pNums % 2 == 0) {
			delay();
			P1_2 = 1;
			P1_3 = 0;
			delay();
			P1_2 = 0;
			P1_3 = 1;
			delay();
			P1_2 = 1;
			delay();
			P1_2 = 0;
			P1_3 = 1;
			delay();
			P1_2 = 1;
			P1_3 = 0;
			delay();
			P1_2 = 1;
			P1_3 = 0;
			delay();
		} else {
			delay();
			P1_2 = 0;
			P1_3 = 1;
			delay();
			P1_2 = 1;
			P1_3 = 0;
			delay();
			P1_2 = 0;
			P1_3 = 1;
			delay();
			P1_2 = 0;
			P1_3 = 1;
			delay();
			P1_2 = 1;
			P1_3 = 0;
			delay();
			P1_2 = 1;
			P1_3 = 0;
			delay();
		}
    while (1) {
        P1_2 = 1;  // Pin P1_2 high
        delay();
        P1_2 = 0;  // Pin P1_2 low
        delay();
    }
}
