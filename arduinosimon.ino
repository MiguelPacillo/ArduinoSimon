#define redLed 12
#define greenLed 11
#define blueLed 10
#define yellowLed 9

#define redTone 100
#define greenTone 150
#define blueTone 200
#define yellowTone 250

#define speakerPin 8

int pinArray [4][2] = {{redLed, redTone},
  {greenLed, greenTone},
  {blueLed, blueTone},
  {yellowLed, yellowTone} // Allows me to map appropriate LED pins and tones according to index
};
int eventArray [4] = {1, 1, 1, 1}; // Represents initial values for button pins (all not pressed by default)

bool currentlyPressing = false;
bool lost = false;

int sequence [30]; // This array holds the sequence for a maximum of 30 before the player wins
int turn = 0;
int playerTurn;

int currentBtn = -1; // Unused index for button being pressed (since all buttons are not being pressed by default)
int startTime, endTime, currSeq;

void setup() {
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
}

void buttonPress(int colorLed, int colorTone) { // Turns on LED and plays tone
  digitalWrite(colorLed, HIGH);
  tone(speakerPin, colorTone);
}

void buttonReset(int colorLed) { // Turns off LED and stops tone
  digitalWrite(colorLed, LOW);
  noTone(speakerPin);
}

void resetGame() { // If the player loses or wins, reset the game
  lost = false;
  memset(sequence, 0, sizeof(sequence)); // Resets the sequence array
  turn = 0;
}

void loop() {

  if (turn == 30) { // If player reaches 30 turns they win, play win tone and reset game
    tone(speakerPin, 200, 500);
    delay(500);
    tone(speakerPin, 250, 500);
    delay(500);
    tone(speakerPin, 300, 500);
    delay(500);

    resetGame();

    delay(1000);
  }

  sequence[turn] = random(0, 4); // Generates a new number for the sequence

  // Loops through every number in the sequence and animates the LED/tone

  for (int j = 0; j <= turn; j++) {
    startTime = millis();
    endTime = startTime;
    currSeq = sequence[j];

    while ((endTime - startTime) <= 800) { // Make LED turn on and play tone for 0.8 seconds
      buttonPress(pinArray[currSeq][0], pinArray[currSeq][1]);
      endTime = millis();
    }

    buttonReset(pinArray[currSeq][0]); // When done, turn off LED and stop tone
    delay(500);
  }

  playerTurn = 0; // Resets playerTurn (incremented when player presses a button to copy sequence)

  while (playerTurn <= turn) {
    
    // Then loop through eventArray and check if a button is being pressed (0 -> pressed, 1 -> not pressed)
    
    for (int i = 0; i < 4; i++) {
      eventArray[i] = digitalRead(7 - i); // This populates eventArray with the state of pins 7, 6, 5, and 4 (button pins)
      
      if (!currentlyPressing && !eventArray[i]) { // If a 0 is read in the array, a button is currently being pressed
        currentBtn = i; // Set current button being pressed to the one that is reading 0 in the array
        currentlyPressing = true;
      } else if (eventArray[i] && i == currentBtn) { // If the button that was once being pressed now reads 1, reset the current button
        currentBtn = -1;
        currentlyPressing = false;
        buttonReset(pinArray[i][0]);

        if (i != sequence[playerTurn]) { // If the player sequence doesn't match the game sequence, player has lost
          lost = true;
        }
        playerTurn++;
      }
    }

    if (currentlyPressing) { // Current button being pressed is animated 
      buttonPress(pinArray[currentBtn][0], pinArray[currentBtn][1]);
    }
  }

  if (lost) { // If the player lost reset the game and play lose tone
    tone(speakerPin, 20, 3000);
    delay(3000);
    resetGame();

  } else { // Else move onto next turn
    turn++;
    delay(3000);
  }


}
