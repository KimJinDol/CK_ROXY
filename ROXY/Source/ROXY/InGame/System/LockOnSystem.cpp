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

// ���� �÷��̾ �����ִ� fight zone ������ set
void ULockOnSystem::SetCurrentFightZone(AFightZoneFinder* fightZone)
{
	_currentFightZone = fightZone;
}

// �÷��̾ fight zone�� �����ϸ� �̹� ��ġ�Ǿ� �ִ� ���� �迭�� _enemyArray �� �Ҵ�
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

	// ���忡 �����ϴ� ������ �迭�� �����Ѵ�
	UGameplayStatics::GetAllActorsOfClass(contextWorld, AEnemyAI::StaticClass(), actorEnemys);


	// ������ ���� ������ �ð����� ������ ����� �ִ� ���鸸 ���ܳ��´�
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

		TArray<float> distanceArray; // ���� enemys �迭�� �ִ� ����� �÷��̾��� �Ÿ��� ���� �迭
		TArray<AEnemyAI*> enemysTempArr; // �Ÿ� ������ ��迭 �� �� ���� �迭

		// �� ����� �÷��̾��� �Ÿ��� �� ����
		for (int i = 0; i < _enemysArrayMaxNum; i++) {
			distanceArray.Push(FVector::Distance(_ownerCharacter->GetActorLocation(), _enemysArray[i]->GetActorLocation()));
		}

		// �迭 �� �߿��� ���� �Ÿ��� ����� �ε��� ������ �ش� �� ��ȯ�ϱ�
		TArray<float> distanceTempArr = distanceArray;

		distanceArray.Sort(); // �Ÿ����� ���� ������ �迭�� ����

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
	// ���� ������ Ȱ��ȭ �Ǿ����� ���� ��
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

	// ���� ���¿��� ���Է� ��
	else {
		// ���� ���� ���� enemy�� ���� ��带 ����
		_currentTarget->SetLockOnWidgetVisibility(false);

		if (_enemysArrayMaxNum == 0) return NULL;
		else
		{
			// ���� ����Ŭ�� �� ������ �� ����. ���� ������ �켱���� �� ����
			if (_targetIndex >= _enemysArrayMaxNum - 1)
			{
				ArrangeEnemysArray();

				_targetIndex = 0;
				_newTarget = _enemysArray[_targetIndex];

				// ���ο� ���� ����Ŭ�� �� ��, ���ο� Ÿ�ٰ� ���� Ÿ���� ��ġ�� �� �׳� ���� Ÿ������ �Ѿ��
				if (_newTarget == _currentTarget && _enemysArrayMaxNum > 1) {
					_targetIndex++;
				}

				_currentTarget = _enemysArray[_targetIndex];

				ActiveLockOnMode();
				LOG_SCREEN("Lock On Target : %s", *(_currentTarget->GetName()));

				return _currentTarget;
			}

			// ���� ����Ŭ���� ���� �ʾ��� ��
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

