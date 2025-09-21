// C++ code
//
void setup()
{
  pinMode(A2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(A1, INPUT);
  pinMode(5, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(6, OUTPUT);
}

void loop()
{
  analogWrite(3, map(analogRead(A2), 0, 1023, 0, 255));
  analogWrite(5, map(analogRead(A1), 0, 1023, 0, 255));
  analogWrite(6, map(analogRead(A0), 0, 1023, 0, 255));
  delay(10); // Delay a little bit to improve simulation performance
}