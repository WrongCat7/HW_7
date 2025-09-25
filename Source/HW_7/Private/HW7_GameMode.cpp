// Fill out your copyright notice in the Description page of Project Settings.


#include "HW7_GameMode.h"
#include "MyPawn.h"
#include "MyPlayerController.h"

AHW7_GameMode::AHW7_GameMode() {
	DefaultPawnClass = AMyPawn::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
}