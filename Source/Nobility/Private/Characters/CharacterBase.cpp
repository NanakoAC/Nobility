// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Weapons/GunBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Helpers/Nanamath.h"
#include "Net/UnrealNetwork.h"

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


// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->SetOnlyOwnerSee(false);
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetCastShadow(true);
	DebugMessageTest();

}

AGunBase* ACharacterBase::GetEquippedWeapon()
{
	return EquippedWeapon;
}

void ACharacterBase::CycleWeapon(bool bForward)
{
	UE_LOG(LogTemp, Warning, TEXT("cycle weapon 1"));
	if (!WeaponInventory.Num()) return;

	int CurrentEquipIndex = 0;
	if (EquippedWeapon) CurrentEquipIndex = WeaponInventory.Find(EquippedWeapon->GetClass());
	UE_LOG(LogTemp, Warning, TEXT("Cycling weapon num %d "), WeaponInventory.Num());
	UE_LOG(LogTemp, Warning, TEXT("Cycling weapon current %d "), CurrentEquipIndex);
	int TargetIndex = CurrentEquipIndex + (bForward ? 1 : -1);

	UE_LOG(LogTemp, Warning, TEXT("Cycling weapon target1 %d "), TargetIndex);
	TargetIndex = Nanamath::WrapToRange(TargetIndex, 0, WeaponInventory.Num() - 1);

	//My own implementation of Wrap


	UE_LOG(LogTemp, Warning, TEXT("Cycling weapon target2 %d "), TargetIndex);
	EquipWeapon(WeaponInventory[TargetIndex]);
}

void ACharacterBase::DebugMessageTest_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Debug message test"));
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("We are running on server"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("We are running on client"));
	}
}

void ACharacterBase::EquipWeapon_Implementation(TSubclassOf<AGunBase> NewWeapon)
{
	UE_LOG(LogTemp, Warning, TEXT("Equipping weapon 1"));
	if (EquippedWeapon)
	{
		if (EquippedWeapon->GetClass() == NewWeapon)	return;//Already got it on

		//Remove current
		UnEquipWeapon();
	}
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	EquippedWeapon = (GetWorld()->SpawnActor<AGunBase>(NewWeapon, Params));

	if (IsPlayerControlled())
	{
		//Todo: Make this multiplayer compatible
		EquippedWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachSocket);
	}
	else
	{ 
		EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachSocket);
	}
	UE_LOG(LogTemp, Warning, TEXT("Equipping weapon 2 : %s"), EquippedWeapon && EquippedWeapon->IsValidLowLevel() ? *EquippedWeapon->GetName() : TEXT("None"));

	OnEquipped(EquippedWeapon);
}

void ACharacterBase::OnEquipped_Implementation(AGunBase* NewWeapon)
{
}

void ACharacterBase::GrantWeapon(TSubclassOf<AGunBase> NewWeapon, bool bEquip)
{
	UE_LOG(LogTemp, Warning, TEXT("granting weapon"));
	WeaponInventory.AddUnique(NewWeapon);
	if (bEquip)
	{
		EquipWeapon(NewWeapon);
	}
}


//A weapon type can optionally be passed in, and we will only unequip if that is what we currently have on
void ACharacterBase::UnEquipWeapon(TSubclassOf<AGunBase> NewWeapon)
{
	if (EquippedWeapon)
	{
		if (NewWeapon && EquippedWeapon->GetClass()->IsChildOf(NewWeapon))	return;
		EquippedWeapon->OnUnequipped();
		EquippedWeapon->Destroy();
	}
}

void ACharacterBase::RemoveWeapon(TSubclassOf<AGunBase> NewWeapon)
{
	if (EquippedWeapon->GetClass()->IsChildOf(NewWeapon))
	{
		UnEquipWeapon();
	}
	WeaponInventory.AddUnique(NewWeapon);
}






void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACharacterBase, WeaponInventory, COND_None); 
	DOREPLIFETIME_CONDITION(ACharacterBase, EquippedWeapon, COND_None);
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
	DebugMessageTest();
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

void ACharacterBase::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Drop();
		
		EquippedWeapon = nullptr;
	}
	Super::Destroyed();
}

void ACharacterBase::Ragdoll_Implementation()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Drop();
		
		EquippedWeapon = nullptr;
	}

	TArray<UPrimitiveComponent*> PrimitiveComponents;
	GetComponents<UPrimitiveComponent>(PrimitiveComponents, true);

	// Loop through the array and do something with each primitive component
	for (auto& Collider : PrimitiveComponents)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor component name: %s, class: %s"), *Collider->GetName(), *Collider->GetClass()->GetName());
		Collider->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		Collider->SetCollisionProfileName("Ragdoll");
		Collider->SetSimulatePhysics(true);
		Collider->SetEnableGravity(true);
		Collider->RecreatePhysicsState();

	}
}
