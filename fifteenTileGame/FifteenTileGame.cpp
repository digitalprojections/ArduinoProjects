#include "Adafruit.h"
#include "FifteenTileGame.h"

int numbers[16];
int x;  // Top-left corner X coordinate
int y;  // Top-left corner Y coordinate
int w = 60;  // Width of the square
int h = 60;  // Height of the square

int srcX = -1;
int srcY = -1;
int dstX = -1;
int dstY = -1;

extern int leftBtn;
extern int upBtn;
extern int downBtn;
extern int rightBtn;
extern int resetGameBtn;
extern int stopGameBtn;

extern unsigned long startTime;
extern unsigned long elapsedTime;

extern int inputNumber;
extern bool gameOver;
extern Adafruit_ST7789 tft;

void Init() {

  startTime = millis();  // Record start time
  generateDistinctRandomNumbers();
  drawSquareWithNumber();
}

int checkInput() {
  if (digitalRead(leftBtn) == HIGH) {
    return 0;
  } else if (digitalRead(upBtn) == HIGH) {
    return 1;
  } else if (digitalRead(downBtn) == HIGH) {
    return 2;
  } else if (digitalRead(rightBtn) == HIGH) {
    return 3;
  } else if (digitalRead(resetGameBtn) == HIGH) {
    //gameOver = true;
    return 4;
  } else if (digitalRead(stopGameBtn) == HIGH) {
    return 5;
  } else {
    return -1;
  }
}

void FTGLoop() {
  bool legaMove = false;
  //only process, if the input is onw of the 4 buttons
  inputNumber = checkInput();

  if (gameOver && inputNumber >= 0) {
    gameOver = false;
    generateDistinctRandomNumbers();
    drawSquareWithNumber();
  }

  

  if (inputNumber >= 0 && elapsedTime > 200) {
    startTime = millis();
    int zero = indexOf(0);
    Serial.println(zero);
    switch (inputNumber) {
      case 0:  //Left
        legaMove = LeftMove(zero);
        break;
      case 1:  //Up
        legaMove = UpMove(zero);
        break;
      case 2:  //Down
        legaMove = DownMove(zero);
        break;
      case 3:  //Right
        legaMove = RightMove(zero);
        break;
      case 4:  //reset
        generateDistinctRandomNumbers();
        drawSquareWithNumber();
        gameOver = false;
        break;
      case 5:
        gameOver = true;
        break;
      default:
        break;
    }
    if (legaMove) {
      drawSquareWithNumber();
    } else {
      //illegal move
      tft.drawRect(0, 0, 240, 240, ST77XX_RED);
      delay(500);
      tft.drawRect(0, 0, 240, 240, ST77XX_WHITE);
      delay(500);
      tft.drawRect(0, 0, 240, 240, ST77XX_RED);
      delay(500);
      tft.drawRect(0, 0, 240, 240, ST77XX_ORANGE);
    }

    inputNumber = -1;
  }
}

void setNumbers(int sn = -1, int dn = -1) {
  //get thw x, y positions of the number

  srcX = sn % 4;
  srcY = sn / 4;
  dstX = dn % 4;
  dstY = dn / 4;

  //Serial.print("src:");
  //Serial.println(sn);
  //Serial.print("dst:");
  //Serial.println(dn);
}

//Left
bool LeftMove(int zero) {
  if (zero < 15 && zero % 4 < 3) {
    numbers[zero] = numbers[zero + 1];
    numbers[zero + 1] = 0;

    setNumbers(zero, zero + 1);
    return true;
  }
  setNumbers();
  return false;
}

//Right
bool RightMove(int zero) {

  if (zero > 0 && zero % 4 > 0) {
    numbers[zero] = numbers[zero - 1];
    numbers[zero - 1] = 0;
    setNumbers(zero - 1, zero);
    return true;
  }
  setNumbers();
  return false;
}
//Upwards
bool UpMove(int zero) {
  if (zero < 12) {
    numbers[zero] = numbers[zero + 4];
    numbers[zero + 4] = 0;
    setNumbers(zero, zero + 4);
    return true;
  }
  setNumbers();
  return false;
}
//Down
bool DownMove(int zero) {
  if (zero > 3) {
    numbers[zero] = numbers[zero - 4];
    numbers[zero - 4] = 0;
    setNumbers(zero, zero - 4);
    return true;
  }
  setNumbers();
  return false;
}


int indexOf(int value) {
  int foundIndex = -1;  // Initialize with an invalid index
  // Search for the target value
  for (int i = 0; i < sizeof(numbers) / sizeof(numbers[0]); i++) {
    if (numbers[i] == value) {
      foundIndex = i;  // Store the index where the value is found
      break;           // Exit the loop once found
    }
  }
  return foundIndex;
  /*
  We declare an integer array numbers and initialize it with some values.
  The targetValue is the value we want to find.
  The loop iterates through the array elements and checks if each element matches the targetValue.
  If found, we store the index in foundIndex.
  If not found, foundIndex remains -1.
  */
}

void generateDistinctRandomNumbers() {
  elapsedTime = millis() - startTime;
  randomSeed(elapsedTime);

  for (int i = 0; i < 16; i++) {
    int randomNumber;
    do {
      randomNumber = random(16);  // Generate a random number between 0 and 15
    } while (isNumberAlreadyInArray(randomNumber, i));
    numbers[i] = randomNumber;
    //numbers[i] = i + 1; //test setup
  }

  if (!isSolvable()) {
    Serial.println("retry");
    //try another set
    generateDistinctRandomNumbers();
  }
}

bool isSolvable() {

  int invCount = 0;
  for (int i = 0; i < sizeof(numbers) / sizeof(numbers[0]) - 1; i++) {
    for (int j = i + 1; j < sizeof(numbers) / sizeof(numbers[0]); j++) {
      if (i < j && numbers[i] > 0 && numbers[j] > 0 && numbers[i] > numbers[j]) {
        invCount++;
      }
    }
  }
  int zeroInd = indexOf(0);

  Serial.print("inv count: ");
  Serial.println(invCount);
  Serial.println(invCount % 2);
  Serial.print("zero row: ");
  Serial.println(zeroInd / 4);
  Serial.println(zeroInd / 4 % 2);
  if (invCount % 2 != zeroInd / 4 % 2) {
    return true;
  } else {
    return false;
  }
}

bool isNumberAlreadyInArray(int num, int endIndex) {
  for (int i = 0; i < endIndex; i++) {
    if (numbers[i] == num) {
      return true;  // Number already exists in the array
    }
  }
  return false;  // Number is distinct
}

void drawSquareWithNumber() {
  tft.setTextWrap(false);

  //Only redraw the area that changed
  if (srcX >= 0 && dstX >= 0) {
    tft.fillRect(srcX * 60 + 1, srcY * 60 + 1, 58, 58, ST77XX_BLACK);
    tft.fillRect(dstX * 60 + 1, dstY * 60 + 1, 58, 58, ST77XX_BLACK);
  } else {
    tft.fillScreen(ST77XX_BLACK);
  }

  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(4);

  int numIndex = 0;
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      x = i * 60;
      y = j * 60;
      tft.drawRect(x, y, w, h, ST77XX_ORANGE);
      tft.setCursor(x + 10, y + 10);
      if (numbers[numIndex] > 0) {
        tft.println(numbers[numIndex]);
      }
      numIndex++;
    }
  }

  //check if game is WON
  for (int j = 0; j < 16; j++) {
    if (j + 1 == numbers[j]) {
      if (j + 1 == 15) {
        //Won
        endGame();
      }
    } else {
      //Not solved yet
      break;
    }
  }
}

void endGame() {
  srcX = srcY = dstX = dstY = -1;

  tft.setTextSize(4);
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(20, 110);
  tft.fillRect(0, 105, 240, 40, ST77XX_ORANGE);
  tft.println("You Won!");
  delay(2000);
  gameOver = true;  
}
void _endGame() {
}