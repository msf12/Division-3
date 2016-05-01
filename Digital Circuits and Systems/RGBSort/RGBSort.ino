#define B_RED 16
#define B_GRE 15
#define B_BLU 14

uint32_t *intsToSort;
uint32_t numInts;

void RGBSort(uint32_t *intArr, uint32_t len, uint32_t start=0);

void setup()
{
	Serial.begin(9600);
	randomSeed(analogRead(0));
	pinMode(B_RED, OUTPUT);
	pinMode(B_GRE, OUTPUT);
	pinMode(B_BLU, OUTPUT);
	analogWrite(B_RED, 255);
	analogWrite(B_GRE, 255);
	analogWrite(B_BLU, 255);
	while(!Serial);
	String input;
	do
	{
		Serial.println("How many integers would you like to sort?");
		while(!Serial.available());
		input = Serial.readString();
		numInts = input.toInt();
		if(numInts < 2)
		{
			Serial.println("A number greater than 1 please.");
		}
	} while(numInts < 2);

	intsToSort = new uint32_t[numInts];
	bool randGen = false;
	uint32_t inputInt;
	for(uint32_t i = 0; i < numInts; ++i)
	{
		if(!randGen)
		{
			Serial.println("Would you like to input integer " + String(i) + "? [Y/n] (Selecting no will randomly generate ALL remaining integers.)");
			while(!Serial.available());
			input = Serial.readString();
			if(input == "Y" || input =="y")
			{
				do {
					Serial.println("What should the integer be? (Integer must be between 0 and 0xFFFFFF. Non-integer inputs will be interpreted as 0.)");
					while(!Serial.available());
					inputInt = Serial.readString().toInt();
					Serial.println(String (inputInt));
				} while(inputInt >= 0xFFFFFF || inputInt < 0);
				continue;
			}
			randGen = true;
		}
		intsToSort[i] = random(0, 0x1000000);
	}

	for(uint32_t i = 0; i < numInts; ++i)
	{
		Serial.println(intsToSort[i]);
	}
	RGBSort(intsToSort, numInts);

	for(uint32_t i = 0; i < numInts; ++i)
	{
		Serial.println(String(intsToSort[i]));
	}
	analogWrite(B_RED, 255);
	analogWrite(B_GRE, 255);
	analogWrite(B_BLU, 255);
}

void loop()
{
}

void RGBSort(uint32_t *intArr, uint32_t len, uint32_t start)
{
	if(len == 1)
		return;

	uint32_t *p = intArr + start + len - 1;
	Serial.println("\nPivot = " + String(*p));
	uint32_t *a = intArr + start,
		*b = a + len - 2;
	// Serial.println("a = " + String(*a));
	// Serial.println("b = " + String(*b));

	while(a != b)
	{
		delay(500);
		analogWrite(B_RED, 255 - (*a >> 16 & 0xFF));
		analogWrite(B_GRE, 255 - (*a >> 8 & 0xFF));
		analogWrite(B_BLU, 255 - (*a & 0xFF));
		if(*a >= *p && *b < *p)
		{
			uint32_t temp = *a;
			*a = *b;
			*b = temp;
		}
		if(*a < *p)
		{
			++a;
		}
		if(*b >= *p && b != a)
		{
			--b;
		}
		// Serial.println("a = " + String(*a));
		// Serial.println("b = " + String(*b));
	}

	if(*b < *p)
	{
		++b;
	}
	else
	{
		--a;
	}
	uint32_t temp = *b;
	*b = *p;
	*p = temp;

	RGBSort(intArr, a - (intArr + start) + 1, start);
	RGBSort(intArr, p - b + 1, b - intArr);
}