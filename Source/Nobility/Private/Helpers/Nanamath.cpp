// Fill out your copyright notice in the Description page of Project Settings.


#include "Helpers/Nanamath.h"

Nanamath::Nanamath()
{
}

Nanamath::~Nanamath()
{
}

int Nanamath::WrapToRange(int X, int Min, int Max)
{
	int Size = (Max - Min) + 1;
	if (Size == 0)
	{
		// Guard against zero-sized ranges causing an infinite loop.
		return Max;
	}

	int EndVal = X;
	while (EndVal < Min)
	{
		EndVal += Size;
	}

	while (EndVal > Max)
	{
		EndVal -= Size;
	}
	return EndVal;
}