#include <vector.h>

void setup() {
  Serial.begin(9800);
}

void loop() {
  vector<int>a;
  a.push_back(2);
  a.push_back(9);
  a.push_back(5);
  a.push_back(3);

  Serial.print(a.begin());
  delay(2000);
   Serial.print(a.size());
  delay(2000);
}
