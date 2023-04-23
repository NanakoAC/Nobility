// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HealthComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

#include "Net/UnrealNetwork.h"

void UHealthComponent::OnOwnerTakenDamage_Implementation(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("About to take damage!"));
	float formerhealth = GetCurrentHealth();
	UE_LOG(LogTemp, Warning, TEXT("Taking damage1! former health is  %f"), formerhealth);
	UE_LOG(LogTemp, Warning, TEXT("Taking damage1! Current health is  %f"), CurrentHealth);
	CurrentHealth = FMath::Clamp<float>(CurrentHealth - Damage, 0, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Taking damage2! Current health is now %f"), CurrentHealth);
	if (CurrentHealth != formerhealth)
	{
		OnHealthChanged.Broadcast(CurrentHealth - formerhealth);
		if (GetIsDead())
		{
			OnDied.Broadcast();
		}
	}
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UHealthComponent, CurrentHealth, COND_None);
	DOREPLIFETIME_CONDITION(UHealthComponent, MaxHealth, COND_None);
}

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		GetOwner()->SetReplicates(true);
	}

	CurrentHealth = (StartingHealth <= 0 ? MaxHealth : StartingHealth);
	// ...
	
	if (GetOwner())
	{
		AActor* MyOwner = GetOwner();
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnOwnerTakenDamage);
	}
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UHealthComponent::GetIsDead()
{
	return (CurrentHealth <= 0);
}

float UHealthComponent::GetCurrentHealth()
{
	return CurrentHealth;
}

