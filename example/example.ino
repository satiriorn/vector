#include <vector.h>

void setup() {
  Serial.begin(5700);
}

void loop() {
  vector<int>a;
  a.push_back(2);
  a.push_back(9);
  a.push_back(5);
  a.push_back(3);
  for (auto i = 0; i < a.size(); ++i) {
    Serial.print(a[i]);
    delay(2000);
  }
  Serial.print(a.begin());
  delay(2000);
   Serial.print(a.end());
  delay(2000);
}
