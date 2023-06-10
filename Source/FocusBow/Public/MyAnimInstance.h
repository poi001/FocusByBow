// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnResetCombo)

/**
 * 
 */
UCLASS()
class FOCUSBOW_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMyAnimInstance();

	//Tick
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
private:
	//��Ÿ��
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
		class UAnimMontage* FireMontage;			//�⺻ ���� ��Ÿ��

	//��
	UPROPERTY()
		class AMyCharacter* Pawn;

	//��Ƽ����
	UFUNCTION()
		void AnimNotify_AttackSave();					//�߻� ��Ƽ����
	UFUNCTION()
		void AnimNotify_ResetCombo();					//�߻� ��Ƽ����

	//��������Ʈ
	FOnResetCombo OnResetCombo;		//���� ������ ��, ��������Ʈ

public:
	//���ǵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
		FVector PawnVelocity;
	//����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
		bool IsInAir;
	//����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
		bool IsDead;
	//���� ( �����ϸ鼭 �����̴� �ִϸ��̼��� �ϼ����� ���� ���� )
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
		float YawOffset;			//ĳ������ ȸ�������� ī�޶��� ȸ���������� Yaw�� ���� ( �����ϸ鼭 ������ �� ��� )
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
		FRotator BaseAimRotation;	//ī�޶� �����ִ� ȸ����

	//��Ÿ�� ���
	void PlayFireMontage();			//�߻� ��Ÿ�� ���
};
