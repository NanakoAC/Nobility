// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructionComponent.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

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
	TCHAR* Filepath = TEXT("Blueprint'/Engine/EditorResources/FieldNodes/FS_MasterField.FS_MasterField'");
	//--------------------------------------------------------
	UE_LOG(LogTemp, Error, TEXT("Attempting LoadObject"));
	UBlueprint* BlueprintObj = LoadObject<UBlueprint>(nullptr, TEXT("Masterfield"),Filepath);

	UE_LOG(LogTemp, Error, TEXT("Attempting LoadClass"));
	UClass* BlueprintClass = LoadClass<AActor>(nullptr, TEXT("Masterfield"), Filepath);


	UE_LOG(LogTemp, Error, TEXT("Attempting FindObject"));
	UObject* LoadedBlueprint = FindObject<UObject>(nullptr, Filepath);

	if (LoadedBlueprint)
	{
		UE_LOG(LogTemp, Error, TEXT("Success FindObject"));
		UClass* BPClass = Cast<UClass>(LoadedBlueprint);
	}

	UE_LOG(LogTemp, Error, TEXT("Attempting StaticLoadClass"));
	UClass* BPClass = StaticLoadClass(UObject::StaticClass(), nullptr, Filepath);
	if (BPClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Success StaticLoadClass"));
	}

	//----------------------------------------------
	UE_LOG(LogTemp, Error, TEXT("Blueprintobj 1"));
	// Check if the Blueprint object was loaded successfully
	if (BlueprintObj)
	{	
		UE_LOG(LogTemp, Error, TEXT("Blueprintobj 2 %s"), *BlueprintObj->GetName());
		// Get the generated C++ class of the Blueprint
		//UClass* BlueprintClass = BlueprintObj->GeneratedClass;

		// Spawn the Blueprint using the SpawnActor method
		AActor* SpawnedActor = GetWorld()->SpawnActor(BlueprintClass, &TransformOwner, params);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Blueprintobj 3"));
	}



	//Unless we fail to find this thing
	UGeometryCollectionComponent* GCC = Cast<UGeometryCollectionComponent>(Debris->GetComponentByClass(UGeometryCollectionComponent::StaticClass()));
	
	//This will not work
	GCC->EnableClustering = false;

	if (DebrisLifetime)
	{
		Debris->SetLifeSpan(DebrisLifetime);
	}

	PostShatter();
}

void UDestructionComponent::PostShatter()
{
	//If we are no longer able to shatter, lets cleanup
	if (!CanEverShatter())
	{
		//Maybe we destroy our owner
		if (DestroyOnShatter)
		{
			//But only if they are not already being/been destroyed by something else
			if (IsValid(GetOwner()))
			{
				GetOwner()->Destroy();
			}
		}
	}
}

void UDestructionComponent::SetCooldown()
{
	LastShatter = GetWorld()->GetTimeSeconds();
}
