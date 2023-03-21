// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HealthComponent.h"
#include "GameFramework/Actor.h"

void UHealthComponent::OnOwnerTakenDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	float formerhealth = GetCurrentHealth();
	CurrentHealth = FMath::Clamp<float>(CurrentHealth - Damage, 0, MaxHealth);
	if (CurrentHealth != formerhealth)
	{
		OnHealthChanged.Broadcast(CurrentHealth - formerhealth);
		if (GetIsDead())
		{
			OnDied.Broadcast();
		}
	}
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
	CurrentHealth = (StartingHealth == 0 ? MaxHealth : StartingHealth);
	// ...

	if (GetOwner())
	{
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnOwnerTakenDamage);
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

