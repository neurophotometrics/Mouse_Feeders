/********************************************************
  Feed function.  This turns the stepper forward and backwards to dispense a pellet,
  stopping once pellet is dispensed.
********************************************************/
void Feed()  {
  if (FEDmode != 5) { //if not an extinction session
    display.fillTriangle (12, 55, 18, 59, 12, 63, WHITE);
    display.fillTriangle (12, 75, 18, 79, 12, 83, WHITE);
    
    digitalWrite (MOTOR_ENABLE, HIGH);  //Enable motor driver
    if (digitalRead (PELLET_WELL) == HIGH) {
      delay (200); //simple debounce to ensure pellet is removed for >200ms
      if (digitalRead (PELLET_WELL) == HIGH) {

        DisplayDispense();

        CountReady = true;
        for (int i = 0; i < 8 + random(0, 2); i++) {
          if (digitalRead (PELLET_WELL) == HIGH) {
            stepper.step(-i);
          }
        }
        ReleaseMotor ();
      }
    }
    numMotorTurns++;
    delay (500);  //delay before switching directions on motor

    //Move pellet disk backwards a variable amount between 20 and 40
    if (digitalRead (PELLET_WELL) == HIGH) {
      digitalWrite (MOTOR_ENABLE, HIGH);  //Enable motor driver

      for (int i = 0; i < 5 + random(0, 2); i++) {
        if (digitalRead (PELLET_WELL) == HIGH) {
          stepper.step(i);
        }
      }
      ReleaseMotor ();
    }
    if (numMotorTurns == 10) {
      VibrateJam();
    }
    if (numMotorTurns == 20) {
      ClearJam();
    }
  }
}

/********************************************************
    ClearJam makes large movements if the Feed function fails to dispense, to try to dislodge a pellet jam.
  ********************************************************/
void ClearJam() {
  if (FEDmode != 5) {
    numJamClears++;
    PelletJam = false;
    DisplayJamClear();
    if (digitalRead (PELLET_WELL) == HIGH) {
      delay (500); //simple debounce to ensure pellet is out for at least 500ms
      if (digitalRead (PELLET_WELL) == HIGH) {
        digitalWrite (MOTOR_ENABLE, HIGH);  //Enable motor driver
        for (int i = 0; i < 21 + random(0, 20); i++) {
          if (digitalRead (PELLET_WELL) == HIGH) {
            stepper.step(i);
          }
        }
      }
      ReleaseMotor ();
    }
    if (digitalRead (PELLET_WELL) == HIGH) {
      delay (500); //simple debounce
      if (digitalRead (PELLET_WELL) == HIGH) {
        digitalWrite (MOTOR_ENABLE, HIGH);  //Enable motor driver
        for (int i = 0; i < 21 + random(0, 20); i++) {
          if (digitalRead (PELLET_WELL) == HIGH) {
            stepper.step(-i);
          }
        }
        ReleaseMotor ();
      }
    }
    display.fillRoundRect (5, 15, 120, 80, 1, WHITE);  //erase the "Jam clear" text without clearing the entire screen by pasting a white box over it
    numMotorTurns = 0;
  }
}

/********************************************************
   Try vibrations to clear jam
  ********************************************************/
void VibrateJam() {
  if (FEDmode != 5) {
    numJamClears++;
    PelletJam = false;
    DisplayJamClear();
    if (digitalRead (PELLET_WELL) == HIGH) {
      delay (500); //simple debounce to ensure pellet is out for at least 500ms
      if (digitalRead (PELLET_WELL) == HIGH) {
        digitalWrite (MOTOR_ENABLE, HIGH);  //Enable motor driver
        for (int j = 0; j < 100; j++) {
          if (digitalRead (PELLET_WELL) == HIGH) {
            stepper.step(-10);
            stepper.step(12);
          }
        }
        ReleaseMotor ();
      }
    }

  }
  display.fillRoundRect (5, 15, 120, 80, 1, WHITE);  //erase the "Jam clear" text without clearing the entire screen by pasting a white box over it
}
