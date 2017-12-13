// NeoPixel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdint.h"
#include "math.h"

#define TOTAL_SQUARES 4
#define FADE_LENGTH 10
#define SPIRAL_NUMBER 10
#define ALTERNATE_DELAY 50
#define PULSE_NUMBER 200

class colourClass;

uint32_t strip1[100];
uint32_t strip2[100];
uint32_t strip3[100];
uint32_t strip4[100];

int HyperSpaceDelay[10] = { 1000, 500, 250, 125, 62, 30, 15, 7, 3, 2 };

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

	uint32_t SubtractColourDifference(colourClass colourDifference)
	{
		int newRed = GetRed() - colourDifference.GetRed();
		int newGreen = GetGreen() - colourDifference.GetGreen();
		int newBlue = GetBlue() - colourDifference.GetBlue();

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

	colourClass GetDifferenceColour(colourClass newColour, int length)
	{
		uint8_t red1 = GetRed();
		uint8_t green1 = GetGreen();
		uint8_t blue1 = GetBlue();

		uint8_t red2 = newColour.GetRed();
		uint8_t green2 = newColour.GetGreen();
		uint8_t blue2 = newColour.GetBlue();

		uint8_t dRed = (red1 - red2) / length;
		uint8_t dGreen = (green1 - green2) / length;
		uint8_t dBlue = (blue1 - blue2) / length;

		colourClass colourDiff;
		colourDiff.SetColour(dRed, dGreen, dBlue);

		return colourDiff;
	}

private:
	uint32_t fullcolour;
};

uint32_t* AllSquares[TOTAL_SQUARES];

/*void trailMoveON()
{
	int temp;

	pixelNum++;
	if (pixelNum == 100)
		pixelNum = 0;

	for (int i = 0; i < 8; i++)
	{
		temp = pixelNum - i;
		if (temp < 0)
			temp = 100 + temp;

		LEDstrip[temp] = trailColourArray[i];
	}
}*/


void FadeFunction(colourClass colour1, colourClass colour2)
{
	colourClass colourDifference = colour1.GetDifferenceColour(colour2, FADE_LENGTH);
	uint32_t tempColour;

	for (int i = 0; i < FADE_LENGTH; i++)
	{
		tempColour = colour2.AddColourDifference(colourDifference);
		
		for (int j = 0; j < 4; j++)
		{
			AllSquares[j][i] = tempColour;
		}	
	}
	for (int i = 0; i < FADE_LENGTH; i++)
	{
		tempColour = colour2.SubtractColourDifference(colourDifference);

		for (int j = 0; j < 4; j++)
		{
			AllSquares[j][i] = tempColour;
		}
	}
}

void SpiralFunction(colourClass colour1, colourClass colour2)
{
	colourClass colourDifference = colour1.GetDifferenceColour(colour2, SPIRAL_NUMBER);
	uint32_t tempColour;

	for (int k = 0; k < SPIRAL_NUMBER; k++)
	{
		tempColour = colour2.AddColourDifference(colourDifference);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 100; j++)
			{
				AllSquares[i][j] = tempColour;
				// LEDStripArray[i].show();
				//delay(SPIRAL_DELAY);
			}
		}
	}
	for (int k = 0; k < SPIRAL_NUMBER; k++)
	{
		tempColour = colour2.SubtractColourDifference(colourDifference);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 100; j++)
			{
				AllSquares[i][j] = tempColour;
				//delay(SPIRAL_DELAY);
			}
		}
	}
}

void HyperSpaceJump(colourClass colour1, colourClass colour2)
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int pixel = 0; pixel < 0; pixel++)
			{
				AllSquares[j][pixel] = colour2.GetColour();
			}
			//delay(HyperSpaceDelay[i];
			for (int pixel = 0; pixel < 0; pixel++)
			{
				AllSquares[j][pixel] = 0;
			}
		}
	}
}

void Pulse(colourClass colour1, int frequency)
{
	for (int sampleNumber = 0; sampleNumber < PULSE_NUMBER; sampleNumber++)
	{
		for (int square = 0; square < 4; square++)
		{
			for (int pixel = 0; pixel < 100; pixel++)
			{
				AllSquares[square][pixel] = colour1.GetColour() + colour1.GetColour()*sin(2* 3.14 * sampleNumber / PULSE_NUMBER);
			}
		}
	}	
}

void Alternate(colourClass colour1, colourClass colour2)
{
	for (int pixel = 0; pixel < 100; pixel++)
	{
		AllSquares[0][pixel] = colour1.GetColour();
		AllSquares[2][pixel] = colour1.GetColour();

		AllSquares[1][pixel] = colour2.GetColour();
		AllSquares[3][pixel] = colour2.GetColour();
	}

	//delay(ALTERNATE_DELAY);

	for (int pixel = 0; pixel < 100; pixel++)
	{
		AllSquares[0][pixel] = colour2.GetColour();
		AllSquares[2][pixel] = colour2.GetColour();

		AllSquares[1][pixel] = colour1.GetColour();
		AllSquares[3][pixel] = colour1.GetColour();
	}

	//delay(ALTERNATE_DELAY);
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

	return 0;
}