uint32_t *intsToSort;
uint32_t numInts;

void RGBSort(uint32_t *intArr, uint32_t len, uint32_t start=0);

void setup()
{
	Serial.begin(9600);
	randomSeed(analogRead(0));
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
		intsToSort[i] = random(0, 15);//0x1000000);
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
}

void loop()
{
}

void RGBSort(uint32_t *intArr, uint32_t len, uint32_t start)
{
	uint32_t *p = intArr + random(start,start + len);
	Serial.println("\nPivot = " + String(*p));
	uint32_t *a = intArr + start,
		*b = a + len - 1;
	Serial.println("a = " + String(*a));
	Serial.println("b = " + String(*b));

	while(a+1 != b)
	{
		if(*a >= *p && (*b < *p || b == p))
		{
			uint32_t temp = *a;
			*a = *b;
			*b = temp;
			if(p == a)
			{
				p = b;
			}
			else if(p == b)
			{
				p = a;
			}
		}
		if(*a < *p)
		{
			++a;
		}
		if(*b >= *p && b != p)
		{
			--b;
		}
		Serial.println("\nPivot = " + String(*p));
		Serial.println("a = " + String(*a));
		Serial.println("b = " + String(*b));
	}
}