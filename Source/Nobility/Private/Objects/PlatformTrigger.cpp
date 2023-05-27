// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/PlatformTrigger.h"
#include "MovingPlatform.h"
#include "Components/BoxComponent.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
	PrimaryActorTick.bCanEverTick = true;


	Base = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Base"));
	RootComponent = Base;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(GetRootComponent());
	TriggerBox->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnComponentBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnComponentEndOverlap);


}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("button beginplay "));
	if (AssignedPlatform)
	{
		AssignedPlatform->AutoProcess = false; //Just in case we boot before the platform does

	}
	SwitchOff();
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatformTrigger::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("pressed 1 "));
	if (OtherActor->IsA<APawn>())
	{
		APawn* Pawn = Cast<APawn>(OtherActor);
		if (Pawn->IsPlayerControlled()) SwitchOn();
	}
}

void APlatformTrigger::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("release 1 "));
	SwitchOff();
}

void APlatformTrigger::SwitchOn_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("pressed 2 "));
	Base->PlayAnimation(PressAnimation, false);
	if (AssignedPlatform)
	{
		AssignedPlatform->SetActorTickEnabled(true);
	}
	SwitchedOn();
}

void APlatformTrigger::SwitchOff_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("release 2 "));
	Base->PlayAnimation(ReleaseAnimation, false);
	if (AssignedPlatform)
	{
		AssignedPlatform->SetActorTickEnabled(false);
	}
	SwitchedOff();
}

void APlatformTrigger::SwitchedOn_Implementation()
{
}

void APlatformTrigger::SwitchedOff_Implementation()
{
}

