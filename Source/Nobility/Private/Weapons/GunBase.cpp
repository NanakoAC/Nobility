// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/GunBase.h"
#include "Weapons/BulletBase.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"


// Sets default values
AGunBase::AGunBase()
	:BulletClass(ABulletBase::StaticClass())
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	MuzzlePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Muzzle"));
	MuzzlePoint->SetupAttachment(RootComp);
	MuzzlePoint->ArrowSize = 0.25;
	MuzzlePoint->ArrowColor = FColor::Blue;

	

	
}

void AGunBase::StartFiring_Implementation()
{
	if (!CanFire())return;

	if (bFullyAutomatic)
	{
		GetWorldTimerManager().SetTimer(RefireHandle, this, &AGunBase::HandleFire, RefireRate, true);
	}
	HandleFire();

}

void AGunBase::StopFiring_Implementation()
{
	GetWorldTimerManager().ClearTimer(RefireHandle);

}

void AGunBase::HandleFire()
{
	if (CanFire())
	{
		Fire();
		PostFire();
	}
}

//unsafe 
void AGunBase::Fire_Implementation()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Firing on server"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Firing on client"));
	}
	
	AActor* BulletOwner = GetOwner() ? GetOwner() : this;
	FTransform SpawnTransform = MuzzlePoint->GetComponentTransform();

	FActorSpawnParameters Params;
	//Lets add a random spin to the bullet

	//----------------------------
	FQuat QSpin = FQuat(FVector::ForwardVector, FMath::RandRange(-PI, PI));
	SpawnTransform = (FTransform(QSpin) * SpawnTransform); //The ordering of these is very important, if swapped it will go badly

	//-------------------------------

	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(BulletClass, SpawnTransform, Params);

	PostFire();
}

void AGunBase::PostFire_Implementation()
{
}

bool AGunBase::CanFire()
{
	if (!BulletClass) return false;
	if (RemainingAmmo <= 0) return false;
	return true;
}

// Called when the game starts or when spawned
void AGunBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

// Called every frame
void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGunBase::Drop()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	UE_LOG(LogTemp, Warning, TEXT("Gun drop 1"));
	//Lets drop it on the ground
	USkeletalMeshComponent* Collider = FindComponentByClass<USkeletalMeshComponent>();
	if (Collider)
	{
		UE_LOG(LogTemp, Warning, TEXT("Collider is %s"), *Collider->GetName());
		Collider->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		Collider->SetCollisionProfileName("Ragdoll");
		Collider->SetSimulatePhysics(true);
		Collider->SetEnableGravity(true);
		Collider->RecreatePhysicsState();

	}
	else
	{
		Destroy();
	}
}

