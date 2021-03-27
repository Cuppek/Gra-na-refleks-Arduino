int led1 = 2;
int led2 = 3;
int led3 = 4;
int led4 = 5;
int ledOdliczanie = 6;

int buttonPin1 = 10;
int buttonPin2 = 11;
int buttonPin3 = 12;
int buttonPin4 = 13;

int buttonState1;
int buttonState2;
int buttonState3;
int buttonState4;

int i;
int number = 0;
int penalty = 0;
int clickCounter = 0;
int allClicksCounter = 0;
int lvl = 0;
int minRndTime = 0;
int maxRndTime = 0;
int maxLed = 0;
int maxPenalties = 0;
int czasOdliczanie = 1000;
int mode = 0;
int clickFourthModeCounter = 0;
int missedBonks = 0;
int missedBonksInRow = 0;
int numGoodElements = 0;
int numBadElements = 0;

unsigned long actTime = 0;
unsigned long endTime = 0;
unsigned long wholeTime = 0;
unsigned long playerTime = 0;
unsigned long reactionTime = 0;
unsigned long fiveSecTime = 0;
unsigned long bonkRndTime = 0;
unsigned long bonkStartTime = 500;
unsigned long bonkWindowTime = 1000;

int val1 = 0;
int val11 = 0;
int val2 = 0;
int val21 = 0;
int val3 = 0;
int val31 = 0;

bool again = false;
bool btnPressedBefore = false;
bool bonkAdjusted = false;
bool bonkMissed = false;

String goodArray[] = {"Nice!", "Good One", "Excelent", "WOW", "Pure luck"};
String badArray[] = {"Wrong!", "Bad One", "Not Your day", "No luck", "LOL"};

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

  // Deklaracja pinów LED
  for (i = 2; i <= 9; i++){
    pinMode(i, OUTPUT);
  }

  // Deklaracja pinów przycisków
  for (i = 10; i < 14; i++){
    pinMode(i, INPUT);
  }

  numGoodElements = (sizeof(goodArray) / sizeof(goodArray[0]));
  numBadElements = (sizeof(badArray) / sizeof(badArray[0]));
}

void loop() {
  // Światełka
  while (0>1){
    for (int x = 0; x < 1024; x++){
      int val = x;
      val1  = map(val, 0, 200, 0, 255);
      val11 = map(val, 201, 400, 255, 0);
      val2  = map(val, 300, 500, 0, 255);
      val21 = map(val, 501, 700, 255, 0);
      val3  = map(val, 600, 800, 0, 255);
      val31 = map(val, 801, 1023, 255, 0);
  
      // Wyłączenia
      if (val > 200)              {val1   = 0;};
      if (val < 200 || val > 400) {val11  = 0;};
      if (val < 300 || val > 500) {val2   = 0;};
      if (val < 500 || val > 700) {val21  = 0;};
      if (val < 600 || val > 800) {val3   = 0;};
      if (val < 800)              {val31  = 0;};
  
      // Deklaracje
      if      (val > 0    && val <= 200)  {analogWrite(7, val1);}
      else if (val > 200  && val <= 400)  {analogWrite(7, val11);}
      if      (val >= 300 && val <= 500)  {analogWrite(8, val2);}
      else if (val > 500  && val <= 700)  {analogWrite(8, val21);}   
      if      (val >= 600 && val <= 800)  {analogWrite(9, val3);}
      else if (val > 800  && val < 1023)  {analogWrite(9, val31);}
      if      (val == 0   || val == 1023) {analogWrite(7, 0);analogWrite(8, 0);analogWrite(9, 0);}
  
      delay(5);
    }
  }

  delay(1000);
  clickFourthModeCounter = 0;
  waitForAllLowDelay();
  while (mode == 0){
    Serial.println("Choose game mode: ");
    Serial.println("1. 20 moves, final score = average reaction time + penalty point * 20 ms.");
    Serial.println("2. 5 sec, make as many moves as possible, penalty points are substract from your score.");
    Serial.println("3. Infinite mode.");
    switch (clickFourthModeCounter){
      case 0:
        Serial.println("4. There is nothing.");
        break;
      case 1:
        Serial.println("4. There is nothing.");
        break;
      case 2:
        Serial.println("4. Looking for something, huh?.");
        break;
      case 3:
        Serial.println("4. Why You even try'in.");
        break;
      case 4:
        Serial.println("4. rly?");
        break;
    }
    waitForAnyHigh();
    buttonState();
    if (buttonState1 == HIGH) {
      mode = 1;
    } else if (buttonState2 == HIGH) {
      mode = 2;
    } else if (buttonState3 == HIGH) {
      mode = 3;
    }
    if (buttonState4 == HIGH) {
      mode = 0;
      clickFourthModeCounter += 1;
      if (clickFourthModeCounter == 5){
        Serial.println("NOW YOU GONNA PLAY BONK MODE!");
        Serial.println("LET'S GO!");
        mode = 4;
        minRndTime = 50;
        maxRndTime = 500;
        maxLed = 4 + 1;
      } else {
        Serial.println(" ");
        Serial.println("Choose correct mode (1-3): ");
      }
    } else {
      Serial.print("So You chose mode ");
      Serial.println(mode);
      Serial.println(" ");
    }
    waitForAllLowDelay();
  }

  waitForAllLow();
  while (again == false && mode != 4){
    delay(500);
    Serial.println("Choose difficulty level: ");
    switch (mode) {
      case (1):
        Serial.println("1. Easy - 3 LEDs + normal time to prepare.");
        Serial.println("2. Medium - 4 LEDs + normal time to prepare.");
        Serial.println("3. Hard - 4 LEDs + short time to prepare.");
        Serial.println("4. Fast Hard - 2 LEDs + short time to prepare.");
        break;     
      case (2):
        Serial.println("1. Easy - 2 LEDs.");
        Serial.println("2. Medium - 3 LEDs.");
        Serial.println("3. Hard - 4 LEDs.");
        Serial.println("4. Fast Hard - 4 LEDs + no time between LEDs.");
        break;
      case (3):
        Serial.println("1. Easy - 3 LEDs + normal time to prepare + max 7 wrong clicks.");
        Serial.println("2. Medium - 4 LEDs + normal time to prepare + max 5 wrong clicks.");
        Serial.println("3. Hard - 4 LEDs + short time to prepare + max 3 wrong clicks.");
        Serial.println("4. Fast Hard - 2 LEDs + short time to prepare + max 3 wrong clicks.");
        break;     
    }
  
    waitForAnyHigh();
    buttonState();
    if (buttonState1 == HIGH) { 
      lvl = 1;
      minRndTime = 500;
      maxRndTime = 2000;
      maxPenalties = 7;
      if (mode == 2) {maxLed = 2 + 1;} else {maxLed = 3 + 1;}
    } else if (buttonState2 == HIGH) {
      lvl = 2;
      minRndTime = 500;
      maxRndTime = 2000;
      maxPenalties = 5;
      if (mode == 2) {maxLed = 3 + 1;} else {maxLed = 4 + 1;}  
    } else if (buttonState3 == HIGH) {
      lvl = 3;
      minRndTime = 50;
      maxRndTime = 250;
      maxPenalties = 3;
      if (mode == 2) {maxLed = 4 + 1;} else {maxLed = 4 + 1;}    
    } else {
      lvl = 4;
      minRndTime = 50;
      maxRndTime = 250;
      maxPenalties = 3;
      if (mode == 2) {maxLed = 4 + 1;} else {maxLed = 2 + 1;}
    } 
    waitForAllLowDelay();
  
    Serial.print("So You chose lvl ");
    Serial.println(lvl);
    again = true;
  }

  if (mode != 4){
    waitForAllLowDelay();
    Serial.println("Press any button when You are ready!");
    waitForAnyHigh();
    Serial.println("Get READY!");
  }
  odliczanie();

  fiveSecTime = millis() + 5050;

  // Główna pętla gry, program wychodzi z niej w momencie końca gry
  while (true) {
    if (mode == 2){
      if (lvl == 4) {
        delay(5);
        }
      else {
        delay(50);
        } 
    } else {
      delay(random(minRndTime, maxRndTime));
    }
    
    actTime = millis();
    
    if (mode == 4 && allClicksCounter == 5){
      bonkAdjusted = true;
      if (clickCounter == 0){
        bonkWindowTime = 1500;
        bonkStartTime = 500;
        }
      else {
        bonkWindowTime = (wholeTime / clickCounter) * 1.5;
        bonkStartTime = (wholeTime / clickCounter) * 0.7;
        }
    }
    
    if (bonkMissed) {
      bonkMissed = false;
      missedBonks += 1;
      if (bonkAdjusted) {
        bonkWindowTime = bonkWindowTime * 1.2;
        bonkStartTime = bonkStartTime * 1.2;
      }
    }
    
    bonkRndTime = millis() + random(bonkStartTime, (bonkStartTime + bonkWindowTime));   
//    Serial.println(bonkStartTime);
//    Serial.println(bonkWindowTime);
    
    buttonState();
    if (buttonState1 == LOW && buttonState2 == LOW && buttonState3 == LOW && buttonState4 == LOW) {
      lightLED();
      waitForAnyHighInGame();
      switch (number) {
        buttonState();
        case 1:
          if (buttonState1 == HIGH) {
            dobrze();
          } else {
            dodajKare();
          }
          digitalWrite(led1, LOW);
          break;
        case 2:
          if (buttonState2 == HIGH) {
            dobrze();
          } else {
            dodajKare();
          }
          digitalWrite(led2, LOW);
          break;
        case 3:
          if (buttonState3 == HIGH) {
            dobrze();
          } else {
            dodajKare();
          }
          digitalWrite(led3, LOW);
          break;
        case 4:
          if (buttonState4 == HIGH) {
            dobrze();
          } else {
            dodajKare();
          }
          digitalWrite(led4, LOW);
          break;  
    }
    waitForAllLow();
    
    } else {
      Serial.println("Button pressed before the game!");
      btnPressedBefore = true;
      dodajKare();
      waitForAllLow();
    }
//     Serial.println(missedBonksInRow);
    // Pętla kończąca grę
    if ((mode == 1 && allClicksCounter == 20) || 
        (mode == 2 && millis() >= fiveSecTime) || 
        (mode == 3 && penalty == maxPenalties) ||
        (mode == 4 && allClicksCounter == 40) ||
        (mode == 4 && missedBonksInRow >= 4)) {
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      digitalWrite(led4, HIGH);
      if (mode == 3){   
      Serial.println("Game over!");
      } else {
        Serial.println("The end!");
      }
      Serial.print("You did ");
      Serial.print(clickCounter);
      Serial.print("/");
      Serial.print(allClicksCounter);
      Serial.print(" right clicks with average time ");
      if (clickCounter == 0) {
        Serial.print("-");
      } else {
      Serial.print(wholeTime / clickCounter);
      }
      Serial.println(" ms/click");
      
      if (mode == 4) {
        Serial.print("And You missed ");
        Serial.print(missedBonks);
        Serial.println(" Bonks");
      }
      
      if (mode == 1 || mode == 2){
        Serial.print("Your final score is: ");
        if (mode == 1) {
          Serial.println((wholeTime / clickCounter) + (penalty * 20));
        } else if (mode == 2) {
          Serial.println(clickCounter - penalty);
        }
      }
      Serial.println("");
      
      if (mode == 4) {
        Serial.println("Wanna try again?");
        Serial.println("1. Gimme more");
        Serial.println("2. Momy take me from here");
      } else {
        Serial.print("Actual mode ");
        Serial.print(mode);
        Serial.print(" with difficulty ");
        Serial.println(lvl);
        Serial.println("Press button 1 to start again in the same mode and with the same difficulty."); 
        Serial.println("Press button 2 to change only difficulty level in mode You just played."); 
        Serial.println("Press button 3 to change game mode and difficulty level.");
        Serial.println("");
      }
      waitForAllLowDelay();
      delay(1000);

      while (true) {
        waitForAnyHigh();
        if (buttonState1 == HIGH){
          again = true;
          break;
        } else if (buttonState2 == HIGH && mode != 4){
          again = false;
          break;
        } else if (buttonState3 == HIGH && mode != 4){
          again = false;
          mode = 0;
          break;
        } else if (mode == 4 && buttonState2 == HIGH){
          again = false;
          mode = 0;
          break;
        } else if (mode == 4 && (buttonState3 == HIGH || buttonState4 == HIGH )){
          Serial.println("Press correct button (1-2).");
          Serial.println(" ");
          waitForAllLowDelay();
          clickFourthModeCounter += 1;
          if (clickFourthModeCounter == 5){
            Serial.println("FUCK YOU");
            delay(100);
            exit(0);
          }
        } else {
          Serial.println("Press correct button (1-3).");
          Serial.println(" ");
          waitForAllLow();
          clickFourthModeCounter += 1;
          if (clickFourthModeCounter == 5){
            Serial.println("FUCK YOU");
            delay(100);
            exit(0);
          }
        }
      }

      waitForAllLowDelay();
      missedBonksInRow = 0;
      missedBonks = 0;
      penalty = 0;
      wholeTime = 0;
      clickCounter = 0;
      allClicksCounter = 0;
      bonkAdjusted = false;
      bonkStartTime = 500;
      bonkWindowTime = 1000;
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
      break;
    }
  }
}

void dobrze() {
  clickCounter += 1;
  allClicksCounter += 1;
  playerTime = Time(actTime);
  wholeTime += playerTime;
  Serial.println(goodArray[random(0, numGoodElements)]);
  Serial.print("Your reaction time: ");
  Serial.print(playerTime);
  Serial.println(" ms");
  Serial.print("Your average reaction time: ");
  Serial.print(wholeTime / clickCounter);
  Serial.println(" ms"); 
  Serial.println("");
  if (bonkAdjusted) {
    if (playerTime <= (wholeTime / clickCounter)){
      bonkStartTime = bonkStartTime * 0.85;
      bonkWindowTime = bonkWindowTime * 0.85;
    }
      bonkStartTime = bonkStartTime * 0.95;
      bonkWindowTime = bonkWindowTime * 0.95;
  }
  missedBonksInRow = 0;
}

void dodajKare() {
  if (btnPressedBefore == false){
    Serial.println(badArray[random(0, numBadElements)]);
  }
  if (bonkMissed) {Serial.println("LOSER");}
  if (bonkAdjusted && !bonkMissed) {
    bonkStartTime = bonkStartTime * 1.2;
    bonkWindowTime = bonkWindowTime * 1.2;
    }
  penalty += 1;
  allClicksCounter += 1;
  Serial.println("1 penalty point has been added.");
  Serial.print("Total penalty points: ");
  Serial.println(penalty); 
  Serial.println("");
  btnPressedBefore = false;
}

void lightLED() {
  switch (number = random(1, maxLed)) {
    case 1:
      digitalWrite(led1, HIGH);
      break;
    case 2:
      digitalWrite(led2, HIGH);
      break;
    case 3:
      digitalWrite(led3, HIGH);
      break;
    case 4:
      digitalWrite(led4, HIGH);
      break;
  }
}

void odliczanie() {
  for (int j = 0; j < 3; j++){
    digitalWrite(ledOdliczanie, HIGH);
    delay(czasOdliczanie);
    digitalWrite(ledOdliczanie, LOW);
    delay(czasOdliczanie);
  }
}

unsigned long Time(unsigned long startTime) {
  endTime = millis();
  reactionTime = endTime - startTime;
  return reactionTime;
}

void waitForAnyHigh() {
  while (buttonState1 == LOW && buttonState2 == LOW && buttonState3 == LOW && buttonState4 == LOW) {
    buttonState();
  }
}

void waitForAnyHighInGame() {
  while (buttonState1 == LOW && buttonState2 == LOW && buttonState3 == LOW && buttonState4 == LOW) {
    buttonState();
    if  (mode == 4 && millis() >= bonkRndTime) {
      bonkMissed = true;
      missedBonksInRow += 1;
      break;
    }
    if ((mode == 2 && millis() >= fiveSecTime) || (millis() >= (actTime + 5000))) {
      break;
    }
  }
}

void waitForAllLow() {
  while (buttonState1 == HIGH || buttonState2 == HIGH || buttonState3 == HIGH || buttonState4 == HIGH) {
    buttonState();
  }
}

void waitForAllLowDelay() {
  waitForAllLow();
  delay(250);
  waitForAllLow();
}

void buttonState() {
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);
}
