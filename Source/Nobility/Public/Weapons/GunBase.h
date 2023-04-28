// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunBase.generated.h"

class UArrowComponent;
class ABulletBase;

UCLASS()
class NOBILITY_API AGunBase : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* MuzzlePoint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootComp;
	
public:	
	// Sets default values for this actor's properties
	AGunBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ABulletBase> BulletClass;

	FTimerHandle RefireHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float RefireRate = 0.1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float DamageFactor = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bFullyAutomatic = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float RemainingAmmo = 999;

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void StartFiring();

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void StopFiring();
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void HandleFire();

	UFUNCTION(Server, Reliable, Category = "Weapon")
	void Fire(FTransform SpawnTransform);

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void PostFire();			

	bool CanFire();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void OnUnequipped();

	void Drop();
};
 