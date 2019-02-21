#include <FlexActuator.h>

#define SERIAL_SPEED 115200  // baud
#define SERIAL_TIMEOUT  100  // ms
#define SERIAL_DELAY     50  // ms

#define FLEX_NUM 2
#define FLEX_PIPES 4

FlexActuator *flex[FLEX_NUM];

const byte valves[FLEX_NUM][FLEX_PIPES] = {{38, 39, 42, 43}, {40, 41, 44, 45}};
const byte sensors[FLEX_NUM][FLEX_PIPES] = {{A0, A1, A4, A5}, {A2, A3, A6, A7}};

void setup()
{
  for (auto i = 0; i < FLEX_NUM; i++) {
    flex[i] = new FlexActuator(FLEX_PIPES);
    flex[i]->attachValves(valves[i]);
    flex[i]->attachSensors(sensors[i]);
  }

  Serial.setTimeout(SERIAL_TIMEOUT);
  Serial.begin(SERIAL_SPEED);
}

void loop()
{
  char c = 0;  // Command
  byte a = 0;  // Actuator number
  byte n = 0;  // Pipe (valve or sensor) number

  if (Serial.available()) {
    c = Serial.read();
    a = Serial.read(); a -= '0'; if (a >= FLEX_NUM) return;
    n = Serial.read(); n -= '0'; if (n >= flex[a]->pipes()) return;

    switch (c) {
      case 'o': case '+': flex[a]->open(n);   printState(a, n); break;
      case 'c': case '-': flex[a]->close(n);  printState(a, n); break;
      case 't': case '*': flex[a]->toggle(n); printState(a, n); break;

      case 's': case '.': printState(a, n); break;

      default: Serial.print("?CMD:"); Serial.println(c);
    }
  }

  delay(SERIAL_DELAY);
}

float toBar(int p)
{
  return float(p) * 8. / 1023.;
}

void printState(byte a, byte n)
{
  // Get valve state and pressure
  int state = flex[a]->getState(n);
  float pressure = flex[a]->getPressure(n, toBar);

  // Print actuator ID
  Serial.print("A"); Serial.print(a);
  Serial.print("N"); Serial.print(n);

  // Print valve state
  Serial.print(" is ");
  if (state >= 0) {
    Serial.print(state ? "OPENED" : "closed");
  } else {
    Serial.print("!ERROR:"); Serial.print(state); Serial.print("!");
  }

  // Print pressure
  Serial.print(" p=");
  if (pressure >= 0) {
    Serial.print(pressure, 2);
  } else {
    Serial.print("!ERROR:"); Serial.print(pressure, 0); Serial.print("!");
  }

  Serial.println();
}

void printState(byte a)
{
  byte pipes = flex[a]->pipes();

  // Print pipe numbers
  for (byte i = 0; i < pipes; i++) {
    if (i) Serial.print('|');
    Serial.print("  "); Serial.print(i); Serial.print(' ');
  }
  Serial.println();

  // Print valve states
  for (byte i = 0; i < pipes; i++) {
    int s = flex[a]->getState(i);

    if (i) Serial.print('|');
    if (s >= 0) {
      Serial.print("  "); Serial.print(s ? 'O' : '.'); Serial.print(' ');
    } else {
      Serial.print('!'); Serial.print(s); Serial.print('!');
    }
  }
  Serial.println();

  // Print pressure
  for (byte i = 0; i < pipes; i++) {
    float p = flex[a]->getPressure(i, toBar);

    if (i) Serial.print('|');
    if (p >= 0) {
      Serial.print(p, 2);
    } else {
      Serial.print('!'); Serial.print(p, 0); Serial.print('!');
    }
  }
  Serial.println();
}

void printState()
{
  for (byte a = 0; a < FLEX_NUM; a++)
    printState(a);
}
