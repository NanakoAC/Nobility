// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UCameraComponent;
class AGunBase;

UCLASS()
class NOBILITY_API ACharacterBase : public ACharacter
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
	ACharacterBase();

	UCameraComponent* GetCameraComponent() { return CameraComp; }
	USkeletalMeshComponent* GetMesh1P() { return Mesh1P; }

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapons")
	AGunBase* EquippedWeapon;

	UPROPERTY(Replicated, BlueprintReadOnly, EditAnywhere, Category = "Weapons")
	TArray<TSubclassOf<AGunBase>> WeaponInventory;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapons")
	FName AttachSocket = TEXT("GripPoint");

	UFUNCTION(BlueprintPure, Category = "Weapons")
	AGunBase* GetEquippedWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void CycleWeapon(bool bForward);

	//Call this to do the equipping of a weapon on the server
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Weapons")
	void EquipWeapon(TSubclassOf<AGunBase> NewWeapon);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Weapons")
	void DebugMessageTest();

	//Called FROM EquipWeapon to let clients know that the equipping is done
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Weapons")
	void OnEquipped(AGunBase* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void GrantWeapon(TSubclassOf<AGunBase> NewWeapon, bool bEquip);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void UnEquipWeapon(TSubclassOf<AGunBase> NewWeapon = NULL);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void RemoveWeapon(TSubclassOf<AGunBase> NewWeapon);

	

	int NanaWrap(int input, int min, int max);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	virtual void Destroyed() override;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Weapons")
	void Ragdoll();
};
