// Fill out your copyright notice in the Description page of Project Settings.
#include "Generator.h"
#include "InGame/DataTable/GeneratorDataTable.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/System/NewEnemySpawner.h"
#include "InGame/System/GeneratorWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGenerator::AGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOUNDARYZONE(TEXT("/Game/Resources/Generator/Mesh/generator_low.generator_low"));
	if (SM_BOUNDARYZONE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_BOUNDARYZONE.Object);
	}

	InteractionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollision"));
	InteractionCollision->SetCollisionProfileName(TEXT("Trigger"));
	InteractionCollision->SetSphereRadius(500.f);
	InteractionCollision->SetupAttachment(RootComponent);
	InteractionCollision->SetRelativeLocation(FVector(0.f, 0.f, -140.f));


	static ConstructorHelpers::FClassFinder<UGeneratorWidget> UI_GENERATOR(TEXT("/Game/Blueprints/InGame/System/BP_GeneratorWidget.BP_GeneratorWidget_C"));
	if (UI_GENERATOR.Succeeded())
	{
		WidgetTemplate = UI_GENERATOR.Class;
	}

	// 머테리얼
	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_ORIGIN(TEXT("/Game/Resources/Generator/Materials/M_generator.M_generator"));
	if (MAT_ORIGIN.Succeeded())
	{
		OriginalMat = MAT_ORIGIN.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_DAMAGED(TEXT("/Game/Resources/Generator/Materials/Mat_ProtectStone_Hit.Mat_ProtectStone_Hit"));
	if (MAT_DAMAGED.Succeeded())
	{
		DamagedMat = MAT_DAMAGED.Object;
	}

	PC_ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PS_ParticleComponent"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_SPARK(TEXT ("/Game/Resources/Generator/Particle/Eff_spark/P_spark.P_spark"));
	if (P_SPARK.Succeeded ())
	{
		PS_Spark = P_SPARK.Object;
		PC_ParticleComponent->SetTemplate(PS_Spark);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_SPART_ATTACK(TEXT ("/Game/Resources/Generator/Particle/Eff_spark/P_spark_attack.P_spark_attack"));
	if (P_SPART_ATTACK.Succeeded ())
	{
		PS_SparkAttack = P_SPART_ATTACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SB_REPAIR_START (TEXT ("/Game/Resources/Sound/Generator/Effect/SC_Generator_Start.SC_Generator_Start"));
	if (SB_REPAIR_START.Succeeded ())
	{
		SB_RepairStart = SB_REPAIR_START.Object;
	}
	

	// 데이터 테이블
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_GENERATOR(TEXT("/Game/DataTable/DT_Generator.DT_Generator"));
	if (DT_GENERATOR.Succeeded())
	{
		DataTable = DT_GENERATOR.Object;
	}

}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
{
	Super::BeginPlay();

	InteractionCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PlayerController = GetWorld()->GetFirstPlayerController();
	Roxy = Cast<ARoxyCharacter>(PlayerController->GetPawn());
	PlayerController->GetCharacter()->InputComponent->BindAction("Interaction", IE_Pressed, this, &AGenerator::BeginSkillCheck);
	

	WidgetInstance = CreateWidget<UGeneratorWidget>(GetWorld(), WidgetTemplate);
	WidgetInstance->BindOwner(this);
	WidgetInstance->AddToViewport();

	_bCanRepair = false;

	BreakDown();
}

void AGenerator::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InteractionCollision->OnComponentBeginOverlap.AddDynamic(this, &AGenerator::OnInteractionCollisionBeginOverlap);
	InteractionCollision->OnComponentEndOverlap.AddDynamic(this, &AGenerator::OnInteractionCollisionEndOverlap);

	BreakDown_Delegate.BindUObject(this, &AGenerator::SetEnableInteraction);
	RepairPause_Delegate.BindUObject(this, &AGenerator::ReleaseRepairPause);

	// 데이터 테이블 값 할당 ---------------------------
	if (DataTable)
	{
		FGeneratorStatTableRow* statTableRow = DataTable->FindRow<FGeneratorStatTableRow>(TEXT("Generator"), FString("Roxy Data Table is not exist!"));

		if (statTableRow)
		{
			_maxRp = statTableRow->maxRp;
			_RpIncrementPerSec = statTableRow->RpIncrementPerSec;
			_RpDecrementPerSec = statTableRow->RpDecrementPerSec;
			_maxSkillCheckTime = statTableRow->skillCheckTime_Max;
			_skillCheckTimeMinRange = statTableRow->skillCheckableTime_Min;
			_toleranceTime = statTableRow->skillCheckToleranceTime;
			_repairPauseDuration = statTableRow->repairPauseDuration;
			_breakDownPeriod_Min = statTableRow->breakDownPeriod_Min;
			_breakDownPeriod_Max = statTableRow->breakDownPeriod_Max;
		}
	}
// 	_maxRp = 100.f;

// 	_RpIncrementPerSec = _maxRp / 90; // 90초 동안 돌려야 수리 끝 
// 	_RpDecrementPerSec = _maxRp / 90;
// 
// 	_maxSkillCheckTime = 1.f;
// 	_skillCheckTimeMinRange = 0.6f;
// 	_toleranceTime = 0.15f;
// 	_repairPauseDuration = 5.f;
// 
// 	_breakDownPeriod_Min= 5.f;
// 	_breakDownPeriod_Max= 7.f;
}

// Called every frame
void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (_bCanRepair) // 정상적으로 발전기가 돌아갈 때
	{
		_currentRp = FMath::Clamp(_currentRp + (DeltaTime * 0.f), 0.f, _maxRp);

		// 게이지 위험 알림 타이머가 가동된 상태일 때
		if (EnemySpawner->GetSumPlayTime() > 15.f)
		{
			if (GetWorldTimerManager().IsTimerActive(DangerousGaugeTimer))
			{
				// 게이지가 위험수치 이상이 되면 타이머 클리어
				if (GetRpRatio() > 0.3f)
					GetWorldTimerManager().ClearTimer(DangerousGaugeTimer);
			}
			else
			{
				// 게이지가 위험수치 이하가 되면 타이머 가동
				if (GetRpRatio() < 0.3f)
					GetWorldTimerManager().SetTimer(DangerousGaugeTimer, this, &AGenerator::OnDangerousGauge, _RpLessThanDuration);
			}
		}
	}
	else if (_bIsBreakDown)
	{
		_currentRp = FMath::Clamp(_currentRp - (DeltaTime * 0.f), 0.f, _maxRp);
	}


	if (_bIncreaseCheckTime)
	{
		_currentSkillCheckTime += DeltaTime;

		// 스킬 체크 시간 초과 시 실패 처리
		if (_currentSkillCheckTime >= _maxSkillCheckTime)
		{
			FailedSkillCheck();
		}
	}

	// 게이지를 다 채웠을 시 게임 클리어
	//if (_currentRp >= _maxRp)
	//{
	//	OpenRoxyWinWidget();
	//	//GetWorldTimerManager().SetTimer(FullChargeTimer, this, &AGenerator::OpenRoxyWinWidget, 1.5f, false);
	//}
	
	// 게이지가 0이 됐을 시 게임 오버
	if (_currentRp < 1 && !_bIsFirstSkillCheck && EnemySpawner->GetIsCanSpawn() && EnemySpawner->GetSumPlayTime() > 3.0f)
	{
		
	}
}

void AGenerator::StartBreakDownTimer()
{
	// 다음으로 고장날 시간을 지정한다.
	float nextBreakDownPeriod = UKismetMathLibrary::RandomFloatInRange(_breakDownPeriod_Min, _breakDownPeriod_Max);
	GetWorldTimerManager().SetTimer(BreakDownTimer, BreakDown_Delegate, nextBreakDownPeriod, false);
}

void AGenerator::SetEnableInteraction()
{
	LOG_SCREEN("Can Interaction Generator");

	InteractionCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// 만약 플레이어가 이미 들어와 있는 상태라면 
	// Begin Overlap 이 실행 안되니까 한 번 검사해본다.
	{
		TSet<AActor*> overlappedActors;
		InteractionCollision->GetOverlappingActors(overlappedActors);
	
		for (auto i : overlappedActors)
		{
			if (i->IsA(ARoxyCharacter::StaticClass()))
			{
				WidgetInstance->SetVisibilityInteraction(true);
				_bCanSkillCheck = true;
			}
		}
	}
}

void AGenerator::BreakDown()
{
	_bCanRepair = false;
	_bIsBreakDown = true;
	WidgetInstance->BreakdownGenerator();


	PC_ParticleComponent->Activate();

	PlayBreakDownParticle();
}

void AGenerator::ReleaseRepairPause()
{
	WidgetInstance->RepairGenerator();

	if (_bIsBreakDown)
	{
		PC_ParticleComponent->Deactivate();

		_bIsSkillCheck = false;
		_currentSkillCheckTime = 0.f;
		_bIncreaseCheckTime = false;

		// _skill check max num 만큼 상호작용 이벤트 발생
		// 상호작용이 완전히 끝날 시 EnemySpawner 다음 웨이브로 설정.
		if (!_bIsFirstSkillCheck && EnemySpawner != nullptr)
		{
			_currentSkillCheckNum++;
			LOG(Warning, "Current Skill Check Num : %d", _currentSkillCheckNum);
			if (_currentSkillCheckNum < _skillCheckMaxNum)
				BeginSkillCheck();
			else
			{
				WidgetInstance->SetVisibilitySkillCheck(false);
				_bCanRepair = true;
				_bCanSkillCheck = false;
				_bIsBreakDown = false;
				InteractionCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Roxy->SetEnableInput(true, true, true);

				OnFinishSkillCheck.Broadcast();

				EnemySpawner->SetCurrentWaveToNext();

				_currentSkillCheckNum = 0;
				_skillCheckSuccessNum = 0;

				PlaySound(SB_RepairStart);
			}
		}
		else if (_bIsFirstSkillCheck && EnemySpawner != nullptr)
		{
			WidgetInstance->SetVisibilitySkillCheck(false);
			_bCanRepair = true;
			_bCanSkillCheck = false;
			_bIsBreakDown = false;
			InteractionCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Roxy->SetEnableInput(true, true, true);

			OnFinishSkillCheck.Broadcast();

			_skillCheckSuccessNum = 0;
			EnemySpawner->BeginSpawnWave();
			PlaySound(SB_RepairStart);
			_bIsFirstSkillCheck = false;
		}
		else if (EnemySpawner == nullptr)
		{
			LOG(Error, "Generator : Not Exist EnemySpawner!");
		}
	}
}


void AGenerator::BeginSkillCheck()
{
	LOG(Warning, "can repair : %d, is breakDown : %d, Can Skill Check : %d", _bCanRepair, _bIsBreakDown, _bCanSkillCheck);
	if (_bOnLast || _bCanRepair || !_bIsBreakDown || !_bCanSkillCheck) return;

	if (!_bIsSkillCheck)
	{
		_bIsSkillCheck = true;
		_skillCheckTime = UKismetMathLibrary::RandomFloatInRange(_skillCheckTimeMinRange, _maxSkillCheckTime - _toleranceTime);
		Roxy->GetCharacterMovement()->Velocity = FVector::ZeroVector;
		Roxy->SetEnableInput (false, false, false); // 플레이어 Freeze
		SkillCheck();
	}
	else
	{
		if (!EnemySpawner->IsLastWave())
		{
			// 스킬 체크 성공 시
			if (UKismetMathLibrary::NearlyEqual_FloatFloat(_currentSkillCheckTime, _skillCheckTime, _toleranceTime))
			{
				SuccessSkillCheck();
			}
			// 스킬 체크 실패 시
			else
			{
				FailedSkillCheck();
			}
		}
		// 마지막 웨이브가 끝났다면 스테이지도 끝낸다
		else
		{
			LOG_SCREEN("Wave Index : %d", EnemySpawner->GetCurrentWaveIndex());
			WidgetInstance->SetVisibilitySkillCheck(false);
			_bCanRepair = true;
			_bCanSkillCheck = false;
			_bIsBreakDown = false;
			_bIncreaseCheckTime = false;
			InteractionCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			_bOnLast = true;
			Roxy->SetIsGameClear(true);
			Roxy->OpenWinWidget();
			LastSkillCheck();
		}
	}
}

void AGenerator::SkillCheck()
{
	WidgetInstance->SetVisibilitySkillCheck(true);
	WidgetInstance->SetVisibilityInteraction(false);

	_bIncreaseCheckTime = true;
}

void AGenerator::SuccessSkillCheck()
{
	// 스킬 체크 성공!!

	if (!_bIsFirstSkillCheck)
	{
		// 스킬체크 성공시의 보상 (수리량 5퍼센트 증가)
		_currentRp = FMath::Clamp(_currentRp + _RpIncrementAtSuccessSkillCheck, _RpIncrementAtSuccessSkillCheck, _maxRp);
	}
	_skillCheckSuccessNum++;

	ReleaseRepairPause();

	LOG_SCREEN("Skill Check Success");
}

void AGenerator::FailedSkillCheck()
{
	// 스킬 체크 실패!!

	ReleaseRepairPause();

	LOG_SCREEN("Skill Check Failed");
}

void AGenerator::OnInteractionCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ARoxyCharacter::StaticClass()) && !_bCanRepair && _bIsBreakDown)
	{
		WidgetInstance->SetVisibilityInteraction(true);
		_bCanSkillCheck = true;
	}
}

void AGenerator::OnInteractionCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ARoxyCharacter::StaticClass()) && !_bCanRepair && _bIsBreakDown)
	{
		WidgetInstance->SetVisibilityInteraction(false);
		_bCanSkillCheck = false;
	}
}

void AGenerator::SetDamagedMat()
{
	Mesh->SetMaterial(0, DamagedMat);

	GetWorldTimerManager().SetTimer(_matTimer, this, &AGenerator::SetOriginMat, 0.2f, false);
}

void AGenerator::SetOriginMat()
{
	Mesh->SetMaterial(0, OriginalMat);
}

void AGenerator::OpenRoxyWinWidget()
{
	Roxy->OpenWinWidget();
}

float AGenerator::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	SetDamagedMat();
	PlayDamagedSound();

	WidgetInstance->TakeDamageGenerator();

	// 수리 일시 정지
	GetWorldTimerManager().ClearTimer(RepairPauseTimer);
	GetWorldTimerManager ().SetTimer (RepairPauseTimer, RepairPause_Delegate, _repairPauseDuration, false);

	_decrementAtDamaged = DamageAmount;

	_currentRp = FMath::Clamp(_currentRp - _decrementAtDamaged, 0.f, _maxRp);
	if (_currentRp <= 0)
	{
		Roxy->SetIsGameOver(true);
		Roxy->OpenLoseWidget();
	}
	return _currentRp;
}


void AGenerator::PlaySound(USoundBase* sound)
{
	UGameplayStatics::PlaySoundAtLocation(this, sound, GetActorLocation());
}

void AGenerator::SetRp(float rp)
{
	_currentRp = FMath::Clamp(rp, 0.f, _maxRp);
}

float AGenerator::GetRp()
{
	return _currentRp;
}

float AGenerator::GetRpRatio()
{
	return _currentRp < KINDA_SMALL_NUMBER ? 0 : _currentRp / _maxRp;
}

float AGenerator::GetMaxRp()
{
	return _maxRp;
}

bool AGenerator::IsDangerousGauge()
{
	return _bIsDangerousGauge;
}

void AGenerator::OnDangerousGauge()
{
	DangerousGaugeNotify.Broadcast(TEXT("1"));
}

bool AGenerator::IsFirstCheck ()
{
	return _bIsFirstSkillCheck;
}

bool AGenerator::IsCanSkillCheck ()
{
	return _bCanSkillCheck;
}

bool AGenerator::IsCanInteraction()
{
	return _bIsBreakDown;
}

float AGenerator::GetMaxSkillCheckTime()
{
	return _maxSkillCheckTime;
}

float AGenerator::GetSkillCheckTime()
{
	return _skillCheckTime;
}

float AGenerator::GetCurrentSkillCheckTime()
{
	return _currentSkillCheckTime;
}

float AGenerator::GetSkillCheckToleranceTime()
{
	return _toleranceTime;
}

float AGenerator::GetCurrentSkillCheckTimeRatio()
{
	return  _currentSkillCheckTime < KINDA_SMALL_NUMBER ? 0 : _currentSkillCheckTime / _maxSkillCheckTime;
}

int32 AGenerator::GetSkillCheckSuccessNum()
{
	return _skillCheckSuccessNum;
}
