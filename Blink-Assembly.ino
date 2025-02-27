// Arduino "Blink" example in Assembly Language (AVR Assembly for ATmega328P) - Assembly Delays

// Define constants for pin 13 (built-in LED on many Arduinos)
#define LED_PIN 13
#define LED_PORT PORTB
#define LED_DDR DDRB
#define LED_BIT 5  // PB5 is Arduino pin 13 on ATmega328P

void setup() {
  // Set LED pin as output (DDRB register)
  asm volatile (
    "  ldi r16, (1 << %0)  \n"  // Load immediate value with bit for LED pin set
    "  out %1, r16        \n"  // Set LED pin as output (DDRB register)
    : // No outputs
    : "I" (LED_BIT), "I" (_SFR_IO_ADDR(LED_DDR)) // Inputs: immediate values
  );
}

void loop() {
  asm volatile (
  "loop_start:\n"
    // -------- Turn LED ON --------
    "  sbi %1, %0       \n"  // Set bit in PORTB (turn LED ON)

    // -------- Delay for ON state (Assembly Delay) --------
    "  ldi r24, 255            \n"  // Load delay counter high byte (Adjust for desired delay)
    "delay_on_loop:\n"
    "  ldi r25, 255            \n"  // Load delay counter low byte (Adjust for desired delay)
    "inner_delay_on_loop:\n"
    "  nop                     \n"  // No operation (small delay)
    "  dec r25                 \n"  // Decrement low byte counter
    "  brne inner_delay_on_loop\n"  // Branch if not equal to zero (loop again)
    "  dec r24                 \n"  // Decrement high byte counter
    "  brne delay_on_loop      \n"  // Branch if not equal to zero (loop again)

    // -------- Turn LED OFF --------
    "  cbi %1, %0       \n"  // Clear bit in PORTB (turn LED OFF)

    // -------- Delay for OFF state (Assembly Delay) --------
    "  ldi r24, 255            \n"  // Load delay counter high byte (Adjust for desired delay)
    "delay_off_loop:\n"
    "  ldi r25, 255            \n"  // Load delay counter low byte (Adjust for desired delay)
    "inner_delay_off_loop:\n"
    "  nop                     \n"  // No operation (small delay)
    "  dec r25                 \n"  // Decrement low byte counter
    "  brne inner_delay_off_loop\n"  // Branch if not equal to zero (loop again)
    "  dec r24                 \n"  // Decrement high byte counter
    "  brne delay_off_loop     \n"  // Branch if not equal to zero (loop again)

    // -------- Jump back to the beginning of the loop (optional, as loop() repeats anyway) --------
    //"  rjmp loop_start         \n"  // No need for explicit jump, loop() repeats

    : // No outputs
    : "I" (LED_BIT), "I" (_SFR_IO_ADDR(LED_PORT)) // Inputs: immediate values
  );
  // No C++ delay() needed anymore
}