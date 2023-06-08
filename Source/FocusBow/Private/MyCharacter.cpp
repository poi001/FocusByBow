// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//������Ʈ ����
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));	//CameraBoom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));			//Camera

	//������Ʈ ����
	SpringArm->SetupAttachment(GetCapsuleComponent());	//CameraBoom
	Camera->SetupAttachment(SpringArm);					//Camera

	//�÷��̾� ��Ʈ�ѷ� ȸ�� ���� ( ��Ʈ�ѷ��� ���缭 ĳ���Ͱ� ȸ���ϴ� ���� )
	bUseControllerRotationPitch = false;										// ��Ʈ�ѷ��� Pitch�� ��� ����
	bUseControllerRotationYaw = true;											// ��Ʈ�ѷ��� Yaw�� ��� ����
	bUseControllerRotationRoll = false;											// ��Ʈ�ѷ��� Roll�� ��� ����

	//ī�޶� ���� ( SpringArm )
	SpringArm->TargetArmLength = 750;											// ������ ���� ����
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));				// ������ ���� ȸ������ �����Ѵ�
	SpringArm->SocketOffset = FVector(0.0f, 120.0f, 75.0f);						// ������ ���� �� �κ��� �����Ѵ�
	SpringArm->bDoCollisionTest = true;											// ī�޶� ���� �浹�ϴ����� ���� ����
	SpringArm->bUsePawnControlRotation = true;									// ���������� ���� �����̼� ���� ����� ������ ����
	SpringArm->bInheritPitch = true;											// ������ ���� ȸ������ ��ӹ޴´�. ( ī�޶� )
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;

	//ĳ���� �����Ʈ ����
	GetCharacterMovement()->bOrientRotationToMovement = false;					// ���� �ִ� ���� ������ ĳ���Ͱ� �Ĵ� ��
	GetCharacterMovement()->JumpZVelocity = 600.0f;								// ���� ����
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;								// �̵� �ӵ�

	//�⺻ ���̷�Ż �޽� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MANNEQUIN(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Sparrow.Sparrow"));
	if (MANNEQUIN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MANNEQUIN.Object);
	}
	//���̷�Ż �޽� ��ġ �� ȸ�� ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	//�ִϸ��̼� ��带 �ִϸ��̼� �������Ʈ�� ����
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//�⺻ �ִ� �ν��Ͻ� ����

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//�̵�
void AMyCharacter::MoveForward(float NewAxisValue)
{
	//��Ʈ�ѷ��� Yaw��ŭ�� ȸ�� ��Ų ȸ������� ���� ���� X���ͷ� �̵�
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
}
void AMyCharacter::MoveRight(float NewAxisValue)
{
	//��Ʈ�ѷ��� Yaw��ŭ�� ȸ�� ��Ų ȸ������� ���� ���� Y���ͷ� �̵�
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);
}
void AMyCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}
void AMyCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

//����
void AMyCharacter::StartJump()
{
	bPressedJump = true;
}
void AMyCharacter::StopJump()
{
	bPressedJump = false;
}

//����
void Fire()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, TEXT("Fire"));
}