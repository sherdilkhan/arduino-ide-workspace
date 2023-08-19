#include <Arduino.h>

// Recursive function to calculate factorial
unsigned int factorial(unsigned int n) {
    // Base case: factorial of 0 is 1
    if (n == 0) {
        return 1;
    }
    // Recursive case: n! = n * (n-1)!
    else {
        return n * factorial(n - 1);
    }
}

void setup() {
    Serial.begin(115200);
    
    unsigned int num = 5000;
    unsigned int result = factorial(num);
    
    Serial.printf("Factorial of %u is %u\n", num, result);
}

void loop() {
    // Empty loop
}
