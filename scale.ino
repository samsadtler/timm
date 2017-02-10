#include "Particle.h"
#include "papertrail.h"

const unsigned long SEND_INTERVAL_MS = 22000;
const size_t READ_BUF_SIZE = 18;

PapertrailLogHandler papertailHandler("logs5.papertrailapp.com", 54518, "timm");

// Forward declarations
void processBuffer();
void webHook();
void tareFunction();
// Global variables
int counter = 0;
char readSignal = 'R';
char tareSignal = 'T';
char zeroSignal = 'Z';
char grossSignal = 'G';
bool tare = false;
char signals [] = {readSignal};
	String tempMessage = "trash data";
unsigned long lastSend = 0;
int i;
char readBuf[READ_BUF_SIZE];
size_t readBufOffset = 0;

void setup() {

	USBSerial1.begin();
	Serial1.begin(9600, SERIAL_8N1);
	tareFunction();
	Log.info("Weight Zeroed on Startup --> %d", tareSignal);
}

void loop() {
	if (millis() - lastSend >= SEND_INTERVAL_MS) {
		lastSend = millis();
		Serial1.write(signals[0]);

	}

	// Read data from serial
	while(Serial1.available()) {
		if (readBufOffset < READ_BUF_SIZE) {
			char c = Serial1.read();
			if (c != '\r' && c!= '\n') {
				readBuf[readBufOffset++] = c;
			}
			else {
				readBuf[readBufOffset] = 0;
				processBuffer();
				readBufOffset = 0;
			}
		}
		else {
			Log.error("readBuf overflow, emptying buffer");
			readBufOffset = 0;
		}
	}

}
void tareFunction(){
	Serial1.write(tareSignal);
	if (tare){ tare = false;}
	else {tare = true;}

};

void webHook(int weight){
		int change = 5;

		String form = String::format(
			"{\"weight\":\"%d\",\"change\":\"%d\",\"tare\":\"%d\"}",
			 weight, change, tare
		);
		Particle.publish("google-docs", form, 60, PRIVATE);
}

void processBuffer() {
	Log.info("Received from Optima: %s", readBuf);
	String buffer = String(readBuf);
	int weight = atoi(buffer.substring(0,8));

	webHook(weight);
}
