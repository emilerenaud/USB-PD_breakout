#include <Arduino.h>

// pins definition
#define LED1 1
#define LED2 0

#define VoltagePin 3

// CFG1 CFG2 CFG3
// 1    x    x    5V
// 0    0    0    9V
// 0    0    1    12V
// 0    1    1    15V
// 0    1    0    20V
#define CFG1 6
#define CFG2 5
#define CFG3 4
#define PWR_EN 7
#define PG 10

// Variables
enum voltage {V5, V9, V12, V15, V20};

// Functions prototypes
void LED1_task(void *pvParameters);
void configVoltage(voltage v);
float readVoltage(void);


void setup() {
  // put your setup code here, to run once:
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(CFG1, OUTPUT);
  pinMode(CFG2, OUTPUT);
  pinMode(CFG3, OUTPUT);
  pinMode(PWR_EN, OUTPUT);
  pinMode(PG, INPUT_PULLUP);

  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);

  // digitalWrite(CFG1, LOW);
  // digitalWrite(CFG2, LOW);
  // digitalWrite(CFG3, 1);
  configVoltage(V20);
  digitalWrite(PWR_EN, HIGH);


  Serial.begin(115200);
  Serial.println("USB-PD start here");

  // Create task to flash LED1
  xTaskCreate(LED1_task,"LED1_task",10000,NULL,1,NULL);

  // Config the voltage
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(PG) == HIGH)
  {
    Serial.println("Power bad");
  }

  float voltage = readVoltage();
  Serial.print("Voltage: ");
  Serial.println(voltage);
  delay(1000);
}

void LED1_task(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  while (1) {
    digitalWrite(LED1, LOW);
    // digitalWrite(LED2, HIGH);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    digitalWrite(LED1, HIGH);
    // digitalWrite(LED2, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Function that read the voltage with a voltage divider of 1.69k and 10k
float readVoltage() {
  double voltage = 0;
  // Read the voltage 10 times and get the average
  for (int i = 0; i < 10; i++) {
    voltage += analogReadMilliVolts(VoltagePin)/1000.0;
    delay(10);
  }
  voltage = voltage / 10.0;
  // Transform the voltage to the real voltage
  voltage = voltage / (1.69 / (1.69 + 10.0));
  // Serial.println(analogReadMilliVolts(VoltagePin) / (1.69 / (1.69 + 10)));
  return voltage;
}

// Function that config the voltage
void configVoltage(voltage v)
{
  switch(v)
  {
    case V5:
      digitalWrite(CFG1, HIGH);
      digitalWrite(CFG2, LOW);
      digitalWrite(CFG3, LOW);
      break;
    case V9:
      digitalWrite(CFG1, LOW);
      digitalWrite(CFG2, LOW);
      digitalWrite(CFG3, LOW);
      break;
    case V12:
      digitalWrite(CFG1, LOW);
      digitalWrite(CFG2, LOW);
      digitalWrite(CFG3, HIGH);
      break;
    case V15:
      digitalWrite(CFG1, LOW);
      digitalWrite(CFG2, HIGH);
      digitalWrite(CFG3, HIGH);
      break;
    case V20:
      digitalWrite(CFG1, LOW);
      digitalWrite(CFG2, HIGH);
      digitalWrite(CFG3, LOW);
      break;
  }
}

