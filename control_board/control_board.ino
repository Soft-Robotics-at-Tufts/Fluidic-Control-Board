int prescaler = 256; // set this to match whatever prescaler value you set in CS registers below

// intialize values for the PWM duty cycle set by pots
float potDC1 = 0;
float potDC2 = 0;
float potDC3 = 0;
float potDC4 = 0;
float potDC5 = 0;

void setup() {

  Serial.begin(9600);

  // input pins for valve switches
  pinMode(45, INPUT);
  pinMode(47, INPUT);
  pinMode(49, INPUT);
  pinMode(51, INPUT);

  // output pins for valve PWM
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  int eightOnes = 255;  // this is 11111111 in binary
  TCCR3A &= ~eightOnes;   // this operation (AND plus NOT), set the eight bits in TCCR registers to 0 
  TCCR3B &= ~eightOnes;
  TCCR4A &= ~eightOnes;
  TCCR4B &= ~eightOnes;

  // set waveform generation to frequency and phase correct, non-inverting PWM output
  TCCR3A = _BV(COM3A1);
  TCCR3B = _BV(WGM33) | _BV(CS32);
  
  TCCR4A = _BV(COM4A1) | _BV(COM4B1) | _BV(COM4C1);
  TCCR4B = _BV(WGM43) | _BV(CS42);
}

void pPWM(float pwmfreq, float pwmDC1, float pwmDC2, float pwmDC3, float pwmDC4, float pwmDC5) {

  // set PWM frequency by adjusting ICR (top of triangle waveform)
  ICR3 = F_CPU / (prescaler * pwmfreq * 2);
  ICR4 = F_CPU / (prescaler * pwmfreq * 2);
  
  // set duty cycles
  OCR3A = (ICR4) * (pwmDC1 * 0.01);
  OCR4A = (ICR4) * (pwmDC2 * 0.01);
  OCR4B = (ICR4) * (pwmDC3 * 0.01);
  OCR4C = (ICR4) * (pwmDC4 * 0.01);
  OCR2B = (ICR4) * (pwmDC5 * 0.01);

  //Serial.println("PWM Set");
}

void loop() {

  potDC1 = 0; potDC2 = 0; potDC3 = 0; potDC4 = 0; potDC5 = 0;

  // if statement for manual switch override
  if (digitalRead(45) == HIGH) {
    potDC1 = analogRead(A0)*100.0/1024.0; // scale values from pot to 0 to 100, which gets used for duty cycle percentage
    //digitalWrite(2, HIGH);
  }
  //else{digitalWrite(2, LOW);}
  
  if (digitalRead(47) == HIGH) { 
    potDC2 = analogRead(A1)*100.0/1024.0; 
    //Serial.println("S2 on");
    //digitalWrite(3, HIGH);
  }
  //else{digitalWrite(3, LOW);}
  
  if (digitalRead(49) == HIGH) {
    potDC3 = analogRead(A2)*100.0/1024.0; 
    //Serial.println("S3 on");
    //digitalWrite(4, HIGH);
  }
  //else{digitalWrite(4, LOW);}
  
  if (digitalRead(51) == HIGH) { 
    potDC4 = analogRead(A3)*100.0/1024.0; 
    //Serial.println("S4 on");
    //digitalWrite(5, HIGH);
  }
  //else{digitalWrite(5, LOW);}

  if (digitalRead(53) == HIGH) { 
    potDC5 = analogRead(A4)*100.0/1024.0; 
    Serial.println("S5 on");
    digitalWrite(9, HIGH);
  }
  else{digitalWrite(9, LOW);}

  

  float potPWMfq = analogRead(A5)*100.0/1024.0; // scale values from pot to 0 to 100, which gets used for frequency (Hz)
  potPWMfq = round(potPWMfq/5)*5+1; //1 to 91 Hz in increments of 5 (rounding helps to deal with noisy pot)
  //Serial.println(potPWMfq);

  // update PWM output based on the above values from pots
  pPWM(potPWMfq,potDC1,potDC2,potDC3,potDC4,potDC5);//hardcode here

  // transfer function for sensor Honeywell ASDXRRX100PGAA5 (100 psi, 5V, A-calibration)
  // Vout = 0.8*Vsupply/(Pmax - Pmin)*(Papplied - Pmin) + 0.1*Vsupply
  // Rearrange to get: Papplied = (Vout/Vsupply - 0.1)*(Pmax - Pmin)/0.8 + Pmin;

  // read output voltages from sensors and convert to pressure reading in PSI


  delay(200);
}
