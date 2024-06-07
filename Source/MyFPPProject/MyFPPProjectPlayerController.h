// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyFPPProjectPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

/**
 *
 */
UCLASS()
class MYFPPPROJECT_API AMyFPPProjectPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	// Begin Actor interface
protected:

	virtual void BeginPlay() override;

	// End Actor interface

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = HUD_Assistant)
	TSubclassOf<UUserWidget> W_Hud;
};
