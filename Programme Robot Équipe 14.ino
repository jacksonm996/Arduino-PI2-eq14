// Microcontroller Program

// Controller pins
const byte pin_direction = A0;
const byte pin_propulsion = A1;
const byte pin_up = A2;
const byte pin_down = A3;
const byte pin_open_close = A5;
const byte pin_aux = A4;

// Output pins
const int pin_PWM_motor_1 = 3;
const int pin_1_motor_1 = 2;
const int pin_2_motor_1 = 4;

const int pin_PWM_motor_2 = 6;
const int pin_1_motor_2 = 5;
const int pin_2_motor_2 = 7;

const int pin_PWM_motor_3 = 9;
const int pin_1_motor_3 = 8;
const int pin_2_motor_3 = 10;

const int pin_PWM_servo = 11;

// Global variables
long speed = 0;
long motor_1_speed = 0;
long motor_2_speed = 0;
float power = 1500;
int direction = 0;
float turn = 0;
byte state_1 = LOW;
byte state_2 = LOW;
int lift = 1500;
int drop = 1500;
int aux = 0;
int p_2_a = 1500;

// Servo
#include <Servo.h>
Servo servo_pince;
int open_close = 1500;
float pos = 45;
float pos1 = 0;
float pos2 = 45;
int n = 25;
float step = (pos2-pos1)/n;
bool marvin = false;

// Pulse duration variables
unsigned long p_1;
unsigned long p_2;
unsigned long p_3;
unsigned long p_4;
unsigned long p_5;
unsigned long p_6;

// Pulse duration test results
const int direction_max = 2005;
const int direction_min = 995;
const int propulsion_max = 2005;
const int propulsion_min = 995;
const int lift_max = 2005;
const int lift_min = 995;
const int open_close_max = 2005;
const int open_close_min = 995;
const int aux_max = 2005;
const int aux_min = 995;

// Setup
void setup()
{
  pinMode(pin_direction, INPUT);
  pinMode(pin_propulsion, INPUT);
  pinMode(pin_up, INPUT);
  pinMode(pin_down, INPUT);
  pinMode(pin_open_close, INPUT);
  pinMode(pin_aux, INPUT);

  pinMode(pin_PWM_motor_1, OUTPUT);
  pinMode(pin_1_motor_1, OUTPUT);
  pinMode(pin_2_motor_1, OUTPUT);

  pinMode(pin_PWM_motor_2, OUTPUT);
  pinMode(pin_1_motor_2, OUTPUT);
  pinMode(pin_2_motor_2, OUTPUT);

  pinMode(pin_PWM_motor_3, OUTPUT);
  pinMode(pin_1_motor_3, OUTPUT);
  pinMode(pin_2_motor_3, OUTPUT);

  pinMode(pin_PWM_servo, OUTPUT);

  servo_pince.attach(pin_PWM_servo);
  servo_pince.write(pos);
  Serial.begin(9600);
}

void loop()
{
// Pulse input receiver
p_1 = pulseIn(pin_direction, HIGH);
p_2 = pulseIn(pin_propulsion, HIGH);
p_3 = pulseIn(pin_up, HIGH);
p_4 = pulseIn(pin_down, HIGH);
p_5 = pulseIn(pin_open_close, HIGH);
p_6 = pulseIn(pin_aux, HIGH);

// Pulse test
//Serial.println(p_3);
Serial.println(pos1);

// Direction + propulsion
  if((p_2 < 2010) and (p_2 > 990)){
    if (p_2 < 1400){
      power = max(p_2_a, p_2);
    }
    else{
      power = p_2;
    }
    p_2_a = p_2;
  }
  if((p_1 < 2010) and (p_1 > 990)){
    direction = p_1;
  }
  else{
    direction = 1500;
  }
  turn = (direction-1000) / 1000.00;

  if(power < 1450){
    state_1 = HIGH;
    state_2 = LOW;
    speed = map(power, 1500, propulsion_min, 0, 255);
  }
  else if(power > 1550){
    state_1 = LOW;
    state_2 = HIGH;
    speed = map(power, 1500, propulsion_max, 0, 255);
  }
  else{
    state_1 = LOW;
    state_2 = LOW;
    speed = 0;
  }
  if(turn > 0.51){
    motor_1_speed = (1-turn)*speed;
  	motor_2_speed = speed;
  }
  else if (turn < 0.49){
  	motor_2_speed = turn*speed;
    motor_1_speed = speed;
  }
  else {
    motor_1_speed = speed;
    motor_2_speed = speed;
  }

  //Serial.println(motor_1_speed);
	digitalWrite(pin_1_motor_1,state_1);
	digitalWrite(pin_2_motor_1,state_2);
	analogWrite(pin_PWM_motor_1,motor_1_speed);
  digitalWrite(pin_1_motor_2,state_2);
	digitalWrite(pin_2_motor_2,state_1);
	analogWrite(pin_PWM_motor_2,motor_2_speed);
  delay(20);
  // NEED SOME SORT OF MEMORY BUFFER TO PREVENT RANDOM CHANGES IN DIRECTION
  // Change direction control so that the wheels turn opposite directions?

// Pince
  if((p_5 < 2010) and (p_5 > 990)){
    open_close = p_5;
  }
  else{
    open_close = 1500;
  }
  if (open_close > 1985){
    for (; pos <= pos2; pos += step){
      servo_pince.write(pos);
      delay(20);
    }
  }
  else if (open_close < 1015){
    for (; pos >= pos1; pos -= step) {
      servo_pince.write(pos);
      delay(20);
    }
  }
// Lift system
  // DOUBLE CHECK HOW BUTTONS 3 AND 4 WORK TOGETHER - FIX THE MATH ACCORDINGLY
  if((p_3 < 2010) and (p_3 > 990)){
    lift = p_3;
  }
  if((p_4 < 2010) and (p_4 > 990)){
    drop = p_4;
  }
  if ((lift < 1400) and (drop < 1400)){
    digitalWrite(pin_1_motor_3, HIGH);
    digitalWrite(pin_2_motor_3, LOW);
  }
  else if ((lift > 1600) and (drop > 1600)){
    digitalWrite(pin_1_motor_3, LOW);
    digitalWrite(pin_2_motor_3, HIGH);
  }
  else {
    digitalWrite(pin_1_motor_3, LOW);
    digitalWrite(pin_2_motor_3, LOW);
  }

// AUX
  aux = p_6;
  if (aux > 1500 and marvin == true){
    pos1 = 0;
    marvin = false;
  }
  else if (aux > 1500 and marvin == false){
    pos1 = 0;
    marvin = true;
  }
}