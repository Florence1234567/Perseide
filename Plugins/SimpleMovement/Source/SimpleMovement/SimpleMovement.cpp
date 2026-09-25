// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleMovement.h"

#define LOCTEXT_NAMESPACE "FSimpleMovementModule"

void FSimpleMovementModule::StartupModule()
{
	// This code will execute after your module is loaded into memory
}

void FSimpleMovementModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSimpleMovementModule, SimpleMovement)
