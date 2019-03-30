#ifndef _NODECONTROL_H // header guard - to make sure header is not imported more than once
#define _NODECONTROL_H

void setSpeed(String inputVal, int pinNum) {
  int newinputVal = inputVal.toInt();
  int PWMVal = map(newinputVal, 0, 100, 0, 1023);
  analogWrite(pinNum, PWMVal);
}

void flashLight(int pinNum) {
  digitalWrite(pinNum, LOW);
  delay(20);
  digitalWrite(pinNum, HIGH);
}

#endif
