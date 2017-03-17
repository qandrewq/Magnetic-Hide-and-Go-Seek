#include "meArm.h"
#include <SparkFun_MAG3110.h>

// Set pins for servos on robotic arm
int basePin = 2;
int shoulderPin = 3;
int elbowPin = 4;
int gripperPin = 5; // Not used

// Create x,y,z variables
int x, y, z;

// Create variables for magnetometer ouput
int xf, yf, zf;

// Create magnitude
float magnitude;

// Create maximum magnitude and points to determine strongest reading
int maximum = 0;
int highPoint;

// Used in for loops
int i,j,k;

//Instantiate MAG3110
MAG3110 mag = MAG3110();

// Instantiate arm with defined range
meArm arm(
      0,180, -pi/2, pi/2,    //Base     (-90 to  90 degrees)
      160,70, pi/4, 3*pi/4,   //Shoulder ( 45 to 135 degrees)
      160,125, 0, -pi/4,      //Elbow    (  0 to -45 degrees)
      0,0, pi/2, 0);        //Gripper  ( 90 to   0 degrees) Not used

void setup() {
  arm.begin(basePin, shoulderPin, elbowPin, gripperPin); // Initialize arm with given
  mag.initialize(); //Initialize the MAG3110

  Serial.begin(9600);
}

void loop() {

  arm.gotoPoint(0,100,50); // Go to start position
  delay(500);
  
  // Go to initial scanning position
  while (arm.getX() != -90) {
    if (x >= -90) {
      x = arm.getX() - 20;
    }
    else {
      x = -90;
    }
    arm.gotoPoint(x,75,50);
    delay(150);
  }

  int scan[9][0];
  int n=0;
  
  // Begin scanning process
  while(arm.getX() <= 80) {

    if(mag.dataReady()) {
    //Read the data
    mag.readMag(&xf, &yf, &zf);
    magnitude = zf;
    }
    
    n+=1;    
    x += 20;
    y=125;

    arm.gotoPoint(x,y,25);
    delay(450);

    mag.triggerMeasurement();
    
    scan[n][0]=x;
    scan[n][1]=y;
    scan[n][2]=magnitude;
    Serial.print(scan[n][0]);
    Serial.print(scan[n][1]);
    Serial.print(scan[n][2]);
  }

  for (i=0;i<9;i++) {
      if (scan[i][2] > maximum) {
        highPoint = i;
        maximum = scan[highPoint][2];
      }
    }
  
  int magnetFound = false;
  while (magnetFound == false) {

    int x1,y1;
    
    if(mag.dataReady()) {
    //Read the data
    mag.readMag(&xf, &yf, &zf);
    magnitude = zf;
    }
  
    // Identify strongest x & y values
    n=0;
    x = scan[highPoint][0];
    y = scan[highPoint][1];
  
    // Rotate about highest magnitude x,y
 
    x1=x;
    y1=y;
    arm.gotoPoint(x1,y1,25); // 0
    delay(350);
    mag.triggerMeasurement();
    scan[n][0]=x1;
    scan[n][1]=y1;
    scan[n][2]=magnitude;
    n+=1;
    
    x1 = x - 20;
    y1 = y;
    arm.gotoPoint(x1,y1,25); // 1
    delay(350);
    mag.triggerMeasurement();
    scan[n][0]=x1;
    scan[n][1]=y1;
    scan[n][2]=magnitude;
    n+=1;

    x1 = x-10;
    y1 = y+10;
    arm.gotoPoint(x1,y1,25); // 2
    delay(350);
    mag.triggerMeasurement();
    scan[n][0]=x1;
    scan[n][1]=y1;
    scan[n][2]=magnitude;
    n+=1;

    x1 = x;
    y1 = y+20;
    arm.gotoPoint(x1,y1,25); // 3
    delay(350);
    mag.triggerMeasurement();
    scan[n][0]=x1;
    scan[n][1]=y1;
    scan[n][2]=magnitude;
    n+=1;

    x1=x+10;
    y1=y+10;
    arm.gotoPoint(x1,y1,25); // 4
    delay(350);
    mag.triggerMeasurement();
    scan[n][0]=x1;
    scan[n][1]=y1;
    scan[n][2]=magnitude;
    n+=1;

    x1=x+20;
    y1=y;
    arm.gotoPoint(x1,y1,25); // 5
    delay(350);
    mag.triggerMeasurement();
    scan[n][0]=x1;
    scan[n][1]=y1;
    scan[n][2]=magnitude;
    n+=1;

    x1=x+10;
    y1=y-10;
    arm.gotoPoint(x1,y1,25); // 6
    delay(350);
    mag.triggerMeasurement();
    scan[n][0]=x1;
    scan[n][1]=y1;
    scan[n][2]=magnitude;
    n+=1;

    x1=x;
    y1=y-20;
    arm.gotoPoint(x1,y1,25); // 7
    delay(350);
    mag.triggerMeasurement();
    scan[n][0]=x1;
    scan[n][1]=y1;
    scan[n][2]=magnitude;
    n+=1;

    x1=x-10;
    y1=y-10;
    arm.gotoPoint(x1,y1,25); // 8
    delay(350);
    mag.triggerMeasurement();
    scan[n][0]=x1;
    scan[n][1]=y1;
    scan[n][2]=magnitude;
    n+=1;

    for (i=0;i<9;i++) {
      if (scan[i][2] > maximum) {
        highPoint = i;
        maximum = scan[highPoint][2];
      }
    }

    if (highpoint == 0) {
      magnetFound = true;
    }    
    } 
  }
