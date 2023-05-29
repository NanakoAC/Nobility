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
	UE_LOG(LogTemp, Warning, TEXT("Mainmenu 1"));
	if (!MainMenuClass) return;
	UE_LOG(LogTemp, Warning, TEXT("Mainmenu 2"));
	UUserWidget* Menu = CreateWidget<UUserWidget>(this, MainMenuClass);
	UE_LOG(LogTemp, Warning, TEXT("Mainmenu 3 %s"), *Menu->GetName());

	Menu->AddToViewport();
	UE_LOG(LogTemp, Warning, TEXT("Mainmenu 4"));
}

void UNobilityGameInstance::Join(FString IP)
{
	UEngine* Engine = GetEngine();
	Engine->AddOnScreenDebugMessage(-1, 2.0, FColor::White, FString::Printf(TEXT("Joining %s"), *IP));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	PlayerController->ClientTravel(IP, ETravelType::TRAVEL_Absolute);
}
