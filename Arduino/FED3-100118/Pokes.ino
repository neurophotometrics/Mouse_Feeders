void CheckPokes() {
  CheckLeft();
  CheckRight();
  CheckReset();

  if (FEDmode == 0) {  // If it's a free feeding session
    Ratio_Met = true;
  }
}

/********************************************************
  Reset FED by holding left and right pokes for 1 second
********************************************************/
void CheckReset() {  //
  if (digitalRead(LEFT_POKE) == LOW & digitalRead(RIGHT_POKE) == LOW ) {
    delay (1000);
    if (digitalRead(LEFT_POKE) == LOW & digitalRead(RIGHT_POKE) == LOW ) {
      display.clearDisplay();
      display.setRotation(3);
      display.setTextColor(BLACK);
      display.setCursor(15, 40);
      display.setTextSize(1);
      display.println("Resetting FED...");
      display.refresh();
      tone (BUZZER, 5000, 400); delay (200); tone (BUZZER, 2000, 300); delay (200); tone (BUZZER, 4000, 600);
      colorWipe(strip.Color(2, 0, 0), 40);  delay (100); // Color wipe
      colorWipe(strip.Color(2, 0, 2), 40); delay (100); // Color wipe
      colorWipe(strip.Color(0, 2, 2), 40); delay (100); // Color wipe
      colorWipe(strip.Color(0, 0, 0), 20); // OFF
      NVIC_SystemReset();      // processor software reset
    }
  }
}

/********************************************************
  Check Left Poke
********************************************************/
bool CheckLeft () {
  if (digitalRead(LEFT_POKE) == HIGH  && FEDmode != 0) {
    LeftReady = true;
  }
  if (LeftReady == true & digitalRead(LEFT_POKE) == LOW) {
    LeftReady = false;
    LeftCount ++;

    if (FEDmode > 0) {
      display.fillCircle(25, 59, 5, BLACK);
      display.refresh();
    }

    if (FEDmode == 4 && LeftCount >= (ratio)) { // if it's a progressive ratio schedule and the LeftCount is greater than or equal to the ratio
      Ratio_Met = true;
      ConditionedStimulus();
    }

    if (FEDmode < 4 && LeftCount % FR < 1 && LeftCount != 0) { //For fixed ratio sessions, test if the number of left counts is divisible by the FR
      Ratio_Met = true;
      ConditionedStimulus();
    }

    if (FEDmode == 5) { //If it's an extintion session do conditioned stimulus and log data after each poke
      ConditionedStimulus();
      logdata();
    }

    if (FEDmode == 6) { //If it's a compulsivity test session
      Ratio_Met = true;
      if (LeftCount % trialsToAversStim == 0 && compulsive == true) {
        aversiveStimulus();
        aversiveStim = 1;
      }
      else {
        ConditionedStimulus();
        aversiveStim = 0;
      }
    }

    if (FEDmode == 7) { //If it's a light tracking task
      if (activePoke == 1) {
        Ratio_Met = true;
        ConditionedStimulus();
      }
      if (Ratio_Met == false) logdata(); // only log pokes here when ratio isn't met, as they will be logged again when it's met
    }

    if (FEDmode == 8) { //If it's a Prob Reversal task
      if (activePoke == 1 && rewardedTrial == true) {  // if a LEFT poke is made, and active poke is LEFT, and it is a HIGH-PROBABILITY trial
        Ratio_Met = true;
        ConditionedStimulus();
        consecCorrect += 1;
      }
      if (activePoke == 1 && rewardedTrial == false) {  // if a LEFT poke is made, and active poke is LEFT, and it is an LOW-PROBABILITY trial
        // ConditionedStimulus();
        consecCorrect += 1;
        //  rewardedTrial =  (random (0, 100) < rewardProb);
        UpdateDisplay();
        logdata();
      }

      if (activePoke == 0 && rewardedTrial == true) {  // if a LEFT poke is made, and active poke is RIGHT , and it is a HIGH-PROBABILITY trial
        consecCorrect = 0;
        //   rewardedTrial =  (random (0, 100) < rewardProb);
        UpdateDisplay();
        logdata();
      }

      if (activePoke == 0 && rewardedTrial == false) { // if a LEFT poke is made, and active poke is RIGHT , and it is a LOW-PROBABILITY trial
        Ratio_Met = true;
        ConditionedStimulus();
        consecCorrect = 0;
      }

    }
    UpdateDisplay();
  }
}

/********************************************************
  Check Right Poke
********************************************************/
bool CheckRight () {
  if (digitalRead(RIGHT_POKE) == HIGH && FEDmode != 0) {
    RightReady = true;
  }
  if (RightReady == true & digitalRead(RIGHT_POKE) == LOW ) {
    RightReady = false;
    RightCount ++;
    if (FEDmode > 0) {
      display.fillCircle(25, 79, 5, BLACK);
      display.refresh();
    }

    if (FEDmode == 5) { //If it's an extintion session log data after every poke
      logdata();
    }

    if (FEDmode == 7) { //If it's a light tracking task we'll log data after every poke
      if (activePoke == 0) {
        Ratio_Met = true;
        RConditionedStim();
      }
      if (Ratio_Met == false)   logdata();
    }

    if (FEDmode == 8) { //If it's a Prob Reversal task
      if (activePoke == 0 && rewardedTrial == true) {  // if a RIGHT poke is made, and active poke is RIGHT, and it is a HIGH-PROBABILITY trial
        Ratio_Met = true;
        ConditionedStimulus();
        consecCorrect += 1;
      }
      if (activePoke == 0 && rewardedTrial == false) {  // if a RIGHT poke is made, and active poke is RIGHT, and it is an LOW-PROBABILITY trial
        // ConditionedStimulus();
        consecCorrect += 1;
        //  rewardedTrial =  (random (0, 100) < rewardProb);
        UpdateDisplay();
        logdata();
      }

      if (activePoke == 1 && rewardedTrial == true) {  // if a RIGHT poke is made, and active poke is LEFT , and it is a HIGH-PROBABILITY trial
        consecCorrect = 0;
        //    rewardedTrial =  (random (0, 100) < rewardProb);
        UpdateDisplay();
        logdata();
      }

      if (activePoke == 1 && rewardedTrial == false) { // if a RIGHT poke is made, and active poke is LEFT , and it is a LOW-PROBABILITY trial
        Ratio_Met = true;
        consecCorrect = 0;
        ConditionedStimulus();
      }

    }
    UpdateDisplay();
  }
}
