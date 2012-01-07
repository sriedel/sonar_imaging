#include <Servo.h>

const int BAUD_RATE = 19200;

const int DISTANCE_PIN = 1;

const int H_SERVO_PIN = 8;
const int V_SERVO_PIN = 9;

const int SERVO_MIN = 30;
const int SERVO_MAX = 150;

const int SERVO_SWEEP_MS = 500;

class SweepServo {
  public:
  unsigned char pin;
  unsigned char minDegrees;
  unsigned char maxDegrees;
  unsigned char heading;
  int fullSweepDelay;
  unsigned char headingIncrement;
  boolean directionChanged;
  char headingDirection;
  float oneDegreeSweepDelay;
  Servo servo;
  
  SweepServo( const int pin, const int headingIncrement, const int minDegrees, const int maxDegrees, const int fullSweepDelay ):
    pin( pin ), 
    headingIncrement( headingIncrement ), 
    minDegrees(minDegrees), 
    maxDegrees(maxDegrees), 
    fullSweepDelay(fullSweepDelay ),
    headingDirection(1), 
    directionChanged(false)
  {
    this->oneDegreeSweepDelay = (float)(fullSweepDelay) / (float)(maxDegrees - minDegrees);
    this->servo.attach( pin );
  }
  
  void center( unsigned char heading = 90 ) {
    unsigned char currentHeading = this->servo.read();
    this->servo.write( heading );
    this->heading = heading;
    delay( (int)( abs( heading - currentHeading ) * this->oneDegreeSweepDelay ) );
    this->directionChanged = false;
    this->headingDirection = 1;
  }

  void sweepStep() {
    this->directionChanged = false;
    this->heading += this->headingDirection * this->headingIncrement;
    this->servo.write( this->heading );
    
    if( this->heading >= this->maxDegrees || this->heading <= this->minDegrees ) {
      this->headingDirection = -this->headingDirection;    
      this->directionChanged = true;
    }
  }
  
  int sweepStepDelay() {
    return (int)(this->headingIncrement * this->oneDegreeSweepDelay);
  }
};

SweepServo *horizontalServo;
SweepServo *verticalServo;

void setup() {
  Serial.begin(BAUD_RATE );
  horizontalServo = new SweepServo( H_SERVO_PIN, 1, SERVO_MIN, SERVO_MAX, SERVO_SWEEP_MS );
  verticalServo = new SweepServo( V_SERVO_PIN, 1, 35, 90, SERVO_SWEEP_MS );
  horizontalServo->center();
  verticalServo->center(45);
}

struct Point {
  int x;
  int y;
  int z;
};

void loop() {
 int sweepDelay = 0;
 int distanceUnits = 0;
 static int horizontalDirectionChange = false;
 
 if( horizontalDirectionChange ) {
   verticalServo->sweepStep();
   sweepDelay = verticalServo->sweepStepDelay();
   horizontalDirectionChange = false;
 } else {
   horizontalServo->sweepStep();
   sweepDelay = verticalServo->sweepStepDelay();
   horizontalDirectionChange = horizontalServo->directionChanged;
 }
   
 delay( sweepDelay ); 
 delay( 50 ); // Ensure that the sensor gets a stable output
 distanceUnits = analogRead( DISTANCE_PIN );
 
 int distance = analogUnitsToDistance( distanceUnits );
 int shown_heading = horizontalServo->heading - horizontalServo->headingDirection;

 Serial.print( shown_heading );
 Serial.print( " " );
 Serial.print( verticalServo->heading );
 Serial.print( " " );
 Serial.print( distance );
 Serial.println();
}

int analogUnitsToDistance( int analogUnits ) {
  float a, b;
  
  if( analogUnits > 530 ) {
    a = 27.77;
    b = 3.31;
  } else if( analogUnits > 409 ) {
    a = 16.95;
    b = 3.77;
  } else if( analogUnits > 317 ) {
    a = 22.22;
    b = 3.35;
  } else if( analogUnits > 251 ) {
    a = 31.25;
    b = 2.83;
  } else if( analogUnits > 215 ) {
    a = 55.55;
    b = 2.13;
  } else if( analogUnits > 186 ) {
    a = 71.43;
    b = 1.89;
  } else if( analogUnits > 167 ) {
    a = 111.11;
    b = 1.54;
  } else if( analogUnits > 120 ) {
    a = 200.0;
    b = 1.14;
  } else if( analogUnits > 112 ) {
    a = 250.0;
    b = 1.03;
  } else if( analogUnits > 102 ) {
    a = 200.0;
    b = 1.15;
  } else {
    a = 250.0;
    b = 1.01;
  }
  
  float voltage = analogUnits * 5.0 / 1024.0;
  return (int)(a * (b - voltage) );
}
