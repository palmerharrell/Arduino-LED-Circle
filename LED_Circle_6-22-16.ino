//Source of shift register code:
//http://bildr.org/2011/02/74hc595/
//Individual control over each pin
//Support for 40+ shift registers

int SER_Pin = 11;   //pin 14 on the 75HC595 Shift Register
int RCLK_Pin = 8;   //pin 12 on the 75HC595
int SRCLK_Pin = 12; //pin 11 on the 75HC595

//Number of shift registers - change as necessary
#define number_of_74hc595s 3

//do not touch
#define numOfRegisterPins number_of_74hc595s * 8

boolean registers[numOfRegisterPins];

//       ***********************************
//                  My Variables
//       ***********************************

//  Pot1 is wired backwards:
//  all the way left is 1023
//  all the way right is 0

int Pot1 = 0;
int Pot2 = 1;
int SwitchPin = 7;
int PurpleStatusLED = 6;

int Pot1Val;
int Pot2Val;
int SwitchState;
int speedDelay;
int LEDb;
int LEDc;
int LEDd;
int LEDposition = 0;

int demoSwitchPin = 2;
int demoSwitchState;
long randNum;

void setup(){
  pinMode(SER_Pin, OUTPUT);
  pinMode(RCLK_Pin, OUTPUT);
  pinMode(SRCLK_Pin, OUTPUT);
  pinMode(Pot1, INPUT);
  pinMode(Pot2, INPUT);
  pinMode(demoSwitchPin, INPUT);
  pinMode(SwitchPin, INPUT);
  pinMode(PurpleStatusLED, OUTPUT);
  
  randomSeed(analogRead(5));
  Serial.begin(9600);

  //reset all register pins
  clearRegisters();
  writeRegisters();
}


void loop(){

  demoSwitchState = digitalRead(demoSwitchPin);
  if (demoSwitchState == LOW)
  {
    Pot2Val = analogRead(Pot2);
    SwitchState = digitalRead(SwitchPin);
    // Serial.println(SwitchState);

    if (SwitchState == HIGH)
    {
      digitalWrite(PurpleStatusLED, HIGH);
      clearRegisters();
      group1();
    }
    else 
    {
      digitalWrite(PurpleStatusLED, LOW);
      clearRegisters();
      group2();
    }
  }
  else
  {
    Serial.println("Demo ON");
    demoMode();
  }

} // END LOOP

// Selectable patterns when switch is ON
void group1(){
  Pot2Val = analogRead(Pot2);
  if (Pot2Val/102.3 <2)
  {
    spin_clockwise();
  }
  else if (Pot2Val/102.3 >2 && Pot2Val/102.3 <4)
  {
    LEDs_x3_clockwise();
  }
  else if (Pot2Val/102.3 >4 && Pot2Val/102.3 <6)
  {
    LEDs_x2_chase_clockwise();    
  }
  else if (Pot2Val/102.3 >6 && Pot2Val/102.3 <8)
  {
    LEDs_x3_chase_clockwise();
  }
  else
  {
    LEDs_x4_chase_clockwise();
  }
}

// Selectable patterns when switch is OFF
void group2(){
  Pot2Val = analogRead(Pot2);
  if (Pot2Val/102.3 <2)
  {
    all_on_all_off();
  }
  else if (Pot2Val/102.3 >2 && Pot2Val/102.3 <4)
  {
    spin_change_direction();
  }
  else if (Pot2Val/102.3 >4 && Pot2Val/102.3 <6)
  {
    direct_control();
  }
  else if (Pot2Val/102.3 >6 && Pot2Val/102.3 <8)
  {
    direct_control_x2();
  }
  else
  {
    LEDs_x2_opposite_directions();
  }

}

//set all register pins to LOW
void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
} 

//Set and display registers
//Only call AFTER all values are set
// It's slow if called too often
void writeRegisters(){
  digitalWrite(RCLK_Pin, LOW);
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
    digitalWrite(SRCLK_Pin, LOW);
    int val = registers[i];
    digitalWrite(SER_Pin, val);
    digitalWrite(SRCLK_Pin, HIGH);
  }
  digitalWrite(RCLK_Pin, HIGH);
}

//set an individual pin HIGH or LOW
void setRegisterPin(int index, int value){
  registers[index] = value;
}

void demoMode(){
  randNum = random(1, 9);
  Serial.print("Random Number: ");
  Serial.print(randNum);
  Serial.println();

  switch (randNum) {
    case 1:
      LEDs_x2_opposite_directions();
      break;
    case 2:
      spin_clockwise();
      break;
    case 3:
      LEDs_x2_chase_clockwise();
      break;
    case 4:
      LEDs_x3_chase_clockwise();
      break;
    case 5:
      LEDs_x4_chase_clockwise();
      break;
    case 6:
      LEDs_x3_clockwise();
      break;
    case 7:
      testPattern();
      break;
    case 8:
      all_on_all_off();
      break;
    default:
      testPattern();
      break;
  }
}

//        *********************************
//       ***********************************
//                    Patterns
//       ***********************************
//        *********************************

void testPattern(){
  LEDb =24;
  for (int i = 0; i <= 23; ++i)
  {
    Pot1Val = analogRead(Pot1);
    speedDelay = Pot1Val/10.23;
    setRegisterPin(i, HIGH);
    if (LEDb < 24)
    {
      setRegisterPin(LEDb, HIGH);
    }
    writeRegisters();
    delay (speedDelay+8);
    // setRegisterPin(i, LOW);
    // setRegisterPin(LEDb, LOW);
    writeRegisters();
    delay (speedDelay);
    if (LEDb > 12)
    {
      --LEDb;
    }
    
    else
    {
      ++LEDb;
    }
  }
  for (int i = 24; i >= 0; --i)
  {
    Pot1Val = analogRead(Pot1);
    speedDelay = Pot1Val/10.23;
    setRegisterPin(i, LOW);
    if (LEDb < 24)
    {
      setRegisterPin(LEDb, LOW);
    }
    writeRegisters();
    delay (speedDelay+8);
    // setRegisterPin(i, LOW);
    // setRegisterPin(LEDb, LOW);
    writeRegisters();
    delay (speedDelay);
    --LEDb;
    if (LEDb == 0)
    {
      LEDb = 24;
    }
  }
}

void spin_clockwise(){
  clearRegisters();
  for (int i = 0; i <= 23; ++i)
  {
    Pot1Val = analogRead(Pot1);
    speedDelay = Pot1Val/10.23;
    setRegisterPin(i, HIGH);
    writeRegisters();
    delay (speedDelay+8);
    setRegisterPin(i, LOW);
    writeRegisters();
    delay (speedDelay/2);
  }
}

void LEDs_x2_chase_clockwise(){
  LEDb =12;
  for (int i = 0; i <= 23; ++i)
  {
    Pot1Val = analogRead(Pot1);
    speedDelay = Pot1Val/10.23;
    setRegisterPin(i, HIGH);
    setRegisterPin(LEDb, HIGH);
    writeRegisters();
    delay (speedDelay+8);
    setRegisterPin(i, LOW);
    setRegisterPin(LEDb, LOW);
    writeRegisters();
    delay (speedDelay);
    ++LEDb;
    if (LEDb == 24)
    {
      LEDb = 0;
    }
  }
}

void LEDs_x3_chase_clockwise(){
  LEDb =8;
  LEDc =16;
  for (int i = 0; i <= 23; ++i)
  {
    Pot1Val = analogRead(Pot1);
    speedDelay = Pot1Val/10.23;
    setRegisterPin(i, HIGH);
    setRegisterPin(LEDb, HIGH);
    setRegisterPin(LEDc, HIGH);
    writeRegisters();
    delay (speedDelay+8);
    setRegisterPin(i, LOW);
    setRegisterPin(LEDb, LOW);
    setRegisterPin(LEDc, LOW);
    writeRegisters();
    delay (speedDelay/2);
    ++LEDb;
    ++LEDc;
    if (LEDb == 24)
    {
      LEDb = 0;
    }
    if (LEDc == 24)
    {
      LEDc = 0;
    }
  }
}

void LEDs_x4_chase_clockwise(){
  LEDb =12;
  LEDc =6;
  LEDd =18;
  for (int i = 0; i <= 23; ++i)
  {
    Pot1Val = analogRead(Pot1);
    speedDelay = Pot1Val/10.23;
    setRegisterPin(i, HIGH);
    setRegisterPin(LEDb, HIGH);
    setRegisterPin(LEDc, HIGH);
    setRegisterPin(LEDd, HIGH);
    writeRegisters();
    delay (speedDelay+8);
    setRegisterPin(i, LOW);
    setRegisterPin(LEDb, LOW);
    setRegisterPin(LEDc, LOW);
    setRegisterPin(LEDd, LOW);
    writeRegisters();
    delay (speedDelay/2);
    ++LEDb;
    ++LEDc;
    ++LEDd;
    if (LEDb == 24)
    {
      LEDb = 0;
    }
    if (LEDc == 24)
    {
      LEDc = 0;
    }
    if (LEDd == 24)
    {
      LEDd = 0;
    }
  }
}

void LEDs_x3_clockwise(){
  Pot1Val = analogRead(Pot1);
  for (int i = 0; i <= 23; ++i)
  {
    LEDb = i+1;
    LEDc = i+2;
    if (i == 22)
    {
      LEDc = 0;
    }
    if (i == 23)
    {
      LEDb = 0;
      LEDc = 1;
    }
    Pot1Val = analogRead(Pot1);
    speedDelay = Pot1Val/10.23;
    setRegisterPin(i, HIGH);
    setRegisterPin(LEDb, HIGH);
    setRegisterPin(LEDc, HIGH);
    delay (speedDelay+6);
    if (i == 0)
    {
      setRegisterPin(23, LOW);
    }
    else
    {
      setRegisterPin(i-1, LOW);
    }
    writeRegisters();
    delay (speedDelay);
  }
}

void spin_change_direction(){
  int ReversespeedDelay;
  clearRegisters();
  for (int i = LEDposition; i <= 23; ++i)
  {
    Pot1Val = analogRead(Pot1);
    do
    {
      setRegisterPin(i, HIGH);
      writeRegisters();
      Pot1Val = analogRead(Pot1);
    } while (Pot1Val >460 && Pot1Val <560);
    clearRegisters();
    if (Pot1Val < 511)
    {
    Pot1Val = analogRead(Pot1);
    speedDelay = map(Pot1Val, 511, 0, 100, 0);
    setRegisterPin(i, HIGH);
    writeRegisters();
    delay (speedDelay+8);
    setRegisterPin(i, LOW);
    writeRegisters();
    delay (speedDelay/2);
    }
    else if (Pot1Val > 511)
    {
      break;
    }
    LEDposition = i;
    if (LEDposition == 23)
    {
      LEDposition = 0;
    }
  }
  for (int i = LEDposition; i > -1 ; --i)
  {
    Pot1Val = analogRead(Pot1);
    do
    {
      setRegisterPin(i, HIGH);
      writeRegisters();
      Pot1Val = analogRead(Pot1);
    } while (Pot1Val >460 && Pot1Val <560);
    clearRegisters();
    if (Pot1Val > 511)
    {
    Pot1Val = analogRead(Pot1);
    ReversespeedDelay = map(Pot1Val, 512, 1023, 100, 0);
    setRegisterPin(i, HIGH);
    writeRegisters();
    delay (ReversespeedDelay+8);
    setRegisterPin(i, LOW);
    writeRegisters();
    delay (ReversespeedDelay/2);      
    }
    else if (Pot1Val < 511)
    {
      break;
    }
    LEDposition = i;
    if (LEDposition == 0)
    {
      LEDposition = 23;
    }
  }
}

void direct_control(){
    clearRegisters();
    Pot1Val = analogRead(Pot1);
    Pot1Val = map(Pot1Val, 1023, 0, 0, 24);
    setRegisterPin(Pot1Val, HIGH);
    setRegisterPin(Pot1Val - 1, LOW);
    writeRegisters();  
}

void direct_control_x2(){
    clearRegisters();
    Pot1Val = analogRead(Pot1);
    
    if (Pot1Val > 511)
    {
        Pot1Val = map(Pot1Val, 1023, 512, 0, 23);
    } 
    else
    {
      Pot1Val = map(Pot1Val, 512, 0, 0, 24);
    }
    setRegisterPin(Pot1Val, HIGH);
    setRegisterPin(Pot1Val - 1, LOW);
    writeRegisters();  
}

void LEDs_x2_opposite_directions(){
  LEDb =0;
  clearRegisters();
  for (int i = 0; i <= 23; ++i)
  {
    Pot1Val = analogRead(Pot1);
    speedDelay = Pot1Val/10.23;
    setRegisterPin(i, HIGH);
    setRegisterPin(LEDb, HIGH);
    writeRegisters();
    delay (speedDelay+8);
    setRegisterPin(i, LOW);
    setRegisterPin(LEDb, LOW);
    writeRegisters();
    delay (speedDelay);
    if (LEDb == 0)
    {
      LEDb = 24;
    }
    --LEDb;
  }
}

void all_on_all_off(){
  // clearRegisters();
  for (int i = 0; i <= 23; ++i)
  {
    Pot1Val = analogRead(Pot1);
    speedDelay = Pot1Val/10.23;
    setRegisterPin(i, HIGH);
    writeRegisters();
    delay (speedDelay+8);
    if (i == 23)
    {
      for (int i = 0; i <= 23; ++i)
      {
        setRegisterPin(i, LOW);
        writeRegisters();
        delay (speedDelay+8);
      }
    }
    // setRegisterPin(i, LOW);
    // writeRegisters();
    // delay (speedDelay/2);
  }
}
