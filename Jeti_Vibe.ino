/*
  Jeti Transmitter Vibrator v1.2

  For use with DC/DS-16/24 internal PPM-output

  - Vibrates three times on radio power-up
  - Vibration off when PPM-channel 14/16 (Selectable) is -100%
  - 10 different vibration patterns when channel 14/16 is used between -100% and +100%
  - Motor Off at 0% or if no signal
  - Internal Led (Red) follows motor for debugging purpose when no computer is connected

  by Tero Salminen 2016 - 2017 www.RC-Thoughts.com

  Based on Read_any_PPM_V05 by Hasi http://www.rcgroups.com/forums/showthread.php?t=1808432
*/

// Define functions and constants
#define Motor_Pin 4 // Motor and Led drive (N-FET Gate, Source to GND, Drain to motor negative)
#define PPM_Pin 2  // This must be 2 or 3
#define Led_Pin 13 // Internal led
#define multiplier (F_CPU/8000000)  // Do not touch - needed for PPM timing also
int ppm[16];  // Array for storing Jeti PPM16 output
// Only one of these should be uncommentented!
#define chan (ppm[13]) // Uncomment for DC/DC-14 (PPM-channel 14)
//#define chan (ppm[15]) // Uncomment for DC/DS-16/24 (PPM-channel 16)

void setup() // Do not touch unless you know what you are doing
{
  Serial.begin(115200);
  Serial.println("ready");
  pinMode(PPM_Pin, INPUT);
  pinMode(Motor_Pin, OUTPUT);
  pinMode(Led_Pin, OUTPUT);
  attachInterrupt(PPM_Pin - 2, read_ppm, CHANGE);

  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS11);

  for (int i = 0; i <= 2; i++) { // Run motor in start three times
    digitalWrite(Motor_Pin, HIGH);
    digitalWrite(Led_Pin, HIGH);
    delay(200); // ON period for motor in ms
    digitalWrite(Motor_Pin, LOW);
    digitalWrite(Led_Pin, LOW);
    delay(100); // OFF period for motor in ms
  }
}

void loop()
{
  Serial.print("CH value: "); // Print channelvalue to serial port for debug
  Serial.print(chan);
  Serial.println("");
  delay(50);

  if (chan < 10) { // Motor off if no signal
    digitalWrite(Motor_Pin, LOW);
    digitalWrite(Led_Pin, LOW);
    Serial.print("No signal");
  }
  else if (chan > 1480 && chan < 1511) { // Zero position -0% Motor off
    digitalWrite(Motor_Pin, LOW);
    digitalWrite(Led_Pin, LOW);
    Serial.print("0%");
  }
  else if (chan > 10 && chan < 1091) { // Position 1 <-80%
    digitalWrite(Motor_Pin, HIGH);
    digitalWrite(Led_Pin, HIGH);
    Serial.print("Pos 1");
  }
  else if (chan > 1090 && chan < 1191) { // Position 2 -70%
    digitalWrite(Motor_Pin, HIGH);
    digitalWrite(Led_Pin, HIGH);
    delay(900); // ON period for motor in ms
    digitalWrite(Motor_Pin, LOW);
    digitalWrite(Led_Pin, LOW);
    delay(300); // OFF period for motor in ms
    Serial.print("Pos 2");
  }
  else if (chan > 1190 && chan < 1291) { // Position 3 -50%
    digitalWrite(Motor_Pin, HIGH);
    digitalWrite(Led_Pin, HIGH);
    delay(900); // ON period for motor in ms
    digitalWrite(Motor_Pin, LOW);
    digitalWrite(Led_Pin, LOW);
    delay(200); // OFF period for motor in ms
    Serial.print("Pos 3");
  }
  else if (chan > 1290 && chan < 1391) { // Position 4 -30%
    digitalWrite(Motor_Pin, HIGH);
    digitalWrite(Led_Pin, HIGH);
    delay(700); // ON period for motor in ms
    digitalWrite(Motor_Pin, LOW);
    digitalWrite(Led_Pin, LOW);
    delay(200); // OFF period for motor in ms
    Serial.print("Pos 4");
  }
  else if (chan > 1390 && chan < 1481) { // Position 5 -10%
    digitalWrite(Motor_Pin, HIGH);
    digitalWrite(Led_Pin, HIGH);
    delay(600); // ON period for motor in ms
    digitalWrite(Motor_Pin, LOW);
    digitalWrite(Led_Pin, LOW);
    delay(100); // OFF period for motor in ms
    Serial.print("Pos 5");
  }
  else if (chan > 1510 && chan < 1591) { // Position 6 10%
    digitalWrite(Motor_Pin, HIGH);
    digitalWrite(Led_Pin, HIGH);
    delay(500); // ON period for motor in ms
    digitalWrite(Motor_Pin, LOW);
    digitalWrite(Led_Pin, LOW);
    delay(100); // OFF period for motor in ms
    Serial.print("Pos 6");
  }
  else if (chan > 1590 && chan < 1691) { // Position 7 30%
    digitalWrite(Motor_Pin, HIGH);
    digitalWrite(Led_Pin, HIGH);
    delay(400); // ON period for motor in ms
    digitalWrite(Motor_Pin, LOW);
    digitalWrite(Led_Pin, LOW);
    delay(100); // OFF period for motor in ms
    Serial.print("Pos 7");
  }
  else if (chan > 1690 && chan < 1791) { // Position 8 50%
    digitalWrite(Motor_Pin, HIGH);
    digitalWrite(Led_Pin, HIGH);
    delay(300); // ON period for motor in ms
    digitalWrite(Motor_Pin, LOW);
    digitalWrite(Led_Pin, LOW);
    delay(75); // OFF period for motor in ms
    Serial.print("Pos 8");
  }
  else if (chan > 1790 && chan < 1891) { // Position 9 70%
    digitalWrite(Motor_Pin, HIGH);
    digitalWrite(Led_Pin, HIGH);
    delay(200); // ON period for motor in ms
    digitalWrite(Motor_Pin, LOW);
    digitalWrite(Led_Pin, LOW);
    delay(50); // OFF period for motor in ms
    Serial.print("Pos 9");
  }
  else if (chan > 1890) { // Position 10 90%
    digitalWrite(Motor_Pin, HIGH);
    digitalWrite(Led_Pin, HIGH);
    delay(150); // ON period for motor in ms
    digitalWrite(Motor_Pin, LOW);
    digitalWrite(Led_Pin, LOW);
    delay(25); // OFF period for motor in ms
    Serial.print("Pos 10");
  }
  else {
    digitalWrite(Motor_Pin, LOW);
    digitalWrite(Led_Pin, LOW); // If nothing matches then motor off
    Serial.print("No match");
  }
}

// Do not touch anything below this
void read_ppm() {
  static unsigned int pulse;
  static unsigned long counter;
  static byte channel;
  static unsigned long last_micros;

  counter = TCNT1;
  TCNT1 = 0;

  if (counter < 510 * multiplier) { // Must be a pulse if less than 510us
    pulse = counter;
  }
  else if (counter > 1910 * multiplier) { // Sync pulses over 1910us
    channel = 0;
  }
  else { // Servo values between 510us and 2420us will end up here
    ppm[channel] = (counter + pulse) / multiplier;
    channel++;
  }
}
