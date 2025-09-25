// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"

AMyPlayerController::AMyPlayerController()
	: inputmappingcontext(nullptr),
		IAmove(nullptr),
		IAjump(nullptr),
		IAlook(nullptr),
		IAsprint(nullptr)
{
}

void AMyPlayerController::BeginPlay() {
	Super::BeginPlay();

	if (ULocalPlayer* localplayer = GetLocalPlayer()) {
		if (UEnhancedInputLocalPlayerSubsystem* subsystem =
			localplayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
			if (inputmappingcontext) {
				subsystem->AddMappingContext(inputmappingcontext, 0);
			}
		}
	}
}
