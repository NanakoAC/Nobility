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
		UE_LOG(LogTemp, Warning, TEXT("starting firing"));
		UE_LOG(LogTemp, Warning, TEXT("Muzzle is at: %s"), *MuzzlePoint->GetComponentLocation().ToString());

		GetWorldTimerManager().SetTimer(RefireHandle, this, &AGunBase::Fire, RefireRate, true);
	}
	Fire();
}

void AGunBase::StopFiring_Implementation()
{
	GetWorldTimerManager().ClearTimer(RefireHandle);

}

//unsafe 
void AGunBase::Fire_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Firing"));
	AActor* BulletOwner = GetOwner() ? GetOwner() : this;
	const FTransform SpawnTransform = MuzzlePoint->GetComponentTransform();
	
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(BulletClass, SpawnTransform, Params);

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
	
}

// Called every frame
void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


