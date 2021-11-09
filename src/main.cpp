// Library import
#include <Arduino.h>
#include <SoftwareSerial.h>

// Constant variable definition
#define MOTOR_1 9
#define MOTOR_2 10
const int delay_time = 10;

// Private variable definition
SoftwareSerial master_communication(2, 3);
String command;
int pwm, dir;
int last_pwm, last_dir = 0;

// Private function prototypes
boolean isValid(String command);
void increase_pwm(int pin, int start, int target);
void decrease_pwm(int pin, int start, int target);
void adjust_pwm(int pin, int start, int target);
void communication_init(void);

void setup()
{
  communication_init();
}

void loop()
{
  command = master_communication.readStringUntil('F');

  if (isValid(command))
  {
    Serial.println(command);

    //The received string gets parsed into variables.
    pwm = command.substring(2, 5).toInt();
    dir = command.substring(1, 2).toInt();

    Serial.println(pwm);
    Serial.println(dir);

    //PWM values get adjusted according to the direction data
    if (dir == 1)
    {
      if (last_dir == 0)
      {
        adjust_pwm(MOTOR_2, last_pwm, 0);
        last_pwm = 0;
      }
      adjust_pwm(MOTOR_1, last_pwm, pwm);
      last_dir = 1;
    }
    else if (dir == 0)
    {
      if (last_dir == 1)
      {
        adjust_pwm(MOTOR_1, last_pwm, 0);
        last_pwm = 0;
      }
      adjust_pwm(MOTOR_2, last_pwm, pwm);
      last_dir = 0;
    }

    //Current PWM value get saved as last PWM value.
    last_pwm = pwm;
  }
}

// Explicit functions

//This function checks if the received data is valid or not
boolean isValid(String command)
{
  return ((command.charAt(0) == 'S') && (command.length() == 6));
}

//PWM signal gets increased with a delay
void increase_pwm(int pin, int start, int target)
{
  for (int i = start; i <= target; i++)
  {
    Serial.print("Analog 1:");
    Serial.println(i);
    analogWrite(pin, i);
    delay(delay_time);
  }
}

//PWM signal gets decreased with a delay
void decrease_pwm(int pin, int start, int target)
{
  for (int i = start; i >= target; i--)
  {
    Serial.print("Analog 2:");
    Serial.println(i);
    analogWrite(pin, i);
    delay(delay_time);
  }
}

//This function decides if the PWM value should get increased or decreased.
void adjust_pwm(int pin, int start, int target)
{
  if (target - start > 0)
  {
    increase_pwm(pin, start, target);
  }
  else if (target - start < 0)
  {
    decrease_pwm(pin, start, target);
  }
}

//Necessary initialization commands for serial communication
void communication_init(void)
{
  master_communication.begin(9600);
  Serial.begin(9600);
}
