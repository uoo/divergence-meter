#include <Wire.h>
#include <TimerOne.h>

// PORTB 8,9,10,11,12,13
// PORTC a0,a1,a2,a3,a4,a5,rst
// PORTD 0,1,2,3,4,5,6,7

// even digits BCD 0,1,2,3
// odd digits BCD 4,5,6,7
// anodes 10, 11, 12, 13
// switch 9

#define NDIGITS 8
#define SWITCH  9
#define ENABLE A0

#define SCANPERIOD 4000
#define DEBOUNCE 10000

struct digit {
  byte value;
  byte time;
};

static byte           pair;
static byte           offset;
static byte           on;
static byte           lastswitch;
static struct digit   digits[NDIGITS];
static unsigned long  last;
static unsigned long  lastswitchtime;

static void
tick()
{
  byte            bits;
  struct digit *  dp;

  digitalWrite(ENABLE, on);

  if (on == LOW) {
    return;
  }

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

static void
updateswitch(
  byte  state)
{
  if (state == HIGH) {
    on = HIGH;
    initdigits();
  } else {
    on = LOW;
  }
}

static void
initdigits()
{
  struct digit *  dp;

  for (dp = digits; dp < digits + NDIGITS; ++dp) {
    if (dp == digits) {
      dp->value = random(2);
      dp->time = random(10);
    } else {
      dp->value = random(10);
      dp->time = random(0x100);
    }
  }
}

static void
initcountdigits()
{
  struct digit *  dp;

  for (dp = digits; dp < digits + NDIGITS; ++dp) {
    dp->value = ((dp - digits) + offset) % 10;
  }
}

void
setup()
{
  Timer1.initialize(SCANPERIOD);
  Timer1.attachInterrupt(tick);
  // PORTD (BCD outputs)
  pinMode( 0, OUTPUT);
  pinMode( 1, OUTPUT);
  pinMode( 2, OUTPUT);
  pinMode( 3, OUTPUT);
  pinMode( 4, OUTPUT);
  pinMode( 5, OUTPUT);
  pinMode( 6, OUTPUT);
  pinMode( 7, OUTPUT);

  // PORTB (anodes)
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  // switch
  pinMode(SWITCH, INPUT);

  // enable
  pinMode(ENABLE, OUTPUT);

  // initialize
  randomSeed(analogRead(A0));
  pair       = 0;
  on         = HIGH;
  lastswitch = LOW;
  initdigits();
}

void
loop()
{
  int		  pair;
  int		  time;
  byte            curswitch;
  boolean         done = false;
  unsigned long   now;
  struct digit *  dp;

  time = 0;

  for (dp = digits; dp < digits + NDIGITS; ++dp) {
    if (dp->time > time) {
      dp->value = random(10);
    }
  }
    
  now = micros() / 10000;
  
  if (now > last) {
    last = now;
    ++time;
  }
  
  if (lastswitchtime != 0) {
    curswitch = digitalRead(SWITCH);
    if (curswitch == lastswitch) {
      if ((micros() - lastswitchtime) > DEBOUNCE) {
        updateswitch(curswitch);
      }
    } else {
      lastswitch = curswitch;
      lastswitchtime = micros();
    }
  }
}
