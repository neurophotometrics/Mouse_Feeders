void setup() {

  Serial.begin(9600);
  Serial.setTimeout(3);
}

char val;
void loop() {
  Serial.write("arduino");
  if (Serial.available()){
    val = Serial.read();
  }
  Serial.write(val);
  delay(1000);

  
}
