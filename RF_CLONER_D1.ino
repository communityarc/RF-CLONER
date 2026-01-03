#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

const int recvPin = D2;   
const int transPin = D1;  
const int captureBtn = D5; 
const int sendBtn = D6;    

unsigned long capturedCode = 0;
unsigned int capturedBitLength = 0;
unsigned int capturedProtocol = 0;

const int burstCount = 1;
const int ledPin = D4;  

void setup() {
    Serial.begin(115200); 
    mySwitch.enableReceive(digitalPinToInterrupt(recvPin)); 
    mySwitch.enableTransmit(transPin);

    pinMode(captureBtn, INPUT_PULLUP);
    pinMode(sendBtn, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH); 
}

void loop() {
    if (digitalRead(captureBtn) == LOW) {
        if (mySwitch.available()) {
            digitalWrite(ledPin, LOW);
            delay(200);  
            digitalWrite(ledPin, HIGH);
            delay(200);
            digitalWrite(ledPin, LOW);
            delay(200); 
            digitalWrite(ledPin, HIGH);
            delay(200);
            digitalWrite(ledPin, LOW);
            delay(100); 
            digitalWrite(ledPin, HIGH);
            delay(100);
            digitalWrite(ledPin, LOW);

            capturedCode = mySwitch.getReceivedValue();
            capturedBitLength = mySwitch.getReceivedBitlength();
            capturedProtocol = mySwitch.getReceivedProtocol();
            
            Serial.print("Captured Code: ");
            Serial.println(capturedCode);

            mySwitch.resetAvailable();
            
            delay(300);  
            digitalWrite(ledPin, HIGH); 
        }
    }

    while (digitalRead(sendBtn) == LOW && capturedCode != 0) {
        Serial.println("==== Sending Packets ====");
        for (int i = 0; i < burstCount; i++) {
            digitalWrite(ledPin, LOW);
            mySwitch.send(capturedCode, capturedBitLength);
            Serial.print("Packet ");
            Serial.print(i + 1);
            Serial.println(" Sent");
            delay(50); 
        }
        digitalWrite(ledPin, HIGH);
        Serial.println("==== Transmission Completed ====");
        delay(500);
    }
}
