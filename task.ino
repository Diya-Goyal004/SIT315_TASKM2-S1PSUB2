const byte LED_PIN = 13;
const byte POTENTIOMETER_PIN = A4;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(POTENTIOMETER_PIN, INPUT);  // Set potentiometer pin as input
  
  Serial.begin(9600);
  
  // Initialize Timer1 for interrupt-based blinking
  // Clear Timer1 control registers
  TCCR1A = 0;
  TCCR1B = 0;
  // Initialize counter value to 0
  TCNT1 = 0;
  
  // Set Timer1 to CTC (Clear Timer on Compare Match) mode
  TCCR1B |= (1 << WGM12);
  
  // Set prescaler to 1024 and start the timer
  TCCR1B |= (1 << CS12) | (1 << CS10);
  
  // Calculate and set the initial compare match value for 2 seconds (16MHz / 1024 / 2 - 1)
  OCR1A = 7812;
  
  // Enable Timer1 compare match interrupt
  TIMSK1 |= (1 << OCIE1A);
}

void loop() {
  // Read the analog value from the potentiometer (0 - 1023)
  int potValue = analogRead(POTENTIOMETER_PIN);
  
  // Map the potentiometer value to a suitable range of timer frequencies (0.1 - 10 Hz)
  double timerFrequency = map(potValue, 0, 1023, 1, 100) / 10.0;
  
  // Calculate and set the compare match value based on the mapped frequency
  double clockCyclesPerInterrupt = F_CPU / (timerFrequency * 1024.0) - 1.0;
  uint16_t compareMatchValue = (uint16_t)clockCyclesPerInterrupt;
  OCR1A = compareMatchValue;
  
  delay(100);  // Delay to allow users to adjust the potentiometer
}

// Interrupt Service Routine for Timer1 compare match A
ISR(TIMER1_COMPA_vect) {
  // Toggle the LED by reading its current state and then writing the opposite state
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}


