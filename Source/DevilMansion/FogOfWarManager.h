// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FogOfWarManager.generated.h"


UCLASS()
class DEVILMANSION_API AFogOfWarManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFogOfWarManager(const FObjectInitializer& FOI);
	virtual ~AFogOfWarManager();

	//How far will an actor be able to see
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FogOfWar)
	TArray<float> SightRange;

	//The number of samples per 100 unreal units
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FogOfWar)
	float SamplesPerMeter = 2.0f;

	//If the last texture blending is done
	UPROPERTY(BlueprintReadWrite)
	bool bIsDoneBlending;

	//Should we blur? It takes up quite a lot of CPU time...
	UPROPERTY(EditAnywhere)
	bool bIsBlurEnabled = true;

	//The size of our textures
	uint32 TextureSize = 1024;

	//Array containing what parts of the map we've unveiled.
	UPROPERTY()
	TArray<bool> UnfoggedData;

	//Temp array for horizontal blur pass
	UPROPERTY()
	TArray<uint8> HorizontalBlurData;

	//Our texture data (result of vertical blur pass)
	UPROPERTY()
	TArray<struct FColor> TextureData;

	//Our texture data from the last frame
	UPROPERTY()
	TArray<FColor> LastFrameTextureData;

	//Check to see if we have a new FOW-texture.
	bool bHasFOWTextureUpdate = false;

	//Blur size
	uint8 blurKernelSize = 15;

	//Blur kernel
	UPROPERTY()
	TArray<float> blurKernel;

	//Store the actors that will be unveiling the FOW-texture.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FogOfWar)
	TMap<AActor*,uint8> FowActorsAndSightIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FogOfWar)
	TSubclassOf<AActor> FowActorClass;

	//DEBUG: Time it took to update the fow texture
	float fowUpdateTime = 0;

	//Getter for the working thread
	bool GetIsBlurEnabled();

private:
	void UpdateFowTexture();

	//Triggers the start of a new FOW-texture-update
	void StartFOWTextureUpdate();

	//Our dynamically updated texture
	UPROPERTY()
	class UTexture2D* FOWTexture;

	//Texture from last update. We blend between the two to do a smooth unveiling of newly discovered areas.
	UPROPERTY()
	UTexture2D* LastFOWTexture;

	//Texture regions	
	struct FUpdateTextureRegion2D* textureRegions;

	class FogOfWarWorker* FowThread;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnFowTextureUpdated(UTexture2D* currentTexture, UTexture2D* lastTexture);

	//Register an actor to influence the FOW-texture
	void RegisterFowActor(AActor* Actor,uint8 idx);

	void UpdateTextureRegions(
		UTexture2D* Texture,
		int32 MipIndex,
		uint32 NumRegions,
		struct FUpdateTextureRegion2D* Regions,
		uint32 SrcPitch,
		uint32 SrcBpp,
		uint8* SrcData,
		bool bFreeData);

};
