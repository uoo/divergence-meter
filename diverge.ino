#include <Wire.h>
#include <TimerOne.h>

// PORTB 8,9,10,11,12,13
// PORTC a0,a1,a2,a3,a4,a5,rst
// PORTD 0,1,2,3,4,5,6,7

#define NDIGITS 8

struct digit {
  byte value;
  byte time;
};

static struct digit   digits[NDIGITS];
static byte           pair;
static byte           offset;
static unsigned long  last;

static void
tick()
{
  byte            bits;
  struct digit *  dp;

  //PORTB = 0x0f;

  dp = digits + (2 * pair);
  
  bits  = dp[0].value << 4;
  bits |= dp[1].value << 0;

#if 0
  if (dp[0].time < time) {
    bits = dp[0].value << 4;
  } else {
    bits = random(10) << 4;
    done = false;
  }

  if (dp[1].time < time) {
    bits |= dp[1].value;
  } else {
    bits |= random(10);
    done = false;
  }
#endif

  PORTD = bits;
  PORTB = 0x3c ^ (4 << pair);

  ++pair;

  if (pair >= (NDIGITS / 2)) {
    pair = 0;
  }
}

void
setup()
{
  Timer1.initialize(4000);
  Timer1.attachInterrupt(tick);
  // PORTD
  pinMode( 0, OUTPUT);
  pinMode( 1, OUTPUT);
  pinMode( 2, OUTPUT);
  pinMode( 3, OUTPUT);
  pinMode( 4, OUTPUT);
  pinMode( 5, OUTPUT);
  pinMode( 6, OUTPUT);
  pinMode( 7, OUTPUT);

  // PORTB
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  pinMode(9, INPUT);
  randomSeed(analogRead(A0));
  pair = 0;
}

void
loop()
{
  int		  pair;
  int		  time;
  boolean         done = false;
  unsigned long   now;
  struct digit *  dp;

  //for (dp = digits; dp < digits + NDIGITS; ++dp) {
  //  dp->value = ((dp - digits) + offset) % 10;
  //}
  
  for (dp = digits; dp < digits + NDIGITS; ++dp) {
    if (dp == digits) {
      dp->value = random(2);
      dp->time = random(10);
    } else {
      dp->value = random(10);
      dp->time = random(0x100);
    }
  }
  
  time = 0;

  while (!done) {
    done = true;
    for (dp = digits; dp < digits + NDIGITS; ++dp) {
      if (dp->time > time) {
        dp->value = random(10);
        done = false;
      }
    }
    
    now = micros() / 10000;
  
    if (now > last) {
      last = now;
      ++time;
    }
  }
  
  last = micros() / 2000000;
  
  while ((micros() / 2000000) != last) {
  }

  while (digitalRead(9)) {
  }
}



