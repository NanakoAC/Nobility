// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/PlatformTrigger.h"
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
	
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatformTrigger::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SwitchOn();
}

void APlatformTrigger::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SwitchOff();
}

void APlatformTrigger::SwitchOn_Implementation()
{
	Base->PlayAnimation(PressAnimation, false);
}

void APlatformTrigger::SwitchOff_Implementation()
{
	Base->PlayAnimation(ReleaseAnimation, false);
}

