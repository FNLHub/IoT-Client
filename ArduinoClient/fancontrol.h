#ifndef _NODECONTROL_H // header guard - to make sure header is not imported more than once
#define _NODECONTROL_H

void setSpeed(String inputVal, int pinNum) { 
  int newinputVal = inputVal.toInt();
	int PWMVal = map(newinputVal, 0, 100, 0, 1023);
	analogWrite(pinNum, PWMVal);
}

void flashLight(pinNum) {
  digitalWrite(pinNumber, HIGH);
  delay(20);
  digitalWrite(pinNumber, LOW);
}
#endif 
