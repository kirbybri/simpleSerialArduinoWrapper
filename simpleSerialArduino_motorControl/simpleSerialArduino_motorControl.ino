#include <AFMotor.h>

const byte buffSize = 40;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromPC = false;

char cvar0[buffSize] = {0};
int ivar0 = 0;
int ivar1 = 0;
float fvar0 = 0.0;
float fvar1 = 0.0;

AF_DCMotor motor_1(1);
AF_DCMotor motor_2(4);

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  
  //setup pins etc
  pinMode(LED_BUILTIN, OUTPUT);
  
  // turn on motor
  motor_1.setSpeed(200);
  motor_1.run(RELEASE);
  motor_2.setSpeed(200);
  motor_2.run(RELEASE);
}

void loop() {
  getDataFromPC();
  performAction();
  replyToPC();
  newDataFromPC = false; //explicitly make sure PC message is not processed twice
  delay(10);
}

void getDataFromPC() {
  
  if(Serial.available() > 0) {
    char x = Serial.read();

    // the order of these IF clauses is significant  
    if (x == endMarker) {
      readInProgress = false;
      newDataFromPC = true;
      inputBuffer[bytesRecvd] = 0;
      parseData();
    }
    
    if(readInProgress) {
      inputBuffer[bytesRecvd] = x;
      bytesRecvd ++;
      if (bytesRecvd == buffSize) {
        bytesRecvd = buffSize - 1;
      }
    }

    if (x == startMarker) { 
      bytesRecvd = 0; 
      readInProgress = true;
    }
  }
  
}

void parseData() {
  char * strtokIndx; // this is used by strtok() as an index
  
  strtokIndx = strtok(inputBuffer,","); // get the first part - the string
  strcpy(cvar0, strtokIndx); // copy it to char array
  
  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  ivar0 = atoi(strtokIndx);     // convert this part to an integer
  
  strtokIndx = strtok(NULL, ","); 
  //fvar0 = atof(strtokIndx);     // convert this part to a float
  ivar1 = atoi(strtokIndx);     // convert this part to an integer
}

void replyToPC() {
  if (newDataFromPC) {
    newDataFromPC = false;
    Serial.print("<");
    Serial.print(cvar0);
    Serial.print(",");
    Serial.print(ivar0);
    Serial.print(",");
    Serial.print(ivar1);
    Serial.println(">");
  }
}

void performAction() {
  
  if (!newDataFromPC)
    return;
  
  if (strcmp(cvar0, "TEST") == 0) {
    //do something here
    for (byte n = 0; n < 10; n++) {
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(50);                       // wait for a second
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      delay(50);                       // wait for a second
    }
  }
  
  if (strcmp(cvar0, "FWD") == 0) {
    //motor.setSpeed(200);
    if( ivar1 == 1 ){
      motor_1.run(FORWARD);
      delay(ivar0);
      motor_1.run(RELEASE);
    }
    if( ivar1 == 2 ){
      motor_2.run(FORWARD);
      delay(ivar0);
      motor_2.run(RELEASE);
    }    
  }
  
  if (strcmp(cvar0, "BWD") == 0) {
    //motor.setSpeed(200);
    if( ivar1 == 1 ){
      motor_1.run(BACKWARD);
      delay(ivar0);
      motor_1.run(RELEASE);
    }
    if( ivar1 == 2 ){
      motor_2.run(BACKWARD);
      delay(ivar0);
      motor_2.run(RELEASE);
    } 
  }
}
