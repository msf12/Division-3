#define DEBUG 0 // set to 1 for serial debugging output

#define PIN_6811 3
#define OUTPIN_1 8
#define OUTPIN_2 9
#define OUTPIN_3 10

uint32_t pulseLength = 0, val = 0; //the length of time that the pulse read stayed HIGH
bool updatePulseLength = true; //whether pulseLength needs to be updated this loop

void setup()
{
	pinMode(PIN_6811, INPUT);
	pinMode(OUTPIN_1, OUTPUT);
	pinMode(OUTPIN_2, OUTPUT);
	pinMode(OUTPIN_3, OUTPUT);

#if DEBUG
	Serial.begin(9600);
#endif DEBUG
}

void loop()
{
	// pulseIn courtesy of http://www.benripley.com/diy/arduino/three-ways-to-read-a-pwm-signal-with-arduino/
	val = pulseIn(PIN_6811, HIGH, 10000000);

#if DEBUG
	Serial.println(val);
#endif

	// this makes sure that only the first pulse is saved when the signal changes
	if(val && updatePulseLength)
	{
		pulseLength = val;
		updatePulseLength = false;
	}
	else if(!val && !updatePulseLength)
	{
		updatePulseLength = true;
		pulseLength = 0;
	}

#if DEBUG
	Serial.println("Pulse length: " + String(pulseLength));
#endif

	// these are the pulse-length buckets used to determine which pulse is being sent to the arduino
	// the actual pulse-length possibilities are in much smaller ranges, but the buckets are left large
	// to ensure accuracy in the outputs
	if(pulseLength == 0)
	{
		digitalWrite(OUTPIN_1, LOW);
		digitalWrite(OUTPIN_2, LOW);
		digitalWrite(OUTPIN_3, LOW);
	}
	else if(pulseLength < 1000000)
	{
		digitalWrite(OUTPIN_1, HIGH);
		digitalWrite(OUTPIN_2, LOW);
		digitalWrite(OUTPIN_3, LOW);
	}
	else if(pulseLength < 3000000)
	{
		digitalWrite(OUTPIN_1, LOW);
		digitalWrite(OUTPIN_2, HIGH);
		digitalWrite(OUTPIN_3, LOW);
	}
	else
	{
		digitalWrite(OUTPIN_1, LOW);
		digitalWrite(OUTPIN_2, LOW);
		digitalWrite(OUTPIN_3, HIGH);
	}
}