// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class NOBILITY_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMovingPlatform();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform")
	float MoveSpeed = 200.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform")
	float StopTolerance = 5;//When we are within this distance of the target we stop


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform", Meta = (MakeEditWidget = true))
	TArray<FVector> TargetLocations;

	

private:
	int TargetIndex = 0;

	//The currently selected target
	FVector TargetLocation;

	UFUNCTION()
	void IterateTarget();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
