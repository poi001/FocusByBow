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
	// ĳ���� �ʱ�ȭ ����
	virtual void PostInitializeComponents() override;

private:
	//ī�޶�
	UPROPERTY(EditAnywhere, Category = "Camera")
		USpringArmComponent* SpringArm;				//CameraBoom
	UPROPERTY(EditAnywhere, Category = "Camera")
		UCameraComponent* Camera;					//Camera
	//�ִ� �ν��Ͻ�
	UPROPERTY()
		class UMyAnimInstance* PlayerAnim;			//�÷��̾� �ִ� �ν��Ͻ�

	//�߻�ü
	UPROPERTY()
		TSubclassOf<class AActor> Arrow;			//ȭ�� �������Ʈ Ŭ����

	//����
	UPROPERTY()
		TSubclassOf<class UUserWidget> CrosshairClass;	//������ HUD (Class)
	UPROPERTY()
		class UUserWidget* CrosshairWidget;				//������ HUD (Object)

	//����
	bool CanFire = true;

	//�̵�
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	//����
	void StartJump();
	void StopJump();
	//����
	void Fire();
};
