// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/LuxCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Weapons/WeaponBase.h"

// Sets default values
ALuxCharacterBase::ALuxCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")); //This can only be used in constructors?
	CameraComp->SetupAttachment(GetRootComponent());
	CameraComp->bUsePawnControlRotation = true;

	
	
	
	

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P")); //This can only be used in constructors?
	Mesh1P->SetupAttachment(CameraComp);
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetCastShadow(false);
	Mesh1P->SetReceivesDecals(false);


	

	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

AWeaponBase* ALuxCharacterBase::GetEquippedWeapon()
{
	return EquippedWeapon;
}

AWeaponBase* ALuxCharacterBase::EquipWeapon(TSubclassOf<AWeaponBase> NewWeapon)
{
	UE_LOG(LogTemp, Warning, TEXT("Equipping weapon 1"));
	if (EquippedWeapon)
	{
		UnEquipWeapon();
	}
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	EquippedWeapon = (GetWorld()->SpawnActor<AWeaponBase>(NewWeapon, Params));
	EquippedWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachSocket);
	UE_LOG(LogTemp, Warning, TEXT("Equipping weapon 2 : %s"), EquippedWeapon && EquippedWeapon->IsValidLowLevel() ? *EquippedWeapon->GetName() : TEXT("None"));

	return EquippedWeapon;
}

void ALuxCharacterBase::UnEquipWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->OnUnequipped();
		EquippedWeapon->Destroy();
	}
}

// Called when the game starts or when spawned
void ALuxCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->SetOnlyOwnerSee(false);
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetCastShadow(true);

	
}

// Called every frame
void ALuxCharacterBase::Tick(float ValueTime)
{
	Super::Tick(ValueTime);

}

// Called to bind functionality to input
void ALuxCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForwards"), this, &ALuxCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ALuxCharacterBase::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MoveUp"), this, &ALuxCharacterBase::MoveUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ALuxCharacterBase::Turn);
	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &ALuxCharacterBase::TurnRate);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ALuxCharacterBase::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &ALuxCharacterBase::LookUpRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ALuxCharacterBase::StartJump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ALuxCharacterBase::StopJump);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ALuxCharacterBase::StartSprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ALuxCharacterBase::StopSprint);

	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &ALuxCharacterBase::StartShoot);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Released, this, &ALuxCharacterBase::StopShoot);

}

void ALuxCharacterBase::MoveForward(float Value)
{
	if (Value != 0.0)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
	else
	{
		StopSprint();
	}
}



void ALuxCharacterBase::MoveRight(float Value)
{
	if (Value != 0.0)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ALuxCharacterBase::MoveUp(float Value)
{
	if (Value != 0.0)
	{
		AddMovementInput(GetActorUpVector(), Value);
	}
}

void ALuxCharacterBase::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ALuxCharacterBase::TurnRate(float Value)
{
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ALuxCharacterBase::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ALuxCharacterBase::LookUpRate(float Value)
{
	AddControllerPitchInput(Value * BaseLookAtRate * GetWorld()->GetDeltaSeconds());
}

void ALuxCharacterBase::StartJump()
{
	UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshComponent name: %s"), *GetMesh()->GetName());
	UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshComponent unique ID: %d"), GetMesh()->GetUniqueID());
	UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshComponent ownernosee: %d"), GetMesh()->bOwnerNoSee);
	Jump();
}

void ALuxCharacterBase::StopJump()
{
	StopJumping();
}

void ALuxCharacterBase::StartShoot()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->StartFiring();
	}
}

void ALuxCharacterBase::StopShoot()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->StopFiring();
	}
}

void ALuxCharacterBase::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;
}

void ALuxCharacterBase::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}
