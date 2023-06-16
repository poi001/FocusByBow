// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UCLASS()
class FOCUSBOW_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Arrow", Meta = (AllowPrivateAccess = true))
		class USceneComponent* Root;			//씬 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = "Arrow", Meta = (AllowPrivateAccess = true))
		class UStaticMeshComponent* Arrow;		//스태틱 메시
	UPROPERTY(VisibleDefaultsOnly, Category = "Arrow", Meta = (AllowPrivateAccess = true))
		class UBoxComponent* Collision;			//박스 콜리전
	UPROPERTY(VisibleDefaultsOnly, Category = "Arrow", Meta = (AllowPrivateAccess = true))
		class UProjectileMovementComponent* ProjectileMovementComponent;	//투사체 무브먼트 컴포넌트
	UPROPERTY(EditAnywhere, Category = "Particle", Meta = (AllowPrivateAccess = true))
		class UParticleSystem* Particle;		//파티클

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
