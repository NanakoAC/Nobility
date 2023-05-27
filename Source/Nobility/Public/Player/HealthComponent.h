// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, Health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDied);

class UDamageType;
class AController;
class AActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NOBILITY_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	
	
public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float StartingHealth;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDied OnDied;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetCurrentHealth();

	UFUNCTION(BlueprintPure, Category = "Health")
	bool GetIsDead();

	//A delegate function must be a ufunction or it will throw a runtime error
	UFUNCTION(Server, Reliable)
	void OnOwnerTakenDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
		

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
