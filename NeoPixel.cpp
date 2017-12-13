// NeoPixel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdint.h"
#include "math.h"
#include "arduino.h"

#define FADE_LENGTH 10 // number of steps in the fade function
#define FADE_SPEED 200 // time in milliseconds between fade updates

class colourClass;

struct ColourDifference
{
	int8_t dRed;
	int8_t dGreen;
	int8_t dBlue;
};

uint32_t strip1[100];
uint32_t strip2[100];
uint32_t strip3[100];
uint32_t strip4[100];

// Fade global variables
ColourDifference fadeColourDifference;
int fadecount;
bool incrementFlag;
colourClass currentFadeColour;
bool setupFlag;
unsigned long LastFadeTime;

class colourClass
{
public:
	void SetColour(int red, int green, int blue)
	{
		fullcolour = (red << 16) + (green << 8) + blue;
	}

	uint32_t AddColourDifference(int red, int green, int blue)
	{
		int newRed = red + GetRed();
		int newGreen = green + GetGreen();
		int newBlue = blue + GetBlue();

		SetColour(newRed, newGreen, newBlue);

		return fullcolour;
	}

	uint32_t AddColourDifference(colourClass colourDifference)
	{
		int newRed = GetRed() + colourDifference.GetRed();
		int newGreen = GetGreen() + colourDifference.GetGreen();
		int newBlue = GetBlue() + colourDifference.GetBlue();

		SetColour(newRed, newGreen, newBlue);

		return fullcolour;
	}

	uint32_t SubtractColourDifference(ColourDifference colourDifference)
	{
		int newRed = GetRed() - colourDifference.dRed;
		int newGreen = GetGreen() - colourDifference.dGreen;
		int newBlue = GetBlue() - colourDifference.dBlue;

		SetColour(newRed, newGreen, newBlue);

		return fullcolour;
	}

	uint32_t GetColour()
	{
		return fullcolour;
	}

	uint8_t GetRed()
	{
		return (fullcolour & 0x00FF0000) >> 16;
	}

	uint8_t GetGreen()
	{
		return (fullcolour & 0x0000FF00) >> 8;
	}

	uint8_t GetBlue()
	{
		return (fullcolour & 0x000000FF);
	}

	ColourDifference GetDifferenceColour(colourClass newColour, int length)
	{
		uint8_t red1 = GetRed();
		uint8_t green1 = GetGreen();
		uint8_t blue1 = GetBlue();

		uint8_t red2 = newColour.GetRed();
		uint8_t green2 = newColour.GetGreen();
		uint8_t blue2 = newColour.GetBlue();

		ColourDifference colourdifference;

		colourdifference.dRed = (red1 - red2) / length;
		colourdifference.dGreen = (green1 - green2) / length;
		colourdifference.dBlue = (blue1 - blue2) / length;

		return colourdifference;
	}

private:
	uint32_t fullcolour;
};

uint32_t* AllSquares[4];


void FadeSetup(colourClass colour1, colourClass colour2)
{
	fadeColourDifference = colour1.GetDifferenceColour(colour2, FADE_LENGTH);
	fadecount = 0;
	incrementFlag = 1;
	currentFadeColour = colour2;
	setupFlag = 1;
	//LastFadeTime = now;
}

void FadePoll()
{
	if (!setupFlag)
		FadeSetup();

	unsigned long currentFadeTime = millis();
	if ((currentFadeTime - LastFadeTime) >= FADE_SPEED)
	{
		switch (incrementFlag)
		{
		case 1:
			currentFadeColour.AddColourDifference(fadeColourDifference);
			SetAll(currentFadeColour);
			ShowAll(0);
			break;
		case 0:
			currentFadeColour.SubtractColourDifference(fadeColourDifference);
			SetAll(currentFadeColour);
			ShowAll(0);
			break;
		}
		fadecount++;
		if (fadecount == FADE_LENGTH)
		{
			fadecount = 0;
			incrementFlag = !incrementFlag;
		}
	}
	LastFadeTime = currentFadeTime;
}

void SetAll(colourClass colour)
{
	for (int square = 0; square < 4; square++)
	{
		for (int pixel = 0; pixel < AllSquares.numPixels(); pixel++)
		{
			AllSquares[square]->setPixel(pixel, colour.GetColour());
		}
	}
}

void ShowAll(int stripdelay)
{
	for (int square = 0; square < 4; square++)
	{
		AllSquares[square]->show();
		delay(stripdelay);
	}
}


void setup()
{
	AllSquares[0] = &strip1[0];
	AllSquares[1] = &strip2[0];
	AllSquares[2] = &strip3[0];
	AllSquares[3] = &strip4[0];
}

int main()
{
	/*colourClass colour1;
	colour1.SetColour(0, 0, 255);
	colourClass colour2;
	colour2.SetColour(0, 0, 60);

	//FadeFunction(colour1, colour2);

	SpiralFunction(colour1, colour2);*/

	setup();

	while (1)
	{
		FadePoll();
	}

	return 0;
}