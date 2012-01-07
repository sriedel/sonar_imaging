static class SensorData {
  private int phi;
  private int rho;
  private int r;
 
  SensorData( int phi, int rho, int r ) {
    this.phi = phi;
    this.rho = rho;
    this.r = r;
  }
  
  static SensorData fromString( String dataString ) {
    SensorData data = null;
  
    if( dataString != null ) {
      final int[] datapoints = int( split( trim( dataString ), " " ) );
      if( datapoints.length == 3 ) {
        data = new SensorData( datapoints[0], datapoints[1], datapoints[2] );
      }
    }
    
    return data;
  }
  
  int getPhi() {
    return this.phi;
  }
  
  int getRho() {
    return this.rho;
  }
  
  int getR() {
    return this.r;
  }
  
  int x() {
    return int(this.r * sin( this.rho ) * cos( this.phi ));
  }
  
  int y() {
    return int(this.r * sin( this.rho ) * sin( this.phi ));
  }
  
  int z() {
    return int(this.r * cos( rho ));
  }
}
