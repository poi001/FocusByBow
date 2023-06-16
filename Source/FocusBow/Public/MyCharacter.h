// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class FOCUSBOW_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// 캐릭터 초기화 이후
	virtual void PostInitializeComponents() override;

private:
	//카메라
	UPROPERTY(EditAnywhere, Category = "Camera")
		USpringArmComponent* SpringArm;				//CameraBoom
	UPROPERTY(EditAnywhere, Category = "Camera")
		UCameraComponent* Camera;					//Camera
	//애님 인스턴스
	UPROPERTY()
		class UMyAnimInstance* PlayerAnim;			//플레이어 애님 인스턴스

	//발사체
	UPROPERTY()
		TSubclassOf<class AActor> Arrow;			//화살 블루프린트 클래스

	//위젯
	UPROPERTY()
		TSubclassOf<class UUserWidget> CrosshairClass;	//조준점 HUD (Class)
	UPROPERTY()
		class UUserWidget* CrosshairWidget;				//조준점 HUD (Object)

	//공격
	bool CanFire = true;

	//이동
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	//점프
	void StartJump();
	void StopJump();
	//공격
	void Fire();
};
