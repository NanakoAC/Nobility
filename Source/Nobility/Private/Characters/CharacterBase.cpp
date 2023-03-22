// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Weapons/GunBase.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")); //This can only be used in constructors?
	CameraComp->SetupAttachment(GetRootComponent());
	CameraComp->bUsePawnControlRotation = true;

	AttachSocket = TEXT("GripPoint");//Default
	
	
	

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P")); //This can only be used in constructors?
	Mesh1P->SetupAttachment(CameraComp);
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetCastShadow(false);
	Mesh1P->SetReceivesDecals(false);


	

	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

AGunBase* ACharacterBase::GetEquippedWeapon()
{
	return EquippedWeapon;
}

AGunBase* ACharacterBase::EquipWeapon(TSubclassOf<AGunBase> NewWeapon)
{
	UE_LOG(LogTemp, Warning, TEXT("Equipping weapon 1"));
	if (EquippedWeapon)
	{
		UnEquipWeapon();
	}
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	EquippedWeapon = (GetWorld()->SpawnActor<AGunBase>(NewWeapon, Params));
	EquippedWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachSocket);
	UE_LOG(LogTemp, Warning, TEXT("Equipping weapon 2 : %s"), EquippedWeapon && EquippedWeapon->IsValidLowLevel() ? *EquippedWeapon->GetName() : TEXT("None"));

	return EquippedWeapon;
}

void ACharacterBase::UnEquipWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->OnUnequipped();
		EquippedWeapon->Destroy();
	}
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->SetOnlyOwnerSee(false);
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetCastShadow(true);

	
}

// Called every frame
void ACharacterBase::Tick(float ValueTime)
{
	Super::Tick(ValueTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForwards"), this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACharacterBase::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MoveUp"), this, &ACharacterBase::MoveUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ACharacterBase::Turn);
	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &ACharacterBase::TurnRate);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ACharacterBase::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &ACharacterBase::LookUpRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacterBase::StartJump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacterBase::StopJump);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ACharacterBase::StartSprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ACharacterBase::StopSprint);

	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &ACharacterBase::StartShoot);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Released, this, &ACharacterBase::StopShoot);

}

void ACharacterBase::MoveForward(float Value)
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



void ACharacterBase::MoveRight(float Value)
{
	if (Value != 0.0)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ACharacterBase::MoveUp(float Value)
{
	if (Value != 0.0)
	{
		AddMovementInput(GetActorUpVector(), Value);
	}
}

void ACharacterBase::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ACharacterBase::TurnRate(float Value)
{
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ACharacterBase::LookUpRate(float Value)
{
	AddControllerPitchInput(Value * BaseLookAtRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::StartJump()
{
	UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshComponent name: %s"), *GetMesh()->GetName());
	UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshComponent unique ID: %d"), GetMesh()->GetUniqueID());
	UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshComponent ownernosee: %d"), GetMesh()->bOwnerNoSee);
	Jump();
}

void ACharacterBase::StopJump()
{
	StopJumping();
}

void ACharacterBase::StartShoot()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->StartFiring();
	}
}

void ACharacterBase::StopShoot()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->StopFiring();
	}
}

void ACharacterBase::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;
}

void ACharacterBase::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}
