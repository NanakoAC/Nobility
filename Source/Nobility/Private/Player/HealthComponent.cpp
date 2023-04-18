// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HealthComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

void UHealthComponent::OnOwnerTakenDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
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
	CurrentHealth = (StartingHealth <= 0 ? MaxHealth : StartingHealth);
	// ...
	
	if (GetOwner())
	{
		AActor* MyOwner = GetOwner();
		UE_LOG(LogTemp, Warning, TEXT("Spawning healthcomponent on actor %s"), *GetOwner()->GetActorLabel());
		UE_LOG(LogTemp, Warning, TEXT("Spawning healthcomponent on actor of class %s"), *GetOwner()->GetClass()->GetName());
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnOwnerTakenDamage);
		MyOwner->OnTakeAnyDamage.BindUFunction(this, FName("OnOwnerTakenDamage"));
		UE_LOG(LogTemp, Warning, TEXT("Spawning healthcomponent on actor %s"), *GetOwner()->GetActorLabel());
	}
	
	//UGameplayStatics::ApplyDamage(GetOwner(), 100, nullptr, GetOwner(), UDamageType::StaticClass());
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

