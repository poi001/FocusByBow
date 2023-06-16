// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "Components/BoxComponent.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//컴포넌트 생성
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("SCENE"));
	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ARROW"));
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("COLLISION"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILEMOVEMENTCOMPONENT"));

	//컴포넌트 부착
	Arrow->SetupAttachment(Root);
	Collision->SetupAttachment(Arrow);

	//Arrow 설정
	Arrow->SetCollisionProfileName(TEXT("NoCollision"));

	//Collision 설정
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetBoxExtent(FVector(32.0f, 32.0f, 32.0f));
	Collision->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	Collision->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));

	//ProjectileMovementComponent 설정
	ProjectileMovementComponent->InitialSpeed = 6500.0f;
	ProjectileMovementComponent->MaxSpeed = 6500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;

	//Overlap Begin 델리게이트
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnOverlapBegin);


}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArrow::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		ProjectileMovementComponent->StopMovementImmediately();
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
		this->AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
		Collision->SetCollisionProfileName(TEXT("NoCollision"));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, Collision->GetComponentTransform());
	}
}