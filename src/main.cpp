// Including the required Arduino libraries
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <Arduino.h>

// Uncomment according to your hardware type
#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

// Defining size, and output pins
#define MAX_DEVICES 1
#define CS_PIN 3

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);




struct entity {
  int x = -1;
  int y = -1;
};

struct entity snake[64];
struct entity apple;
char direction;
int length;
bool selectingApple;
int tempX;
int tempY;

char serRead(char button) {

  if(button == 'w') {
    //Serial.println("Up");
    if(direction != 'd') {
          return 'u';
    }
  } else if(button == 's') {
    //Serial.println("Down");
    if(direction != 'u') {
          return 'd';
    }
  } else if(button == 'd') {
    //Serial.println("Right");
    if(direction != 'l') {
          return 'r';
    }
  } else if(button == 'a') {
    //Serial.println("Left");
    if(direction != 'r') {
          return 'l';
    }
  }

return direction;

}

void resetMatrix(void)
{
  mx.control(MD_MAX72XX::INTENSITY, MAX_INTENSITY/2);
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  mx.clear();
}


void gameOver() {
  mx.clear();

    analogWrite(6, true);
    delay(90);
    analogWrite(6, false);
    delay(90);
    analogWrite(6, true);
    delay(90);
    analogWrite(6, false);
    delay(90);
    analogWrite(6, true);
    delay(90);
    analogWrite(6, false);
    delay(90);
    analogWrite(6, true);
    delay(90);
    analogWrite(6, false);

  for(int i = 0; i < 64; i++) {
    snake[i].x = -1;
    snake[i].y = -1;
  }
  length = 2;
  direction = 'r';
  snake[0].x = 1;
  snake[0].y = 0;
  snake[1].x = 0;
  snake[1].y = 0;
  tempX = snake[0].x;
  tempY = snake[0].y;
  mx.setPoint(snake[0].y, snake[0].x, true);
  mx.setPoint(snake[1].y, snake[1].x, true);
}



void setup() {

  Serial.begin(9600);

  mx.begin();

  resetMatrix();

  snake[0].x = 1;
  snake[0].y = 0;

  snake[1].x = 0;
  snake[1].y = 0;
  
  length = 2;

  apple.x = 5;
  apple.y = 5;

  selectingApple = false;

  direction = 'r';

  pinMode(5, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  

}



void loop() {

  // if(Serial.available()) {
  //   direction = serRead(Serial.read());
  // }

    if(digitalRead(8) == 0) {
      //Serial.println("Up");
      if(direction != 'd') {
          direction = 'u';
      }
    }
    else if(digitalRead(5) == 0) {
      //Serial.println("Down");
      if(direction != 'u') {
          direction = 'd';
      }
    }
    else if(digitalRead(7) == 0) {
      //Serial.println("Left");
      if(direction != 'r') {
          direction = 'l';
      }
    }
        if(digitalRead(9) == 0) {
      //Serial.println("Up");
      if(direction != 'l') {
          direction = 'r';
      }
    }



  mx.clear();

  tempX = snake[0].x;
  tempY = snake[0].y;

  if(direction == 'u') {
    snake[0].y++;
  }
  else if(direction == 'd') {
    snake[0].y--;
  }
  else if(direction == 'l') {
    snake[0].x--;
  }
  else if(direction == 'r') {
    snake[0].x++;
  }

  if(snake[0].x < 0 || snake[0].x > 7 || snake[0].y < 0 || snake[0].y > 7) {

    gameOver();

  }

  for(int i = 1; i < length; i++){
    if(snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
      gameOver();
    }
  }

  if(snake[0].x == apple.x && snake[0].y == apple.y) {
    length++;
    selectingApple = true;
    analogWrite(6, true);
    delay(10);
    analogWrite(6, false);
    delay(10);
    analogWrite(6, true);
    delay(10);
    analogWrite(6, false);
  }



  while(selectingApple == true) {
    int appleX = random(0,7);
    int appleY = random(0,7);
        Serial.print("Generated: ");
        Serial.print(appleX);
        Serial.print(" ");
        Serial.println(appleY);

    for(int i = 0; i < length; i++) {

      if(appleX == snake[i].x && appleY == snake[i].y) {
        Serial.println("HERE");
        break;
      }

      if(i == length -1) {
        apple.x = appleX;
        apple.y = appleY;
        selectingApple = false;
        Serial.print("Decided ");
        Serial.print(appleX);
        Serial.print(" ");
        Serial.println(appleY);
      }

    }

  }

  mx.setPoint(snake[0].y, snake[0].x, true);
  mx.setPoint(apple.y, apple.x, true);

  int tempX2;
  int tempY2;
  for(int i = 1; i < length; i++){

    if(i%2 == 0){
      tempX = snake[i].x;
      tempY = snake[i].y;
      snake[i].x = tempX2;
      snake[i].y = tempY2;
    }else {
      tempX2 = snake[i].x;
      tempY2 = snake[i].y;
      snake[i].x = tempX;
      snake[i].y = tempY;
    }

    mx.setPoint(snake[i].y, snake[i].x, true);
  }

  analogWrite(6, true);
  delay(50);
  analogWrite(6, false);
  delay(450);

}

