// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "NobilityGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NOBILITY_API UNobilityGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	UNobilityGameInstance();
	
	virtual void Init() override;

	UFUNCTION(Exec)
	void Host();

	UFUNCTION(Exec)
	void MainMenu();

	UFUNCTION(Exec)
	void Join(FString IP);
	
	TSubclassOf<class UUserWidget> MainMenuClass;


};
