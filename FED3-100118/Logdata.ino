void logdata() {
  DisplaySDLogging();
  WriteToSD();
}

// Create new file on uSD incrementing file name as required
void CreateFile() {
  //put this next line *Right Before* any file open line:
  SdFile::dateTimeCallback(dateTime);

  // see if the card is present and can be initialized:
  if (!SD.begin(cardSelect)) {
    error(2);
  }
  // create files if they dont exist and grab device name and ratio
  configfile = SD.open("DeviceNumber.csv", FILE_WRITE);
  configfile = SD.open("DeviceNumber.csv", FILE_READ);
  FED = configfile.parseInt();
  configfile.close();

  ratiofile = SD.open("FEDmode.csv", FILE_WRITE);
  ratiofile = SD.open("FEDmode.csv", FILE_READ);
  FEDmode = ratiofile.parseInt();
  ratiofile.close();

  // Name filename in format F###_MMDDYYNN, where MM is month, DD is day, YY is year, and NN is an incrementing number for the number of files initialized each day
  strcpy(filename, "FED_____________.CSV");  // placeholder filename
  getFilename(filename);
}

void CreateDataFile () {
  getFilename(filename);
  logfile = SD.open(filename, FILE_WRITE);
  if ( ! logfile ) {
    error(3);
  }
}

// Write data header to file of uSD.
void writeHeader() {
  logfile.println("MM:DD:YYYY hh:mm:ss, Device_Number, Battery_Voltage, Motor_Turns, FR_Ratio, Warning_Stim, Aversive_Stim, Active_Poke, Reward_Prob, Left_Poke_Count, Right_Poke_Count, Pellet_Count, Retrieval_Time");
}

void writeConfigFile() {
  configfile = SD.open("DeviceNumber.csv", FILE_WRITE);
  configfile.rewind();
  configfile.println(FED);
  configfile.flush();
  configfile.close();
}

void writeFEDmode() {
  ratiofile = SD.open("FEDmode.csv", FILE_WRITE);
  ratiofile.rewind();
  ratiofile.println(FEDmode);
  ratiofile.flush();
  ratiofile.close();
}

// Print data and time followed by pellet count and motorturns to SD card
void WriteToSD() {
  DateTime now = rtc.now();

  logfile.print(now.month());
  logfile.print("/");
  logfile.print(now.day());
  logfile.print("/");
  logfile.print(now.year());
  logfile.print(" ");
  logfile.print(now.hour());
  logfile.print(":");
  if (now.minute() < 10)
    logfile.print('0');      // Trick to add leading zero for formatting
  logfile.print(now.minute());
  logfile.print(":");
  if (now.second() < 10)
    logfile.print('0');      // Trick to add leading zero for formatting
  logfile.print(now.second());
  logfile.print(",");

  logfile.print(FED); // Print device name
  logfile.print(",");

  logfile.print(measuredvbat); // Print battery voltage
  logfile.print(",");

  logfile.print((numJamClears * 10) + numMotorTurns); // Print the number of attempts to dispense a pellet, including through jam clears
  numMotorTurns = 0; //reset numMotorTurns
  numJamClears = 0; // reset numJamClears
  logfile.print(",");

  if (FEDmode == 4) {
    logfile.print(round ((5 * exp (0.2 * PelletCount)) - 5)); // Print current PR ratio
    logfile.print(",");
  }

  else if (FEDmode == 0) {
    logfile.print("FED"); // Print trial type
    logfile.print(",");
  }

  else if (FEDmode == 5) {
    logfile.print("Extinction"); // Print trial type
    logfile.print(",");
  }

  else if (FEDmode == 6) {
    logfile.print("Compulsivity_test"); // Print trial type
    logfile.print(",");
  }

  else if (FEDmode == 7) {
    logfile.print("FR1_Light_tracking"); // Print trial type
    logfile.print(",");
  }

  else if (FEDmode == 7) {
    logfile.print("Prob_Reversal"); // Print trial type
    logfile.print(",");
  }

  else {
    logfile.print(FR); // Print ratio
    logfile.print(",");
  }

  logfile.print(compulsive); // Print whether trial had warning stimulus
  logfile.print(",");

  logfile.print(aversiveStim); // Print whether trial had aversive stimulus
  logfile.print(",");

  if (FEDmode == 7 | FEDmode == 8) {
    if (activePoke == 0)  logfile.print("Right"); //
    if (activePoke == 1)  logfile.print("Left"); //
  }
  else {
    logfile.print("Left"); // Print whether trial had aversive stimulus
  }
  logfile.print(",");

  if (FEDmode == 8) {
    if (rewardedTrial == true)       logfile.print ("High");
    if (rewardedTrial == false)       logfile.print ("Low");
    logfile.print(",");
  }
  else {
    logfile.print ("NA");
    logfile.print(",");
  }

  logfile.print(LeftCount); // Print Left poke count
  logfile.print(",");

  logfile.print(RightCount); // Print Right poke count
  logfile.print(",");

  logfile.print(PelletCount); // print Pellet counts
  logfile.print(",");

  logfile.println(retInterval); // print interval between pellet dispensing and being taken

  Blink(GREEN_LED, 100, 2);
  logfile.flush();
  // logfile.close();
}

void error(uint8_t errno) {
  DisplaySDError();
  while (1) {
    uint8_t i;
    for (i = 0; i < errno; i++) {
      Blink(GREEN_LED, 25, 2);
      colorWipe(strip.Color(5, 0, 0), 25); // RED
    }
    for (i = errno; i < 10; i++) {
      colorWipe(strip.Color(0, 0, 0), 25); // clear
    }
    CheckReset();
  }
}
void getFilename(char *filename) {
  DateTime now = rtc.now();

  filename[3] = FED / 100 + '0';
  filename[4] = FED / 10 + '0';
  filename[5] = FED % 10 + '0';
  filename[7] = now.month() / 10 + '0';
  filename[8] = now.month() % 10 + '0';
  filename[9] = now.day() / 10 + '0';
  filename[10] = now.day() % 10 + '0';
  filename[11] = (now.year() - 2000) / 10 + '0';
  filename[12] = (now.year() - 2000) % 10 + '0';
  filename[16] = '.';
  filename[17] = 'C';
  filename[18] = 'S';
  filename[19] = 'V';
  for (uint8_t i = 0; i < 100; i++) {
    filename[14] = '0' + i / 10;
    filename[15] = '0' + i % 10;

    if (! SD.exists(filename)) {
      break;
    }
  }
  return;
}
