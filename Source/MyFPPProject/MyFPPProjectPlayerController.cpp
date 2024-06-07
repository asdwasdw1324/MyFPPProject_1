// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyFPPProjectPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint\UserWidget.h"

DEFINE_LOG_CATEGORY_STATIC(PlayerControllerLog, All, All)

void AMyFPPProjectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);

		UE_LOG(PlayerControllerLog, Warning, TEXT("BeginPlay!\nBind base InputMappingContext!"));
	}

	//Put CreateWidget in the PlayerController class
	if (W_Hud)
	{
		UUserWidget* Hud = CreateWidget<UUserWidget>(this, W_Hud, FName(TEXT("FPS_HUD")));

		Hud->AddToViewport(9);
	}

}