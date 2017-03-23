const byte buffSize = 40;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromPC = false;

char cvar0[buffSize] = {0};
int ivar0 = 0;
float fvar0 = 0.0; // fraction of servo range to move

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  
  //setup pins etc
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  getDataFromPC();
  performAction();
  replyToPC();
  newDataFromPC = false; //explicitly make sure PC message is not processed twice
  delay(100);
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
  // split the data into its parts, expected format CHARS,INT,FLOAT... (note markers < and > already stripped out)
  char * strtokIndx; // this is used by strtok() as an index
  
  strtokIndx = strtok(inputBuffer,","); // get the first part - the string
  strcpy(cvar0, strtokIndx); // copy it to char array
  
  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  ivar0 = atoi(strtokIndx);     // convert this part to an integer
  
  strtokIndx = strtok(NULL, ","); 
  fvar0 = atof(strtokIndx);     // convert this part to a float
}

void replyToPC() {
  if (newDataFromPC) {
    newDataFromPC = false;
    Serial.print("<");
    Serial.print(cvar0);
    Serial.print(",");
    Serial.print(ivar0);
    Serial.print(",");
    Serial.print(fvar0);
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
  
}
