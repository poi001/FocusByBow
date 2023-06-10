// Copyright Epic Games, Inc. All Rights Reserved.


#include "FocusBowGameModeBase.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"

AFocusBowGameModeBase::AFocusBowGameModeBase()
{
	DefaultPawnClass = AMyCharacter::StaticClass();					//�÷��̾�(��)
	PlayerControllerClass = AMyPlayerController::StaticClass();		//�÷��̾� ��Ʈ�ѷ�
}

void AFocusBowGameModeBase::PostLogin(APlayerController* NewController)
{
	Super::PostLogin(NewController);


}