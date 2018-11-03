/********************************************************
  Conditioned stimlus
********************************************************/
void ConditionedStimulus() {
  tone (BUZZER, 4000, 300);
  colorWipe(strip.Color(0, 2, 2), 40); // Color wipe
  colorWipe(strip.Color(0, 0, 0), 20); // OFF
}

void RConditionedStim() {
  tone (BUZZER, 4000, 300);
  RcolorWipe(strip.Color(0, 2, 2), 40); // Color wipe
  RcolorWipe(strip.Color(0, 0, 0), 20); // OFF
}

/********************************************************
  Warning stimulus
********************************************************/
void warningStimulus() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0, 1 ) );
    strip.show();
  }
}

/********************************************************
  Aversive stimulus
********************************************************/
void aversiveStimulus() {
  for (int i = 1; i < 4; i++) {
    tone (BUZZER, random (1000, 6000), 300);
    colorWipe(strip.Color(255, 255, 255, 255), 10); // MAXIMUM BRIGHTNESS!
    colorWipe(strip.Color(0, 0, 0), 10); // OFF
    delay (50);
  }
}

/********************************************************
  Visual tracking stimulus - left
********************************************************/
void leftStimulus() {
  strip.setPixelColor(0, strip.Color(2, 0, 2, 2) );
  strip.show();
}


/********************************************************
  Visual tracking stimulus - left
********************************************************/
void rightStimulus() {
  strip.setPixelColor(7, strip.Color(2, 0, 2, 2) );
  strip.show();
}
