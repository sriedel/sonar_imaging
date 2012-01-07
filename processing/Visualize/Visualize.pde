import processing.serial.*;

final int WIDTH = 900;
final int HEIGHT = 900;
final int xCenter = WIDTH / 2;
final int yCenter = HEIGHT / 2;
final int LINE_FEED = 10;
final int BAUD_RATE = 19200;

Serial arduinoPort;
SensorData sensorData;

int phi, rho, r;
int x, y, z;

void serialEvent( Serial port ) {
  sensorData = getSensorData();
  if( sensorData == null ) {
    return;
  }
  
  println( "Phi/Rho/R: " + sensorData.getPhi() + "/" + sensorData.getRho() + "/" + sensorData.getR() );
  println( "X/Y/Z: " + sensorData.x() + "/" + sensorData.y() + "/" + sensorData.z() );
  println();
}

SensorData getSensorData() {
  SensorData data = null;
  if( arduinoPort.available() > 0 ) {
    final String arduinoOutput = arduinoPort.readStringUntil( LINE_FEED );
    data = SensorData.fromString( arduinoOutput );
  }
  return data;
}

final double DegreesToRadiansFactor = Math.PI / 180;

void setup() {
  size( WIDTH, HEIGHT );
  
  println( Serial.list() );
  String arduinoPortname = Serial.list()[0];
  
  arduinoPort = new Serial( this, arduinoPortname, BAUD_RATE );
  arduinoPort.bufferUntil( LINE_FEED );
  background( 0 );
  strokeWeight( 5 );
}

void draw() {
  if( sensorData == null ) {
    return;
  }
  
  int x = 5 * sensorData.getPhi();
  int y = 5 * sensorData.getRho();
  
  stroke( 255 - sensorData.getR() );
  
  point( x, y );
}
