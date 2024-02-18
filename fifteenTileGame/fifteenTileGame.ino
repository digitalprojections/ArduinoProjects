#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

 int leftBtn = 4;
 int upBtn = 7;
 int downBtn = 2;
 int rightBtn = 6;
 int inputNumber = -1;

 bool gameOver = false;

 int numbers[16];

  // Draw a square around the number
  int x = 0; // Top-left corner X coordinate
  int y = 0; // Top-left corner Y coordinate
  int w = 60; // Width of the square
  int h = 60; // Height of the square

String text = "";

#define TFT_CS 10 // if your display has CS pin
#define TFT_RST 9 // reset pin
#define TFT_DC 8 // data pin
#define TFT_MOSI 11
#define TFT_SCLK 13

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

float p = 3.1415926;

void setup(void) {
  Serial.begin(9600);
  Serial.print(F("Hello! ST77xx TFT Test"));

  // Use this initializer (uncomment) if using a 1.3" or 1.54" 240x240 TFT:
  tft.init(240, 240, SPI_MODE3);           // Init ST7789 240x240
  tft.setRotation(2);
  
  // SPI speed defaults to SPI_DEFAULT_FREQ defined in the library, you can override it here
  // Note that speed allowable depends on chip and quality of wiring, if you go too fast, you
  // may end up with a black screen some times, or all the time.
  //tft.setSPISpeed(40000000);
  Serial.println(F("Initialized"));

  initializeButtons();

  // tft print function!
  tftPrintTest();
  delay(2000);

  //tft.print("No: ");
  //tft.println(inputNumber);
  generateDistinctRandomNumbers();
  
  drawSquareWithNumber();
}

void loop() {  

  inputNumber = checkInput();  
  text = "";
  bool legaMove = false;

  //only process, if the input is onw of the 4 buttons
  if(gameOver && inputNumber >= 0)
  {
    generateDistinctRandomNumbers();
    drawSquareWithNumber();
    gameOver = false;
  }

  if(inputNumber >= 0)
  {
    int zero = indexOf(0);  
    Serial.println(zero);
    switch(inputNumber)
    {
      case 0://Left
        legaMove = LeftMove(zero);
      break;
      case 1://Up
        legaMove = UpMove(zero);
      break;
      case 2://Down
        legaMove = DownMove(zero);
      break;
      case 3://Right
        legaMove = RightMove(zero);
      break;
      default:
      break;
    }
    if(legaMove){
      drawSquareWithNumber();
    }
    else
    {
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
  delay(300);
}

//Left
bool LeftMove(int zero)
{  
  if(zero < 15 && zero % 4 < 3)
  {
    numbers[zero] = numbers[zero+1];
    numbers[zero+1] = 0;
    return true;
  }
  return false;
}

//Right
bool RightMove(int zero){  
  
  if(zero > 0 && zero % 4 > 0)
  {
    numbers[zero] = numbers[zero-1];
    numbers[zero-1] = 0;
    return true;
  }
  return false;
}
//Upwards
bool UpMove(int zero){
  if(zero < 12)
  {
    numbers[zero] = numbers[zero + 4];
    numbers[zero + 4] = 0;
    return true;
  }  
  return false;
}
//Down
bool DownMove(int zero){
  if(zero > 3)
  {
    numbers[zero] = numbers[zero - 4];
    numbers[zero - 4] = 0;
    return true;
  }
  return false;
}


int indexOf(int value)
{
  int foundIndex = -1; // Initialize with an invalid index
  // Search for the target value
  for (int i = 0; i < sizeof(numbers) / sizeof(numbers[0]); i++) {
    if (numbers[i] == value) {
      foundIndex = i; // Store the index where the value is found
      break; // Exit the loop once found
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
  for (int i = 0; i < 16; i++) {
    int randomNumber;
    do {
      randomNumber = random(16); // Generate a random number between 0 and 15
    } while (isNumberAlreadyInArray(randomNumber, i));
    numbers[i] = randomNumber;
  }

  if(isNotSolvable())
  {
    Serial.println("retry");
    //try another set
    generateDistinctRandomNumbers();
  }
}

bool isNotSolvable(){
  
  int invCount = 0;
    for (int i = 0; i < 16 - 1; i++) {
        for (int j = i + 1; j < 16; j++) {
            if (numbers[j] != numbers[i] && numbers[i] > numbers[j])
                invCount++;
        }
    }
    if(invCount & 1)
    {
      return !indexOf(0) & 1;
    }
    else{
      return indexOf(0) & 1;
    }  
}

bool isNumberAlreadyInArray(int num, int endIndex) {
  for (int i = 0; i < endIndex; i++) {
    if (numbers[i] == num) {
      return true; // Number already exists in the array
    }
  }
  return false; // Number is distinct
}

void drawSquareWithNumber()
{
  tft.setTextWrap(false);
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0, 30);
    tft.setTextColor(ST77XX_GREEN);
    tft.setTextSize(4);

  int numIndex = 0;
  for(int j=0; j<4; j++)
  {
    for(int i=0; i<4; i++)
    {
      x = i * 60;
      y = j * 60;      
      tft.drawRect(x, y, w, h, ST77XX_ORANGE);
      tft.setCursor(x + 10, y + 10);
      if(numbers[numIndex]>0)
        {
          tft.println(numbers[numIndex]);
        } 
      numIndex++;
    }
  }
}

int checkInput()
{
  if(digitalRead(leftBtn) == HIGH)
  {
    return 0;
  }
  else if(digitalRead(upBtn) == HIGH)
  {   
    return 1;
  }
  else if(digitalRead(downBtn) == HIGH)
  {   
    return 2;
  }
  else if(digitalRead(rightBtn) == HIGH)
  {    
    return 3;
  }
  else
  {
    return -1;
  }
}

void initializeButtons()
{
  pinMode(leftBtn, OUTPUT);
  pinMode(upBtn, OUTPUT);
  pinMode(downBtn, OUTPUT);
  pinMode(rightBtn, OUTPUT);
}

void endGame()
{
  uint16_t time = millis();
  tft.fillScreen(ST77XX_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);

  // large block of text
  tft.fillScreen(ST77XX_BLACK);
  delay(1000);  

  // a single pixel
  tft.drawPixel(tft.width()/2, tft.height()/2, ST77XX_GREEN);
  delay(500);

  // line draw test
  testlines(ST77XX_YELLOW);
  delay(500);

  // optimized lines
  testfastlines(ST77XX_RED, ST77XX_BLUE);
  delay(500);

  testdrawrects(ST77XX_GREEN);
  delay(500);

  testfillrects(ST77XX_YELLOW, ST77XX_MAGENTA);
  delay(500);

  tft.fillScreen(ST77XX_BLACK);
  testfillcircles(10, ST77XX_BLUE);
  testdrawcircles(10, ST77XX_WHITE);
  delay(500);

  testroundrects();
  delay(500);

  testtriangles();
  delay(500);

  mediabuttons();
  delay(500);

  Serial.println("done");
  delay(1000);
}

void testlines(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, 0, x, tft.height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, 0, tft.width()-1, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, 0, x, tft.height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, 0, 0, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, tft.height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, tft.height()-1, tft.width()-1, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, 0, y, color);
    delay(0);
  }
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t y=0; y < tft.height(); y+=5) {
    tft.drawFastHLine(0, y, tft.width(), color1);
  }
  for (int16_t x=0; x < tft.width(); x+=5) {
    tft.drawFastVLine(x, 0, tft.height(), color2);
  }
}

void testdrawrects(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color);
  }
}

void testfillrects(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=tft.width()-1; x > 6; x-=6) {
    tft.fillRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color1);
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color2);
  }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=radius; x < tft.width(); x+=radius*2) {
    for (int16_t y=radius; y < tft.height(); y+=radius*2) {
      tft.fillCircle(x, y, radius, color);
    }
  }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < tft.width()+radius; x+=radius*2) {
    for (int16_t y=0; y < tft.height()+radius; y+=radius*2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
}

void testtriangles() {
  tft.fillScreen(ST77XX_BLACK);
  uint16_t color = 0xF800;
  int t;
  int w = tft.width()/2;
  int x = tft.height()-1;
  int y = 0;
  int z = tft.width();
  for(t = 0 ; t <= 15; t++) {
    tft.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  tft.fillScreen(ST77XX_BLACK);
  uint16_t color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
    int x = 0;
    int y = 0;
    int w = tft.width()-2;
    int h = tft.height()-2;
    for(i = 0 ; i <= 16; i+=1) {
      tft.drawRoundRect(x, y, w, h, 5, color);
      x+=2;
      y+=3;
      w-=4;
      h-=6;
      color+=1100;
    }
    color+=100;
  }
}

void tftPrintTest() {
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(4);
  tft.println("15 Tile");
  tft.println("Puzzle");
  tft.println("Game");  
  delay(1500);
}

void mediabuttons() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_CYAN);
  tft.println("Press Any Button");
  tft.println("To PLAY again");
}