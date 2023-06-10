// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyCharacter.h"

UMyAnimInstance::UMyAnimInstance()
{
	//공격 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage>FIRE_MONTAGE
	(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Animations/Primary_Fire_Med_Montage.Primary_Fire_Med_Montage"));
	if (FIRE_MONTAGE.Succeeded())FireMontage = FIRE_MONTAGE.Object;

	//프로퍼티 초기화
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;
	YawOffset = 0.0f;
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//Pawn에 폰의 권한을 줌
	Pawn = Cast<AMyCharacter>(TryGetPawnOwner());
	if (!::IsValid(Pawn))
	{
		UE_LOG(LogTemp, Error, TEXT("Character is nullptr"));
		return;
	}

	//캐릭터가 죽지 않았을 때는 캐릭터의 스피드 변수, 점프여부 변수에 값을 기입한다.
	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();				//스피드
		PawnVelocity = Pawn->GetVelocity();							//폰의 속도(Vector)
		if (Pawn)
		{
			IsInAir = Pawn->GetMovementComponent()->IsFalling();	//점프

			BaseAimRotation = Pawn->GetBaseAimRotation();			//시선
		}
	}
}

//노티파이
void UMyAnimInstance::AnimNotify_AttackSave()
{

}
void UMyAnimInstance::AnimNotify_ResetCombo()
{
	OnResetCombo.Broadcast();
}

//몽타주 재생
void UMyAnimInstance::PlayFireMontage()
{
	Montage_Play(FireMontage, 1.0f);
}