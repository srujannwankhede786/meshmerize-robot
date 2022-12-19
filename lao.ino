

/* This is the code for the Arduino Uno Microcontroller
 * Input: the sensor array at the front of the vehicle
 * Outputs signals to the motors to control movement
 * Algorithm: "Left-Hand-on-Wall" Algorithm to solve maze (priorities of checking movements are Left, Straight, Right, Back)
 */


#define leftCenterSensor   A2
#define leftNearSensor     A1
#define leftFarSensor      A0
#define rightCenterSensor  A3
#define rightNearSensor    A4
#define rightFarSensor     A5

int leftCenterReading;
int leftNearReading;
int leftFarReading;
int rightCenterReading;
int rightNearReading;
int rightFarReading;

int leftNudge;
int replaystage;
int rightNudge;

#define leapTime 120

#define leftMotor1  3
#define leftMotor2  5

#define rightMotor1 6
#define rightMotor2 9

#define led 13

char path[30] = {};
int pathLength;
int readLength;

void setup(){
  
  pinMode(leftCenterSensor, INPUT);
  pinMode(leftNearSensor, INPUT);
  pinMode(leftFarSensor, INPUT);
  pinMode(rightCenterSensor, INPUT);
  pinMode(rightNearSensor, INPUT);
  pinMode(rightFarSensor, INPUT);
    
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(led, OUTPUT);
  //Serial.begin(115200);
  digitalWrite(led, HIGH);
  delay(1000);
}


void loop(){
  
 readSensors();                                                                                     
 
 if(leftFarReading<200 && rightFarReading<200 && (leftCenterReading>200 || rightCenterReading>200) ) //if only the middle two sensors can see the black line
 { 
    straight();//Moves straight                                                                                      
 }
 else//otherwise goes to the leftHandWall method
 {                                                                                              
    leftHandWall();                                                                                   
 }

}

void readSensors()//accepts input from sensors
{
  
  leftCenterReading  = analogRead(leftCenterSensor);
  leftNearReading    = analogRead(leftNearSensor);
  leftFarReading     = analogRead(leftFarSensor);
  rightCenterReading = analogRead(rightCenterSensor);
  rightNearReading   = analogRead(rightNearSensor);
  rightFarReading    = analogRead(rightFarSensor);  

}


void leftHandWall(){
  

  if( leftFarReading>200 && rightFarReading>200)//indicates either 3way intersection or end of maze
  {
      analogWrite(leftMotor1, 110);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 110);
      analogWrite(rightMotor2, 0);
    delay(100);
    readSensors();
    
    if(leftFarReading>200 || rightFarReading>200)//if it moves forward and still sees all black, maze is done
    {
      done();
    }
    if(leftFarReading<200 && rightFarReading<200)//otherwise it turns left
    { 
      turnLeft();
    }
    
  }
  
  if(leftFarReading>200)//indicates that a left turn is possible
  { 
      analogWrite(leftMotor1, 90);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 90);
      analogWrite(rightMotor2,0);
    delay(leapTime);
    readSensors();
      
      if(leftFarReading<200 && rightFarReading<200)//checks to make sure maze isn't done
      {
        turnLeft();
      }
      else
      {
        done();
      }
  }
   
  if(rightFarReading>200)//indicates a right turn is possible
  {
     analogWrite(leftMotor1, 90);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 90);
      analogWrite(rightMotor2, 0);
    delay(10);//CHANGED FROM 30 to 10
    readSensors();
    
    if(leftFarReading>200)//left turn is also possible
    {
      delay(leapTime-30);
      readSensors();
      
      if(rightFarReading>200 && leftFarReading>200)//end of maze
      {
        done();
      }
      else//make the left turn
      {
        turnLeft();
        return;
      }
   }
    delay(leapTime+30);
    readSensors();
    if(leftFarReading<200 && leftCenterReading<200 &&
      rightCenterReading<200 && rightFarReading<200)
      {
      turnRight();
      return;
    }
    path[pathLength]='S';
   // Serial.println("s");
    pathLength++;
    //Serial.print("Path length: ");
    //Serial.println(pathLength);
    if(path[pathLength-2]=='B'){
    //Serial.println("shortening path");
    shortPath();
     }
    straight();
  }
  readSensors();
  if(leftFarReading<200 && leftCenterReading<200 && rightCenterReading<200 
    && rightFarReading<200 && leftNearReading<200 && rightNearReading<200)\
  {
    turnAround();
  }

}
void done(){
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 0);
      analogWrite(rightMotor2, 0);
  replaystage=1;
  path[pathLength]='D';
  pathLength++;
 while(analogRead(leftFarSensor)>200){
   digitalWrite(led, LOW);
   delay(150);
   digitalWrite(led, HIGH);
   delay(150);
 }
 delay(500);
  replay();
}

void turnLeft(){

 
      analogWrite(leftMotor1, 0);
      analogWrite(leftMotor2, 60);
      analogWrite(rightMotor1, 60);//jhhguyutfuytfuytfyut
      analogWrite(rightMotor2, 0);
    delay(120);
 
 
  while(analogRead(rightCenterSensor)>200||analogRead(leftCenterSensor)>200)
  {
     analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, 60);
   analogWrite(rightMotor1, 60);
    analogWrite(rightMotor2, 0);
    delay(2);
    
     analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, 0);
    delay(1);
  }
    
  while(analogRead(rightCenterSensor)<200){
   analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, 60);
    analogWrite(rightMotor1, 60);
   analogWrite(rightMotor2, 0);
    delay(2);
    analogWrite(leftMotor1, 0);
   analogWrite(leftMotor2, 0);
   analogWrite(rightMotor1, 0);
   analogWrite(rightMotor2, 0);
    delay(1);
    analogWrite(leftMotor1, 0);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 0);//jhhguyutfuytfuytfyut
      analogWrite(rightMotor2, 0);
   delay(1);
  
  }
 
  if(replaystage==0){
    path[pathLength]='L';
    //Serial.println("l");
    pathLength++;
    //Serial.print("Path length: ");
    //Serial.println(pathLength);
      if(path[pathLength-2]=='B'){
        //Serial.println("shortening path");
        shortPath();
      }
  }
}

void turnRight(){

 //while(a1nalogRead(rightCenterSensor)<200 && leftCentreSensor<200 && leftFarSensor<200 && )
    
  
      
      
      
      analogWrite(leftMotor1, 60);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 0);
      analogWrite(rightMotor2, 60);
    delay(120);
  
  
    
  while(analogRead(rightCenterSensor)>200){
     analogWrite(leftMotor1, 60);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, 60);
    delay(2);
     analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
   analogWrite(rightMotor2, 0);
    delay(1);
  }
   while(analogRead(rightCenterSensor)<200){
    analogWrite(leftMotor1, 60);
    analogWrite(leftMotor2, 0);
  analogWrite(rightMotor1, 0);
  analogWrite(rightMotor2, 60);
    delay(2);
     analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, 0);
    delay(1);
  }
   while(analogRead(leftCenterSensor)<200){
     analogWrite(leftMotor1, 60);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, 60);
    delay(2);
    digitalWrite(leftMotor1, 0);
    digitalWrite(leftMotor2, 0);
    digitalWrite(rightMotor1, 0);
    digitalWrite(rightMotor2, 0);
    delay(1);
  }
  
  
  if(replaystage==0){
  path[pathLength]='R';
  Serial.println("r");
  pathLength++;
  Serial.print("Path length: ");
  Serial.println(pathLength);
    if(path[pathLength-2]=='B'){
      Serial.println("shortening path");
      shortPath();
    }
  }
  
 
}

void straight(){
if( analogRead(leftCenterSensor)<200){
      analogWrite(leftMotor1,50);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 0);
      analogWrite(rightMotor2, 0);
    delay(1);
      analogWrite(leftMotor1, 50);
      analogWrite(leftMotor2, 0);
     analogWrite(rightMotor1, 0);
      analogWrite(rightMotor2, 0);
    delay(1);
    return;
  }
  if(analogRead(rightCenterSensor)<200){
      analogWrite(leftMotor1, 0);
     analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1,30 );
      analogWrite(rightMotor2, 0);
    delay(1);
      analogWrite(leftMotor1, 0);
      analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 30);
      analogWrite(rightMotor2, 0);
    delay(1);
    return;
  }
  
      analogWrite(leftMotor1, 90);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 90);
      analogWrite(rightMotor2, 0);
    delay(4);
      analogWrite(leftMotor1, 0);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 0);
      analogWrite(rightMotor2, 0);
    delay(1);
  
}

void turnAround()
{  readSensors();    
       analogWrite(leftMotor1,50);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 0);
      analogWrite(rightMotor2,50); 
delay(120);
 
     while((analogRead(leftCenterSensor)<200) &&  (analogRead(rightCenterSensor)<200))
      {analogWrite(leftMotor1,50);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 0);
      analogWrite(rightMotor2,50);}
    delay(10);
    
      analogWrite(leftMotor1, 0);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 0);
      analogWrite(rightMotor2, 0);
  delay(5);
    analogWrite(leftMotor1,0);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 0);
      analogWrite(rightMotor2,0);
 
delay(50); 
  path[pathLength]='B';
  pathLength++;
  straight();
  //Serial.println("b");
  //Serial.print("Path length: ");
  //Serial.println(pathLength);
}

void shortPath(){
 int shortDone=0;
  if(path[pathLength-3]=='L' && path[pathLength-1]=='R'){
    pathLength-=3;
    path[pathLength]='B';
    //Serial.println("test1");
    shortDone=1;
  }
   
  if(path[pathLength-3]=='L' && path[pathLength-1]=='S' && shortDone==0){
    pathLength-=3;
    path[pathLength]='R';
    //Serial.println("test2");
    shortDone=1;
  }
   
  if(path[pathLength-3]=='R' && path[pathLength-1]=='L' && shortDone==0){
    pathLength-=3;
    path[pathLength]='B';
    //Serial.println("test3");
    shortDone=1;
  }
  
   
  if(path[pathLength-3]=='S' && path[pathLength-1]=='L' && shortDone==0){
    pathLength-=3;
    path[pathLength]='R';
    //Serial.println("test4");
    shortDone=1;
  }
     
  if(path[pathLength-3]=='S' && path[pathLength-1]=='S' && shortDone==0){
    pathLength-=3;
    path[pathLength]='B';
    //Serial.println("test5");
    shortDone=1;
  }
    if(path[pathLength-3]=='L' && path[pathLength-1]=='L' && shortDone==0){
    pathLength-=3;
    path[pathLength]='S';
    //Serial.println("test6");
    shortDone=1;
  }
  
  path[pathLength+1]='D';
  path[pathLength+2]='D';
  pathLength++;
  //Serial.print("Path length: ");
  //Serial.println(pathLength);
  //printPath();
}




void printPath(){
  Serial.println("+++++++++++++++++");
  int x;
  while(x<=pathLength){
  Serial.println(path[x]);
  x++;
  }
  Serial.println("+++++++++++++++++");
}


void replay(){
   readSensors();
  if(leftFarReading<200 && rightFarReading<200){
    straight();
  }
  else{
    if(path[readLength]=='D'){
         analogWrite(leftMotor1, 90);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 90);
      analogWrite(rightMotor2, 0);
    delay(100);
      analogWrite(leftMotor1, 0);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 0);
      analogWrite(rightMotor2, 0);
      endMotion();
    }
    if(path[readLength]=='L'){
       analogWrite(leftMotor1, 90);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 90);
      analogWrite(rightMotor2, 0);
    delay(leapTime);
      turnLeft();
    }
    if(path[readLength]=='R'){
         analogWrite(leftMotor1, 90);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 90);
      analogWrite(rightMotor2, 0);
    delay(leapTime);
    turnRight();
    }
    if(path[readLength]=='S'){
         analogWrite(leftMotor1, 90);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 90);
      analogWrite(rightMotor2, 0);
    delay(leapTime);
    straight();
    }
    
    readLength++;
  }
    
  replay();
  
}

void endMotion(){
    digitalWrite(led, LOW);
    delay(500);
    digitalWrite(led, HIGH);
    delay(200);
      digitalWrite(led, LOW);
    delay(200);
    digitalWrite(led, HIGH);
    delay(500);
  endMotion();
}


