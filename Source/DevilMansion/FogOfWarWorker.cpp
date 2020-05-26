// Fill out your copyright notice in the Description page of Project Settings.

#include "FogOfWarWorker.h"
#include "DevilMansion.h"
#include "FogOfWarManager.h"
#include "HAL/RunnableThread.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"


FogOfWarWorker::FogOfWarWorker()
{
}

FogOfWarWorker::FogOfWarWorker(AFogOfWarManager* manager)
{
	Manager = manager;
	Thread = FRunnableThread::Create(this, TEXT("FogOfWarWorker"), 0U, TPri_BelowNormal);
}

FogOfWarWorker::~FogOfWarWorker()
{
	delete Thread;
	Thread = NULL;
}

bool FogOfWarWorker::Init()
{
	if (Manager) 
	{
		Manager->GetWorld()->GetFirstPlayerController()->ClientMessage("Fog of War worker thread started");
		return true;
	}
	return false;
}

uint32 FogOfWarWorker::Run()
{
	FPlatformProcess::Sleep(0.03f);
	while (StopTaskCounter.GetValue() == 0) {
		float time = 0;
		if (Manager && Manager->GetWorld()) {
			time = Manager->GetWorld()->TimeSeconds;
		}
		if (!Manager->bHasFOWTextureUpdate) {
			UpdateFowTexture(Manager->UseSightIndex);
			if (Manager && Manager->GetWorld()) {
				Manager->fowUpdateTime = Manager->GetWorld()->TimeSince(time);
			}
		}
		FPlatformProcess::Sleep(0.1f);
	}
	return 0;
}

void FogOfWarWorker::Stop()
{
	StopTaskCounter.Increment();
}

void FogOfWarWorker::UpdateFowTexture(int SightIdx)
{
	Manager->LastFrameTextureData = TArray<FColor>(Manager->TextureData);
	uint32 halfTextureSize = Manager->TextureSize / 2;
	int signedSize = (int)Manager->TextureSize; 
	TSet<FVector2D> currentlyInSight;
	TSet<FVector2D> texelsToBlur;
	int sightTexels = Manager->SightRange[SightIdx] * Manager->SamplesPerMeter;
	float dividend = 100.0f / Manager->SamplesPerMeter;

	for (auto Itr(Manager->FowActors.CreateIterator()); Itr; Itr++) {
		//Find actor position
		if (!*Itr) return;
		FVector position = (*Itr)->GetActorLocation();

		//We divide by 100.0 because 1 texel equals 1 meter of visibility-data.
		int posX = (int)(position.X / dividend) + halfTextureSize;
		int posY = (int)(position.Y / dividend) + halfTextureSize;
		float integerX, integerY;

		FVector2D fractions = FVector2D(modff(position.X / 50.0f, &integerX), modff(position.Y / 50.0f, &integerY));
		FVector2D textureSpacePos = FVector2D(posX, posY);
		int size = (int)Manager->TextureSize;

		FCollisionQueryParams queryParams(FName(TEXT("FOW trace")), false, (*Itr));
		int halfKernelSize = (Manager->blurKernelSize - 1) / 2;

		//Store the positions we want to blur
		for (int y = posY - sightTexels - halfKernelSize; y <= posY + sightTexels + halfKernelSize; y++) {
			for (int x = posX - sightTexels - halfKernelSize; x <= posX + sightTexels + halfKernelSize; x++) {
				if (x > 0 && x < size && y > 0 && y < size) {
					texelsToBlur.Add(FIntPoint(x, y));
				}
			}
		}

		//Unveil the positions our actors are currently looking at
		for (int y = posY - sightTexels; y <= posY + sightTexels; y++) {
			for (int x = posX - sightTexels; x <= posX + sightTexels; x++) {
				//Kernel for radial sight
				if (x > 0 && x < size && y > 0 && y < size) {
					FVector2D currentTextureSpacePos = FVector2D(x, y);
					int length = (int)(textureSpacePos - currentTextureSpacePos).Size();
					if (length <= sightTexels) {
						FVector currentWorldSpacePos = FVector(
							((x - (int)halfTextureSize)) * dividend,
							((y - (int)halfTextureSize)) * dividend,
							position.Z);

						if (!Manager->GetWorld()->LineTraceTestByChannel(position, currentWorldSpacePos, ECC_WorldStatic, queryParams)) {
							//Unveil the positions we are currently seeing
							Manager->UnfoggedData[x + y * Manager->TextureSize] = true;
							//Store the positions we are currently seeing.
							currentlyInSight.Add(FVector2D(x, y));
						}
					}
				}
			}
		}
	}

	if (Manager->GetIsBlurEnabled()) {
		//Horizontal blur pass
		int offset = floorf(Manager->blurKernelSize / 2.0f);
		for (auto Itr(texelsToBlur.CreateIterator()); Itr; ++Itr) {
			int x = (Itr)->IntPoint().X;
			int y = (Itr)->IntPoint().Y;
			float sum = 0;
			for (int i = 0; i < Manager->blurKernelSize; i++) {
				int shiftedIndex = i - offset;
				if (x + shiftedIndex >= 0 && x + shiftedIndex <= signedSize - 1) {
					if (Manager->UnfoggedData[x + shiftedIndex + (y * signedSize)]) {
						//If we are currently looking at a position, unveil it completely
						if (currentlyInSight.Contains(FVector2D(x + shiftedIndex, y))) {
							sum += (Manager->blurKernel[i] * 255);
						}
						//If this is a previously discovered position that we're not currently looking at, put it into a "shroud of darkness".							
						else {
							sum += (Manager->blurKernel[i] * 100);
						}
					}
				}
			}
			Manager->HorizontalBlurData[x + y * signedSize] = (uint8)sum;
		}


		//Vertical blur pass
		for (auto Itr(texelsToBlur.CreateIterator()); Itr; ++Itr) {
			int x = (Itr)->IntPoint().X;
			int y = (Itr)->IntPoint().Y;
			float sum = 0;
			for (int i = 0; i < Manager->blurKernelSize; i++) {
				int shiftedIndex = i - offset;
				if (y + shiftedIndex >= 0 && y + shiftedIndex <= signedSize - 1) {
					sum += (Manager->blurKernel[i] * Manager->HorizontalBlurData[x + (y + shiftedIndex) * signedSize]);
				}
			}
			Manager->TextureData[x + y * signedSize] = FColor((uint8)sum, (uint8)sum, (uint8)sum, 255);
		}
	}
	else {
		for (int y = 0; y < signedSize; y++) {
			for (int x = 0; x < signedSize; x++) {

				if (Manager->UnfoggedData[x + (y * signedSize)]) {
					if (currentlyInSight.Contains(FVector2D(x, y))) {
						Manager->TextureData[x + y * signedSize] = FColor((uint8)255, (uint8)255, (uint8)255, 255);
					}
					else {
						Manager->TextureData[x + y * signedSize] = FColor((uint8)100, (uint8)100, (uint8)100, 255);
					}
				}
			}
		}
	}
	Manager->bHasFOWTextureUpdate = true;
}

void FogOfWarWorker::ShutDown()
{
	Stop();
	Thread->WaitForCompletion();
}
