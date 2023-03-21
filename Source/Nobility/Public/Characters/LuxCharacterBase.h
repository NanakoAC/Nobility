// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LuxCharacterBase.generated.h"

class UCameraComponent;
class AWeaponBase;

UCLASS()
class NOBILITY_API ALuxCharacterBase : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess="true"))
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate = 45;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	float	BaseLookAtRate = 45;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	float MaxSprintSpeed	=	900;

	float DefaultWalkSpeed = 400;



public:
	// Sets default values for this character's properties
	ALuxCharacterBase();

	UCameraComponent* GetCameraComponent() { return CameraComp; }
	USkeletalMeshComponent* GetMesh1P() { return Mesh1P; }

	UPROPERTY(BlueprintReadOnly, Category = "Weapons")
	AWeaponBase* EquippedWeapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapons")
	FName AttachSocket;

	UFUNCTION(BlueprintPure, Category = "Weapons")
	AWeaponBase* GetEquippedWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	AWeaponBase* EquipWeapon(TSubclassOf<AWeaponBase> NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void UnEquipWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float delta);
	void MoveRight(float delta);
	void MoveUp(float delta);
	void Turn(float delta);
	void TurnRate(float delta);

	void LookUp(float delta);
	void LookUpRate(float delta);

	void StartJump( );
	void StopJump( );
	void StartShoot( );
	void StopShoot( );
	void StartSprint( );
	void StopSprint( );

};
