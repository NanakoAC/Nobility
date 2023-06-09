// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformTrigger.generated.h"


class UBoxComponent;
class AMovingPlatform;

UCLASS()
class NOBILITY_API APlatformTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlatformTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* TriggerBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Base;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform", meta = (AllowPrivateAccess = "true"))
	UAnimationAsset* PressAnimation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform", meta = (AllowPrivateAccess = "true"))
	UAnimationAsset* ReleaseAnimation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Platform", meta = (AllowPrivateAccess = "true"))
	AMovingPlatform* AssignedPlatform;


	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Platform")
	void SwitchOn();

	UFUNCTION(BlueprintNativeEvent)
	void SwitchedOn();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Platform")
	void SwitchOff();

	UFUNCTION(BlueprintNativeEvent)
	void SwitchedOff();

};
