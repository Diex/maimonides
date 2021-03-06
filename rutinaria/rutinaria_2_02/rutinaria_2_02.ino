#include <L298.h>

#define DEBUG_MOTORS 1

// Motor 1
int driver1A1 = 4;
int driver1A2 = 3;
int driver1AS = 2; // Needs to be a PWM pin to be able to control motor speed

// Motor 2
int driver1B1 = 6;
int driver1B2 = 5;
int driver1BS = 7; // Needs to be a PWM pin to be able to control motor speed


// Motor 3
int driver2A1 = 10;
int driver2A2 = 9;
int driver2AS = 8; // Needs to be a PWM pin to be able to control motor speed

// Motor 4
int driver2B1 = 12;
int driver2B2 = 11;
int driver2BS = 13; // Needs to be a PWM pin to be able to control motor speed

// Motor 5
int driver3A1 = 33;
int driver3A2 = 31;
int driver3AS = 45; // Needs to be a PWM pin to be able to control motor speed

//// Motor 6
//int driver3B1 = 30;
//int driver3B2 = 32;
//int driver3BS = 3; // Needs to be a PWM pin to be able to control motor speed

int maxSpeed = 50;
int maxDelay = 20E3;
int minDelay = 2E3;

L298 driverA;
L298 driverB;
L298 driverC;

L298 * drivers[] = {&driverA, &driverB, &driverC};
boolean newData = false;
const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

void setup() {  // Setup runs once per reset
  // initialize serial communication @ 9600 baud:
  Serial.begin(115200);
  driverA.beginMotoA(driver1A1, driver1A2, driver1AS);
  driverA.beginMotoB(driver1B1, driver1B2, driver1BS);

  driverB.beginMotoA(driver2A1, driver2A2, driver2AS);
  driverB.beginMotoB(driver2B1, driver2B2, driver2BS);

  driverC.beginMotoA(driver3A1, driver3A2, driver3AS);

  autoTest();
}



void loop() {
  showNewData();
  delay(100);
}

int motor = -1;
int speed = 0;

void serialEvent() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial.available() > 0 && newData == false) {

    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void showNewData() {

  if (newData == true) {

    char * next = strtok(receivedChars, ":");
    Serial.println(next);
    if (next != NULL) {

      next = strtok(NULL, ":");
      speed = atoi(next);

      switch(receivedChars[0]){
        case '1':
        driverA.setSpeed('A', speed);
        break;

        case '2':
        driverA.setSpeed('B', speed);
        break;

        case '3':
        driverB.setSpeed('A', speed);
        break;

        case '4':
        driverB.setSpeed('B', speed);
        break;

        case '5':
        driverC.setSpeed('A', speed);
        break;
      }
//      if (receivedChars[0] == '1') {
//        driverA.setSpeed('A', speed);
//      }

      Serial.print("message for: ");
      Serial.print(receivedChars[0]);
      Serial.print('\t');
      Serial.println(atoi(next));
    }
    newData = false;
  }
}
void autoTest() {

  for (int i = 0; i < 3; i ++) {
    Serial.print("test driver: ");
    Serial.print(i);
    Serial.println('A');
    testDriver(drivers[i], 'A');
    Serial.print("test driver: ");
    Serial.print(i);
    Serial.println('B');
    testDriver(drivers[i], 'B');
  }
}

void testDriver(L298 * driver, char motor) {
  driver->setSpeed(motor, 100);
  delay(500);
  driver->setSpeed(motor, -100);
  delay(500);
  driver->brake(motor);
 
}

void randomizeDriver(L298 * driver) {

  if (random(1000) < 500) {
    driver->setSpeed('A', random(-maxSpeed, maxSpeed)) ;
  } else {
    //    driver->brake('A');
    driver->setSpeed('B', random(-maxSpeed, maxSpeed)) ;
  }

  if (random(1000) < 10) {
    driver->brake('B');
  }

  if (random(1000) < 10) {
    driver->brake('A');
  }




  //  if(random(1000) < 500) {
  //    driver->setSpeed('B', random(-maxSpeed, maxSpeed)) ;
  //  }else{
  //    driver->brake('B');
  //  }
}
