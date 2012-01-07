class SensorData {
  private int bearing;
  private int range;
 
  SensorData( int bearing, int range ) {
    this.bearing = bearing;
    this.range = range;
  }
  
  int getBearing() {
    return this.bearing;
  }
  
  int getRange() {
    return this.range;
  }
}
