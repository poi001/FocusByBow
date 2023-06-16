// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyAnimInstance.h"
#include "Arrow.h"
#include "Blueprint/UserWidget.h"

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
	static ConstructorHelpers::FClassFinder<UAnimInstance> PLAYER_ANIM(TEXT("/Game/Player/Anim/Archer_AnimBP.Archer_AnimBP_C"));
	if (PLAYER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(PLAYER_ANIM.Class);
	}

	//ȭ��
	static ConstructorHelpers::FClassFinder<AActor> ARROW(TEXT("/Game/Player/Blueprint/Arrow_BP.Arrow_BP_C"));
	if (ARROW.Succeeded()) Arrow = ARROW.Class;

	//������ HUD
	static ConstructorHelpers::FClassFinder<UUserWidget> HUD(TEXT("/Game/Text/Crosshair_BP.Crosshair_BP_C"));
	if (HUD.Succeeded()) CrosshairClass = HUD.Class;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairClass);
	CrosshairWidget->AddToViewport();
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

	//�̵�
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	//�þ�
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);

	//����
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::StartJump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &AMyCharacter::StopJump);
	//�߻�
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AMyCharacter::Fire);
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//�ִ� �ν��Ͻ��� PlayerAnim������ ���� �ִ´�
	PlayerAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (PlayerAnim == nullptr) UE_LOG(LogTemp, Error, TEXT("PlayerAnimInstance is nullptr"));	//nullüũ

	//���� ��Ÿ�ְ� ������ ȣ��Ǵ� ��������Ʈ
	PlayerAnim->OnResetCombo.AddLambda([this]()->void { CanFire = true; });
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
void AMyCharacter::Fire()
{
	if (CanFire != true) return;	//������ �� ���� ��Ȳ�̸� �� �Լ��� �ߵ����� �ʴ´�.
	CanFire = false;				//������ �� ���� ���·� ����

	//����ü �߻� ��ġ ������ �̸�
	FName SocktName(TEXT("BowEmitterSocket"));
	//Trace Start Location
	FVector CrosshairWorldLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	FVector CrosshairForwardVector = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetActorForwardVector();
	//Trace End Location
	FVector ImpactPoint = CrosshairWorldLocation + (CrosshairForwardVector * FVector(15000.0f, 15000.0f, 15000.0f));

	FHitResult HitResult;
	FCollisionQueryParams IgnoreOwner = FCollisionQueryParams::DefaultQueryParam;
	IgnoreOwner.AddIgnoredActor(GetOwner());
	//LineTrace
	bool bIsHitByTrace = GetWorld()->LineTraceSingleByChannel(HitResult, CrosshairWorldLocation, ImpactPoint, 
		ECollisionChannel::ECC_Visibility, IgnoreOwner);
	if (bIsHitByTrace) ImpactPoint = HitResult.ImpactPoint;
	FVector ArrowSpawnLocation = GetMesh()->GetSocketLocation(SocktName);
	FRotator ArrowSpawnRotator = FRotationMatrix::MakeFromX(ImpactPoint - ArrowSpawnLocation).Rotator();

	//��Ÿ�� ���
	PlayerAnim->PlayFireMontage();

	//ȭ�� ��ȯ
	GetWorld()->SpawnActor<AActor>(Arrow, ArrowSpawnLocation, ArrowSpawnRotator);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, TEXT("Fire"));
}