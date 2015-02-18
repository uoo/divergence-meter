#include <Wire.h>





void
setup()
{
  pinMode( 5, OUTPUT);
  pinMode( 6, OUTPUT);
  pinMode( 7, OUTPUT);
  pinMode( 8, OUTPUT);
  pinMode( 9, OUTPUT);
  pinMode(13, OUTPUT);
  randomSeed(analogRead(A0));
}

void
loop()
{
  byte n = random(10);
  
  digitalWrite( 6, n & 0x01 ? HIGH : LOW);
  digitalWrite( 7, n & 0x02 ? HIGH : LOW);
  digitalWrite( 8, n & 0x04 ? HIGH : LOW);
  digitalWrite( 9, n & 0x08 ? HIGH : LOW);
  digitalWrite(13, n & 0x01 ? HIGH : LOW);
  
  digitalWrite( 5, HIGH);
  delay(50);
  digitalWrite( 5, LOW);
  delay(50);
  digitalWrite( 5, HIGH);
  delay(50);
  digitalWrite( 5, LOW);
  delay(50);
  digitalWrite( 5, HIGH);
  delay(50);
}



