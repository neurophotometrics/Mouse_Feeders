/********************************************************
// Fill the dots one after the other with a color
 ********************************************************/
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void RcolorWipe(uint32_t c, uint8_t wait) {  //reverse color wipe
  for(uint16_t j=0; j<strip.numPixels(); j++) {
    strip.setPixelColor(7-j, c);
    strip.show();
    delay(wait);
  }
}

/********************************************************
   HELPER FUNCTION FOR BLINKING ONBOARD LEDS ON FEATHER
 ********************************************************/
void Blink(byte PIN, byte DELAY_MS, byte loops) {
  for (byte i = 0; i < loops; i++)  {
    digitalWrite(PIN, HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN, LOW);
    delay(DELAY_MS);
  }
}
