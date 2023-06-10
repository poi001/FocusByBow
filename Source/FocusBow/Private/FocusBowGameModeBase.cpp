// Copyright Epic Games, Inc. All Rights Reserved.


#include "FocusBowGameModeBase.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"

AFocusBowGameModeBase::AFocusBowGameModeBase()
{
	DefaultPawnClass = AMyCharacter::StaticClass();					//플레이어(폰)
	PlayerControllerClass = AMyPlayerController::StaticClass();		//플레이어 컨트롤러
}

void AFocusBowGameModeBase::PostLogin(APlayerController* NewController)
{
	Super::PostLogin(NewController);


}