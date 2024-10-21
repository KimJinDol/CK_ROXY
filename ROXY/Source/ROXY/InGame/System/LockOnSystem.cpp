// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnSystem.h"
#include "InGame/System/FightZoneFinder.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
ULockOnSystem::ULockOnSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void ULockOnSystem::BeginPlay()
{
	Super::BeginPlay();

	_ownerCharacter = Cast<ARoxyCharacter>(GetOwner());
	_ownerController = Cast<APlayerController>(_ownerCharacter->GetController());
	_ownerCamera = _ownerCharacter->GetFollowCamera();
	_ownerSpringArm = _ownerCharacter->GetCameraBoom();
}

// 현재 플레이어가 속해있는 fight zone 정보를 set
void ULockOnSystem::SetCurrentFightZone(AFightZoneFinder* fightZone)
{
	_currentFightZone = fightZone;
}

// 플레이어가 fight zone에 입장하면 이미 배치되어 있는 적들 배열을 _enemyArray 에 할당
void ULockOnSystem::SetEnemysInformation(TArray<AEnemyAI*> enemys)
{
	_enemysArray = enemys;
}

void ULockOnSystem::AddEnemyInformation(AEnemyAI* enemy)
{
	if (!_enemysArray.Contains(enemy))
		_enemysArray.Push(enemy);
}

void ULockOnSystem::RemoveEnemyInformation(AEnemyAI* enemy)
{
	if (_enemysArray.Contains(enemy)) {
		UnActiveLockOnMode();
	}
}

TArray<AEnemyAI*> ULockOnSystem::GetRenderedEnemys(UWorld* contextWorld)
{
	TArray<AActor*> actorEnemys;
	TArray<AEnemyAI*> enemys;

	// 월드에 존재하는 적들을 배열에 저장한다
	UGameplayStatics::GetAllActorsOfClass(contextWorld, AEnemyAI::StaticClass(), actorEnemys);


	// 포문을 돌려 지정한 시간내에 렌더된 기록이 있는 적들만 남겨놓는다
	for (int i = 0; i < actorEnemys.Num(); i++)
	{
		if (!actorEnemys[i]->WasRecentlyRendered(0.05f))
		{
			/*LOG_SCREEN("%s", *(Enemys[i]->GetName()));*/
			actorEnemys.RemoveAt(i);
			i--;
		}
		else
			enemys.Push(Cast<AEnemyAI>(actorEnemys[i]));
	}
	return enemys;
}

void ULockOnSystem::ArrangeEnemysArray()
{
	if (!_ownerCharacter) LOG_SCREEN("Not Exist Player Character");
	else if(!_currentFightZone) LOG_SCREEN("Not Exist Player Character In Fight Zone!");

	_enemysArray = _currentFightZone->GetEnemys();

	_enemysArrayMaxNum = _enemysArray.Num();

	if (_enemysArrayMaxNum > 0) {

		TArray<float> distanceArray; // 현재 enemys 배열에 있는 적들과 플레이어의 거리를 담을 배열
		TArray<AEnemyAI*> enemysTempArr; // 거리 순으로 재배열 될 적 순서 배열

		// 각 적들과 플레이어의 거리로 재 정렬
		for (int i = 0; i < _enemysArrayMaxNum; i++) {
			distanceArray.Push(FVector::Distance(_ownerCharacter->GetActorLocation(), _enemysArray[i]->GetActorLocation()));
		}

		// 배열 값 중에서 가장 거리가 가까운 인덱스 값으로 해당 적 반환하기
		TArray<float> distanceTempArr = distanceArray;

		distanceArray.Sort(); // 거리값이 작은 순으로 배열을 정리

		for (int i = 0; i < distanceArray.Num(); i++)
		{
			enemysTempArr.Add(_enemysArray[distanceTempArr.Find(distanceArray[i])]);
			LOG_SCREEN("%s", *(enemysTempArr[i]->GetName()));
		}

		_enemysArray = enemysTempArr;
	}
}

AEnemyAI* ULockOnSystem::InputLockOnSystem()
{
	if (_currentFightZone == nullptr) return NULL;

	SetAutoTargeting(0.8f, 10.f);
	// 현재 락온이 활성화 되어있지 않을 때
	if (!_bOnLockOnMode)
	{
		ArrangeEnemysArray();

		if (_enemysArrayMaxNum == 0) return NULL;
		else {
			_targetIndex = 0;
			_currentTarget = _enemysArray[_targetIndex];

			ActiveLockOnMode();
			LOG_SCREEN("Lock On Target : %s", *(_currentTarget->GetName()));

			return _currentTarget;
		}
	}

	// 락온 상태에서 재입력 시
	else {
		// 현재 락온 중인 enemy의 락온 모드를 끈다
		_currentTarget->SetLockOnWidgetVisibility(false);

		if (_enemysArrayMaxNum == 0) return NULL;
		else
		{
			// 락온 싸이클을 다 돌았을 때 실행. 적의 정보와 우선순위 재 측정
			if (_targetIndex >= _enemysArrayMaxNum - 1)
			{
				ArrangeEnemysArray();

				_targetIndex = 0;
				_newTarget = _enemysArray[_targetIndex];

				// 새로운 락온 싸이클을 돌 때, 새로운 타겟과 현재 타겟이 일치할 때 그냥 다음 타겟으로 넘어간다
				if (_newTarget == _currentTarget && _enemysArrayMaxNum > 1) {
					_targetIndex++;
				}

				_currentTarget = _enemysArray[_targetIndex];

				ActiveLockOnMode();
				LOG_SCREEN("Lock On Target : %s", *(_currentTarget->GetName()));

				return _currentTarget;
			}

			// 락온 싸이클을다 돌지 않았을 때
			else
			{
				_targetIndex++;

				if (_enemysArray[_targetIndex] == nullptr) {
					AEnemyAI* enemy = InputLockOnSystem();
					return enemy;
				}
				
				_currentTarget = _enemysArray[_targetIndex];

				ActiveLockOnMode();
				LOG_SCREEN("Lock On Target : %s", *(_currentTarget->GetName()));

				return _currentTarget;
			}
		}
	}
}

void ULockOnSystem::TurnOffAutoTargeting()
{
	_bOnAutoTargeting = false;
}

void ULockOnSystem::ActiveLockOnMode()
{
	_bOnLockOnMode = true;
	/*_bOnLookAtTarget = true;*/
	_bOnAutoTargeting = true;

	GetWorld()->GetTimerManager().SetTimer(_targetingTimer, this, &ULockOnSystem::TurnOffAutoTargeting, _targetingDuration);

	LOG_SCREEN("Lock On Mode : %d", _bOnLockOnMode);

	_currentTarget->SetLockOnWidgetVisibility(true);
}

void ULockOnSystem::UnActiveLockOnMode()
{
	_bOnLockOnMode = false;
	/*_bOnLookAtTarget = false;*/
	_bOnAutoTargeting = false;

	_enemysArray.Empty();

	if (_currentTarget != nullptr) {
		_currentTarget->SetLockOnWidgetVisibility(false);
		_currentTarget = nullptr;
	}

	LOG_SCREEN("Lock On Mode : %d", _bOnLockOnMode);
}



// Called every frame
void ULockOnSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!_bOnLockOnMode) return;

	if (_currentTarget == nullptr)
	{
		UnActiveLockOnMode();
	}

	if (!_currentTarget->WasRecentlyRendered()) {
// 		SetAutoTargeting(, 0.1f);
// 		_bOnAutoTargeting = true;
	}	


	if (_bOnAutoTargeting)
	{
		FRotator CurrentCameraRotation = _ownerController->GetControlRotation();

		FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(_ownerCamera->GetComponentLocation(), _currentTarget->GetActorLocation());
		FRotator _newRotation = FMath::RInterpTo(CurrentCameraRotation, newRotation, DeltaTime, _targetingSpeed);
		_ownerController->SetControlRotation(_newRotation);
	
	}

	if (_bOnLookAtTarget)
	{
		FRotator currentRotation = _ownerCharacter->GetActorRotation();
		FRotator newPlayerRotation = FRotator(currentRotation.Pitch,
			UKismetMathLibrary::FindLookAtRotation(_ownerCharacter->GetActorLocation(), _currentTarget->GetActorLocation()).Yaw,
			currentRotation.Roll);
		_ownerCharacter->SetActorRotation(newPlayerRotation);
	}
}

