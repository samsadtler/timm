int incomingByte = 0;

void setup(){
   Serial.begin(9600);
   Serial1.begin(9600);
   while(!Serial.isConnected()) // wait for Host to open serial port
    Particle.process();

  Serial.println("Hello there!");
}

void serialEvent(){
    char c = Serial.read();
    Serial.print(c);
}

void loop() {
  // send data only when you receive data:
  if (Serial1.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("I received from Scale: ");
    Serial1.println(incomingByte, DEC);
  }
}
