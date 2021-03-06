/********************************************************
   FUNCTION IS CALLED WHEN PELLET_WELL_PIN IS INTERRUPTED
   AND TURNS THE RED LED ON WHENEVER DEVICE IS AWAKE
 *****************************************************/
void wake() {
}

/********************************************************
   FUNCTION FOR PUTTING THE ADALOGGER TO SLEEP
 *****************************************************/
void sleep () {
  ReleaseMotor ();
  digitalWrite (MOTOR_ENABLE, LOW);  //Disable motor driver
  if (digitalRead(PELLET_WELL) == LOW ) {
    delay (1000); //ensure pellet is present for at least 1 second before sleeping
    if (digitalRead(PELLET_WELL) == LOW ) {
      Serial.println ("*****Sleeping*****"); //There is a problem with the M0 Adalogger board where it won't re-attach the USB connection after sleep.  It's a well documented error, but after this line runs the SerialPrints won't work anymore
      DisplaySleep();
      rtc2.standbyMode();
    }
  }
}

void ReleaseMotor () {
  digitalWrite (A2, LOW);
  digitalWrite (A3, LOW);
  digitalWrite (A4, LOW);
  digitalWrite (A5, LOW);
  digitalWrite (MOTOR_ENABLE, LOW);  //Disable motor driver
}
