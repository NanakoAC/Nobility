// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Helpers/Nanamath.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::IterateTarget()
{
	TargetIndex++;
	TargetIndex = Nanamath::WrapToRange(TargetIndex, 0, TargetLocations.Num()-1);
	TargetLocation = TargetLocations[TargetIndex];
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("platform beginplay "));
	//Convert local to world
	for (int i = 0; i<TargetLocations.Num(); i++)
	{
		TargetLocations[i] = GetTransform().TransformPosition(TargetLocations[i]);
	}
	TargetLocation = TargetLocations[TargetIndex];
	

	//It only ticks if its on the server and set to autoprocess
	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
		if (!AutoProcess)
		{
			SetActorTickEnabled(false);
		}
	}
	else
	{
		SetActorTickEnabled(false);
	}
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), TargetLocation, DeltaTime, MoveSpeed));
	if ((GetActorLocation() - TargetLocation).Length() < StopTolerance)
	{
		IterateTarget();
	}
}
 