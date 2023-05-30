// Fill out your copyright notice in the Description page of Project Settings.


#include "NobilityGameInstance.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

UNobilityGameInstance::UNobilityGameInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("Gameinstance constructor"));

	ConstructorHelpers::FClassFinder<UUserWidget> TempMenuClass(TEXT("/Game/UI/MainMenuWBP"));
	if (TempMenuClass.Class != nullptr)	MainMenuClass = TempMenuClass.Class;
}

void UNobilityGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Gameinstance init, menuclass is %s"), *MainMenuClass->GetName());
}

void UNobilityGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	

	UWorld* World = GetWorld();
	World->ServerTravel("/Game/Maps/ThirdPersonMap?listen");
	Engine->AddOnScreenDebugMessage(-1, 2.0, FColor::White, TEXT("Hosting new map"));
}

void UNobilityGameInstance::MainMenu()
{
	if (!MainMenuClass) return;
	UUserWidget* Menu = CreateWidget<UUserWidget>(this, MainMenuClass);

	Menu->AddToViewport();
}

void UNobilityGameInstance::Join(FString IP)
{
	UEngine* Engine = GetEngine();
	Engine->AddOnScreenDebugMessage(-1, 2.0, FColor::White, FString::Printf(TEXT("Joining %s"), *IP));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	PlayerController->ClientTravel(IP, ETravelType::TRAVEL_Absolute);
}
