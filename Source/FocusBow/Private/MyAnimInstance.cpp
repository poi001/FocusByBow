// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyCharacter.h"

UMyAnimInstance::UMyAnimInstance()
{
	//���� ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage>FIRE_MONTAGE
	(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Animations/Primary_Fire_Med_Montage.Primary_Fire_Med_Montage"));
	if (FIRE_MONTAGE.Succeeded())FireMontage = FIRE_MONTAGE.Object;

	//������Ƽ �ʱ�ȭ
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;
	YawOffset = 0.0f;
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//Pawn�� ���� ������ ��
	Pawn = Cast<AMyCharacter>(TryGetPawnOwner());
	if (!::IsValid(Pawn))
	{
		UE_LOG(LogTemp, Error, TEXT("Character is nullptr"));
		return;
	}

	//ĳ���Ͱ� ���� �ʾ��� ���� ĳ������ ���ǵ� ����, �������� ������ ���� �����Ѵ�.
	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();				//���ǵ�
		PawnVelocity = Pawn->GetVelocity();							//���� �ӵ�(Vector)
		if (Pawn)
		{
			IsInAir = Pawn->GetMovementComponent()->IsFalling();	//����

			BaseAimRotation = Pawn->GetBaseAimRotation();			//�ü�
		}
	}
}

//��Ƽ����
void UMyAnimInstance::AnimNotify_AttackSave()
{

}
void UMyAnimInstance::AnimNotify_ResetCombo()
{
	OnResetCombo.Broadcast();
}

//��Ÿ�� ���
void UMyAnimInstance::PlayFireMontage()
{
	Montage_Play(FireMontage, 1.0f);
}