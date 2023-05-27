// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructionComponent.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "TimerManager.h"
#include "UObject/UnrealType.h"

// Sets default values for this component's properties
UDestructionComponent::UDestructionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UDestructionComponent::BeginPlay()
{

	Super::BeginPlay();

	//Lets assume that if one class is loaded, all of them will be. And vice versa

	
	if (!GeometryCollection)
	{
		//You forgot to populate the blueprint with a fractured geometry
		//Future todo: Dynamic fracturing?
		UE_LOG(LogInit, Error, TEXT("Destructible object initialised without geometry collection: %s"), *GetOwner()->GetName());
		DestroyComponent();
		return;
	}

	if (ShatterOnDestroy)
	{
		GetOwner()->OnDestroyed.AddDynamic(this, &UDestructionComponent::OnDestroy);
	}

	if (ShatterOnImpact)
	{
		GetOwner()->OnActorHit.AddDynamic(this, &UDestructionComponent::OnImpact);
	}
}




// Called every frame
void UDestructionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UDestructionComponent::CanShatterNow()
{
	if (!CanEverShatter())	return false;

	//Still on cooldown?
	if ((GetWorld()->GetTimeSeconds() - ShatterCooldown) < LastShatter) return false;

	return true;
}

bool UDestructionComponent::CanEverShatter()
{
	//We have shattered enough
	if (ShatterCount >= MaxShatterCount) return false;

	//Doesnt exist, we're attached to nothing
	if (!GetOwner()) return false;

	//An actor is not valid, but is marked as pending kill, in the time between calling Destroy and actually ceasing to exist
	//We can still shatter during that period
	//if (!GetOwner()->IsValid() && !GetOwner()->IsPendingKill()) return false;

	return true;
}

void UDestructionComponent::OnDestroy(AActor* DestroyedActor)
{
	UE_LOG(LogTemp, Error, TEXT("Destructible object Ondestroy"));
	AttemptShatter();
}

void UDestructionComponent::OnImpact(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Error, TEXT("Destructible object Onimpact"));
	AttemptShatter();
}

//Safety checked
bool UDestructionComponent::AttemptShatter()
{
	UE_LOG(LogTemp, Error, TEXT("Destructible object attemptshatter1"));
	if (!CanShatterNow()) return false;
	Shatter();
	return true;
}

void UDestructionComponent::Shatter()
{
	//We cannot fail from this moment on
	SetCooldown();
	ShatterCount++;
	FActorSpawnParameters params;
	FTransform TransformOwner = GetOwner()->GetActorTransform();
	AActor* Debris = GetWorld()->SpawnActor<AActor>(GeometryCollection, TransformOwner, params);
	
	ScatterDebris(Debris);


	if (DebrisLifetime)
	{
		Debris->SetLifeSpan(DebrisLifetime);
	}

	PostShatter();
}

//Here, we spawn a chaos field according to the chosen scatter type. 
//These fields automatically activate and cleanup themselves, so our work is done once we spawn it and set parameters
void UDestructionComponent::ScatterDebris(AActor* Debris)
{
	FActorSpawnParameters params;
	FTransform TransformOwner = GetOwner()->GetActorTransform();
	//AFieldSystemActor* Field = Cast<AFieldSystemActor>(GetWorld()->SpawnActor(ExplodeClass, &TransformOwner, params));
	switch (ScatterType)
	{
		case EDestructionScatterType::Scatter_Explode:
		{
			//Just blow it up with a field
			AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ExplodeFieldSpawnClass, TransformOwner, params);
		}
		case EDestructionScatterType::Scatter_None:
		{
			//This is more complex. We get the GCC which holds linkage data
			UGeometryCollectionComponent* GCC = Debris->FindComponentByClass<UGeometryCollectionComponent>();

			if (!GCC) return;

			//We set all the damage thresholds to a tiny minimum, so the object will fall apart easily
			for (int32 i = 0; i < GCC->DamageThreshold.Num(); i++)
			{
				GCC->DamageThreshold[i] = 1.0f;
			}

			//Then we blow a light breeze on it
			AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(CollapseFieldSpawnClass, TransformOwner, params);
		}
	}
	// Spawn the Blueprint using the SpawnActor method

}

void UDestructionComponent::PostShatter()
{
	//If we are no longer able to shatter, lets cleanup
	if (!CanEverShatter())
	{
		//Maybe we destroy our owner
		if (DestroyOnShatter && IsValid(GetOwner()) && IsValid(this))
		{
		

			// Set the timer to execute on the next tick
			GetOwner()->GetWorldTimerManager().SetTimerForNextTick(this, &UDestructionComponent::DestroyOwner);
		}
	}
}

void UDestructionComponent::SetCooldown()
{
	LastShatter = GetWorld()->GetTimeSeconds();
}




void UDestructionComponent::DestroyOwner()
{
	if (IsValid(GetOwner()))
	{
		GetOwner()->Destroy();
	}
}

#if WITH_EDITOR
//This returns true when this object's blueprint is being edited. Alone, as the main subject of editing
//It returns false when this object is being edited in ANOTHER object's blueprint, as a component attached to it
//Intended for authortime use only, won't work at runtime
bool UDestructionComponent::IsDirectlyEditedBlueprint() const
{
	UObject* MyOwner = GetOuter();
	bool result = !(MyOwner && MyOwner->IsA<UBlueprintGeneratedClass>());

	return result;
}


bool UDestructionComponent::CanEditChange(const FProperty* InProperty) const
{
	if (InProperty)
	{
		FString PropertyName = InProperty->GetName();

		//The field spawn classes should never be edited as a component, only as base value
		if (PropertyName.Contains("FieldSpawnClass"))
		{
			if (!IsDirectlyEditedBlueprint())
			{
				return false;
			}
		}
	}
	return true;
}
#endif