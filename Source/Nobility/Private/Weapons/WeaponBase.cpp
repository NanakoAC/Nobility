// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"
#include "Weapons/BulletBase.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"


// Sets default values
AWeaponBase::AWeaponBase()
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

void AWeaponBase::StartFiring_Implementation()
{
	if (!CanFire())return;

	if (bFullyAutomatic)
	{
		UE_LOG(LogTemp, Warning, TEXT("starting firing"));
		UE_LOG(LogTemp, Warning, TEXT("Muzzle is at: %s"), *MuzzlePoint->GetComponentLocation().ToString());

		GetWorldTimerManager().SetTimer(RefireHandle, this, &AWeaponBase::Fire, RefireRate, true);
	}
	Fire();
}

void AWeaponBase::StopFiring_Implementation()
{
	GetWorldTimerManager().ClearTimer(RefireHandle);

}

//unsafe 
void AWeaponBase::Fire_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Firing"));
	AActor* BulletOwner = GetOwner() ? GetOwner() : this;
	const FTransform SpawnTransform = MuzzlePoint->GetComponentTransform();
	
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(BulletClass, SpawnTransform, Params);

}

bool AWeaponBase::CanFire()
{
	if (!BulletClass) return false;
	if (RemainingAmmo <= 0) return false;
	return true;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


