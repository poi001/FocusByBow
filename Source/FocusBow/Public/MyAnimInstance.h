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
	//몽타주
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
		class UAnimMontage* FireMontage;			//기본 공격 몽타주

	//폰
	UPROPERTY()
		class AMyCharacter* Pawn;

	//노티파이
	UFUNCTION()
		void AnimNotify_AttackSave();					//발사 노티파이
	UFUNCTION()
		void AnimNotify_ResetCombo();					//발사 노티파이

	//델리게이트
	FOnResetCombo OnResetCombo;		//공격 끝났을 때, 델리게이트

public:
	//스피드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
		FVector PawnVelocity;
	//점프
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
		bool IsInAir;
	//생사
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
		bool IsDead;
	//조준 ( 조준하면서 움직이는 애니메이션의 완성도를 위한 변수 )
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
		float YawOffset;			//캐릭터의 회전율에서 카메라의 회전율까지의 Yaw의 각도 ( 조준하면서 움직일 때 사용 )
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
		FRotator BaseAimRotation;	//카메라가 보고있는 회전율

	//몽타주 재생
	void PlayFireMontage();			//발사 몽타주 재생
};
