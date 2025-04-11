#define ledPin 8
uint8_t input[3];

uint8_t command = 0;
//  0b[11]   [00]    [00]    [00]
//    x      num2    op      num1
// PORTB

// !NOTE: mode is not in consideration for now.

uint8_t output = 0b10000000;
//  0b[1] [00]  [0]   [0]   [0]     [00]
//    x   sum   -ve   zero  carry   x
// PORTC

void setup()
{
  Serial.begin(9600);
  // For arduino uno.
  // DDRB |= 0b00111111;
  // DDRC &= 0b00000111;

  // Coment the esp setups.
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  blink(6, 200);
  blink(10, 50);
}


void loop()
{
  if (Serial.available()) {
    blink(10, 30);
    // Store the first 3 byte in input[] and ignore the rest
    for (uint8_t i = 0; i < 3; i++) { input[i] = Serial.read(); }
    while (Serial.available()) { Serial.read(); }

    // Converting char to int
    // If num++ or --num, the sign stays and the other number becomes 1
    input[0] = (input[0] == '+' || input[0] == '-') ? 1 : input[0] - 48;
    input[2] = (input[2] == '+' || input[2] == '-') ? 1 : input[2] - 48;

    // Operation
    switch (input[1]) {
      case '+': input[1] = 0; break;
      case '-': input[1] = 1; break;
    }

    // Store the full command in the 8 bits of `uint8_t command`
    // Ignore the index 6 and 7 for now.
    setBit(&command, 7, 1);
    setBit(&command, 6, 1);
    setBit(&command, 0, getBit(input + 0, 0));
    setBit(&command, 1, getBit(input + 0, 1));
    setBit(&command, 2, getBit(input + 1, 0));
    setBit(&command, 3, getBit(input + 1, 1));
    setBit(&command, 4, getBit(input + 2, 0));
    setBit(&command, 5, getBit(input + 2, 1));

    // Write command to PORTB
    // PORTB = command;
    // Read output from PORTC
    // output = PORTC;
    output = 0b10101000;
    Serial.print(command, BIN);
    Serial.print("|");
    Serial.print(output, BIN);
  }
}


void blink(uint8_t count, uint16_t interval) {
  for (uint8_t i = 0; i < count; i++) {
    digitalWrite(ledPin, i % 2);
    delay(interval);
  }
}
uint8_t getBit(uint8_t* byteAddress, uint8_t index) {
  return 1 & (*byteAddress >> index);
}
void setBit(uint8_t* byteAddress, uint8_t index, uint8_t value) {
  if (value == 1) *byteAddress |= 1 << index;
  else *byteAddress &= ~(1 << index);
}

