# FocusByBow
활의 조준점에 맞게 화살이 박히도록 조정하는 언리얼 C++ 프로젝트입니다.

## Preview
![Focus_Before](https://github.com/poi001/FocusByBow/assets/107660181/c60d16dc-5177-473a-a8cc-08d31a77de3f)

조정 전

![ezgif com-video-to-gif](https://github.com/poi001/FocusByBow/assets/107660181/907e6a94-3291-4c94-9db4-c8d652ec4c97)

조정 후
## 기능 구현
### 이 기능을 구현한 이유

플레이어의 조준점의 목적지에 화살이 도착해야하기 때문이다. 이 기능을 사용하지 않으면 아래와 같이 화살이 원하는 곳에 도달하지 않는다. 이유는 캐릭터가 화면 정중앙보다 왼쪽에 위치하기 때문이다.

![화면 캡처 2023-06-21 090152](https://github.com/poi001/FocusByBow/assets/107660181/a5bb0230-0323-483c-9027-6d0c3b41717a)

### 코드

```
void AMyCharacter::Fire()
{
	if (CanFire != true) return;	    //공격할 수 없는 상황이면 이 함수가 발동되지 않는다.
	CanFire = false;				          //공격할 수 없는 상태로 변경

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
```
### 설명
* `FVector CrosshairWorldLocation` : 플레이어 카메라의 위치.
* `FVector CrosshairForwardVector` : 플레이어 카메라의 ForwardVector.
* `FVector ImpactPoint` : 조준점(HUD) 기준으로 화살이 도착할 위치. 플레이어 카메라의 ForwardVector에 15000.0f라는 임의의 큰 수를 곱했다.
* `FHitResult HitResult` : LineTrace에 맞은 Object의 정보를 담을 변수.
* `FCollisionQueryParams IgnoreOwner` : LineTrace에 제외할 Collision을 담을 변수. 위에서는 플레이어 캐릭터로 설정함.
* `bIsHitByTrace` : LineTrace에 맞았는 지 확인하는 bool변수.
* `LineTraceSingleByChannel` : 설정한 위치에서 레이저를 쏘아 설정한 Collision채널이 맞았는지 확인하고 맞았다면 그 Object의 정보를 알아내는 RayCast. 인자는 순서대로 `HitResult변수`, `시작위치`, `끝날위치`, `판정할 Collision`, `제외할 Collision`이다
* `if (bIsHitByTrace) ImpactPoint = HitResult.ImpactPoint` : LineTrace에 맞았다면 `ImpactPoint`를 레이저에 맞은 위치로 설정한다.
* `FVector ArrowSpawnLocation` : 화살이 나가는 위치. 위에서는 활의 위치로 설정했다.
* `FRotator ArrowSpawnRotator` : 화살이 소환될 때, 설정할 회전량. 위에서는 `ImpactPoint`벡터에서 `ArrowSpawnLocation` 벡터만큼 뺀 벡터의 회전량을 넣었다.

위 코드를 작성하면 아래와 그림같이 조정이 된다.

![화면 캡처 2023-06-21 090358](https://github.com/poi001/FocusByBow/assets/107660181/d2506139-c949-4280-9a9a-b7d68e8cd3ba)
