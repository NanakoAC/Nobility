// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DestructionComponent.generated.h"

class AGeometryCollectionActor;

UCLASS(Blueprintable, EditInlineNew, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DESTRUCTION_API UDestructionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	

	UDestructionComponent();

	//Configuration
	//------------------------
	//Paste in a geometry collection created through fracturing. This must be done using the fracture mode in the editor
	//Currently this is only possible through blueprint, and it will not work without one
	//These are apparently just normal actors with a special component and setup
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Destruction")
	TSubclassOf<AActor> GeometryCollection;
	

	//If true, destroy the object after we reach shatter limit
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Destruction")
	bool DestroyOnShatter = true;
	
	

	//How many times are we allowed to shatter before we are done?
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Destruction")
	int MaxShatterCount = 1;

	//A minimum quantity of time after shattering before we are allowed to do it again. If its too soon, triggering events will be silently dropped
	//Only used if MaxShatterCount is > 1
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Destruction")
	float ShatterCooldown = 0.3f;

	//How long will the created debris last? Set to 0 to make permanant (not recommended)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Destruction")
	float DebrisLifetime = 5.0f;


	//Triggering
	//If true, trigger shatter as our parent actor is being destroyed.
	//It is fine to pair this with DestroyOnShatter, safety checks will prevent double shattering or loops
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Destruction")
	bool ShatterOnDestroy = false;	

	//Trigger shatter upon physical impacts
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Destruction")
	bool ShatterOnImpact = true;

	//Used with ShatterOnImpact, a minimum amount of force required to make us shatter
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Destruction")
	float MinimumForce = 0;

private:
	//Operations

	//When did we last shatter? used for cooldowns
	float LastShatter = 0;

	//How many times have we shattered already? 
	//In 99% of cases, we should only shatter once, but we can support multiples
	int ShatterCount = 0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Are we able to shatter right now?
	UFUNCTION()
	bool CanShatterNow();

	//Will we ever be able to shatter again?
	UFUNCTION()
	bool CanEverShatter();
		

	//Event Recievers
	UFUNCTION()
	void OnDestroy(AActor* DestroyedActor);

	UFUNCTION()
	void OnImpact(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	

	//Core Functions
	//---------------------

	//Calls shatter if we pass safety checks
	UFUNCTION()
	bool AttemptShatter();

	//Do the thing
	UFUNCTION() 
	void Shatter();

	//Called after the thing
	UFUNCTION()
	void PostShatter();

	//Called just before we do the thing
	UFUNCTION()
	void SetCooldown();
};
