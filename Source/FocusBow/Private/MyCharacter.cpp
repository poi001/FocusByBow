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

	//컴포넌트 생성
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));	//CameraBoom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));			//Camera

	//컴포넌트 부착
	SpringArm->SetupAttachment(GetCapsuleComponent());	//CameraBoom
	Camera->SetupAttachment(SpringArm);					//Camera

	//플레이어 컨트롤러 회전 설정 ( 컨트롤러에 맞춰서 캐릭터가 회전하는 설정 )
	bUseControllerRotationPitch = false;										// 컨트롤러의 Pitch값 사용 여부
	bUseControllerRotationYaw = true;											// 컨트롤러의 Yaw값 사용 여부
	bUseControllerRotationRoll = false;											// 컨트롤러의 Roll값 사용 여부

	//카메라 설정 ( SpringArm )
	SpringArm->TargetArmLength = 750;											// 스프링 암의 길이
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));				// 스프링 암의 회전율을 조정한다
	SpringArm->SocketOffset = FVector(0.0f, 120.0f, 75.0f);						// 스프링 암의 끝 부분을 조정한다
	SpringArm->bDoCollisionTest = true;											// 카메라가 벽에 충돌하는지에 대한 여부
	SpringArm->bUsePawnControlRotation = true;									// 스프링암이 폰의 로테이션 값을 사용할 것인지 여부
	SpringArm->bInheritPitch = true;											// 스프링 암이 회전율을 상속받는다. ( 카메라 )
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;

	//캐릭터 무브먼트 설정
	GetCharacterMovement()->bOrientRotationToMovement = false;					// 가고 있는 방향 쪽으로 캐릭터가 쳐다 봄
	GetCharacterMovement()->JumpZVelocity = 600.0f;								// 점프 높이
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;								// 이동 속도

	//기본 스켈레탈 메쉬 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MANNEQUIN(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Sparrow.Sparrow"));
	if (MANNEQUIN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MANNEQUIN.Object);
	}
	//스켈레탈 메쉬 위치 및 회전 조절
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	//애니메이션 모드를 애니메이션 블루프린트로 설정
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//기본 애님 인스턴스 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> PLAYER_ANIM(TEXT("/Game/Player/Anim/Archer_AnimBP.Archer_AnimBP_C"));
	if (PLAYER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(PLAYER_ANIM.Class);
	}

	//화살
	static ConstructorHelpers::FClassFinder<AActor> ARROW(TEXT("/Game/Player/Blueprint/Arrow_BP.Arrow_BP_C"));
	if (ARROW.Succeeded()) Arrow = ARROW.Class;

	//조준점 HUD
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

	//이동
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	//시야
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);

	//점프
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::StartJump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &AMyCharacter::StopJump);
	//발사
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AMyCharacter::Fire);
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//애님 인스턴스를 PlayerAnim변수에 집어 넣는다
	PlayerAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (PlayerAnim == nullptr) UE_LOG(LogTemp, Error, TEXT("PlayerAnimInstance is nullptr"));	//null체크

	//공격 몽타주가 끝나면 호출되는 델리게이트
	PlayerAnim->OnResetCombo.AddLambda([this]()->void { CanFire = true; });
}

//이동
void AMyCharacter::MoveForward(float NewAxisValue)
{
	//컨트롤러의 Yaw만큼을 회전 시킨 회전행렬을 기준 삼은 X벡터로 이동
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
}
void AMyCharacter::MoveRight(float NewAxisValue)
{
	//컨트롤러의 Yaw만큼을 회전 시킨 회전행렬을 기준 삼은 Y벡터로 이동
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

//점프
void AMyCharacter::StartJump()
{
	bPressedJump = true;
}
void AMyCharacter::StopJump()
{
	bPressedJump = false;
}

//공격
void AMyCharacter::Fire()
{
	if (CanFire != true) return;	//공격할 수 없는 상황이면 이 함수가 발동되지 않는다.
	CanFire = false;				//공격할 수 없는 상태로 변경

	//투사체 발사 위치 소켓의 이름
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

	//몽타주 재생
	PlayerAnim->PlayFireMontage();

	//화살 소환
	GetWorld()->SpawnActor<AActor>(Arrow, ArrowSpawnLocation, ArrowSpawnRotator);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, TEXT("Fire"));
}