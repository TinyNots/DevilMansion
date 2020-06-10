// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEVILMANSION_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
    static UMyGameInstance* GetInstance();

	// Returns the current playing state 
	UFUNCTION(BlueprintPure)
	enum EGamePlayState GetCurrentState() const;

	// Sets a new playing state 
	UFUNCTION(BlueprintCallable)
		void SetCurrentState(EGamePlayState NewState);

	UFUNCTION(BlueprintCallable)
		void Save();
	UFUNCTION(BlueprintCallable)
		void Load();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsSave = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsLoad = false;

private:
    //Keeps track of the current playing state 
   EGamePlayState CurrentState;


	//Handle any function calls that rely upon changing the playing state of our game 
	void HandleNewState(EGamePlayState NewState);
};
