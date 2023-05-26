// Fill out your copyright notice in the Description page of Project Settings.


#include "NobilityGameInstance.h"
#include "Engine/Engine.h"

UNobilityGameInstance::UNobilityGameInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("Gameinstance constructor"));
}

void UNobilityGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Gameinstance init"));
}

void UNobilityGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	

	UWorld* World = GetWorld();
	World->ServerTravel("/Game/Maps/ThirdPersonMap?listen");
	Engine->AddOnScreenDebugMessage(-1, 2.0, FColor::White, TEXT("Hosting new map"));
}

void UNobilityGameInstance::Join(FString IP)
{
	UEngine* Engine = GetEngine();
	Engine->AddOnScreenDebugMessage(-1, 2.0, FColor::White, FString::Printf(TEXT("Joining %s"), *IP));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	PlayerController->ClientTravel(IP, ETravelType::TRAVEL_Absolute);
}
