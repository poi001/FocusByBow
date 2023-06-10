// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FocusBowGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FOCUSBOW_API AFocusBowGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFocusBowGameModeBase();

	virtual void PostLogin(APlayerController* NewController) override;
	
};
