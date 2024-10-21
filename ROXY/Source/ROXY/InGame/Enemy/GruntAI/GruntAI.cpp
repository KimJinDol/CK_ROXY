// Fill out your copyright notice in the Description page of Project Settings.


#include "GruntAI.h"
#include "InGame/Enemy/GruntAI/GruntAnimInstance.h"
#include "InGame/Enemy/GruntAI/GruntWidget.h"
#include "InGame/Enemy/EnemyWidget.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/Enemy/GruntAI/Throw/EliteThrow/EliteThrowAISkullBomb.h"
#include "InGame/Item/Item_HP.h"
#include "InGame/Item/Item_Fever.h"
#include "InGame/System/NunuInGameManager.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BrainComponent.h"
#include "Materials/MaterialInstance.h"
#include "InGame/System/AIObjectPool.h"

AGruntAI::AGruntAI()
{
	PrimaryActorTick.bCanEverTick = true;

	// 공격 콜리전
	AttackCollision = CreateDefaultSubobject<UCapsuleComponent>("AttackCollision");
	AttackCollision->SetupAttachment(GetMesh(), TEXT("AttackPoint"));
	AttackCollision->SetCapsuleSize(60.0f, 60.0f);
	AttackCollision->SetRelativeLocationAndRotation(FVector(0.0f, 50.0f, 0.0f), FRotator(00.0f, 0.0f, 0.0f));
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackCollision->SetCollisionProfileName("RespondOnlyRoxy");
	AttackCollision->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	// 데이터 테이블
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_GRUNT(TEXT("/Game/DataTable/DT_Grunt.DT_Grunt"));
	if (DT_GRUNT.Succeeded())
	{
		DataTable = DT_GRUNT.Object;
	}

	// UI
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HUDWidgetComponent"));
	WidgetComponent->SetupAttachment(GetMesh());
	WidgetComponent->SetVisibility(true);
	WidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UGruntWidget> UI_WIDGET(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/BP_Damage_Widget.BP_Damage_Widget_C"));
	if (UI_WIDGET.Succeeded())
	{
		WidgetComponent->SetWidgetClass(UI_WIDGET.Class);
		WidgetComponent->SetDrawSize(FVector2D(64.0f, 64.0f));
	}

	static ConstructorHelpers::FClassFinder<UEnemyWidget> UI_ELITEWIDGET(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/BP_HPBar_Widget.BP_HPBar_Widget_C"));
	if (UI_ELITEWIDGET.Succeeded())
	{
		// 엘리트 몬스터용 체력바 위젯
		EliteWidgetTemplate = UI_ELITEWIDGET.Class;
	}

	// 파티클
	PC_ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComponent");
	PC_ParticleComponent->SetupAttachment(GetMesh());
	PC_ParticleComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	PC_ParticleComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder <UParticleSystem> PS_SHEILD(TEXT("/Game/Resources/Enemy/Particle/Fx_Sheild/Fx_P_Enemy_Sheild.Fx_P_Enemy_Sheild"));
	if (PS_SHEILD.Succeeded())
	{
		PS_InvinciSheild = PS_SHEILD.Object;
		PC_ParticleComponent->SetTemplate(PS_InvinciSheild);
		PC_ParticleComponent->bAutoActivate = false;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_HIT(TEXT("/Game/Resources/Roxy/Particle/Fx_Hit/Fx_Roxy_Hit/Fx_Roxy_Hit_03.Fx_Roxy_Hit_03"));
	if (PS_HIT.Succeeded())
	{
		PS_Hit = PS_HIT.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_SWIFTHIT(TEXT("/Game/Resources/Roxy/Particle/Fx_SwiftHit/Fx_P_SwiftHit.Fx_P_SwiftHit"));
	if (PS_SWIFTHIT.Succeeded())
	{
		PS_SwiftHit = PS_SWIFTHIT.Object;
	}
	
	//static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_ELITEICON(TEXT("/Game/Resources/Enemy/UI/UI_P_Monster_Elite.UI_P_Monster_Elite"));
	//if (PS_ELITEICON.Succeeded())
	//{
	//	PS_EliteIcon = PS_ELITEICON.Object;
	//}
	//
	//static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_UNDEADICON(TEXT("/Game/Resources/Enemy/UI/UI_P_Monster_Undead.UI_P_Monster_Undead"));
	//if (PS_UNDEADICON.Succeeded())
	//{
	//	PS_BuffIcon = PS_UNDEADICON.Object;
	//}

	// 사운드
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_INVINCIBLE(TEXT("/Game/Resources/Sound/Enemy/Elite/SC_Invincible_Cast.SC_Invincible_Cast"));
	if (SC_INVINCIBLE.Succeeded())
	{
		SC_Invincible = SC_INVINCIBLE.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_DEATH(TEXT("/Game/Resources/Sound/Enemy/SC_Enemy_Death.SC_Enemy_Death"));
	if (SC_DEATH.Succeeded())
	{
		SC_Death = SC_DEATH.Object;
	}

	// 아이템
	static ConstructorHelpers::FClassFinder<AItem_HP> ITEM_HP(TEXT("/Game/Blueprints/InGame/Item/BP_Item_HP.BP_Item_HP_C"));
	if (ITEM_HP.Succeeded())
	{
		DropItem_HP = ITEM_HP.Class;
	}
}

void AGruntAI::Possessed(AController* NewController)
{
	Super::Possessed(NewController);
}

void AGruntAI::UnPossessed()
{
	Super::UnPossessed();
}

/* 기본 공격 */
void AGruntAI::StartAttack()
{
	if (_bIsAttacking) return;

	_bCanMoveToTarget = false;
	_bIsAttacking = true;
	
	AnimInstance->PlayAttackMontage();
}

void AGruntAI::OnAttackCollision()
{
	if (!_bIsAttacking) return;

	//LOG(Warning, "On Attack Collision");

	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// 공격에 대해 피해주기
void AGruntAI::AttackCheck(AActor* attackTarget)
{
	ARoxyCharacter* roxy = Cast<ARoxyCharacter>(attackTarget);

	if (roxy)
	{
		FDamageEvent damageEvent;
		
		if (_bIsAttacking) /* 기본 근접 공격 */
		{
			roxy->TakeDamage(_damage_Attack, damageEvent, GetController(), this);

			if (!_bIsEliteGrunt && !_bIsBuffGrunt)
			{
				// 일반 잡몹의 공격 일시
				//LOG(Warning, "Basic Grunt Attack");
				roxy->SetStiffen(_stiffenTime);
			}
			else
			{
				// 특수 잡몹의 공격에는 경직시간이 1.5배 더 들어감
				//LOG(Warning, "Special Grunt Attack");
				roxy->SetStiffen(_stiffenTime);
			}
		}
	}
}

void AGruntAI::OffAttackCollision()
{
	if (!_bIsAttacking) return;

	if (_overalppedActors.Num() != 0)
	{
		_overalppedActors.RemoveAll([](AActor* actor) {return true; });
	}

	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGruntAI::AttackEnd()
{
	if (!_bIsAttacking) return;

	_bIsAttacking = false;
	_bCanMoveToTarget = true;

	_bHitCheck = false;

	OffAttackCollision();

	OnAttackTaskEnd.Broadcast();
}

/* 일반 잡몹 특수 패턴 */
void AGruntAI::SpecialAttackCoolApply()
{
	if (!_bCanSpecialAttack) return;

	//LOG(Warning, "Special Attack Cool");

	_bCanSpecialAttack = false;

	GetWorldTimerManager().SetTimer(_specialAttackTimer, this, &AGruntAI::OnCanSpecialAttack, _coolTime_SpecialAttack, false);
}

void AGruntAI::OnCanSpecialAttack()
{
	if (_bCanSpecialAttack) return;

	_bCanSpecialAttack = true;
}

/* 엘리트 몬스터 특수 패턴 */
void AGruntAI::EliteSpecialAttackCoolApply()
{
	if (!_bCanEliteSpecialAttack) return;

	_bCanEliteSpecialAttack = false;

	GetWorldTimerManager().SetTimer(_eliteSpecialAttackTimer, this, &AGruntAI::OnCanEliteSpecialAttack, _coolTime_EliteSpecialAttack, false);
}

void AGruntAI::OnCanEliteSpecialAttack()
{
	if (_bCanEliteSpecialAttack) return;
	
	_bCanEliteSpecialAttack = true;
}

/* Target */
void AGruntAI::SetTarget(AActor* targetActor)
{
	Target = targetActor;
}

void AGruntAI::SetTurnToTarget(bool value)
{
	_bCanTurnToTarget = value;
}

void AGruntAI::TurnToTarget(AActor* targetActor, float deltaSec)
{
	if (targetActor)
	{
		FRotator lookTargetRot(GetActorRotation());
		lookTargetRot.Yaw = UKismetMathLibrary::MakeRotFromX(targetActor->GetActorLocation() - GetActorLocation()).Yaw;

		FRotator newRot = UKismetMathLibrary::RInterpTo(GetActorRotation(), lookTargetRot, deltaSec, 7.0f);
		SetActorRotation(newRot);

		if (newRot.Equals(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), targetActor->GetActorLocation()), 1.f))
		{
			SetTurnToTarget(false);
		}
	}
}

void AGruntAI::SetMoveToTarget(bool value)
{
	_bCanMoveToTarget = value;
}

bool AGruntAI::Sensor()
{
	FVector sensorStartPos = GetActorLocation(); /*+ (GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius())*/
	FVector sensorEndPos = sensorStartPos;
	float avoidMultiplier = 0.f;
	_avoidRot = FRotator::ZeroRotator;
	_bAvoiding = false;
	_bIsAllAvoiding = false;
	_bOnFrontSensor = false;
	_bOnFrontRightSensor = false;
	_bOnFrontLeftSensor = false;

	FHitResult hitResult;
	FCollisionQueryParams Param(NAME_None, false, this);
	if (Target)
	{
		Param.AddIgnoredActor(Target);
	}


	// front 센서 체크
	
	sensorEndPos += (GetActorForwardVector().Rotation().RotateVector(_frontSensorPos));
	if (GetWorld()->LineTraceSingleByChannel(hitResult, sensorStartPos, sensorEndPos, ECC_Visibility, Param))
	{
		DrawDebugLine(GetWorld(), sensorStartPos, sensorEndPos, FColor::Red);
		if (!hitResult.GetActor()->IsA(AGruntAI::StaticClass()) || FVector::Dist(GetActorLocation(), Target->GetActorLocation()) >= 250.f)
		{

			_bAvoiding = true;
			_bOnFrontSensor = true;
			avoidMultiplier -= 1.f;
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), sensorStartPos, sensorEndPos, FColor::Blue);
	}

	// front right 센서 체크
	sensorEndPos = sensorStartPos + (GetActorForwardVector().Rotation().RotateVector(_frontRightSensorPos));
	if (GetWorld()->LineTraceSingleByChannel(hitResult, sensorStartPos, sensorEndPos, ECC_Visibility, Param))
	{
		DrawDebugLine(GetWorld(), sensorStartPos, sensorEndPos, FColor::Red);
		if (!hitResult.GetActor()->IsA(AGruntAI::StaticClass()) || FVector::Dist(GetActorLocation(), Target->GetActorLocation()) >= 250.f)
		{
			_bAvoiding = true;
			_bOnFrontRightSensor = true;
			avoidMultiplier -= 1.f;
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), sensorStartPos, sensorEndPos, FColor::Blue);
	}

	// front left 센서 체크
	sensorEndPos = sensorStartPos + (GetActorForwardVector().Rotation().RotateVector(_frontLeftSensorPos));
	if (GetWorld()->LineTraceSingleByChannel(hitResult, sensorStartPos, sensorEndPos, ECC_Visibility, Param))
	{
		DrawDebugLine(GetWorld(), sensorStartPos, sensorEndPos, FColor::Red);
		if (!hitResult.GetActor()->IsA(AGruntAI::StaticClass()) || FVector::Dist(GetActorLocation(), Target->GetActorLocation()) >= 250.f)
		{
			_bAvoiding = true;
			_bOnFrontLeftSensor = true;
			avoidMultiplier += 1.f;
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), sensorStartPos, sensorEndPos, FColor::Blue);
	}

	// right 센서 체크
	sensorEndPos = sensorStartPos + (GetActorForwardVector().Rotation().RotateVector(_rightSensor));
	if (GetWorld()->LineTraceSingleByChannel(hitResult, sensorStartPos, sensorEndPos, ECC_Visibility, Param))
	{
		DrawDebugLine(GetWorld(), sensorStartPos, sensorEndPos, FColor::Red);
		if (!hitResult.GetActor()->IsA(AGruntAI::StaticClass()) || FVector::Dist(GetActorLocation(), Target->GetActorLocation()) >= 250.f)
		{
			_bAvoiding = true;
			_bOnRightSensor = true;
			avoidMultiplier -= 1.f;
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), sensorStartPos, sensorEndPos, FColor::Blue);
	}

	// left 센서 체크
	sensorEndPos = sensorStartPos + (GetActorForwardVector().Rotation().RotateVector(_leftSensor));
	if (GetWorld()->LineTraceSingleByChannel(hitResult, sensorStartPos, sensorEndPos, ECC_Visibility, Param))
	{
		DrawDebugLine(GetWorld(), sensorStartPos, sensorEndPos, FColor::Red);
		if (!hitResult.GetActor()->IsA(AGruntAI::StaticClass()) || FVector::Dist(GetActorLocation(), Target->GetActorLocation()) >= 250.f)
		{
			_bAvoiding = true;
			_bOnLeftSensor = true;
			avoidMultiplier += 1.f;
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), sensorStartPos, sensorEndPos, FColor::Blue);
	}

	// behind 센서 체크
	sensorEndPos = sensorStartPos + (GetActorForwardVector().Rotation().RotateVector(_behindSensor));
	if (GetWorld()->LineTraceSingleByChannel(hitResult, sensorStartPos, sensorEndPos, ECC_Visibility, Param))
	{
		DrawDebugLine(GetWorld(), sensorStartPos, sensorEndPos, FColor::Red);
		if (!hitResult.GetActor()->IsA(AGruntAI::StaticClass()) || FVector::Dist(GetActorLocation(), Target->GetActorLocation()) >= 250.f)
		{
			_bAvoiding = true;
			_bOnBehindSensor = true;
			avoidMultiplier += 1.f;
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), sensorStartPos, sensorEndPos, FColor::Blue);
	}
	

	if (_bAvoiding)
	{
		// front 센서만 감지 됐을 떄
		if (_bOnFrontSensor && !_bOnFrontRightSensor && !_bOnFrontLeftSensor)
		{
			_avoidRot = FRotator(0.f, -90.f, 0.f);
			_avoidVec = _avoidRot.Vector();
		}
		// front right 센서만 감지 됐을 때
		else if (_bOnFrontRightSensor && !_bOnFrontSensor && !_bOnFrontLeftSensor)
		{
			_avoidRot = FRotator(0.f, -45.f, 0.f);
			_avoidVec = _avoidRot.Vector();
		}
		// front left 센서만 감지 됐을 때
		else if (_bOnFrontLeftSensor && !_bOnFrontSensor && !_bOnFrontRightSensor)
		{
			_avoidRot = FRotator(0.f, 45.f, 0.f);
			_avoidVec = _avoidRot.Vector();
		}
		// front , front right 센서가 감지 됐을 떄
		else if (_bOnFrontSensor && _bOnFrontRightSensor && !_bOnFrontLeftSensor)
		{
			_avoidRot = FRotator(0.f, 45.f, 0.f);
			_avoidVec = _avoidRot.Vector();
		}
		// front, front left 센서가 감지 됐을 떄
		else if (_bOnFrontSensor && _bOnFrontLeftSensor && !_bOnFrontRightSensor)
		{
			_avoidRot = FRotator(0.f, 45.f, 0.f);
			_avoidVec = _avoidRot.Vector();
		}
		// front, frontRight, frontLeft 센서 감지 됐을 때
		else if (_bOnFrontSensor && _bOnFrontRightSensor && _bOnFrontLeftSensor && !_bOnBehindSensor)
		{
			_avoidRot = FRotator(0.f, 90.f, 0.f);
			_avoidVec = _avoidRot.Vector();
		}
		// front, frontRight, frontLeft, right, behind 센서 감지 됐을 떄
		else if (_bOnFrontSensor && _bOnFrontRightSensor && _bOnFrontLeftSensor && _bOnBehindSensor && _bOnRightSensor && !_bOnLeftSensor)
		{
			_avoidRot = FRotator(0.f, -135.f, 0.f);
			_avoidVec = _avoidRot.Vector();
		}
		// front, frontRight, frontLeft, right, behind 센서 감지 됐을 떄
		else if (_bOnFrontSensor && _bOnFrontRightSensor && _bOnFrontLeftSensor && _bOnBehindSensor && _bOnLeftSensor && !_bOnRightSensor)
		{
			_avoidRot = FRotator(0.f, 135.f, 0.f);
			_avoidVec = _avoidRot.Vector();
		}
		// 전부 다 감지 됐을 떄
		else if (_bOnFrontSensor && _bOnFrontRightSensor && _bOnFrontLeftSensor && _bOnBehindSensor && _bOnLeftSensor && _bOnRightSensor)
		{
			_bIsAllAvoiding = true;
		}
	}
	else
	{
		_avoidVec = FVector::ZeroVector;
	}
	
	return _bAvoiding;

}

void AGruntAI::MoveToTarget(float deltaTime)
{
	if (!Target || _bIsAllAvoiding || !_bCanMoveToTarget) return;

	FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
	if (_avoidRot != FRotator::ZeroRotator)
	{
		TargetRot += _avoidRot;
	}
	else 
	{
		FVector TargetVec = TargetRot.Vector();
		// DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (TargetVec * 100.f), FColor::Purple, false, -1.0f, (uint8)'\000', 15.f);
		GetCharacterMovement()->AddInputVector(TargetVec);
	}
}

void AGruntAI::ChangeMatOpacity(float deltaTime)
{
	CHECK(OpacityMat);

	if (_bIsDie && _currentMatType == ECurrentMatType::MT_OPACITY && _matOpacity >= 0.0f)
	{
		//float _matOpacity = UKismetMathLibrary::FInterpTo(1.0f, 0.0f, deltaTime, 1.0f);
		_matOpacity -= 0.02f;
		GetMesh()->SetScalarParameterValueOnMaterials(FName("Opacity"), _matOpacity);

		//LOG(Warning, "Opacity : %f", _matOpacity);
	}
}

void AGruntAI::DestroyActor()
{
	if (!_bIsDie) return;
	// AI Object Pool Return

	ObjectPoolReturn();
	
	/*Destroy();*/
}

void AGruntAI::ObjectPoolReturn()
{
	AAIObjectPool* AIObjectPool = Cast<AAIObjectPool>(InGameManager->FindActor(AAIObjectPool::StaticClass()));

	if (!AIObjectPool)
		Destroy();

	if(!AIObjectPool->Contains(this->GetClass()))
		Destroy();

	DeactiveAI();

	AIObjectPool->Push(this);
}

void AGruntAI::DeactiveAI()
{
	// Object Pool에서 관리되는 ai라면 비활성화
	AnimInstance->StopAllMontages(0.f);
	AnimInstance->StopSlotAnimation(0.f);

	AAIController* AIController = Cast<AAIController>(GetController());

	// Stop Logic
	AITool::AIBTStopLogic(this);
	_bCanMoveToTarget = false;
	_bCanTurnToTarget = false;

	GetMesh()->bHiddenInGame = false;
}

void AGruntAI::RevivalAI()
{
	AITool::AIBTRestartLogic(this);
	_bCanMoveToTarget = true;
	_bCanTurnToTarget = true;

	GetMesh()->bHiddenInGame = true;
}

void AGruntAI::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OtherActor);

		if (roxy)
		{
			if (!roxy->GetCharacterIsPierce() || !roxy->GetCharacterIsSwift() || !roxy->GetCharacterIsEvade() || !roxy->GetCharacterIsDie())
			{
				/* ----------- 잡몹 공통 기본 공격 ----------- */
				if (_bIsAttacking && !_bHitCheck)
				{
					//LOG(Warning, "Grunt Basic Attack");

					_overalppedActors.Add(roxy);
					
					_bHitCheck = true;

					AttackCheck(roxy);

					AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				}
			}
		}
	}
}

void AGruntAI::Die()
{
	Super::Die();

	//LOG_SCREEN("Die");

	_bIsDie = true;

	GetCharacterMovement()->SetActive(false);
	
	AITool::AIStopLogic(this);

	SetTarget(nullptr);
	SetMoveToTarget(false);

	WidgetComponent->SetVisibility(false);

	if (_currentStateMode == EGruntStateMode::SM_KNOCKBACK)
	{
		AnimInstance->SetKnockbackDieAnim(true);
	}
	
	AnimInstance->StopAllMontages(0.2);
	AnimInstance->SetDieAnim(true);

	FCollisionResponseContainer capsuleCollision = GetCapsuleComponent()->GetCollisionResponseToChannels();
	capsuleCollision.GameTraceChannel1 = ECollisionResponse(ECR_Overlap);
	capsuleCollision.GameTraceChannel2 = ECollisionResponse(ECR_Ignore);
	capsuleCollision.GameTraceChannel5 = ECollisionResponse(ECR_Ignore);
	capsuleCollision.Pawn = ECollisionResponse(ECR_Ignore);
	capsuleCollision.WorldDynamic = ECollisionResponse(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannels(capsuleCollision);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Ragdoll"));

	OffAttackCollision();

	// 몹 공통 죽는소리
	UGameplayStatics::PlaySoundAtLocation(this, SC_Death, GetActorLocation());

	// 아이템 드랍 (10퍼센트 확률)
	if(_bCanItemDrop && !_bIsBossMonster)
	{
		float randNum_DropHp = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
		if(randNum_DropHp <= 0.05f)
		{
			FHitResult _hitResult;

			FVector _startVec = GetActorLocation();
			_startVec.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			FVector _endVec = GetActorLocation();
			_endVec.Z -= 1000.0f;
			FCollisionQueryParams params(NAME_None, false, this);

			bool result = GetWorld()->LineTraceSingleByChannel(_hitResult, _startVec, _endVec, ECollisionChannel::ECC_Visibility, params);

			if (result)
			{
				float _spawnHeight = _hitResult.Location.Z + 130.0f;
				FVector _itemLoc = FVector(GetActorLocation().X, GetActorLocation().Y, _spawnHeight);
				FTransform _itemSpawnTransform = FTransform(GetActorRotation(), _itemLoc, FVector(1.0f, 1.0f, 1.0f));
				GetWorld()->SpawnActor<AItem_HP>(DropItem_HP, _itemSpawnTransform);
			}
		}
	}
}

void AGruntAI::BeginPlay()
{
	Super::BeginPlay();

	Roxy = InGameManager->GetPlayerCharacter();

	// UI
	GruntWidget = Cast<UGruntWidget>(WidgetComponent->GetUserWidgetObject());
	if (nullptr != GruntWidget)
	{
		//GruntWidget->BindAIStat(HpComponent);

		if (_bIsEliteGrunt)
		{
			//GruntWidget->OnEliteIcon(true);

			//FVector _iconLoc = GetActorLocation() + (GetActorUpVector() * 200.0f);
			//FVector _iconScale = FVector(0.5f, 0.5f, 0.5f);
			//
			//UGameplayStatics::SpawnEmitterAttached(PS_EliteIcon, GetCapsuleComponent(), FName("IconPoint"), _iconLoc,
			//	GetActorRotation(), _iconScale, EAttachLocation::KeepWorldPosition, false);
		}
	}

	if (EliteWidget)
	{
		if (_bIsEliteGrunt || _bIsBossMonster)
		{
			EliteWidget->BindAIStat(HpComponent);
		}
	}

	if (!_bIsBossMonster)
	{
		AnimInstance->PlaySpawnMontage(_spawnAnimRate);
	}
}

void AGruntAI::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	HpComponent->OnDie.AddUObject(this, &AGruntAI::Die);

	stiffEndDelegate.BindUFunction(this, FName("SetStiffState"), false);
	MarkTimerDelegate.BindUFunction(this, FName("SetGruntStateMode"), EGruntStateMode::SM_MARK);

	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AGruntAI::OnCollisionBeginOverlap);

	// 애니메이션
	AnimInstance = Cast<UGruntAnimInstance>(GetMesh()->GetAnimInstance());
	CHECK(AnimInstance != nullptr);

	AnimInstance->OnAttackCheck.AddUObject(this, &AGruntAI::OnAttackCollision);
	AnimInstance->OnOffAttackCollision.AddUObject(this, &AGruntAI::OffAttackCollision);
	AnimInstance->OnAttackEnd.AddUObject(this, &AGruntAI::AttackEnd);
	//AnimInstance->OnKnockbackEnd.AddUObject(this, &AGruntAI::SetGruntStateMode, EGruntStateMode::SM_IDLE);
	AnimInstance->OnChangeToOpacityMat.AddUObject(this, &AEnemyAI::SetOpacityMat);
	AnimInstance->OnDieAnimEnd.AddUObject(this, &AGruntAI::DestroyActor);
	AnimInstance->OnSpawnAnimStart.AddUFunction(this, "SpawningState", true);
	AnimInstance->OnSpawnAnimEnd.AddUFunction(this, "SpawningState", false);

	// UI
	EliteWidget = CreateWidget<UEnemyWidget>(GetWorld(), EliteWidgetTemplate);

	// 칼군무 . . .해결 . .
	//_startLogicTime = FMath::RandRange(1.4f, 1.9f);
	//SetStiffState(true, _startLogicTime);

	// 스폰 애니 다 똑같이 재생돼서 재생 속도 랜덤 조절
	_spawnAnimRate = FMath::RandRange(0.8f, 1.3f);
	_bIsInvincibleState = true;
}

void AGruntAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*Sensor();*/
	if (/*_bAvoiding ||*/ (_bCanMoveToTarget && Target != nullptr && _currentStateMode != EGruntStateMode::SM_KNOCKBACK && !_bIsStiff
		&& FVector::Distance(Target->GetActorLocation(), GetActorLocation()) >= 200.0f))
	{
		MoveToTarget(DeltaTime);
	}

	if (_bCanTurnToTarget && Target != nullptr && _currentStateMode != EGruntStateMode::SM_KNOCKBACK)
	{
		TurnToTarget(Target, DeltaTime);
	}

	// 죽었을 시 머테리얼 변화
	if (_bIsDie)
	{
		ChangeMatOpacity(DeltaTime);
	}

	// 엘리트 아이콘
	//if (WidgetComponent->IsVisible() && WidgetComponent->GetWidgetSpace() == EWidgetSpace::World)
	//{
	//	RotateEliteIcon();
	//}
}

void AGruntAI::SetGruntStateMode(EGruntStateMode newStateMode)
{
	_currentStateMode = newStateMode;
	
	switch (_currentStateMode)
	{
	case EGruntStateMode::SM_IDLE:
		AITool::AIRestartLogic(this);
		_bCanMoveToTarget = true;
		break;
	case EGruntStateMode::SM_KNOCKBACK:
		GetWorldTimerManager().ClearTimer(StiffTimer);
		_bCanMoveToTarget = false;
		break;
	case EGruntStateMode::SM_MARK:
		if (!_bIsMarking)
		{
			_bIsMarking = true;
			SetMarkTimer();
		}
		else
		{
			_bIsMarking = false;
		}
		break;
	}
}

float AGruntAI::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (_bIsDie || _bIsInvincibleState) return 0;

	ARoxyCharacter* roxy = Cast<ARoxyCharacter>(DamageCauser);
	AEliteThrowAISkullBomb* bomb = Cast<AEliteThrowAISkullBomb>(DamageCauser);

	/* 히트 머테리얼 */
	SetDamagedMat();

	/* 데미지 폰트 UI */
	//LOG(Warning, "Damage : %f", DamageAmount);
	//GruntWidget->BindDamage(DamageAmount);
	//SetDamageUI();
	//GruntWidget->PlayDamageAnim();

	// 엘리트Throw의 폭탄에 맞을 시 몬스터도 데미지를 입음
	if (bomb)
	{
		_offStiffTime = FMath::RandRange(0.7f, 1.3f);
		SetStiffState(true, _offStiffTime);

		AnimInstance->StopAllMontages(0.2f);
		AnimInstance->PlayHitMontage();

		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}

	FVector _hitParticleScale = FVector(1.0f, 1.0f, 1.0f);
	FVector _swiftHitParticleScale = FVector(1.0f, 1.0f, 1.0f);

	/* --------- 애니메이션 및 경직 --------- */
	if (_bIsEliteGrunt) // 엘리트형 몬스터
	{
		// 경직을 안먹어서 히트몽타주에 붙여둔 히트 이펙트가 안나옴ㄱ-
		if (roxy->GetCharacterIsSwift())
		{
			// 연참 피격 이펙트
			UGameplayStatics::SpawnEmitterAttached(PS_SwiftHit, GetMesh(), FName("HitPoint"), GetActorLocation(),
				GetActorRotation(), _swiftHitParticleScale, EAttachLocation::KeepWorldPosition, true);
		}
		else
		{
			UGameplayStatics::SpawnEmitterAttached(PS_Hit, GetMesh(), FName("HitPoint"), GetActorLocation(),
				GetActorRotation(), _hitParticleScale, EAttachLocation::KeepWorldPosition, true);
		}

		if ((!_bIsSpecialAttack && !_bIsEliteSpecialAttack)) // 특수 패턴 실행중이 아닐때. 일반 상태.
		{
			// 엘리트 몬스터는 스킬에만 경직 먹음.
			//if (roxy->GetCharacterIsPierce())
			//{
			//	_offStiffTime = FMath::RandRange(0.7f, 1.3f);
			//	SetStiffState(true, _offStiffTime);
			//
			//	AnimInstance->StopAllMontages(0.2f);
			//	AnimInstance->PlayHitMontage();
			//}
			//else if (roxy->GetCharacterIsSwift())
			//{
			//	_offStiffTime = FMath::RandRange(0.7f, 1.3f);
			//	SetStiffState(true, _offStiffTime);
			//
			//	AnimInstance->StopAllMontages(0.2f);
			//	AnimInstance->PlaySwiftHitMontage();
			//}
			//else if (_bIsStiff && roxy->GetCharacterIsPierce())
			//{
			//	_bIsStiff = false;
			//	SetStiffState(false);
			//
			//	AnimInstance->StopAllMontages(0.2f);
			//	AnimInstance->PlayHitMontage();
			//}
			if (_currentStateMode == EGruntStateMode::SM_KNOCKBACK)
			{
				SetGruntStateMode(EGruntStateMode::SM_IDLE);
			}
			else
			{
				//LOG(Warning, "Elite Is Strong");
			}

			if (_bIsMarking)
			{
				return Super::TakeDamage(DamageAmount * 1.3f, DamageEvent, EventInstigator, DamageCauser);
			}
			else
				return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		}
		else // 특수 패턴 중 피격시 데미지 피해만 입음. 피격 애니 무시.
		{
			//LOG(Warning, "Special Attack Hit");
		
			if (_bIsMarking)
			{
				return Super::TakeDamage(DamageAmount * 1.3f, DamageEvent, EventInstigator, DamageCauser);
			}
			else
				return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		}
	}
	else if (_bIsBuffGrunt) // 버프형 몬스터
	{
		// 버프 몬스터는 스킬과 발차기에만 경직 먹음.
		if (roxy->GetCharacterIsPierce())
		{
			_offStiffTime = FMath::RandRange(0.3f, 0.4f);

			SetStiffState(true, _offStiffTime);

			AnimInstance->StopAllMontages(0.2f);
			AnimInstance->PlayHitMontage();
		}
		else if (roxy->GetCharacterIsSwift())
		{
			_offStiffTime = FMath::RandRange(0.3f, 0.4f);
			SetStiffState(true, _offStiffTime);

			AnimInstance->StopAllMontages(0.2f);
			AnimInstance->PlaySwiftHitMontage();
		}
		else if (roxy->GetCharacterIsKick())
		{
			AITool::AIStopLogic(this);

			_offStiffTime = 2.2f;
			SetStiffState(true, _offStiffTime);

			AnimInstance->StopAllMontages(0.2f);
			AnimInstance->PlayKnockBackMontage();
		}
		else if (_currentStateMode == EGruntStateMode::SM_KNOCKBACK)
		{
			UGameplayStatics::SpawnEmitterAttached(PS_Hit, GetMesh(), FName("HitPoint"), GetActorLocation(),
				GetActorRotation(), _hitParticleScale, EAttachLocation::KeepWorldPosition, true);
		}
		else if (_bIsStiff && roxy->GetCharacterIsPierce())
		{
			_offStiffTime = FMath::RandRange(0.3f, 0.4f);
			SetStiffState(true, _offStiffTime);

			AnimInstance->StopAllMontages(0.2f);
			AnimInstance->PlayHitMontage();
		}
		else
		{
			//LOG(Warning, "Buff Is Strong");

			_offStiffTime = FMath::RandRange(0.7f, 0.9f);
			_stiffenProbability = FMath::RandRange(0.0f, 1.0f);
			if (_stiffenProbability <= 0.3f) // 30퍼센트의 확률로 경직을 먹임
			{
				SetStiffState(true, _offStiffTime);
				AnimInstance->StopAllMontages(0.2f);
				AnimInstance->PlayHitMontage();
			}
			else
			{
				UGameplayStatics::SpawnEmitterAttached(PS_Hit, GetMesh(), FName("HitPoint"), GetActorLocation(),
					GetActorRotation(), _hitParticleScale, EAttachLocation::KeepWorldPosition, true);
			}
		}

		if (_bIsMarking)
		{
			return Super::TakeDamage(DamageAmount * 1.3f, DamageEvent, EventInstigator, DamageCauser);
		}
		else
			return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	else // 기본 몬스터
	{
		// 공격 중일 시
		if (_bIsAttacking)
		{
			AttackEnd();
		}

		// 넉백
		if (_currentStateMode == EGruntStateMode::SM_KNOCKBACK || roxy->GetCharacterIsKick())
		{
			//LOG_SCREEN("KNOCKBACK");

			AITool::AIStopLogic(this);

			_offStiffTime = 2.6f;
			SetStiffState(true, _offStiffTime);

			AnimInstance->StopAllMontages(0.2f);
			AnimInstance->PlayKnockBackMontage();
		}
		// 연참으로 맞았을 시
		else if (roxy->GetCharacterIsSwift())
		{
			//LOG(Warning, "Hit From SwiftSlash");
			_offStiffTime = FMath::RandRange(0.8f, 1.1f);
			SetStiffState(true, _offStiffTime);

			AnimInstance->StopAllMontages(0.2f);
			AnimInstance->PlaySwiftHitMontage();
		}
		// 기본 피격
		else
		{
			_offStiffTime = FMath::RandRange(0.8f, 1.0f);
			_stiffenProbability = FMath::RandRange(0.0f, 1.0f);
			if (_stiffenProbability <= 0.9f) // 기본 공격 시 90퍼센트의 확률로 경직을 먹임
			{
				SetStiffState(true, _offStiffTime);
				AnimInstance->StopAllMontages(0.2f);
				
				if (AnimInstance->GetSideHitMontage())
				{
					float _randHitDir = FMath::RandRange(0.0f, 1.0f);

					if (roxy->GetCurrentCombo() == 1)
					{
						if(_randHitDir < 0.7f)
							AnimInstance->PlaySideHitMontage(EHitDirection::HD_R);
						else
							AnimInstance->PlayHitMontage();
					}
					else if (roxy->GetCurrentCombo() == 2)
					{
						if (_randHitDir < 0.7f)
							AnimInstance->PlaySideHitMontage(EHitDirection::HD_L);
						else
							AnimInstance->PlayHitMontage();
					}
					else
					{
						AnimInstance->PlayHitMontage();
					}
				}
				else
				{
					AnimInstance->PlayHitMontage();
				}
			}
			else
			{
				//LOG(Warning, "Grunt Avoid Stiffen From Roxy Basic Attack");
			}
		}

		if (_bIsMarking)
		{
			return Super::TakeDamage(DamageAmount * 1.3f, DamageEvent, EventInstigator, DamageCauser);
		}
		else
			return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}

	//if (_currentStateMode == EGruntStateMode::SM_KNOCKBACK) {
	//	AnimInstance->PlayKnockBackMontage();
	//}
	//else {
	//	/*4방향 피격 애니메이션 출력------------------------------------------*/
	//	FVector PlayerLoc = DamageCauser->GetActorLocation();
	//	// 데미지를 입힌 가해자가 있는 방향
	//	FRotator DamageCauserRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerLoc);
	//
	//
	//	if ((-45.f < DamageCauserRot.Yaw && 0.f > DamageCauserRot.Yaw) || (0.f <= DamageCauserRot.Yaw && 45.f >= DamageCauserRot.Yaw))
	//	{
	//		AnimInstance->PlayHitMontage(EHitDirection::HD_F);
	//	}
	//	else if ((-120.f >= DamageCauserRot.Yaw && -180.f < DamageCauserRot.Yaw) || (180.f > DamageCauserRot.Yaw && 120.f < DamageCauserRot.Yaw)) {
	//		AnimInstance->PlayHitMontage(EHitDirection::HD_B);
	//	}
	//	else if (-45.f > DamageCauserRot.Yaw && -120.f < DamageCauserRot.Yaw)
	//	{
	//		AnimInstance->PlayHitMontage(EHitDirection::HD_L);
	//	}
	//	else if (45.f < DamageCauserRot.Yaw && 120.f > DamageCauserRot.Yaw)
	//	{
	//		AnimInstance->PlayHitMontage(EHitDirection::HD_R);
	//	}
	//}
}

void AGruntAI::SetStiffState(bool value /*= true*/, float time /*= 0.0f*/)
{
	if (value) {
		_bIsStiff = true;
		_bCanMoveToTarget = false;
		AITool::AIStopLogic(this);
		GetWorldTimerManager().ClearTimer(StiffTimer);
		GetWorldTimerManager().SetTimer(StiffTimer, stiffEndDelegate, time, false);
	}
	else{
		_bIsStiff = false;
		_bCanMoveToTarget = true;
		AITool::AIRestartLogic(this);

		if (_currentStateMode == EGruntStateMode::SM_KNOCKBACK)
		{
			SetGruntStateMode(EGruntStateMode::SM_IDLE);
			AttackEnd();
		}

		if (_bIsAttacking)
		{
			AttackEnd();
		}

		GetWorldTimerManager().ClearTimer(StiffTimer);
	}
}

void AGruntAI::SetInvinciState(bool value)
{
	if (value)
	{
		//LOG(Warning, "Grunt is Invincibility");
		_bIsInvincibleState = true;

		// 무적 상태 오라 이펙트
		//PC_ParticleComponent = UGameplayStatics::SpawnEmitterAttached(PS_InvinciSheild, GetMesh(), FName("Bip001"),
		//	FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), EAttachLocation::KeepRelativeOffset, false);

		// 피격 중이라 DamageMat 상태면 파라미터값 조정해준게 안먹음
		SetOriginMat();

		// 무적용 셰이더 켜주기
		GetMesh()->SetScalarParameterValueOnMaterials(FName("Shield_pow"), 1.0f);

		UGameplayStatics::PlaySoundAtLocation(this, SC_Invincible, GetActorLocation());
	}
	else
	{
		//LOG(Warning, "End Invincibility State");
		_bIsInvincibleState = false;

		// 이펙트 끔
		//PC_ParticleComponent->SetVisibility(false);

		// 셰이더 끄기
		GetMesh()->SetScalarParameterValueOnMaterials(FName("Shield_pow"), 0.0f);
	}
}

void AGruntAI::SpawningState(bool value)
{
	if (value)
	{
		// 혹시 스폰시에 캐릭터 안쳐다보고있으면..
		AActor* roxy = InGameManager->GetPlayerCharacter();

		if (roxy)
		{
			if (!_bIsEliteGrunt)
			{
				FVector _targetLoc = roxy->GetActorLocation();
				FVector _originLoc = GetActorLocation();
				FRotator _originRot = GetActorForwardVector().Rotation();

				FRotator _tempRot = UKismetMathLibrary::FindLookAtRotation(_originLoc, _targetLoc);
				FRotator _newRot = FRotator(_originRot.Pitch, _tempRot.Yaw, _originRot.Roll);

				SetActorRotation(_newRot);
			}
		}

		// 스폰 애니 진행동안 무적상태
		_bIsStiff = true;
		_bCanMoveToTarget = false;
		AITool::AIStopLogic(this);

		_bIsInvincibleState = true;
	}
	else
	{
		// 스폰이 종료되면 무적 해제, ai 로직 실행
		_bIsStiff = false;
		_bCanMoveToTarget = true;
		AITool::AIRestartLogic(this);

		SetStiffState(false);

		if(!_bIsBossMonster)
			_bIsInvincibleState = false;

		OnSpawnEnd.Broadcast();
	}
}

void AGruntAI::SetEliteHUDVisibility(bool value)
{
	if(!EliteWidget) return;

	if(value)
	{
		EliteWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		EliteWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AGruntAI::SetDamageUI()
{
	GruntWidget->OnDamageIcon(true);

	GetWorldTimerManager().SetTimer(DamageUITimer, this, &AGruntAI::OffDamageUI, 1.3f, false);
}

void AGruntAI::OffDamageUI()
{
	GruntWidget->OnDamageIcon(false);
}

void AGruntAI::RotateEliteIcon()
{
	if (WidgetComponent != nullptr)
	{
		AActor* roxy = InGameManager->GetPlayerCharacter();

		if (roxy)
		{
			// 위젯 스페이스를 스크린모드로 해두면 거리에 따라 크기가 왔다갔다 하는게 거슬리고 몬스터도 가려서
			// 스페이스를 월드모드로 해두되 ui 이미지가 똑바로 보이도록 월드회전값을 캐릭터를 바라보도록
			FVector _targetLoc = roxy->GetActorLocation();
			FVector _originLoc = GetActorLocation();
			FRotator _originRot = GetActorForwardVector().Rotation();

			FRotator _tempRot = UKismetMathLibrary::FindLookAtRotation(_originLoc, _targetLoc);
			FRotator _newRot = FRotator(_originRot.Pitch, _tempRot.Yaw, _originRot.Roll);

			WidgetComponent->SetWorldRotation(_newRot);
		}
	}
}

void AGruntAI::SetMarkUI()
{

}

void AGruntAI::SetMarkTimer()
{
	if (_bIsMarking)
	{
		GetWorldTimerManager().ClearTimer(MarkDurationTimer);
	}
	GetWorldTimerManager().SetTimer(MarkDurationTimer, MarkTimerDelegate, _markStateDuration, false);
}

UWidgetComponent* AGruntAI::GetGruntHUDWidget()
{
	return WidgetComponent;
}

bool AGruntAI::GetIsStiff()
{
	return _bIsStiff;
}

bool AGruntAI::GetIsDie()
{
	return _bIsDie;
}
