// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class NOBILITY_API Nanamath
{
public:
	Nanamath();
	~Nanamath();

	static int WrapToRange(int input, int min, int max);
};
