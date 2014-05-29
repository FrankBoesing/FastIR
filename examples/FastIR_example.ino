#include "FastIR.h"

FastIR ir;


void setup() {
  Serial.begin(9600);
}

void loop(){
uint32_t key;

    key = ir.getkeypress();

    if (key) {
        Serial.printf("%U \r\n",key);
    }

}
