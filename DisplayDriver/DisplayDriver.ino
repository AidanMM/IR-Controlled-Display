//Shift registers being used ar 74HC595N CMOS 8-bit shift registers
//https://www.nxp.com/documents/data_sheet/74HC_HCT595.pdf

int SIN = 0; //
int SRCLK = 1; //Goes to white wire
int RCLK = 2; //Goes to brown brown wire
int OE = 3; //Output Enabled is active LOW
int IRIN1 = A0; // ANALOG Input from the IR sensor
int IRIN2 = A1; // ANALOG Input from the IR sensor
int IRIN3 = A2; // ANALOG Input from the IR sensor
int IRIN4 = A3; // ANALOG Input from the IR sensor
int IRIN5 = A4; // ANALOG Input from the IR sensor
int clkRate = 1;
int shiftValue = 0;
int rows = 5;
int collumns = 5;
int shiftOffset = 3; 
int ledTotal = 20;
//int APIN = 0;
//int APINVAL = 0;
int count = 0;
int screenSaverTimer = 0;
int dir = 1;
volatile unsigned long serialdata = 0;
volatile int inbyte;
bool recieveInput = false;

int animX = 1;
int animY = 0;

boolean state[5][5];

struct Frame{
bool pixels[20];
int rows = 4;
int collumns = 5;
int size = 20;
Frame(){
  for(int i = 0; i < size; i++)
  {
    pixels[i] = false; 
  }
}
Frame(bool pixels[]){
  memcpy(this->pixels, pixels, sizeof(this->pixels));  
}
Frame(int pixels[]){
 for(int i = 0; i < size; i++)
 {
  if(pixels[i] > 0)
  {
    this->pixels[i] = true;
  }
  else
  {
    this->pixels[i] = false;
  }
 }
}
};

int frameA1[] = {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};
int frameA2[] = {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
int frameB1[] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int frameB2[] = {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int frameB3[] = {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int frameB4[] = {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int frameB5[] = {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int frameB6[] = {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int frameB7[] = {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0};
int frameB8[] = {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0};
int frameB9[] = {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0};
int frameB10[] = {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
int frameB11[] = {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0};
int frameB12[] = {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0};
int frameB13[] = {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0};
int frameB14[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0};
int frameB15[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0};
int frameB16[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0};
int frameB17[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0};
int frameB18[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0};
int frameB19[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0};
int frameB20[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
int emptyFrame[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int fullFrame[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

int frameC1[] = {1,0,0,0,1,
                 0,0,0,0,0,
                 0,0,0,0,0,
                 1,0,0,0,1};
int frameC2[] = {0,0,0,1,0,
                 1,0,0,0,0,
                 0,0,0,0,1,
                 0,1,0,0,0};
int frameC3[] = {0,0,1,0,0,
                 0,0,0,0,1,
                 1,0,0,0,0,
                 0,0,1,0,0};
int frameC4[] = {0,1,0,0,1,
                 0,0,0,0,0,
                 0,0,0,0,0,
                 1,0,0,1,0};
int frameC5[] = {1,0,0,1,0,
                 0,0,0,0,0,
                 0,0,0,0,0,
                 0,1,0,0,1};

  int frameD1[] = {1,0,1,0,1,
                 1,1,1,0,1,
                 1,0,1,0,0,
                 1,0,1,0,1};

Frame anim1[] = {Frame(frameA1), Frame(frameA2)};

Frame anim3[] = {Frame(frameB1), Frame(frameB2), Frame(frameB3), Frame(frameB4), Frame(frameB5),
                 Frame(frameB10), Frame(frameB15), Frame(frameB20), Frame(frameB19),
                 Frame(frameB18), Frame(frameB17), Frame(frameB16), Frame(frameB11),
                 Frame(frameB6)};

Frame anim4[] = {Frame(frameC1), Frame(frameC2), Frame(frameC3), Frame(frameC4)};

Frame animHi[] = {Frame{frameD1}};                 

void setup() {
  // put your setup code here, to run once:
  pinMode(SIN, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(OE, OUTPUT);
  digitalWrite(SIN,0);
  digitalWrite(OE, 0);
  //Serial.begin(9600);
  //AllOff();
  AllOn();
  //Toggle(4, 0);
  //Toggle(4,4);
  //Toggle(1,0);
  //Toggle(1,3);
  PrintState();
}

void loop() {
  
  delay(50);
  AllOff();
  int ir1 = analogRead(IRIN1);
  int ir2 = analogRead(IRIN2);
  int ir3 = analogRead(IRIN3);
  int ir4 = analogRead(IRIN4);
  int ir5 = analogRead(IRIN5);
  //Serial.print(irX);
  //Serial.print(", ");
  //Serial.println(irY)
  Toggle(roundToFour(ir1), 3, true);
  Toggle(roundToFour(ir2), 1, true);
  Toggle(roundToFour(ir3), 4, true);
  Toggle(roundToFour(ir4), 0, true);
  Toggle(roundToFour(ir5), 2, true);
  PrintState();

  if(ir1 > 600 || ir2 > 600 || ir3 > 600 || ir4 > 600 || ir5 > 600) {
    screenSaverTimer = 0;
  }
  else {
    screenSaverTimer++;
  }
  
  if(screenSaverTimer > 100) {
    AnimateWithFrames(animHi, 1000,1);
    AnimateFrame(emptyFrame);
    delay(200);
    AnimateWithFrames(animHi, 1000,1);
    AnimateWithFrames(anim4, 200, 4);
    ReverseAnimateWithFrames(anim4, 100, 4);
    AnimateWithFrames(anim4, 100, 4);
    ReverseAnimateWithFrames(anim4, 100, 4);
    AnimateWithFrames(anim3, 40, 14);
    AnimateWithFrames(anim3, 40, 14);
    AnimateWithFrames(anim1, 200, 2);
    AnimateWithFrames(anim1, 200, 2);
    AnimateWithFrames(anim1, 200, 2);
  }

  //getSerial(); */
}

long getSerial()
{
  serialdata = 0;
  while (inbyte != '/')
  {
    inbyte = Serial.read();  
    if (inbyte > 0 && inbyte != '/')
    { 
      serialdata = serialdata * 10 + inbyte - '0';
      Serial.println(serialdata);
    }
  }
  
  return serialdata;
  inbyte = 0;
}

void PulseClock()
{
   //Set up clock to change every 20 microseconds
  digitalWrite(SRCLK, 1);
  digitalWrite(RCLK, 0);  
  delayMicroseconds(clkRate); 
  digitalWrite(SRCLK, 0);
  digitalWrite(RCLK, 1);
  delayMicroseconds(clkRate);
}

bool AnimateWithFrames(Frame frames[], int delayTime, int numFrames) {
  for(int i = 0; i < numFrames; i++)
  {
    AnimateFrame(frames[i]);
    delay(delayTime);
  }
  return true;
}

bool AnimateWithFramesToDelay(Frame frames[], int delayTime, int numFrames, int endDelayTime) {
  for(int i = 0; i < numFrames; i++)
  {
    AnimateFrame(frames[i]);
    delay(delayTime);
    delayTime -= (delayTime - endDelayTime) / numFrames;
  }
  return true;
}

bool ReverseAnimateWithFrames(Frame frames[], int delayTime, int numFrames) {
  for(int i = numFrames - 1; i > 0; i--)
  {
    AnimateFrame(frames[i]);
    delay(delayTime);
  }
  return true;
}
bool ReverseAnimateWithFramesToDelay(Frame frames[], int delayTime, int numFrames, int endDelayTime) {
  for(int i = numFrames - 1; i > 0; i--)
  {
    AnimateFrame(frames[i]);
    delay(delayTime);
    delayTime -= (delayTime - endDelayTime) / numFrames;
  }
  return true;
}

void AnimateFrame(Frame frame){
  int count = 0;
  for(int i = 1; i < frame.size / frame.rows + 1; i++)
  {
    if(i == 1)
    {
      Toggle(i,0, frame.pixels[count]);
      count++;
    }
    for(int j = 1; j < frame.size / frame.collumns + 2; j++)
    {
      if(count >= frame.size) 
          break;
      Toggle(i,j, frame.pixels[count]);
      count++;
    }
  }
  PrintState();
}
void AnimateFrame(int pixels[]) {
  AnimateFrame(Frame(pixels));
}
void AnimationReset(){
  dir = 1;
  count = 1;
  animX = 1;
  animY = 0;
  AllOff();
  PrintState();
}

void OnOff(){
    //Get ready to write to the shift register
  if(shiftValue == 0)
    shiftValue = 1;
   else
     shiftValue = 0;
   digitalWrite(SIN,shiftValue);
}

void OneInEight(){
  digitalWrite(SIN,0);
  for(int i = 0; i < 7; i++){
  PulseClock();
 } 
 digitalWrite(SIN,1);
 PulseClock();
}

void OneInEight(int On){
  digitalWrite(SIN,0);
  for(int i = 0; i < On; i++)
 {
  PulseClock();
 } 
 digitalWrite(SIN,1);
 PulseClock();
 for(int i = On + 1; i < 8; i++)
 {
  digitalWrite(SIN,0);
 PulseClock(); 
 }
}


void ShiftToLastRegister(int On)
{
  OneInEight(On);
  digitalWrite(SIN,0);
  for(int i = 0; i < 120; i++)
  {
   PulseClock(); 
  }
}

void Toggle(int x, int y){
  if(state[x][y] == true)
  {
    state[x][y] = false;
  }
  else
  {
    state[x][y] = true;
  }
}

void Toggle(int x, int y, bool mode){
    state[x][y] = mode;
}

void PrintState(){
  
 digitalWrite(OE, 1); //Turn off output of the shift registers to prevent flicker
 for(int x = 0; x < rows; x++)
 {
  for(int y = 0; y < collumns; y++)
  {
     if(state[x][y] == true )
       digitalWrite(SIN, 1);
     else if(state[x][y] == false)
       digitalWrite(SIN, 0);
     PulseClock();
   } 
  }
 for(int i = 0; i < shiftOffset - 1; i++)
 {
  PulseClock();
 }
 digitalWrite(OE, 0); //Turn on output for the shift registers to power LEDs
}

void AllOn(){
for(int x = 0; x < rows; x++)
{
  for(int y = 0; y < collumns; y++)
  {
    state[x][y] = true;  
  }
 }
}

int roundToFour(int toRound)
{
  if(toRound > 750) {
    return 4;
  }
  else if(toRound > 700) {
    return 3;
  }
  else if(toRound > 600) {
    return 2;
  }
  else {
    return 1;
  }
}

void AllOff(){
for(int x = 0; x < rows; x++)
{
  for(int y = 0; y < collumns; y++)
  {
    state[x][y] = false;  
  }
 }
}
