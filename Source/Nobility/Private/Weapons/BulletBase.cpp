// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BulletBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Weapons/GunBase.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABulletBase::ABulletBase()
{
	UE_LOG(LogTemp, Warning, TEXT("Bullet starting constructing at: %s"), *GetActorLocation().ToString());
	PrimaryActorTick.bCanEverTick = true;
	

	HitSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphere"));
	SetRootComponent(HitSphere);
	HitSphere->InitSphereRadius(8);
	HitSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HitSphere->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	HitSphere->SetNotifyRigidBodyCollision(true);
	if (GetOwner())
	{
		HitSphere->IgnoreActorWhenMoving(GetOwner(), true);
		HitSphere->IgnoreActorWhenMoving(GetOwner()->GetOwner(), true);
	}

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(HitSphere);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComp->InitialSpeed = 3000;
	ProjectileMovementComp->Velocity = FVector(1, 0, 0);
	UE_LOG(LogTemp, Warning, TEXT("Bullet finished constructing at: %s"), *GetActorLocation().ToString());
}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();

	HitSphere->OnComponentHit.AddDynamic(this, &ABulletBase::OnImpact);
	
}

// Called every frame
void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletBase::OnImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor)
	{
		return;
	}

	float DamageToDeal = Damage;
	if (GetOwner() && GetOwner()->IsA<AGunBase>())
	{
		AGunBase* MyGun = Cast<AGunBase>(GetOwner());
		DamageToDeal *= MyGun->DamageFactor;
	}

	if (Hit.BoneName == TEXT("HEAD")) //This is shitcode
	{
		DamageToDeal *= 2;
	}
	//OtherActor->TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor * DamageCauser);
	UGameplayStatics::ApplyDamage(OtherActor, DamageToDeal, nullptr, GetOwner(), UDamageType::StaticClass());
}

