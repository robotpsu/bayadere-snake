#define SERIAL_SPEED 115200  // baud
#define SERIAL_TIMEOUT  100  // ms
#define SERIAL_DELAY     50  // ms

#define MAX_ITER 20

const byte valves[] = {38, 39, 40, 41, 42, 43, 44, 45};
const byte sensors[] = {A0, A1, A2, A3, A4, A5, A6, A7};

const size_t valvesNum = sizeof(valves) / sizeof(byte);
const size_t sensorsNum = sizeof(sensors) / sizeof(byte);

int iteration = 0;

void setup()
{
  for (auto i = 0; i < valvesNum; i++) {
    pinMode(valves[i], OUTPUT);
    digitalWrite(valves[i], LOW);
  }

  Serial.setTimeout(SERIAL_TIMEOUT);
  Serial.begin(SERIAL_SPEED);
}

void loop()
{
  char cmd = 0;
  long n = 0;

  if (Serial.available()) {
    cmd = Serial.read();
    if (Serial.available())
      n = Serial.parseInt();

    switch (cmd) {
      case 'o': case 'c': case 't': case 'v':
      case '+': case '-': case '*': case '.':
        if (!checkValvesNum(n))
          break;
        n -= 1;
      case 'o': case '+':
        digitalWrite(valves[n], HIGH);
        printValve(n);
        break;
      case 'c': case '-':
        digitalWrite(valves[n],  LOW);
        printValve(n);
        break;
      case 't': case '*':
        digitalWrite(valves[n], !digitalRead(valves[n]));
        printValve(n);
        break;
      case 'v': case '.':
        printValve(n);
        break;

      case 'p': case '/':
        if (!checkSensorsNum(n))
          break;
        n -= 1;
        printPressure(n);
        break;

      default: Serial.println("?CMD");
    }
  }

  if (++iteration >= MAX_ITER) {
    printState();
    iteration = 0;
  }

  delay(SERIAL_DELAY);
}

bool checkValvesNum(int n)
{
  if (n < 1 || n > valvesNum) {
    Serial.print("!N=1..");
    Serial.print(valvesNum);
    Serial.print(" (");
    Serial.print(n);
    Serial.println(")");

    return false;
  }

  return true;
}

bool checkSensorsNum(int n)
{
  if (n < 1 || n > sensorsNum) {
    Serial.print("!N=1..");
    Serial.print(sensorsNum);
    Serial.print(" (");
    Serial.print(n);
    Serial.println(")");

    return false;
  }

  return true;
}

void printValve(int n)
{
  Serial.print('V');
  Serial.print(n);
  Serial.print('=');
  Serial.println(digitalRead(valves[n]));
}

void printPressure()
{
  Serial.print('P');
  Serial.print(n);
  Serial.print('=');
  Serial.println(analogRead(sensors[n]));
}

void printState()
{
  // Valve numbers
  for (auto i = 0; i < valvesNum; i++) {
    if (i) Serial.print('|');
    Serial.print("  ");
    Serial.print(i + 1);
    Serial.print(' ');
  }
  Serial.println();

  // Valve states
  for (auto i = 0; i < valvesNum; i++) {
    if (i) Serial.print('|');
    Serial.print("  ");
    Serial.print(digitalRead(valves[i]));
    Serial.print(' ');
  }
  Serial.println();

  // Pressure
  for (auto i = 0; i < sensorsNum; i++) {
    if (i) Serial.print('|');
    int p = analogRead(sensors[i]);
    if (p < 1000) Serial.print(' ');
    if (p <  100) Serial.print(' ');
    if (p <   10) Serial.print(' ');
    Serial.print(p);
  }
  Serial.println();
}
