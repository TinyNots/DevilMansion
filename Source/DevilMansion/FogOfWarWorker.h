// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAL/Runnable.h"

/**
 * 
 */
class FogOfWarWorker: public FRunnable
{
	//Thread to run the FRunnable on
	class FRunnableThread* Thread;

	//Pointer to our manager
	class AFogOfWarManager* Manager;

	//Thread safe counter 
	class FThreadSafeCounter StopTaskCounter;
public:
	FogOfWarWorker();
	FogOfWarWorker(AFogOfWarManager* manager);
	virtual ~FogOfWarWorker();

	//FRunnable interface
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();

	//Method to perform work
	void UpdateFowTexture();

	bool bShouldUpdate = false;

	void ShutDown();
};
