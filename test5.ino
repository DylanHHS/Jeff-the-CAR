#define MOTORLATCH 12
#define MOTORCLK 4
#define MOTORENABLE 7
#define MOTORDATA 8
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR3_A 5
#define MOTOR3_B 7
#define MOTOR4_A 0
#define MOTOR4_B 6
#define MOTOR1_PWM 11
#define MOTOR2_PWM 3
#define MOTOR3_PWM 6
#define MOTOR4_PWM 5
#define SERVO1_PWM 10
#define SERVO2_PWM 9
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4
#define LINKS 5
#define RECHTS 6
#define Lshuffle 7

const int trigPin = A8;
const int echoPin  = A9;
long duration;
int distance;

const int trigPin1 = A10;
const int echoPin1  = A11;
long duration1;
int distance1;

const int trigPin2 = A12;
const int echoPin2  = A13;
long duration2;
int distance2;

void setup()
{
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);

Serial.begin(9600);

}
void loop()
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin,  HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin,  HIGH);
distance= duration*0.034/2;
Serial.print("Distance: ");
Serial.println(distance);
delay(50);


uit();
delay(500);

if(distance > 25 ){
  
Vooruit();

digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin,  HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin,  HIGH);
distance= duration*0.034/2;
Serial.print("Distance: ");
Serial.println(distance);
delay(50);

}
else
{
 Achteruit();
 Links();
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin,  HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin,  HIGH);
distance= duration*0.034/2;
Serial.print("Distance: ");
Serial.println(distance);
delay(50);
}
}






void motor(int nMotor, int command, int speed)
{
int motorA, motorB;
if (nMotor >= 1 && nMotor <= 4)
{
switch (nMotor)
{
case 1:
motorA = MOTOR1_A;
motorB = MOTOR1_B;
break;
case 2:
motorA = MOTOR2_A;
motorB = MOTOR2_B;
break;
case 3:
motorA = MOTOR3_A;
motorB = MOTOR3_B;
break;
case 4:
motorA = MOTOR4_A;
motorB = MOTOR4_B;
break;
default:
break;
}
switch (command)
{
case BACKWARD:
motor_output (motorA, HIGH, speed);
motor_output (motorB, LOW, -1); 
break;
case FORWARD:
motor_output (motorA, LOW, speed);
motor_output (motorB, HIGH, -1);
break;
case RELEASE:
motor_output (motorA, LOW, 0); 
motor_output (motorB, LOW, -1); 
break;
case LINKS:
motor_output (MOTOR1_A, LOW, speed);
motor_output (MOTOR2_A, HIGH, 0);
motor_output (MOTOR3_A, HIGH, 0);
motor_output (MOTOR4_A, HIGH, speed);
motor_output (MOTOR1_B, HIGH, -1);
motor_output (MOTOR2_B, HIGH, 0);
motor_output (MOTOR3_B, HIGH, 0);
motor_output (MOTOR4_B, LOW, -1);
break;
case RECHTS:
motor_output (MOTOR1_A, HIGH, speed);
motor_output (MOTOR2_A, HIGH, 0);
motor_output (MOTOR3_A, HIGH, 0);
motor_output (MOTOR4_A, LOW, speed);
motor_output (MOTOR1_B, LOW, -1);
motor_output (MOTOR2_B, HIGH, 0);
motor_output (MOTOR3_B, HIGH, 0);
motor_output (MOTOR4_B, HIGH, -1);
break;
case Lshuffle:
motor_output (MOTOR1_A, HIGH, speed);
motor_output (MOTOR2_A, LOW, speed);
motor_output (MOTOR3_A, HIGH, -1);
motor_output (MOTOR4_A, LOW, speed);
motor_output (MOTOR1_B, LOW, -1);
motor_output (MOTOR2_B, HIGH, -1);
motor_output (MOTOR3_B, LOW, speed);
motor_output (MOTOR4_B, HIGH, -1 );
break;
default:
break;
}
}
}
void motor_output (int output, int high_low, int speed)
{
int motorPWM;
switch (output)
{
case MOTOR1_A:
case MOTOR1_B:
motorPWM = MOTOR1_PWM;
break;
case MOTOR2_A:
case MOTOR2_B:
motorPWM = MOTOR2_PWM;
break;
case MOTOR3_A:
case MOTOR3_B:
motorPWM = MOTOR3_PWM;
break;
case MOTOR4_A:
case MOTOR4_B:
motorPWM = MOTOR4_PWM;
break;
default:
speed = -3333;
break;
}
if (speed != -3333)
{
shiftWrite(output, high_low);
// Doet het alleen wanneer er een geldige PWM waarde is.
if (speed >= 0 && speed <= 255)
{
analogWrite(motorPWM, speed);
}
}
}
void shiftWrite(int output, int high_low)
{
static int latch_copy;
static int shift_register_initialized = false;

if (!shift_register_initialized)
{
// Pins als output zetten
pinMode(MOTORLATCH, OUTPUT);
pinMode(MOTORENABLE, OUTPUT);
pinMode(MOTORDATA, OUTPUT);
pinMode(MOTORCLK, OUTPUT);
// Alle pins laag zetten ;
digitalWrite(MOTORDATA, LOW);
digitalWrite(MOTORLATCH, LOW);
digitalWrite(MOTORCLK, LOW);
// Om shift register te gebruiken moet Motor enable ook low.
digitalWrite(MOTORENABLE, LOW);
latch_copy = 0;
shift_register_initialized = true;
}
// Hier wordt de informatie in geschoven
bitWrite(latch_copy, output, high_low);
shiftOut(MOTORDATA, MOTORCLK, MSBFIRST, latch_copy);
delayMicroseconds(5); // Voor veiligheid
digitalWrite(MOTORLATCH, HIGH);
delayMicroseconds(5); // Voor veiligheid
digitalWrite(MOTORLATCH, LOW);
} 

void Vooruit()
{
motor(1, FORWARD, 128);
motor(2, FORWARD, 128);
motor(3, FORWARD, 128);
motor(4, FORWARD, 128);
delay(500);

}

void Achteruit()
{
motor(1, BACKWARD, 128);
motor(2, BACKWARD, 128);
motor(3, BACKWARD, 128);
motor(4, BACKWARD, 128);
delay(100);
}

void Links()
{
motor(1, LINKS, 100);
motor(2, LINKS, 100);
motor(3, LINKS, 100);
motor(4, LINKS, 100);
delay(1500);
motor(1, RELEASE, 0);
motor(2, RELEASE, 0);
motor(3, RELEASE, 0);
motor(4, RELEASE, 0);
delay(100);
}

void Rechts()
{
motor(1, RECHTS, 100);
motor(2, RECHTS, 100);
motor(3, RECHTS, 100);
motor(4, RECHTS, 100);
delay(1000);
motor(1, RELEASE, 0);
motor(2, RELEASE, 0);
motor(3, RELEASE, 0);
motor(4, RELEASE, 0);
delay(100);
}

void uit()
{
motor(1, RELEASE, 0);
motor(2, RELEASE, 0);
motor(3, RELEASE, 0);
motor(4, RELEASE, 0);
}

void shuffleL()
{
motor(1, Lshuffle, 128);
motor(2, Lshuffle, 128);
motor(3, Lshuffle, 128);
motor(4, Lshuffle, 128);
delay(500);
}


void lees()
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin,  HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin,  HIGH);
distance= duration*0.034/2;
Serial.print("Distance: ");
Serial.println(distance);
delay(50);

digitalWrite(trigPin1, LOW);
delayMicroseconds(2);
digitalWrite(trigPin1,  HIGH);
delayMicroseconds(10);
digitalWrite(trigPin1, LOW);
duration1 = pulseIn(echoPin1,  HIGH);
distance1= duration1*0.034/2;
Serial.print("Distance1: ");
Serial.println(distance1);
delay(50);

digitalWrite(trigPin2, LOW);
delayMicroseconds(2);
digitalWrite(trigPin2,  HIGH);
delayMicroseconds(10);
digitalWrite(trigPin2, LOW);
duration2 = pulseIn(echoPin2,  HIGH);
distance2= duration2*0.034/2;
Serial.print("Distance2: ");
Serial.println(distance2);
delay(50);
}
