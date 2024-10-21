
#include "RoxyCharacter.h"
#include "InGame/DataTable/RoxyDataTable.h"
#include "InGame/Component/PN_SpringArmComponent.h"
#include "InGame/Component/PN_CameraComponent.h"
#include "InGame/Component/HpComponent.h"
#include "InGame/Component/UltimateComponent.h"
#include "InGame/Component/ComboComponent.h"
#include "InGame/NunuCharacter/NunuPlayerController.h"
#include "InGame/NunuCharacter/RoxyAnimInstance.h"
#include "InGame/NunuCharacter/RoxyWidget.h"
#include "Core/NunuGameInstance.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "InGame/Enemy/Boss/NunuBossAI.h"
#include "InGame/Enemy/GruntAI/Revival/RevivalAI.h"
#include "InGame/System/VendingMachine.h"
#include "Components/CapsuleComponent.h"
#include "Components/RectLightComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARoxyCharacter::ARoxyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Roxy(TEXT("/Game/Resources/Roxy/Mesh/SK_Roxy_Sword.SK_Roxy_Sword"));
	if (SK_Roxy.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Roxy.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_Roxy(TEXT("/Game/Resources/Roxy/BP_Roxy_Anim.BP_Roxy_Anim_C"));
	if (ANIM_Roxy.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIM_Roxy.Class);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> ROXY_BODYMAT(TEXT("/Game/Resources/Roxy/Matetials/M_Roxy_body.M_Roxy_body"));
	if (ROXY_BODYMAT.Succeeded())
	{
		BodyMaterial = ROXY_BODYMAT.Object;
		GetMesh()->SetMaterial(0, BodyMaterial);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> ROXY_SWORDMAT(TEXT("/Game/Resources/Roxy/Matetials/M_Roxy_Sword.M_Roxy_Sword"));
	if (ROXY_SWORDMAT.Succeeded())
	{
		SwordMaterial = ROXY_SWORDMAT.Object;
		GetMesh()->SetMaterial(1, SwordMaterial);
		GetMesh()->SetMaterial(2, SwordMaterial);
	}

	// 카메라 쉐이크 ///////////////////////////////////////////////////
	{
		static ConstructorHelpers::FClassFinder<UCameraShake> CS_ATTACK1(TEXT("/Game/Blueprints/InGame/Roxy/CameraShake/BP_CS_Attack1.BP_CS_Attack1_C"));
		if (CS_ATTACK1.Succeeded())
		{
			CS_Attack1 = CS_ATTACK1.Class;
		}
		static ConstructorHelpers::FClassFinder<UCameraShake> CS_ATTACK2(TEXT("/Game/Blueprints/InGame/Roxy/CameraShake/BP_CS_Attack2.BP_CS_Attack2_C"));
		if (CS_ATTACK2.Succeeded())
		{
			CS_Attack2 = CS_ATTACK2.Class;
		}
		static ConstructorHelpers::FClassFinder<UCameraShake> CS_ATTACK3(TEXT("/Game/Blueprints/InGame/Roxy/CameraShake/BP_CS_Attack3.BP_CS_Attack3_C"));
		if (CS_ATTACK2.Succeeded())
		{
			CS_Attack3 = CS_ATTACK3.Class;
		}
		static ConstructorHelpers::FClassFinder<UCameraShake> CS_ATTACK4(TEXT("/Game/Blueprints/InGame/Roxy/CameraShake/BP_CS_Attack4.BP_CS_Attack4_C"));
		if (CS_ATTACK4.Succeeded())
		{
			CS_Attack4 = CS_ATTACK4.Class;
		}
		static ConstructorHelpers::FClassFinder<UCameraShake> CS_PIERCE(TEXT("/Game/Blueprints/InGame/Roxy/CameraShake/BP_CS_Pierce.BP_CS_Pierce_C"));
		if (CS_PIERCE.Succeeded())
		{
			CS_Pierce = CS_PIERCE.Class;
		}
		static ConstructorHelpers::FClassFinder<UCameraShake> CS_PIERCESTART(TEXT("/Game/Blueprints/InGame/Roxy/CameraShake/BP_CS_PierceStart.BP_CS_PierceStart_C"));
		if (CS_PIERCESTART.Succeeded())
		{
			CS_PierceStart = CS_PIERCESTART.Class;
		}
		static ConstructorHelpers::FClassFinder<UCameraShake> CS_HIT(TEXT("/Game/Blueprints/InGame/Roxy/CameraShake/BP_CS_Hit.BP_CS_Hit_C"));
		{
			CS_Hit = CS_HIT.Class;
		}
		static ConstructorHelpers::FClassFinder<UCameraShake> CS_SWIFTSLASH(TEXT("/Game/Blueprints/InGame/Roxy/CameraShake/BP_CS_SwiftSlash.BP_CS_SwiftSlash_C"));
		{
			CS_SwiftSlash = CS_SWIFTSLASH.Class;
		}
		static ConstructorHelpers::FClassFinder<UCameraShake> CS_FEVERSTART(TEXT("/Game/Blueprints/InGame/Roxy/CameraShake/BP_CS_FeverStart.BP_CS_FeverStart_C"));
		{
			CS_FeverStart = CS_FEVERSTART.Class;
		}
	}

	// 파티클 시스템 ///////////////////////////////////////////////////
	PC_ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComponent");

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_SWORDPARTICLE(TEXT("/Game/Resources/Roxy/Particle/Eff_Slash/Eff_P_New_OnlyParticle.Eff_P_New_OnlyParticle"));
	if (PS_SWORDPARTICLE.Succeeded())
	{
		PS_SwordParticle = PS_SWORDPARTICLE.Object;
	}

	// 사운드 ///////////////////////////////////////////////////
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_CRITICAL(TEXT("/Game/Resources/Sound/Roxy/Effect/SC_Roxy_Attack_Critical.SC_Roxy_Attack_Critical"));
	if (SC_CRITICAL.Succeeded())
	{
		SC_Critical = SC_CRITICAL.Object;
	}

	// 캐릭터 라이트 ///////////////////////////////////////////////////
	RectLight = CreateDefaultSubobject<URectLightComponent>("RectLightComponent");
	RectLight->SetRelativeLocationAndRotation(FVector(70.0f, 0.0f, 20.0f), FRotator(180.0f, 130.0f, 00.0f));
	RectLight->Intensity = 50.0f;

	////데이터 테이블///////////////////////////////////////////////////////////////////
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Roxy(TEXT("/Game/DataTable/DT_Roxy.DT_Roxy"));
	if (DT_Roxy.Succeeded())
	{
		DataTable = DT_Roxy.Object;
	}

	// UI ////////////////////////////////////////////////////////////
	static ConstructorHelpers::FClassFinder<URoxyWidget> UI_Roxy(TEXT("/Game/Blueprints/InGame/Roxy/BP_Roxy_Widget.BP_Roxy_Widget_C"));
	if (UI_Roxy.Succeeded())
	{
		WidgetTemplate = UI_Roxy.Class;
	}

	// 환영검 /////////////////////////////////////////////////////
// 	static ConstructorHelpers::FClassFinder<AGhostSword> GHOSTSWORD(TEXT("/Game/Blueprints/InGame/Roxy/BP_GhostSword.BP_GhostSword_C"));
// 	if (GHOSTSWORD.Succeeded())
// 	{
// 		GhostSword = GHOSTSWORD.Class;
// 	}
	
	// 콜리전 세팅
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(false); // 서로 블락된 두 오브젝트가 부딛히면 Hit 이벤트를 발생시킬지에 대한 여부
	GetCapsuleComponent()->SetCapsuleHalfHeight(105.f);
	GetCapsuleComponent()->SetCapsuleRadius(45.f);
	GetCapsuleComponent()->SetCollisionProfileName("RoxyCharacter");

	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -105.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));


	// 무기 콜리전
	WeaponCapsule = CreateDefaultSubobject<UCapsuleComponent>("Weapon");
	WeaponCapsule->SetupAttachment(GetMesh(), TEXT("Sword_CTRL"));
	WeaponCapsule->SetCapsuleSize(40.0f, 120.0f);
	WeaponCapsule->SetRelativeLocationAndRotation(FVector(100.0f, 0.0f, 0.0f), FRotator(90.0f, 0.0f, 0.0f));
	WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCapsule->SetCollisionProfileName("RoxyAttack");
	WeaponCapsule->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;


	// ForwardAttackCollision 콜리전
	ForwardAttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ForwardAttackCollision"));
	ForwardAttackCollision->SetBoxExtent(FVector(50.f, 70.f, 100.f));
	ForwardAttackCollision->SetupAttachment(GetCapsuleComponent());
	ForwardAttackCollision->SetRelativeLocation(FVector(100.f, 0.f, 0.f));
	ForwardAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ForwardAttackCollision->SetCollisionProfileName("RoxyAttack");
	ForwardAttackCollision->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	
	// 검집 콜리전
	SheathCapsule = CreateDefaultSubobject<UCapsuleComponent>("Sheath");
	SheathCapsule->SetupAttachment(GetMesh(), TEXT("Sheath_CTRL"));
	SheathCapsule->SetCapsuleSize(40.0f, 120.0f);
	SheathCapsule->SetRelativeLocationAndRotation(FVector(120.0f, 0.0f, 0.0f), FRotator(90.0f, 0.0f, 0.0f));
	SheathCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SheathCapsule->SetCollisionProfileName("RoxyAttack");
	SheathCapsule->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;


	// Don't rotate when the controller rotates. Let that just affect the camera. 
	bUseControllerRotationPitch = false; // 마우스는 Yaw 회전값밖에 인식 못함.
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	// HpComponent
	HpComponent = CreateDefaultSubobject<UHpComponent>(TEXT("HpComponent"));

	// UltimateComponent
	UltimateComponent = CreateDefaultSubobject<UUltimateComponent>(TEXT("UltimateComponent"));


	// CharacterMovement()
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 750.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->AirControl = 1.f;
	GetCharacterMovement()->GravityScale = 3.3f;
	

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<UPN_SpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 60.f));
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->TargetOffset = FVector(0.f, 0.f, 0.f);
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 80.f);
	_socketOffsetOrigin = CameraBoom->SocketOffset;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 10.f;
	CameraBoom->CameraLagMaxDistance = 100.f;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraRotationLagSpeed = 1000.f;


	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UPN_CameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, UPN_SpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	_cameraLocOrigin = FVector(0.f, 0.f, 0.f);
	CameraBoom->SetCameraLocation(GetFollowCamera(), _cameraLocOrigin, 10.f);
	

	AutoPossessPlayer = TEnumAsByte<EAutoReceiveInput::Type>(EAutoReceiveInput::Player0);
}

/* --------------------------------- 스탯 --------------------------------- */
void ARoxyCharacter::SetAddedMaxHp()
{
	LOG(Warning, "Added HP : %f", GameInstance->GetAddedDamage());

	HpComponent->AddMaxHp(GameInstance->GetAddedMaxHp());

	_addedHPCount += 1;
	WidgetInstance->OnAddedHP();
}

void ARoxyCharacter::SetAddedMoveSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed += GameInstance->GetAddedMoveSpped();
}

void ARoxyCharacter::SetCriticalHitMultiply()
{
	_criticalHitMultiply = GameInstance->GetCriticalHitMultiply();
}

void ARoxyCharacter::SetAddedDamage()
{
	LOG(Warning, "Added Damaged : %f", GameInstance->GetAddedDamage());

	_addedDamage = GameInstance->GetAddedDamage();

	_bIsAddedDamage = true;

	_addedDamageCount += 1;
	WidgetInstance->OnAddedDamage();
}

void ARoxyCharacter::SetDecreasedCoolTimePercent()
{
	LOG(Warning, "Decreased CoolTimePercent : %f", GameInstance->GetDecreasedCoolTimePercent());

	_decreasedCoolTimePercent = GameInstance->GetDecreasedCoolTimePercent();

	_decreasedCoolTimeCount += 1;
	WidgetInstance->OnDereasedCoolTime();
}

/* --------------------------------- 엔딩 데이터 --------------------------------- */
void ARoxyCharacter::SetNumOfStep()
{
	//LOG(Warning, "Ending Data : Step Num Count");
	GameInstance->SetNumOfStep();
}

void ARoxyCharacter::SetUseBasicAttack()
{
	//LOG(Warning, "Ending Data : Use Basic Attack Count");
	GameInstance->SetUseBasicAttack();
}

void ARoxyCharacter::SetUseSkill()
{
	//LOG(Warning, "Ending Data : Use Skill Count");
	GameInstance->SetUseSkill();
}

void ARoxyCharacter::SetNumOfHit()
{
	//LOG(Warning, "Ending Data : Hit Num Count");
	GameInstance->SetNumOfHit();
}

void ARoxyCharacter::SetNumOfHitBlow(int32 value)
{
	//LOG(Warning, "Ending Data : Get HitBlow Num");
	GameInstance->SetNumOfHitBlow(value);
}

void ARoxyCharacter::SetNumOfDamaged()
{
	//LOG(Warning, "Ending Data : Damaged Num Count");
	GameInstance->SetNumOfDamaged();
}

void ARoxyCharacter::SetNumOfFaint()
{
	//LOG(Warning, "Ending Data : Faint Num Count");
	GameInstance->SetNumOfFaint();
}

void ARoxyCharacter::SetNumOfAddicted()
{
	//LOG(Warning, "Ending Data : Addicted Num Count");
	GameInstance->SetNumOfAddicted();
}

void ARoxyCharacter::SetMissKick()
{
	LOG(Warning, "Ending Data : Miss Kick Num Count");
	GameInstance->SetMissKick();
}

/* ------------------------------------------------------------------------- */

TArray<FHitResult> ARoxyCharacter::GetHitResults(FVector _leapAttack_StartVec, FVector _leapAttack_EndVec, float SphereRadius)
{
	TArray<FHitResult> hitResults;

	FCollisionQueryParams Params(NAME_None, false, this);
	GetWorld()->SweepMultiByChannel(
		hitResults,
		_leapAttack_StartVec,
		_leapAttack_EndVec,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(SphereRadius),
		Params);

	return hitResults;
}

void ARoxyCharacter::LookOnNearbyEnemy()
{
	// 우선은 화면에 렌더되고 있는 적들을 받아온다
	TArray<AEnemyAI*> nearbyEnemys = InGameManager->GetAllEnemys();

	// 받아온 적들의 거리를 재서 일정 거리보다 가까운 적을 타겟으로 삼는다
	for (auto i : nearbyEnemys) {
		if (FVector::Distance(i->GetActorLocation(), GetActorLocation()) < _radiusForFindingEnemy) {
			_nearbyEnemy = i;
			break;
		}
	}
	// +++++가장 가까이 있는 적을 최종 타겟으로 삼아야 하지만 귀찮아서 패스한다

	if (_nearbyEnemy) {

		FRotator currentRotation = GetActorRotation();
		FRotator newPlayerRotation = FRotator(currentRotation.Pitch,
			UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), _nearbyEnemy->GetActorLocation()).Yaw,
			currentRotation.Roll);
		SetActorRotation(newPlayerRotation);

		_swiftApproachLoc = _nearbyEnemy->GetActorLocation() + (_nearbyEnemy->GetActorForwardVector() * 200.0f);

		_nearbyEnemy = nullptr;
	}
}

// 스킬 쿨타임 관리 함수
void ARoxyCharacter::SkillCoolApply(const ESkillType& skillNum)
{
	switch (skillNum)
	{
	case ESkillType::ESkill1: // 일섬
		if (!_bIsCoolApply_Skill1 && _coolTime_Skill1 != 0) {
			_bIsCoolApply_Skill1 = true;
			GetWorldTimerManager().SetTimer(Skill1_CoolTimer, Skill1_Delegate, _coolTime_Skill1 / (1 - _decreasedCoolTimePercent), false);
		}
		else {
			_bIsCoolApply_Skill1 = false;
		}
		break;
	case ESkillType::ESkill2: // 연참
		if (!_bIsCoolApply_Skill2 && _coolTime_Skill2 != 0) {
			_bIsCoolApply_Skill2 = true;
			GetWorldTimerManager().SetTimer(Skill2_CoolTimer, Skill2_Delegate, _coolTime_Skill2 / (1 - _decreasedCoolTimePercent), false);
		}
		else {
			_bIsCoolApply_Skill2 = false;
		}
		break;
	case ESkillType::ESkill3: // 환영검
		if (_currentStorableNum_GhostSword < _storableMaxNum_GhostSword) {
			_bIsCoolApply_Skill3 = true;
			GetWorldTimerManager().SetTimer(Skill3_CoolTimer, Skill3_Delegate, _coolTime_Skill3 / (1 - _decreasedCoolTimePercent), true);
			LOG(Warning, "%f", _decreasedCoolTimePercent );
		}
	case ESkillType::ESkill4: // 발차기
		if (!_bIsCoolApply_Skill4 && _coolTime_Skill4 != 0) {
			_bIsCoolApply_Skill4 = true;
			GetWorldTimerManager().SetTimer(Skill4_CoolTimer, Skill4_Delegate, _coolTime_Skill4 / (1 - _decreasedCoolTimePercent), false);
		}
		else {
			_bIsCoolApply_Skill4 = false;
		}
		break;
	case ESkillType::ESkill5: // 대시
		if (!_bIsCoolApply_Skill5 && _coolTime_Skill5 != 0) {
			_bIsCoolApply_Skill5 = true;
			GetWorldTimerManager().SetTimer(Skill5_CoolTimer, Skill5_Delegate, _coolTime_Skill5 / (1 - _decreasedCoolTimePercent), false);
		}
		else {
			_bIsCoolApply_Skill5 = false;
		}
		break;
	default:
		break;
	}
}


void ARoxyCharacter::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && !_weaponOverlapActor.Contains(OtherActor) && !_forwardAttackTargets.Contains(OtherActor) /*&& !_bIsPierceEnemyCheck*/)
	{
		AGruntAI* enemy_grunt = Cast<AGruntAI>(OtherActor);
		ARevivalAI* enemy_boss = Cast<ARevivalAI>(OtherActor);

		if (enemy_grunt && !enemy_grunt->GetIsDie() && !enemy_grunt->GetIsInvinci())
		{
			//////////////////////////////////////////////////
			/*----------------기본 공격---------------------*/
			if (_bIsAttacking)
			{
				APlayerController* PlayerController = Cast<APlayerController>(GetController());

				_weaponOverlapActor.Add(OtherActor);

				// 적에게 맞춘 공격 횟수 카운트
				SetNumOfHit();

				// 불릿타임, 카메라쉐이크, 화면효과
				if (enemy_grunt->GetIsBossMonster() && (enemy_grunt->GetHpComponent()->GetHp() <= _finalDamageAmount || enemy_grunt->GetHpComponent()->GetHp() <= KINDA_SMALL_NUMBER))
				{
					// 보스 막타시
					PlayCameraShake(CS_Attack3);
					StartBulletTime(_lastAttack_BulletTime_dilation, _lastAttack_BulletTime_duration, _currentTargetArmLength - 450.f, _cameraLocOrigin);
					FollowCamera->OnPostProcess(0.15f);
				}
				else
				{
					if (_currentCombo == 1 && !_bIsBulletTime) {
						PlayCameraShake(CS_Attack1);
						StartBulletTime(_attack1_BulletTime_dilation, _attack1_BulletTime_duration, _currentTargetArmLength - 100.f, _cameraLocOrigin);
						FollowCamera->OnPostProcess(0.08f);
					}
					else if (_currentCombo == 2 && !_bIsBulletTime) {
						PlayCameraShake(CS_Attack2);
						StartBulletTime(_attack2_BulletTime_dilation, _attack2_BulletTime_duration, _currentTargetArmLength - 100.f, _cameraLocOrigin);
						FollowCamera->OnPostProcess(0.08f);
					}
					else if (_currentCombo == 3 && !_bIsBulletTime) {
						PlayCameraShake(CS_Attack3);
						StartBulletTime(_attack3_BulletTime_dilation, _attack3_BulletTime_duration, _currentTargetArmLength - 100.f, _cameraLocOrigin);
						FollowCamera->OnPostProcess(0.08f);
					}
				}

				// 적 밀치기
				if (enemy_grunt && (!enemy_grunt->GetIsSpecialAttack() && !enemy_grunt->GetIsSpecialGrunt() && !enemy_grunt->GetIsBuffGrunt() && !enemy_grunt->GetIsBossMonster()))
				{
					enemy_grunt->GetCharacterMovement()->Velocity = FVector::ZeroVector;

					if (_currentCombo == 1 || _currentCombo == 2)
					{
						FVector pushVec = GetActorForwardVector() * 1500.f;
						enemy_grunt->GetCharacterMovement()->Velocity = pushVec;
					}
					else if (_currentCombo == 3)
					{
						FVector pushVec = GetActorForwardVector() * 2500.f;
						enemy_grunt->GetCharacterMovement()->Velocity = pushVec;
					}
				}	

				// Take Damage
				if (!enemy_grunt->GetIsDie() || !enemy_grunt->GetIsInvinci())
				{
					if (_currentCombo == 1 || _currentCombo == 2 || _currentCombo == 3)
					{
						InflictDamage(ESkillType::ESkill0, enemy_grunt);
					}
				}

				// 공격 효과음 UI
				if (!enemy_grunt->GetIsDie() || !enemy_grunt->GetIsInvinci())
				{
					// 크리티컬 효과음과 동시에 표시하지 않음
					if (!_bIsCritical)
					{
						if (_currentCombo == 1)
						{
							OnAttackUI(1);
						}
						else if (_currentCombo == 2)
						{
							OnAttackUI(2);
						}
						else if (_currentCombo == 3)
						{
							OnAttackUI(3);
						}
					}
					else
					{
						WidgetInstance->OnAttack_Critical();
					}
				}

				//LOG_SCREEN("Hit Check");
			}

			///////////////////////////////////////////////////
			/*----------------- Kick ------------------*/
			else if (_bIsKicking)
			{
				//LOG(Warning, "Kick Check");

				_forwardAttackTargets.Add(enemy_grunt);

				// 적에게 맞춘 공격 횟수 카운트
				SetNumOfHit();

				// 밀치기
				if (enemy_grunt && enemy_boss == nullptr)
				{
					// 엘리트 몬스터는 발차기로 넉백모드를 만들 수(공격을 캔슬할 수) 있지만 밀리고 넘어지진 않음.
					// 버프 몬스터는 발차기로 넉백모드를 만들면서 밀치고 넘어트릴 수도 있음.
					if (!enemy_grunt->GetIsSpecialGrunt())
					{
						//LOG(Warning, " Push Enemy");
						enemy_grunt->GetCharacterMovement()->Velocity = FVector::ZeroVector;

						FVector pushVec = GetActorForwardVector() * 15000.f;
						enemy_grunt->GetCharacterMovement()->Velocity = pushVec;
					}

					enemy_grunt->SetGruntStateMode(EGruntStateMode::SM_KNOCKBACK);
				}

				// 불릿타임, 카메라쉐이크, 화면효과
				if (enemy_grunt->GetIsBossMonster() && (enemy_grunt->GetHpComponent()->GetHp() <= _finalDamageAmount || enemy_grunt->GetHpComponent()->GetHp() <= KINDA_SMALL_NUMBER))
				{
					// 보스 막타시
					PlayCameraShake(CS_Attack3);
					StartBulletTime(_lastAttack_BulletTime_dilation, _lastAttack_BulletTime_duration, _currentTargetArmLength - 450.f, _cameraLocOrigin);
					FollowCamera->OnPostProcess(0.15f);
				}
				else
				{
					if (!_bIsBulletTime)
					{
						StartBulletTime(_kick_BulletTime_dilation, _kick_BulletTime_duration, _currentTargetArmLength - 150.f, _cameraLocOrigin);
						CameraBoom->SetTargetArmLength(_currentTargetArmLength + 200.f, 10.f);
						FollowCamera->OnPostProcess(0.08f);
					}
				}
				
				// 공격 효과음 UI
				if (!enemy_grunt->GetIsDie() || !enemy_grunt->GetIsInvinci())
				{
					if (enemy_grunt->GetIsSpecialGrunt() && enemy_grunt->GetIsEliteSpecialAttack())
					{
						// 엘리트 몬스터의 특수패턴 캔슬시 더 강한 효과음
						WidgetInstance->OnKickCancle();
					}
					else
					{
						// 크리티컬 효과음과 동시에 표시하지 않음
						if (!_bIsCritical)
						{
							WidgetInstance->OnKick();
						}
						else
						{
							WidgetInstance->OnAttack_Critical();
						}
					}
				}

				// Take Damage
				InflictDamage(ESkillType::ESkill4, enemy_grunt);
			}

			///////////////////////////////////////////////////
			/*----------------- Pierce ------------------*/
			else if (_bIsPierce)
			{
				//_bIsPierceEnemyCheck = true;

				PierceDamageCheck(enemy_grunt);

				_forwardAttackTargets.Add(enemy_grunt);

				// 적에게 맞춘 공격 횟수 카운트
				SetNumOfHit();

				if (enemy_grunt->GetIsBossMonster() && (enemy_grunt->GetHpComponent()->GetHp() <= _finalDamageAmount || enemy_grunt->GetHpComponent()->GetHp() <= KINDA_SMALL_NUMBER))
				{
					// 보스 막타시
					PlayCameraShake(CS_Attack3);
					StartBulletTime(_lastAttack_BulletTime_dilation, _lastAttack_BulletTime_duration, _currentTargetArmLength - 450.f, _cameraLocOrigin);
					FollowCamera->OnPostProcess(0.15f);
				}

				// 공격 효과음 UI
				if (!enemy_grunt->GetIsDie() || !enemy_grunt->GetIsInvinci())
				{
					// 크리티컬 효과음과 동시에 표시하지 않음
					if (!_bIsCritical)
					{
						WidgetInstance->OnSlash();
					}
					else
					{
						WidgetInstance->OnAttack_Critical();
					}
				}
			}

			///////////////////////////////////////////////////////
			/*------------------SwiftSlash------------------*/
			else if (_bIsSwiftianSlash)
			{
				_weaponOverlapActor.Add(OtherActor);

				if (enemy_grunt->GetIsBossMonster() && enemy_grunt->GetHpComponent()->GetHp() <= _finalDamageAmount)
				{
					// 보스 막타시
					PlayCameraShake(CS_Attack3);
					StartBulletTime(_lastAttack_BulletTime_dilation, _lastAttack_BulletTime_duration, _currentTargetArmLength - 450.f, _cameraLocOrigin);
					FollowCamera->OnPostProcess(0.15f);
				}

				FDamageEvent DamageEvent;
				if (enemy_grunt->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling ||
					enemy_grunt->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying && enemy_grunt && !enemy_boss) {
					enemy_grunt->SetGruntStateMode(EGruntStateMode::SM_KNOCKBACK);
				}
				InflictDamage(ESkillType::ESkill2, enemy_grunt);
			}
		}

		
		if (OtherActor->GetClass()->ImplementsInterface(UHitable::StaticClass()))
		{
			if (_bIsKicking)
			{
				IHitable* hitableObject = Cast<IHitable>(OtherActor);
				hitableObject->HitableTakeDamage();

				_bIsVMKicking = true;

				LOG(Warning, "Kick Vending Machine");

				if (!_bIsBulletTime)
				{
					//StartBulletTime(_kick_BulletTime_dilation, _kick_BulletTime_duration, _currentTargetArmLength - 150.f, _cameraLocOrigin);
					CameraBoom->SetTargetArmLength(_currentTargetArmLength + 200.f, 10.f);
					FollowCamera->OnPostProcess(0.08f);
				}
			}
		}
	}
}

void ARoxyCharacter::OnCollisionBeginHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		AStaticMeshActor* staticMesh = Cast<AStaticMeshActor>(OtherActor);
		
		if (staticMesh)
		{
			if (_bIsPierce) // 일섬시 발전기, 오브젝트와 부딪히면 멈추게
			{
				GetCharacterMovement()->Velocity = FVector::ZeroVector;
			}
			else if (_bIsKnockBack) // 넉백 상태 중 다른 물체와 부딪힐 경우 기절
			{
				FDamageEvent damageEvent;
				TakeDamage(_knockbackDamage, damageEvent, GetController(), this);

				SetFaint(2.0f);
			}
		}
	}
}

/* ---------------------------------------- Bullet Time ---------------------------------------- */
void ARoxyCharacter::StartBulletTime(float dilation, float duration, float targetArmLength, FVector cameraLoc)
{
	_bIsBulletTime = true;
	CameraBoom->bEnableCameraLag = false;
	CameraBoom->TargetArmLength = targetArmLength;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), dilation);


	FLatentActionInfo latentAction;
	latentAction.CallbackTarget = this;
	latentAction.ExecutionFunction = TEXT("EndBulletTime");
	latentAction.UUID = GetUniqueID();
	latentAction.Linkage = 0;
	UKismetSystemLibrary::Delay(GetWorld(), duration, latentAction);
}

void ARoxyCharacter::EndBulletTime()
{
	_bIsBulletTime = false;

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->TargetArmLength = _currentTargetArmLength;
	CameraBoom->TargetOffset = FVector(0.f, 0.f, 0.f);

	OnBulletTimeEnd.ExecuteIfBound();
	OnBulletTimeEnd.Unbind();
}

/* ---------------------------------------- Widget ---------------------------------------- */
void ARoxyCharacter::SetActiveRoxyUI(bool value)
{
	if (value)
	{
		if (Controller->IsPlayerController())
		{
			if (!WidgetInstance && WidgetTemplate)
				WidgetInstance = CreateWidget<URoxyWidget>(GetWorld(), WidgetTemplate);

			if (!WidgetInstance->IsInViewport())
				WidgetInstance->AddToViewport();
		}
		else
		{
			LOG(Warning, "Is Not Possess Player Controller");
		}
	}
	else
	{
		if (WidgetInstance)
		{
			if (WidgetInstance->IsInViewport())
			{
				WidgetInstance->RemoveFromViewport();
			}
		}
	}
}

/* ---------------------------------------- Skill ----------------------------------------*/
void ARoxyCharacter::SetEnableSkill(bool attackEnable /*= true*/, bool pierceEnable /*= true*/, bool kickEnable /*= true*/, bool swiftSlashEnable /*= true*/, bool evadeEnable /*= true*/, bool GhostSwordEnable /*= true*/)
{
	_bCanAttack = attackEnable;
	_bCanPierce = pierceEnable;
	_bCanKick = kickEnable;
	_bCanSwiftSlash = swiftSlashEnable;
	_bCanEvade = evadeEnable;
	_bCanGhostSword = GhostSwordEnable;
}

void ARoxyCharacter::ChangeSwordColor(float deltaTime)
{
	if (!_bCanChangeSwordColor) return;

	// 스킬 시전 시
	//_swordColor = UKismetMathLibrary::FInterpTo(0.0f, 1.0f, deltaTime, 110.0f);
	_swordColor += 0.01f;
	GetMesh()->SetScalarParameterValueOnMaterials(FName("SwordColor"), _swordColor);

	//LOG(Warning, "Sword Material Color : %f", _swordColor);

	if (_swordColor >= 1.0f)
	{
		_bCanChangeSwordColor = false;
		
		_bCanChangeSwordColorOrigin = true;
	}
}

void ARoxyCharacter::ChangeSwordColorOrigin(float deltaTime)
{
	if (!_bCanChangeSwordColorOrigin) return;

	// 스킬 종료 시
	//_swordColor = UKismetMathLibrary::FInterpTo(1.0f, 0.0f, deltaTime, 110.0f);
	_swordColor -= 0.02f;
	GetMesh()->SetScalarParameterValueOnMaterials(FName("SwordColor"), _swordColor);

	//LOG(Warning, "Sword Material Color : %f", _swordColor);

	if (_swordColor <= 0.0f)
	{
		_bCanChangeSwordColorOrigin = false;
	}
}

/* ---------------------------------------- Ultimate ---------------------------------------- */
void ARoxyCharacter::SetUsableUltimate()
{
	_bCanUseUltimate = true;
}

void ARoxyCharacter::UltimateStart()
{
	if (!_bCanUseUltimate) return;

	
	UltimateComponent->SetActiveUltimateComponent(false);
	UltimateComponent->SetInitUltimateGauge();

	GetWorldTimerManager().SetTimer(UltimateDurationTimer, this, &ARoxyCharacter::UltimateEnd, _ultimateDuration, false);
}
	
void ARoxyCharacter::UltimateEnd()
{
	
	_bCanUseUltimate = false;
	_bIsUltimateOn = false;

	UltimateComponent->SetActiveUltimateComponent(true);
}

/* ---------------------------------------- Basic Combo Attack ---------------------------------------- */
void ARoxyCharacter::ComboAttack()
{
	if (!_bCanAttack) return;

	const float KeyValue_F = GetInputAxisValue("MoveForward");
	const float KeyValue_R = GetInputAxisValue("MoveRight");

	if (KeyValue_F || KeyValue_R)
	{ // 입력 방향대로 공격이 나가게 하기
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction_F = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * KeyValue_F;
		const FVector Direction_R = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * KeyValue_R;

		FVector DirectionResult = Direction_F + Direction_R;
		DirectionResult.Z = GetActorForwardVector().Z;

		SetActorRotation(DirectionResult.Rotation());
	}

	if (_bIsAttacking && _bCanInputNextCombo)
	{
		/*FMath::IsWithinInclusive<int32>(_currentCombo, 1, _maxCombo);*/

		if (_bCanNextCombo && _currentCombo < _maxCombo)
		{
			_bCanInputNextCombo = false;

			_currentCombo = FMath::Clamp<int32>(_currentCombo + 1, 1, _maxCombo);
			RoxyAnim->JumpToAttackMontageSection(_currentCombo);

			// 기본 공격 횟수 카운트
			SetUseBasicAttack();

			if (_weaponOverlapActor.Num() != 0)
			{
				// 다음콤보로 넘어갈때 배열 비워주깅
				_weaponOverlapActor.RemoveAll([](AActor* actor) {return true; });
			}
		}
	}
	else if (!_bIsAttacking)
	{
		SetEnableInput(false, true, true);

		_bIsAttacking = true;
		_bIsDealing = true;

		//_bCanSwiftSlash = false;
		_bCanGhostSword = false;

		_currentCombo = FMath::Clamp<int32>(_currentCombo + 1, 1, _maxCombo);
		AttackStartComboState();

		WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		RoxyAnim->PlayAttackMontage();
		RoxyAnim->JumpToAttackMontageSection(_currentCombo);

		// 기본 공격 횟수 카운트
		SetUseBasicAttack();
	}
}

void ARoxyCharacter::OnAttackEnded()
{
	if (!_bIsAttacking) return;
	CHECK(_currentCombo > 0);
	 
	_bIsDealing = false;
	_bIsAttacking = false;

	//_bCanSwiftSlash = true;
	_bCanGhostSword = true;
	
	if (_bIsKicking)
	{
		KickEnd();
	}

	SetEnableInput();

	AttackEndComboState();

	SetLookOnNearbyEnemy(false);

	InitializeVelocity();

	ClearWeaponOverlapActor();

	_bIsCritical = false;

	WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SheathCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ForwardAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ARoxyCharacter::CanInputNextCombo()
{
	_bCanInputNextCombo = true;
}

void ARoxyCharacter::AttackComboCheck()
{
	_bCanNextCombo = false;

	//if (_bIsComboInputOn)
	//{
	//	AttackStartComboState();
	//	RoxyAnim->JumpToAttackMontageSection(_currentCombo);
	//}

	ForwardAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OnAttackEnded();
	
	_bCanInputNextCombo = false;

	_bIsCritical = false;
}

void ARoxyCharacter::ClearWeaponOverlapActor()
{
	if (_weaponOverlapActor.Num() != 0)
	{
		//LOG(Warning, "Clear Weapon Overlap Actor");
		_weaponOverlapActor.RemoveAll([](AActor* actor) {return true; });
	}

	if (_forwardAttackTargets.Num() != 0)
	{
		//LOG(Warning, "Clear Forward Overlap Actor");
		_forwardAttackTargets.RemoveAll([](AActor* actor) {return true; });
	}
}

void ARoxyCharacter::OffAttackCollision()
{
	WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SheathCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ForwardAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ARoxyCharacter::AttackStartComboState()
{
	_bCanNextCombo = true;
	CHECK(FMath::IsWithinInclusive<int32>(_currentCombo, 1, _maxCombo));
	
}

void ARoxyCharacter::AttackEndComboState()
{
	_bCanInputNextCombo = false;
	_bCanNextCombo = false;
	_currentCombo = 0;
}

void ARoxyCharacter::OnAttackUI(int32 value)
{
	switch (value)
	{
	case 1: /* 1타 */
		if (_bIsAddedDamage || _bIsPierce || _bIsSwiftianSlash)
		{
			// 공격력 스탯을 올렸거나 스킬 사용 시 강타 효과음
			WidgetInstance->OnAttack_01(2);
		}
		else
		{
			WidgetInstance->OnAttack_01(1);
		}
		break;
	case 2: /* 2타 */
		if (_bIsAddedDamage || _bIsPierce || _bIsSwiftianSlash)
		{
			WidgetInstance->OnAttack_02(2);
		}
		else
		{
			WidgetInstance->OnAttack_02(1);
		}
		break;
	case 3: /* 3타 */
		if (_bIsAddedDamage || _bIsPierce || _bIsSwiftianSlash)
		{
			WidgetInstance->OnAttack_03(2);
		}
		else
		{
			WidgetInstance->OnAttack_03(1);
		}
		break;
	}
}

int32 ARoxyCharacter::GetComboAttackMaxNum()
{
	return _maxCombo;
}

void ARoxyCharacter::LastAttackEffect()
{
	// 보스 막타시
	PlayCameraShake(CS_Attack3);
	StartBulletTime(_lastAttack_BulletTime_dilation, _lastAttack_BulletTime_duration, _currentTargetArmLength - 300.f, _cameraLocOrigin);
	FollowCamera->OnPostProcess(0.08f);
}

/* ---------------- Kick ---------------- */
void ARoxyCharacter::StartKick()
{
	if (_bIsKicking || !_bCanKick || _bIsCoolApply_Skill4) return;

	//LOG(Warning, "Kick Start");

	_bIsKicking = true;

	_bCanDamaged = false;

	_bIsSkillActive_Skill4 = true;

	SetEnableInput(false, false, false);

	_bCanAttack = false;
	_bCanKick = false;
	_bCanPierce = false;
	_bCanSwiftSlash = false;
	_bCanGhostSword = false;
	_bCanEvade = false;

	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	ClearWeaponOverlapActor();

	// 공격 중간에 캔슬하고 사용할 수 있도록
	if (_bIsAttacking)
	{
		RoxyAnim->StopAttackMontage();
		AttackEndComboState();
		_bIsAttacking = false;
	}

	RoxyAnim->PlayKickMontage();

	CameraBoom->SetSocketOffset(FVector(0, 0, 60.f), 10.f);
	CameraBoom->SetTargetArmLength(_currentTargetArmLength - 100.f, 10.f);

	FCollisionResponseContainer characterCollision = GetCapsuleComponent()->GetCollisionResponseToChannels();
	characterCollision.GameTraceChannel1 = ECollisionResponse(ECR_Ignore);
	characterCollision.GameTraceChannel3 = ECollisionResponse(ECR_Ignore);
	characterCollision.GameTraceChannel4 = ECollisionResponse(ECR_Ignore);
	characterCollision.GameTraceChannel6 = ECollisionResponse(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannels(characterCollision);

	WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SheathCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ARoxyCharacter::OnKickCollision()
{
	if (!_bIsKicking) return;

	//LOG(Warning, "On Kick Collision");

	PlayCameraShake(CS_Attack4);
	CameraBoom->SetTargetArmLength(_currentTargetArmLength, 10.f);
	CameraBoom->SetSocketOffset(_socketOffsetOrigin, 10.f);

	ForwardAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// 이미 콜리전안에 들어와있는 애들이 인식이 잘 안된다 ..ㄱ-
	//TSet<AActor*> alreadyOverlappedActors;
	//ForwardAttackCollision->GetOverlappingActors(alreadyOverlappedActors);
	//
	//for (auto i : alreadyOverlappedActors)
	//{
	//	AGruntAI* grunt = Cast<AGruntAI>(i);
	//
	//	if (grunt)
	//	{
	//		_forwardAttackTargets.Add(grunt);
	//
	//		// 밀치기
	//		if (grunt && !grunt->GetIsSpecialAttack())
	//		{
	//			//LOG(Warning, " Push Enemy");
	//			grunt->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	//
	//			FVector pushVec = GetActorForwardVector() * 15000.f;
	//			grunt->GetCharacterMovement()->Velocity = pushVec;
	//
	//			grunt->SetGruntStateMode(EGruntStateMode::SM_KNOCKBACK);
	//		}
	//
	//		// 불릿타임, 카메라쉐이크, 화면효과
	//		if (!_bIsBulletTime)
	//		{
	//			StartBulletTime(_kick_BulletTime_dilation, _kick_BulletTime_duration, _currentTargetArmLength - 150.f, _cameraLocOrigin);
	//			CameraBoom->SetTargetArmLength(_currentTargetArmLength + 200.f, 10.f);
	//			FollowCamera->OnPostProcess(0.08f);
	//		}
	//
	//		// Take Damage
	//		InflictDamage(ESkillType::ESkill4, grunt);
	//	}
	//}
}

void ARoxyCharacter::KickEnd()
{
	if (!_bIsKicking) return;

	_bIsKicking = false;

	_bCanDamaged = true;

	_bIsSkillActive_Skill4 = false;

	if (_forwardAttackTargets.Num() <= 0 && !_bIsVMKicking)
	{
		// 빗나간 발차기 횟수 카운트
		SetMissKick();
	}

	if (!_bCanInteractionVM)
	{
		// 자판기 상호작용(차서 부수기)가 가능한 상황일때는 쿨이 돌지 않도록
		SkillCoolApply(ESkillType::ESkill4);
	}

	SetEnableInput();

	_bCanAttack = true;
	_bCanKick = true;
	_bCanPierce = true;
	_bCanSwiftSlash = true;
	_bCanGhostSword = true;
	_bCanEvade = true;

	_bIsCritical = false;

	_bIsVMKicking = false;

	if (_bIsPierce)
	{
		PierceEnd();
	}

	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	ClearWeaponOverlapActor();

	CameraBoom->SetTargetArmLength(_currentTargetArmLength, 5.f);

	FCollisionResponseContainer characterCollision = GetCapsuleComponent()->GetCollisionResponseToChannels();
	characterCollision.GameTraceChannel1 = ECollisionResponse(ECR_Block);
	characterCollision.GameTraceChannel3 = ECollisionResponse(ECR_Overlap);
	characterCollision.GameTraceChannel4 = ECollisionResponse(ECR_Block);
	characterCollision.GameTraceChannel6 = ECollisionResponse(ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannels(characterCollision);

	ForwardAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

/* ---------------------------------------- Pierce ---------------------------------------- */
void ARoxyCharacter::PierceStart()
{
	if (!_bCanPierce || _bIsPierce || _currentPierceNum <= 0/* || _bIsCoolApply_Skill1*/ || _bIsSkillLocked_Skill1) return;

	_bIsPierce = true;

	_bCanDamaged = false;

	_bCanAttack = false;
	_bCanKick = false;
	_bCanSwiftSlash = false;
	_bCanGhostSword = false;
	_bCanEvade = false;

	SetEnableInput(false, false, false);

	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	CameraBoom->SetTargetArmLength(_pierce_targetArmLength, 10.f);

	// 콜리전 세팅
	FCollisionResponseContainer characterCollision = GetCapsuleComponent()->GetCollisionResponseToChannels();
	characterCollision.GameTraceChannel1 = ECollisionResponse(ECR_Ignore);
	characterCollision.GameTraceChannel3 = ECollisionResponse(ECR_Ignore);
	characterCollision.GameTraceChannel4 = ECollisionResponse(ECR_Ignore);
	characterCollision.GameTraceChannel6 = ECollisionResponse(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannels(characterCollision);

	WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SheathCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ForwardAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ClearWeaponOverlapActor();
	
	// 공격 중간에 캔슬하고 사용할 수 있도록
	if (_bIsAttacking)
	{
		RoxyAnim->StopAttackMontage();
		AttackEndComboState();
		_bIsAttacking = false;
	}

	RoxyAnim->PlayPierceMontage();

	//_bIsSkillActive_Skill1 = true;

	// 스킬 사용 시 검 색깔 변화
	_bCanChangeSwordColor = true;

	_currentPierceNum -= 1;

	// 스킬 사용 횟수 카운트
	SetUseSkill();
}

void ARoxyCharacter::Pierce()
{
	if (!_bIsPierce) return;

	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	// 	const float KeyValue_F = GetInputAxisValue("MoveForward");
	// 	const float KeyValue_R = GetInputAxisValue("MoveRight");
	// 
	// 	if (KeyValue_F == 0.f && KeyValue_R == 0.f)
	// 	{
	// 		_pierceDirection = FVector(GetActorForwardVector());
	// 	}
	// 	else
// 	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction_F = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X)/* * KeyValue_F*/;
/*		const FVector Direction_R = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y)/ * * KeyValue_R* /;*/

		FVector DirectionResult = Direction_F/* + Direction_R*/;

		SetActorRotation(DirectionResult.Rotation());

		_pierceDirection = DirectionResult;

/*	}*/
	_pierceDirection.Normalize();

	FVector pierceVelocity = _pierceDirection * 100.f * _pierceSpeed; // 100을 곱하는 이유는 m/s를 만들기 위해서!
	pierceVelocity.Z = GetActorForwardVector().Z;

	FHitResult hitResult;
	FHitResult* hitResultPtr = &hitResult;

	GetCharacterMovement()->GroundFriction = 1.f;
	GetCharacterMovement()->Velocity = pierceVelocity;

	PlayCameraShake(CS_PierceStart);
	FollowCamera->ChangeRadialBlurMat(2);
	FollowCamera->OnPostProcess(0.1);

	//OnAttackUI(2);

	// 콜리전 세팅
	ForwardAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ARoxyCharacter::FillPierceNum()
{
	if (_currentPierceNum == _maxPierceNum) return;

	_currentPierceNum = FMath::Clamp(_currentPierceNum += 1, 0, _maxPierceNum);
	if(_currentPierceNum < _maxPierceNum)
		GetWorldTimerManager ().SetTimer (_pierceTimer, this, &ARoxyCharacter::FillPierceNum, 2.0f, false);
}

void ARoxyCharacter::PierceDamageCheck(AEnemyAI * enemy)
{
	if(!_bIsPierce) return;

	//GetCharacterMovement()->Velocity = FVector::ZeroVector;

	// 충돌 시 가장 가까운 적을 원샷 원킬
	//TArray<AActor*> overlappedEnemys;
	//FVector playerLoc = GetActorLocation ();
	//ForwardAttackCollision->GetOverlappingActors(overlappedEnemys, AEnemyAI::StaticClass());
	//overlappedEnemys.Add(enemy);
	//// 콜리전 꺼주기
	//ForwardAttackCollision->SetCollisionEnabled (ECollisionEnabled::NoCollision);
	//
	//float distanceTemp = FVector::Distance (overlappedEnemys[0]->GetActorLocation (), GetActorLocation ());
	//int indexTemp = 0;
	//
	//for (int i = 0; i < overlappedEnemys.Num() - 1; i++)
	//{
	//	float distanceFromPlayer = distanceTemp;
	//	float distanceFromPlayerNext = FVector::Distance (overlappedEnemys[i + 1]->GetActorLocation (), GetActorLocation ());
	//	if (distanceFromPlayer - distanceFromPlayerNext > 0)
	//	{
	//		distanceTemp = distanceFromPlayerNext;
	//		indexTemp = i;
	//	}
	//}

	//AGruntAI* grunt = Cast<AGruntAI>(overlappedEnemys[indexTemp]);

	InflictDamage(ESkillType::ESkill1, enemy);

	FollowCamera->OnPostProcess(0.08f);	
	
	if ((enemy->GetHpComponent()->IsDie()) /*&& !_bResetPierceCool*/)
	{ 
		//_bResetPierceCool = true;
		FillPierceNum();

		_remainingPierceTime = GetWorldTimerManager().GetTimerRemaining(_pierceTimer);
		if (_remainingPierceTime < 2.0f)
		{
			_remainingPierceTime = 2.0f;
		}
	}

	PlayCameraShake(CS_Pierce);

	//LOG(Warning, "Pierce Check");
}

void ARoxyCharacter::PierceStop()
{
	if (!_bIsPierce) return;

	_bCanDamaged = true;

	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	GetCharacterMovement()->GroundFriction = 8.f;

	// 콜리전 세팅
	FCollisionResponseContainer characterCollision = GetCapsuleComponent()->GetCollisionResponseToChannels();
	characterCollision.GameTraceChannel1 = ECollisionResponse(ECR_Block);
	characterCollision.GameTraceChannel3 = ECollisionResponse(ECR_Overlap);
	characterCollision.GameTraceChannel4 = ECollisionResponse(ECR_Block);
	characterCollision.GameTraceChannel6 = ECollisionResponse(ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannels(characterCollision);

	// 콜리전 꺼주기
	ForwardAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ClearWeaponOverlapActor();

	FollowCamera->OffPostProcess();
}

void ARoxyCharacter::PierceEnd()
{
	if (!_bIsPierce) return;

	SetEnableInput(true, true, true);

	CameraBoom->SetTargetArmLength(_currentTargetArmLength, 10.f);
	FollowCamera->SetRadialBlurMatParam();

	//CHECK(_bIsSkillActive_Skill1);
	//_bIsSkillActive_Skill1 = false;
	//
	//if (!_bResetPierceCool)
	//{
	//	SkillCoolApply (ESkillType::ESkill1);
	//}
	//else
	//{
	//	_bResetPierceCool = false;
	//}

	_bIsPierceEnemyCheck = false;

	_bIsPierce = false;

	_bCanAttack = true;
	_bCanKick = true;
	_bCanEvade = true;
	_bCanSwiftSlash = true;
	_bCanGhostSword = true;

	_bIsStiffen = false;

	_bIsCritical = false;

	if(_currentPierceNum < _maxPierceNum && !GetWorldTimerManager().IsTimerActive(_pierceTimer))
		GetWorldTimerManager ().SetTimer(_pierceTimer, this, &ARoxyCharacter::FillPierceNum, 2.0f, false);
}

/* ---------------------------------------- Swift Attack ---------------------------------------- */
void ARoxyCharacter::SwiftSlashStart()
{
	if (!_bCanSwiftSlash || _bIsSwiftianSlash || _bIsCoolApply_Skill2 || _bIsSkillLocked_Skill2) return;

	// 대시중이었다면 마무리 함수를 호출해준다.
	if (_bIsEvasion)
	{
		StopEvade();
		EvadeEnd();
	}

	// 연참 중 데미지 무시
	FCollisionResponseContainer characterCollision = GetCapsuleComponent()->GetCollisionResponseToChannels();
	characterCollision.GameTraceChannel1 = ECollisionResponse(ECR_Ignore);
	characterCollision.GameTraceChannel3 = ECollisionResponse(ECR_Ignore);
	characterCollision.GameTraceChannel4 = ECollisionResponse(ECR_Ignore);
	characterCollision.GameTraceChannel6 = ECollisionResponse(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannels(characterCollision);

	// 공격할 대상이 있을 떄만 스킬이 사용된다.
	TArray<FHitResult> hitResults;
	FVector startVec = GetActorLocation();
	FVector endVec = startVec + GetActorForwardVector() * _swiftSlash_influenceRangeRadius;
	FCollisionQueryParams param(NAME_None, false, this);

	bool result = GetWorld()->SweepMultiByProfile(hitResults,
									startVec,
									endVec, FQuat::Identity,
									TEXT("BlockOnlyPawn"),
									FCollisionShape::MakeSphere(_swiftSlash_influenceRangeRadius * 2),
									param);

	if (result)
	{
		for (auto i : hitResults)
		{
			AEnemyAI* enemys = Cast<AEnemyAI>(i.GetActor());
			if (enemys)
			{
				_swiftSlashEnemys.Push(enemys);
			}
		}

		// 공격 중간에 캔슬하고 사용할 수 있도록
		if (_bIsAttacking)
		{
			RoxyAnim->StopAttackMontage();
			AttackEndComboState();
			_bIsAttacking = false;
		}

		_bIsSwiftianSlash = true;

		_bCanDamaged = false;

		_bCanSwiftSlash = false;
		_bCanAttack = false;
		_bCanKick = false;
		_bCanPierce = false;
		_bCanGhostSword = false;
		_bCanEvade = false;

		SetEnableInput(false, false, false);

		GetCharacterMovement()->Velocity = FVector::ZeroVector;

		_bIsDealing = true;

		RoxyAnim->PlaySwiftSlashMontage();

		// 스킬 사용 시 검 색깔 변화
		_bCanChangeSwordColor = true;

		_bIsSkillActive_Skill2 = true;

		// 스킬 사용 횟수 카운트
		SetUseSkill();
	}
}

void ARoxyCharacter::SwiftSlash()
{
	if (!_bIsSwiftianSlash) return;

	GetMesh()->SetVisibility(false);

	ApplySwiftSlashDamaging();
}

void ARoxyCharacter::ApplySwiftSlashDamaging()
{
	if (!_bIsSwiftianSlash) return;

	if (_swiftSlashEnemys.Num() > 0)
	{
		float enemyHp = InflictDamage(ESkillType::ESkill2, _swiftSlashEnemys[_swiftSlash_DamagingIndex]);

		// 현재 데미지 입은 적이 죽었다면 배열에서 제외 시킨다.
		if (enemyHp < KINDA_SMALL_NUMBER)
		{
			_swiftSlashEnemys.RemoveAt(_swiftSlash_DamagingIndex);
			// 현재 데미지 입힌 적이 배열의 마지막이라면 인덱스를 0으로 초기화한다.
			if (_swiftSlashEnemys.Num() <= _swiftSlash_DamagingIndex + 1)
			{
				_swiftSlash_DamagingIndex = 0;
			}
			else
			{
				_swiftSlash_DamagingIndex;
			}
		}
		if (_swiftSlashEnemys.Num() <= _swiftSlash_DamagingIndex + 1)
		{
			_swiftSlash_DamagingIndex = 0;
		}
		else
		{
			_swiftSlash_DamagingIndex++;
		}

		PlayCameraShake(CS_SwiftSlash);

		// 공격 효과음 UI
		// 크리티컬 효과음과 동시에 표시하지 않음
		if (!_bIsCritical)
		{
			WidgetInstance->OnSlam();
		}
		else
		{
			WidgetInstance->OnAttack_Critical();
		}

		// 적에게 맞춘 공격 횟수 카운트
		SetNumOfHit();

		GetWorldTimerManager().SetTimer(_swiftSlash_DamagingTmer, this, &ARoxyCharacter::ApplySwiftSlashDamaging, _swiftSlash_DamagingPeriod);

	}
	// 연참 데미지 적용 중 주위 모든 적이 죽었을 때 스킬을 끝낸다.
	else
	{
		//LOG(Warning, "Success Swift Slash");

		RoxyAnim->StopAllMontages(0.2f);
	
		SwiftSlashEnd();
	}
}

void ARoxyCharacter::RemoveSwiftOverlapActor()
{
	if (!_bIsSwiftianSlash) return;

	if (_weaponOverlapActor.Num() != 0)
	{
		_weaponOverlapActor.RemoveAll([](AActor* actor) {return true; });
	}
}

void ARoxyCharacter::SwiftSlashEnd()
{
	if(!_bIsSwiftianSlash) return;

	//LOG(Warning, "Swift Slash End");

	_bIsDealing = false;

	_bIsSwiftianSlash = false;

	_bCanDamaged = true;

	_bCanSwiftSlash = true;
	_bCanAttack = true;
	_bCanKick = true;
	_bCanPierce = true;
	_bCanGhostSword = true;
	_bCanEvade = true;

	_bIsStiffen = false;

	_bIsCritical = false;

	ClearWeaponOverlapActor();

	_swiftSlashEnemys.RemoveAll([](AActor* actor) {return true; });
	_swiftSlash_DamagingIndex = 0;

	SetEnableInput(true, true, true);

	SetLookOnNearbyEnemy(false);

	GetMesh()->SetVisibility(true);

	FCollisionResponseContainer characterCollision = GetCapsuleComponent()->GetCollisionResponseToChannels();
	characterCollision.GameTraceChannel1 = ECollisionResponse(ECR_Block);
	characterCollision.GameTraceChannel3 = ECollisionResponse(ECR_Overlap);
	characterCollision.GameTraceChannel4 = ECollisionResponse(ECR_Block);
	characterCollision.GameTraceChannel6 = ECollisionResponse(ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannels(characterCollision);

	_bIsSkillActive_Skill2 = false;
	SkillCoolApply(ESkillType::ESkill2);
}

/* ---------------------------------------- Evade ---------------------------------------- */
void ARoxyCharacter::EvadeStart()
{
	if (_bIsEvasion || !_bCanEvade || _bIsCoolApply_Skill5/* || (!_bCheckForwardFirstInput && !_bCheckRightFirstInput)*/) return;

	//float F_keyValue = GetInputAxisValue("MoveForward");
	//float R_keyValue = GetInputAxisValue("MoveRight");
	//
	//if (F_keyValue != 0 && R_keyValue == 0) {
	//	if (F_keyValue > 0)
	//		_secondInputVector = FVector(1, 0, 0);
	//	else if (F_keyValue < 0)
	//		_secondInputVector = FVector(-1, 0, 0);
	//}
	//else if (R_keyValue != 0 && F_keyValue == 0) {
	//	if (R_keyValue > 0)
	//		_secondInputVector = FVector(0, 1, 0);
	//	else if (R_keyValue < 0)
	//		_secondInputVector = FVector(0, -1, 0);
	//}
	//
	//if (_firstInputVector != _secondInputVector || _firstInputVector == FVector::ZeroVector) {
	//	OffCanEvade();
	//	return;
	//}

	_bIsEvasion = true;

	_bCanDamaged = false;

	_bCanAttack = false;
	_bCanKick = false;
	_bCanPierce = false;
	_bCanSwiftSlash = false;
	_bCanGhostSword = false;

	SetEnableInput(false, false, false);

	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	_bIsSkillActive_Skill5 = true;

	FCollisionResponseContainer characterCollision = GetCapsuleComponent()->GetCollisionResponseToChannels();
	characterCollision.GameTraceChannel1 = ECollisionResponse(ECR_Ignore);
	characterCollision.GameTraceChannel3 = ECollisionResponse(ECR_Ignore);
	characterCollision.GameTraceChannel4 = ECollisionResponse(ECR_Ignore);
	characterCollision.GameTraceChannel6 = ECollisionResponse(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannels(characterCollision);

	// 공격 중간에 캔슬하고 사용할 수 있도록
	if (_bIsAttacking)
	{
		RoxyAnim->StopAttackMontage();
		AttackEndComboState();
		_bIsAttacking = false;
	}

	RoxyAnim->PlayEvadeMontage();
}

void ARoxyCharacter::Evade()
{
	if (!_bIsEvasion) return;

	//FRotator newRot = _secondInputVector.Rotation();
	//SetActorRotation(newRot);
	//
	//FRotator Rotation = Controller->GetControlRotation();
	//const FRotator YawRotation(0, Rotation.Yaw, 0);
	//
	//if (_secondInputVector.Y == 0) { // 앞 혹은 뒤 키가 눌렸을 떄 실행
	//	_evadeDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * _secondInputVector.X;
	//}
	//else if (_secondInputVector.X == 0) { // 좌 혹은 우 키가 눌렸을 때 실행
	//	_evadeDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * _secondInputVector.Y;
	//}
	//
	//_evadeDirection.Normalize();
	//SetActorRotation(_evadeDirection.Rotation());
	//_evadeVelocity = _evadeDirection * 100.f * _evadeSpeed; // 100을 곱하는 이유는 m/s 를 만들기 위해서임
	//_evadeVelocity.Z = GetActorForwardVector().Z;
	//
	//FHitResult hitResult;
	//FHitResult* hitResultPtr = &hitResult;
	//
	//GetCharacterMovement()->GroundFriction = 1.f;
	//GetCharacterMovement()->Velocity = _evadeVelocity;

	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	const float KeyValue_F = GetInputAxisValue("MoveForward");
	const float KeyValue_R = GetInputAxisValue("MoveRight");

	if (KeyValue_F == 0.f && KeyValue_R == 0.f)
	{
		_evadeDirection = FVector(GetActorForwardVector());
	}
	else
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction_F = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * KeyValue_F;
		const FVector Direction_R = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * KeyValue_R;

		FVector DirectionResult = Direction_F + Direction_R;

		SetActorRotation(DirectionResult.Rotation());

		_evadeDirection = DirectionResult;

	}
	_evadeDirection.Normalize();
	LOG_SCREEN("direction : %f, %f, %f", _evadeDirection.X, _evadeDirection.Y, _evadeDirection.Z);

	FVector EvadeVelocity = _evadeDirection * 100.f * _evadeSpeed; // 100을 곱하는 이유는 m/s를 만들기 위해서!
	EvadeVelocity.Z = GetActorForwardVector().Z;

	FHitResult hitResult;
	FHitResult* hitResultPtr = &hitResult;

	GetCharacterMovement()->GroundFriction = 1.f;
	GetCharacterMovement()->Velocity = EvadeVelocity;
}

void ARoxyCharacter::StopEvade()
{
	_bCanDamaged = true;

	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	GetCharacterMovement()->GroundFriction = 8.f;

	FCollisionResponseContainer characterCollision = GetCapsuleComponent()->GetCollisionResponseToChannels();
	characterCollision.GameTraceChannel1 = ECollisionResponse(ECR_Block);
	characterCollision.GameTraceChannel3 = ECollisionResponse(ECR_Overlap);
	characterCollision.GameTraceChannel4 = ECollisionResponse(ECR_Block);
	characterCollision.GameTraceChannel6 = ECollisionResponse(ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannels(characterCollision);
}

void ARoxyCharacter::EvadeEnd()
{
	if (!_bIsEvasion) return;

	//_firstInputVector = FVector::ZeroVector;
	//_secondInputVector = FVector::ZeroVector;
	//_bCheckForwardFirstInput = false;
	//_bCheckRightFirstInput = false;

	_bIsEvasion = false;

	_bCanAttack = true;
	_bCanKick = true;
	_bCanPierce = true;
	_bCanSwiftSlash = true;
	_bCanGhostSword = true;

	_bOnMove = false;

	SetEnableInput(true, true, true);

	_bIsStiffen = false;

	_bIsSkillActive_Skill5 = false;
	SkillCoolApply(ESkillType::ESkill5);

	if (_forwardAttackTargets.Num() != 0)
	{
		_forwardAttackTargets.RemoveAll([](AActor* actor) {return true; });
	}

	if (_weaponOverlapActor.Num() != 0)
	{
		_weaponOverlapActor.RemoveAll([](AActor* actor) {return true; });
	}
}

void ARoxyCharacter::OffCanEvade()
{
	if (_bIsEvasion) return;

	_bCanEvade = false;
	//_bCheckForwardFirstInput = false;
	//_bCheckRightFirstInput = false;
	//_firstInputVector = FVector::ZeroVector;
	//_secondInputVector = FVector::ZeroVector;
}

/* ---------------------------------------- GhostSword ---------------------------------------- */
void ARoxyCharacter::StartInstallGhostSword()
{
	if (!_bCanGhostSword || _currentStorableNum_GhostSword <= 0 || _bIsSkillLocked_Skill3) return;

	_bCanAttack = false;
	_bCanKick = false;
	_bCanPierce = false;
	_bCanSwiftSlash = false;
	_bCanEvade = false;

	SetEnableInput(false, false, false);

	RoxyAnim->PlayGhostSwordInstallMontage();
}

void ARoxyCharacter::InstallGhostSword()
{
	if (_bCanGhostSword && _currentStorableNum_GhostSword > 0)
	{
		GetWorld()->SpawnActor<AGhostSword>(GhostSword, GetTransform());
		_currentStorableNum_GhostSword--;
		SkillCoolApply(ESkillType::ESkill3);
	}

}

void ARoxyCharacter::EndInstallGhostSword()
{
	_bCanAttack = true;
	_bCanKick = true;
	_bCanPierce = true;
	_bCanSwiftSlash = true;
	_bCanEvade = true;

	SetEnableInput(true, true, true);
}

void ARoxyCharacter::ChargeGhostSword()
{
	// 환영검이 모두 충전되었다면 쿨타임 타이머 종료
	LOG(Warning, "current GHost Num : %d", _currentStorableNum_GhostSword);
	if (++_currentStorableNum_GhostSword >= _storableMaxNum_GhostSword)
	{
		GetWorldTimerManager().ClearTimer(Skill3_CoolTimer);
	}
}

void ARoxyCharacter::CheckCursorAndSword()
{
	if(!Controller) return;

	TArray<FHitResult> results;
	FVector cameraLocation = FollowCamera->GetComponentTransform().GetLocation();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	FVector aimLocation = cameraLocation + Rotation.Vector() * 1000000.f;
	FQuat quat = Rotation.Quaternion();
	/*FVector extent = FVector()*/
	/*DrawDebugLine(GetWorld(), cameraLocation, aimLocation, FColor::Blue, false, -1.f, (uint8)'\000', 5.f);*/

	bool bResult = false;
	/*bResult = GetWorld()->SweepMultiByChannel(results, cameraLocation, aimLocation, quat, ECC_Visibility, FCollisionShape::MakeCapsule());*/

	FCollisionQueryParams Params(NAME_None, false, this);
	
	// ECC_GameTraceChannel3 = RespondOnlyRoxy
	bResult = GetWorld()->LineTraceMultiByChannel(results, cameraLocation, aimLocation, ECC_GameTraceChannel3, Params);

	/*LOG(Warning, "Result Num : %d", results.Num());*/
	// 현재 커서와 일치하는 환영검이 있는지 체크하는 변수
	AGhostSword* tempSword = nullptr;

	if (bResult)
	{
		for (int i = 0; i < results.Num(); i++)
		{
			if (Cast<AGhostSword>(results[i].GetActor()) != nullptr && !results[i].GetActor()->IsA(ARoxyCharacter::StaticClass()))
			{
				tempSword = Cast<AGhostSword>(results[i].GetActor());
				break;
			}
		}
	}
	// 커서와 일치하는 환영검이 존재할 때
	if (tempSword)
	{
		if (tempSword != TargetGhostSword)
		{
			TargetGhostSword = tempSword;
		}
	}
	// 커서와 일치하는 환영검이 존재하지 않을 때
	else if (tempSword == nullptr)
	{
		TargetGhostSword = nullptr;
	}
	
}

void ARoxyCharacter::RunGhostSwordAttack()
{
	if (!TargetGhostSword || !_bCanGhostSword || _bIsSkillLocked_Skill3) return;

	_bCanAttack = false;
	_bCanKick = false;
	_bCanPierce = false;
	_bCanSwiftSlash = false;
	_bCanEvade = false;

	SetEnableInput(false, false, false);

	RoxyAnim->PlayGhostSwordMontage();
}

void ARoxyCharacter::StartRushTowardGhostSword()
{
	if (!TargetGhostSword || !_bCanGhostSword) return;

	_bCanRushTowardGhostSword = true;
	_bIsRushTowardGhostSword = true;
	FRotator newRotation = FRotator(GetActorRotation().Pitch, Controller->GetControlRotation().Yaw, GetActorRotation().Roll);
	SetActorRotation(newRotation);

	// 관통하는 모든 적에게 표식을 남긴다
	TArray<FHitResult> hitResults;
	FVector startVec = GetActorLocation();
	FVector endVec = TargetGhostSword->GetActorLocation();

	float capsuleRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	float capsuleHalfHeight = FVector::Distance(startVec, endVec) / 2;
	FQuat quat = UKismetMathLibrary::FindLookAtRotation(startVec, endVec).Quaternion();
	FCollisionQueryParams Params(NAME_None, false, GetOwner());
	FCollisionObjectQueryParams objectParams;
	objectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel2);
	bool bResult = GetWorld()->SweepMultiByObjectType(
		hitResults,
		startVec,
		endVec,
		quat,
		objectParams,
		FCollisionShape::MakeCapsule(capsuleRadius, capsuleHalfHeight),
		Params
	);

	if (bResult)
	{
		for (int i = 0; i < hitResults.Num(); i++)
		{
			if (hitResults[i].GetActor()->IsA(AGruntAI::StaticClass()))
			{
				AGruntAI* enemy = Cast<AGruntAI>(hitResults[i].GetActor());

				// 적에게 이미 표식이 있는 상태라면
				if (enemy->IsMarkState())
				{
					enemy->SetGruntStateMode(EGruntStateMode::SM_MARK);
					enemy->SetMarkTimer();
					FDamageEvent DamageEvent;
					enemy->TakeDamage(_ghostSword_fixedDamage, DamageEvent, GetController(), this);
					enemy->SetGruntStateMode(EGruntStateMode::SM_MARK);
				}
				else
				{
					InflictDamage(ESkillType::ESkill3, enemy);
					enemy->SetGruntStateMode(EGruntStateMode::SM_MARK);
				}
			}
		}
	}

}

void ARoxyCharacter::RushTowardsGhostSword(float deltaTime)
{
	if (TargetGhostSword)
	{
		FVector newLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetGhostSword->GetActorLocation(), deltaTime, 20.f);
		SetActorLocation(newLocation);
		if (FVector::Distance(GetActorLocation(), TargetGhostSword->GetActorLocation()) < 10.f)
		{
			TargetGhostSword->DestroySword();
			TargetGhostSword = nullptr;
			_bCanRushTowardGhostSword = false;
			_bIsRushTowardGhostSword = false;

			_bCanAttack = true;
			_bCanKick = true;
			_bCanPierce = true;
			_bCanSwiftSlash = true;
			_bCanEvade = true;

			SetEnableInput(true, true, true);
		}
	}
}

/* ---------------------------------------- Damaged ---------------------------------------- */
// Stiffen : 몬스터 기본 공격에 피격시 경직(스턴)
void ARoxyCharacter::SetStiffen(float stiffenTime)
{
	if (_bIsStiffen || _bIsFaint || _bIsFury || _bIsConfusion || _bIsKicking
		|| _bIsPierce || _bIsSwiftianSlash || _bIsEvasion || _bIsRushTowardGhostSword || _bIsDie) return;
	
	_bIsStiffen = true;

	SetEnableInput(false, false, false);

	_bCanAttack = false;
	_bCanKick = false;
	_bCanEvade = false;
	_bCanPierce = false;
	_bCanSwiftSlash = false;

	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	if (_bIsAttacking)
	{
		OnAttackEnded();
	}
	//else if (_bIsKicking)
	//{
	//	KickEnd();
	//}

	OffAttackCollision();
	ClearWeaponOverlapActor();

	RoxyAnim->PlayHitMontage();

	GetWorldTimerManager().SetTimer(_stiffenTimer, this, &ARoxyCharacter::GetFreeOfStiffen, stiffenTime, false);
}

void ARoxyCharacter::GetFreeOfStiffen()
{
	if (!_bIsStiffen) return;

	_bIsStiffen = false;

	SetEnableInput(true, true, true);

	_bCanAttack = true;
	_bCanKick = true;
	_bCanEvade = true;
	_bCanPierce = true;
	_bCanSwiftSlash = true;

	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	if (_bIsPierce)
	{
		PierceEnd();
	}
	else if (_bIsEvasion)
	{
		EvadeEnd();
	}
}

// Faint : 몬스터 특수 패턴에 피격시 기절 상태(혼란)
void ARoxyCharacter::SetFaint(float faintTime)
{
	if (_bIsFaint || _bIsFury || _bIsConfusion
		|| _bIsSwiftianSlash || _bIsRushTowardGhostSword || _bIsDie) return;

	LOG(Warning, "Faint");

	_bIsFaint = true;

	if (_bIsAttacking)
	{
		OnAttackEnded();
	}
	else if (_bIsKicking)
	{
		KickEnd();
	}
	else if (_bIsPierce)
	{
		PierceStop();
		PierceEnd();
	}
	else if (_bIsEvasion)
	{
		StopEvade();
		EvadeEnd();
	}

	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	// 기절 상태 = 모든 행동 불능

	SetEnableInput(false, false, false);

	_bCanAttack = false;
	_bCanKick = false;
	_bCanEvade = false;
	_bCanPierce = false;
	_bCanSwiftSlash = false;

	RoxyAnim->StopAllMontages(0.1f);
	RoxyAnim->SetGroggyAnim(true);

	WidgetInstance->OnFaintCondition();
	WidgetInstance->OnFaintDefianceKey(true);

	// 기절 횟수 카운트
	SetNumOfFaint();

	// 기절 상태는 3초동안 지속됨
	GetWorldTimerManager().SetTimer(_faintTimer, this, &ARoxyCharacter::GetFreeOfFaint, faintTime, false);
}

void ARoxyCharacter::GetFreeOfFaint()
{
	if (!_bIsFaint) return;

	_bIsFaint = false;

	SetEnableInput(true, true, true);

	_bCanAttack = true;
	_bCanKick = true;
	_bCanEvade = true;
	_bCanPierce = true;
	_bCanSwiftSlash = true;

	_faintDefianceNum = 0;

	RoxyAnim->SetGroggyAnim(false);

	WidgetInstance->OnFaintDefianceKey(false);

	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	if (_bIsKnockBack)
	{
		_bIsKnockBack = false;
	}
}

void ARoxyCharacter::DefianceFaint()
{
	if (!_bIsFaint) return;

	_faintDefianceNum += 1;

	if (_faintDefianceNum >= 6)
	{
		GetFreeOfFaint();
	}
}

void ARoxyCharacter::SetKnockBack()
{
	if (_bIsKnockBack || _bIsStiffen || _bIsFaint || _bIsFury || _bIsConfusion
		|| _bIsSwiftianSlash || _bIsRushTowardGhostSword || _bIsDie) return;

	//LOG(Warning, "Knockback");

	_bIsKnockBack = true;

	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	RoxyAnim->StopAllMontages(0.1f);

	SetEnableInput(false, false, false);

	_bCanAttack = false;
	_bCanKick = false;
	_bCanEvade = false;
	_bCanPierce = false;
	_bCanSwiftSlash = false;

	if (_bIsAttacking)
	{
		OnAttackEnded();
	}
	else if (_bIsKicking)
	{
		KickEnd();
	}
	else if (_bIsPierce)
	{
		PierceEnd();
	}
	else if (_bIsEvasion)
	{
		EvadeEnd();
	}

	SetStiffen(0.5f);
}

// Fury : 몬스터 특수 패턴에 피격시 도발 상태
void ARoxyCharacter::SetFury()
{
	if (_bIsFury || _bIsStiffen || _bIsKnockBack || _bIsFaint || _bIsConfusion
		|| _bIsSwiftianSlash || _bIsRushTowardGhostSword || _bIsDie) return;

	LOG(Warning, "Fury");

	_bIsFury = true;

	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	RoxyAnim->StopAllMontages(0.1f);

	_moveSpeed = _moveSpeed / 2;

	// 도발 상태 = 모든 행동 불능

	SetEnableInput(false, false, false);

	_bCanAttack = false;
	_bCanKick = false;
	_bCanEvade = false;
	_bCanPierce = false;
	_bCanSwiftSlash = false;

	if (_bIsAttacking)
	{
		OnAttackEnded();
	}
	else if (_bIsKicking)
	{
		KickEnd();
	}
	else if (_bIsPierce)
	{
		PierceEnd();
	}
	else if (_bIsEvasion)
	{
		EvadeEnd();
	}

	// 도발 상태는 6초 후 해제됨
	GetWorldTimerManager().SetTimer(_furyTimer, this, &ARoxyCharacter::GetFreeOfFury, 6.0f, false);
}

void ARoxyCharacter::GetFreeOfFury()
{
	if (!_bIsFury) return;

	_bIsFury = false;

	_furyDefianceNum = 0;
	_furyAttacker = nullptr;

	_moveSpeed = _moveSpeed * 2;

	SetEnableInput(true, true, true);

	_bCanAttack = true;
	_bCanKick = true;
	_bCanEvade = true;
	_bCanPierce = true;
	_bCanSwiftSlash = true;

	GetCharacterMovement()->Velocity = FVector::ZeroVector;
}

void ARoxyCharacter::DefianceFury()
{
	if (!_bIsFury) return;

	_furyDefianceNum += 1;

	if (_furyDefianceNum >= 8)
	{
		GetFreeOfFury();
	}
}

void ARoxyCharacter::SetFuryAttacker(AGruntAI* attacker)
{
	_furyAttacker = attacker;
}

void ARoxyCharacter::MoveToFuryAttacker(float deltaTime)
{
	if (!_bIsFury) return;

	if (_furyAttacker)
	{
		FRotator originRot = GetActorRotation();
		FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), _furyAttacker->GetActorLocation());
		FRotator newRot = FRotator(originRot.Pitch, targetRot.Yaw, originRot.Roll);
		SetActorRotation(newRot);

		FVector targetLoc = targetRot.Vector();
		GetCharacterMovement()->AddInputVector(targetLoc);
	}
}

// Addicted : 몬스터 특수 패턴에 피격시 중독 상태
void ARoxyCharacter::SetAddicted()
{
	if (_bIsAddicted) return;

	LOG(Warning, "Addicted");

	_bIsAddicted = true;

	WidgetInstance->OnAddictedCondition();

	// 중독 횟수 카운트
	SetNumOfAddicted();

	// 중독 상태 셰이더
	GetMesh()->SetScalarParameterValueOnMaterials(FName("poison_pow"), 1.0f);

	// 중독 상태에 빠지면 초당 데미지를 입음
	GetWorldTimerManager().SetTimer(_poisonTimer, this, &ARoxyCharacter::PoisonDamaged, 1.0f, true);
	// 중독 상태는 10초 후 해제됨
	GetWorldTimerManager().SetTimer(_addictedTimer, this, &ARoxyCharacter::GetFreeOfAddicted, 10.0f, false);
}

void ARoxyCharacter::PoisonDamaged()
{
	if (!_bIsAddicted) return;
	
	FDamageEvent _damageEvent;

	TakeDamage(_poisonDamage, _damageEvent, GetController(), this);
}

void ARoxyCharacter::GetFreeOfAddicted()
{
	if (!_bIsAddicted) return;

	_bIsAddicted = false;

	GetMesh()->SetScalarParameterValueOnMaterials(FName("poison_pow"), 0.0f);

	GetWorldTimerManager().ClearTimer(_poisonTimer);
}

// 혼란(스킬 잠금) : 몬스터 특수 패턴에 피격시 스킬 잠금
void ARoxyCharacter::SkillLockApply(int32 skillNum)
{
	if (_bIsFaint || _bIsKnockBack || _bIsFury
		|| _bIsSwiftianSlash || _bIsRushTowardGhostSword || _bIsDie) return;

	_bIsSkillLocked = true;
	_bIsConfusion = true;

	switch (skillNum)
	{
	case 1:
		if (!_bIsSkillLocked_Skill1)
		{
			_bIsSkillLocked_Skill1 = true;
			GetWorldTimerManager().SetTimer(Skill1Lock_CoolTimer, Skill1Lock_Delegate, _skillLockCoolTime, false);
		}
		else
		{
			_bIsSkillLocked_Skill1 = false;
			_bIsSkillLocked = false;
			_bIsConfusion = false;
		}
		break;
	case 2:
		if (!_bIsSkillLocked_Skill2)
		{
			_bIsSkillLocked_Skill2 = true;
			GetWorldTimerManager().SetTimer(Skill2Lock_CoolTimer, Skill2Lock_Delegate, _skillLockCoolTime, false);
		}
		else
		{
			_bIsSkillLocked_Skill2 = false;
			_bIsSkillLocked = false;
			_bIsConfusion = false;
		}
		break;
	case 3:
		if (!_bIsSkillLocked_Skill3)
		{
			_bIsSkillLocked_Skill3 = true;
			GetWorldTimerManager().SetTimer(Skill3Lock_CoolTimer, Skill3Lock_Delegate, _skillLockCoolTime, false);
		}
		else
		{
			_bIsSkillLocked_Skill3 = false;
			_bIsSkillLocked = false;
			_bIsConfusion = false;
		}
		break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Called when the game starts or when spawned
void ARoxyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 게임 인스턴스에 저장된 플레이어 고유 설정 값을 현재 레벨 캐릭터에게도 적용시켜준다.
	GameInstance = Cast<UNunuGameInstance>(GetGameInstance());
	CHECK(GameInstance);
	SetSensitivity(GameInstance->GetPlayerSensitivity());
	BindNunuInGameManager(InGameManager);
	CHECK(InGameManager);

	GameInstance->ChangeMaxHp.AddDynamic(this, &ARoxyCharacter::SetAddedMaxHp);
	GameInstance->ChangeMoveSpeed.AddDynamic(this, &ARoxyCharacter::SetAddedMoveSpeed);
	GameInstance->ChangeDamage.AddDynamic(this, &ARoxyCharacter::SetAddedDamage);
	GameInstance->ChangeCriticalHitMultiply.AddDynamic(this, &ARoxyCharacter::SetCriticalHitMultiply);
	GameInstance->ChangeDecreasedCoolTimePer.AddDynamic(this, &ARoxyCharacter::SetDecreasedCoolTimePercent);

	WidgetInstance->BindOwner(this);

	CameraBoom->SetTargetArmLength(_targetArmLengthOrigin, 10.f);
	CameraBoom->SetSocketOffset(_socketOffsetOrigin, 10.f);
	CameraBoom->SetCameraLocation(GetFollowCamera(), _cameraLocOrigin, 10.f);

	_criticalHitMultiply += GameInstance->GetCriticalHitMultiply(); // 치명타 피해량
	HpComponent->AddMaxHp(GameInstance->GetAddedMaxHp());
	_moveSpeed += GameInstance->GetAddedMoveSpped();
	_addedDamage += GameInstance->GetAddedDamage();
	_decreasedCoolTimePercent += GameInstance->GetDecreasedCoolTimePercent();


}

void ARoxyCharacter::Possessed(AController* NewController)
{
	
	if (Cast<ANunuPlayerController>(NewController))
	{
		if (!WidgetInstance && WidgetTemplate)
			WidgetInstance = CreateWidget<URoxyWidget>(GetWorld(), WidgetTemplate);

		if (WidgetInstance && !WidgetInstance->IsInViewport())
			WidgetInstance->AddToViewport();
	}
}

// Called every frame
void ARoxyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*-----------무브먼트 관련------------*/
	
	_currentSpeed = GetVelocity().Size();
	_currentForwardSpeed = GetVelocity().X;
	_currentRightSpeed = GetVelocity().Y;

	_currentSpeedRate = _currentSpeed / GetCharacterMovement()->GetMaxSpeed();
	_currentForwardSpeedRate = _currentForwardSpeed / GetCharacterMovement()->GetMaxSpeed();
	_currentRightSpeedRate = _currentRightSpeed / GetCharacterMovement()->GetMaxSpeed();

	_currentForwardSpeedRateAbs = fabsf(_currentForwardSpeedRate);
	_currentRightSpeedRateAbs = fabsf(_currentRightSpeedRate);

	_currentSpeedRate >= 0.1f ? _bOnMove = true : _bOnMove = false;
	
	/*------------시스템 관련--------------*/
	if (_bOnLookOnNearbyEnemy)
	{
		LookOnNearbyEnemy();
	}

	/*------------기본공격 관련--------------*/
	if (_currentCombo == 3)
	{
		SheathCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else if (_currentCombo == 4)
	{
		WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SheathCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	/*------------스킬 관련--------------*/
	CheckCursorAndSword();

	if (_bCanRushTowardGhostSword)
	{
		RushTowardsGhostSword(DeltaTime);
	}

	// 검 색깔 바꾸기
	if (_bCanChangeSwordColor)
	{
		ChangeSwordColor(DeltaTime);
	}
	
	if (_bCanChangeSwordColorOrigin)
	{
		ChangeSwordColorOrigin(DeltaTime);
	}

	/*------------상태 이상 관련--------------*/
	if (_bIsFury)
	{
		MoveToFuryAttacker(DeltaTime);
	}
}

void ARoxyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RoxyAnim = Cast<URoxyAnimInstance>(GetMesh()->GetAnimInstance());
	CHECK(nullptr != RoxyAnim);


	// 스킬 쿨타임 델리게이트
	Skill1_Delegate.BindUFunction(this, "SkillCoolApply", ESkillType::ESkill1);
	Skill2_Delegate.BindUFunction(this, "SkillCoolApply", ESkillType::ESkill2);
	Skill3_Delegate.BindUFunction(this, "ChargeGhostSword");
	Skill4_Delegate.BindUFunction(this, "SkillCoolApply", ESkillType::ESkill4);
	Skill5_Delegate.BindUFunction(this, "SkillCoolApply", ESkillType::ESkill5);

	// 스킬 잠금 쿨타임 델리게이트
	Skill1Lock_Delegate.BindUFunction(this, "SkillLockApply", 1);
	Skill2Lock_Delegate.BindUFunction(this, "SkillLockApply", 2);
	Skill3Lock_Delegate.BindUFunction(this, "SkillLockApply", 3);

	HpComponent->OnDie.AddUObject(this, &ARoxyCharacter::OnDie);
	UltimateComponent->OnUltimate.AddUObject(this, &ARoxyCharacter::SetUsableUltimate);


	// 콜리전 히트, 오버랩 관련
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ARoxyCharacter::OnCollisionBeginHit);
	ForwardAttackCollision->OnComponentHit.AddDynamic(this, &ARoxyCharacter::OnCollisionBeginHit);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARoxyCharacter::OnCollisionBeginOverlap);
	WeaponCapsule->OnComponentBeginOverlap.AddDynamic(this, &ARoxyCharacter::OnCollisionBeginOverlap);
	ForwardAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ARoxyCharacter::OnCollisionBeginOverlap);
	SheathCapsule->OnComponentBeginOverlap.AddDynamic(this, &ARoxyCharacter::OnCollisionBeginOverlap);
	

	/* ------------------------------ 애니메이션 노티파이 ------------------------------ */
	RoxyAnim->OnComboCheck.AddUObject(this, &ARoxyCharacter::AttackComboCheck);
	RoxyAnim->OnCanInputNextCombo.AddUObject(this, &ARoxyCharacter::CanInputNextCombo);

	RoxyAnim->OnClearActor.AddUObject(this, &ARoxyCharacter::ClearWeaponOverlapActor);
	RoxyAnim->OnOffCollision.AddUObject(this, &ARoxyCharacter::OffAttackCollision);

	RoxyAnim->OnKickCheck.AddUObject(this, &ARoxyCharacter::OnKickCollision);
	RoxyAnim->OnKickEnd.AddUObject(this, &ARoxyCharacter::KickEnd);

	RoxyAnim->OnPierceCheck.AddUObject(this, &ARoxyCharacter::Pierce);
	RoxyAnim->OnStopPierce.AddUObject(this, &ARoxyCharacter::PierceStop);
	RoxyAnim->OnPierceEnd.AddUObject(this, &ARoxyCharacter::PierceEnd);

	RoxyAnim->OnSwiftSlash.AddUObject(this, &ARoxyCharacter::SwiftSlash);
	RoxyAnim->OnSwiftSlashEnd.AddUObject(this, &ARoxyCharacter::SwiftSlashEnd);

	RoxyAnim->OnEvadeCheck.AddUObject(this, &ARoxyCharacter::Evade);
	RoxyAnim->OnGhostTrail.AddUObject(this, &ARoxyCharacter::MakeGhostTrail);
	RoxyAnim->OnStopLaunch.AddUObject(this, &ARoxyCharacter::StopEvade);
	RoxyAnim->OnEvadeEnd.AddUObject(this, &ARoxyCharacter::EvadeEnd);

	RoxyAnim->OnInstallSword.AddUObject(this, &ARoxyCharacter::InstallGhostSword);
	RoxyAnim->OnEndInstallSword.AddUObject(this, &ARoxyCharacter::EndInstallGhostSword);
	RoxyAnim->OnRushTowardSword.AddUObject(this, &ARoxyCharacter::StartRushTowardGhostSword);

	RoxyAnim->OnStepCheck.AddUObject(this, &ARoxyCharacter::SetNumOfStep);
	/* ----------------------------------------------------------------------------- */


	// 데이터 테이블 값 할당 ---------------------------
	if (DataTable)
	{
		FRoxyStatTableRow* statTableRow = DataTable->FindRow<FRoxyStatTableRow>(TEXT("Roxy"), FString("Roxy Data Table is not exist!"));
		
		if (statTableRow)
		{
			HpComponent->SetMaxHp(statTableRow->maxHp);
			HpComponent->SetHP(HpComponent->GetMaxHp());
			UltimateComponent->SetIncrementPerSec(statTableRow->ultimateGaugeIncrementPerSec);
			_moveSpeed = statTableRow->moveSpeed;
			GetCharacterMovement()->MaxWalkSpeed = _moveSpeed;
			TurnRateOrigin = statTableRow->TurnRate;
			LookUpRateOrigin = statTableRow->LookUpRate;
			_targetArmLengthOrigin = statTableRow->cameraDistanceOrigin;
			// ------
			_ad = statTableRow->ad;
			_criticalHitProbability = statTableRow->criticalHitProbability;
			_criticalHitMultiply = statTableRow->criticalHitMultiply;
			// ------
			_attack_damage = statTableRow->attack_damage;
			_attack_ratio = statTableRow->attack_ratio;
			_attack_stat = statTableRow->attack_stat;
			_attack1_BulletTime_dilation = statTableRow->attack1_bulletTimeDilation;
			_attack2_BulletTime_dilation = statTableRow->attack2_bulletTimeDilation;
			_attack3_BulletTime_dilation = statTableRow->attack3_bulletTimeDilation;
			_lastAttack_BulletTime_dilation = statTableRow->lastAttack_bulletTimeDilation;
			_attack1_BulletTime_duration = statTableRow->attack1_bulletTimeDuration;
			_attack2_BulletTime_duration = statTableRow->attack2_bulletTimeDuration;
			_attack3_BulletTime_duration = statTableRow->attack3_bulletTimeDuration;
			_lastAttack_BulletTime_duration = statTableRow->lastAttack_bulletTimeDuration;
			// ------
			_kick_damage = statTableRow->kick_damage;
			_kick_ratio = statTableRow->kick_ratio;
			_kick_stat = statTableRow->kick_stat;
			_coolTime_Skill4 = statTableRow->kick_coolTime;
			_kick_BulletTime_dilation = statTableRow->kick_bulletTimeDilation;
			_kick_BulletTime_duration = statTableRow->kick_bulletTimeDuration;
			// ------
			_pierce_damage = statTableRow->pierce_damage;
			_pierce_ratio = statTableRow->pierce_ratio;
			_pierce_stat = statTableRow->pierce_stat;
			_coolTime_Skill1 = statTableRow->pierce_coolTime;
			_pierceSpeed = statTableRow->pierce_speed;
			_pierce_targetArmLength = statTableRow->pierce_targetArmLength;
			// ------
			_swiftSlash_damage = statTableRow->swiftSlash_damage;
			_swiftSlash_ratio = statTableRow->swiftSlash_ratio;
			_swiftSlash_stat = statTableRow->swiftSlash_stat;
			_coolTime_Skill2 = statTableRow->swiftSlash_coolTime;
			// ------
			_evadeSpeed = statTableRow->evade_speed;
			_coolTime_Skill5 = statTableRow->evade_coolTime;
			// ------
			_ghostSword_damage = statTableRow->ghostSword_damage;
			_ghostSword_ratio = statTableRow->ghostSword_ratio;
			_ghostSword_stat = statTableRow->ghostSword_stat;
			_ghostSword_fixedDamage = statTableRow->ghostSword_fixedDamage;
			_coolTime_Skill3 = statTableRow->ghostSword_coolTime;

			LOG(Warning, "--------------Roxy Stat--------------");
			LOG(Warning, "Max Hp : %f", HpComponent->GetMaxHp());
			LOG(Warning, "Increment Per 1 Sec : %f", UltimateComponent->GetIncrementPerSec());
			LOG(Warning, "Move Speed : %f", _moveSpeed);
			LOG(Warning, "Turn Rate : %f", BaseTurnRate);
			LOG(Warning, "Look Up Rate : %f", BaseLookUpRate);
			LOG(Warning, "camera Distance Origin : %f", _targetArmLengthOrigin);
			LOG(Warning, "Attack Damage : %f", _attack_damage);
			LOG(Warning, "Kick CoolTime : %f", _coolTime_Skill4);
			LOG(Warning, "Pierce Damage : %f", _pierce_damage);
			LOG(Warning, "Pierce CoolTime : %f", _coolTime_Skill1);
			LOG(Warning, "Pierce Speed : %f", _pierceSpeed);
			LOG(Warning, "SwiftSlash Damage : %f", _swiftSlash_damage);
			LOG(Warning, "SwiftSlash CoolTime : %f", _coolTime_Skill2);
			LOG(Warning, "Evade Speed : %f", _evadeSpeed);
			LOG(Warning, "Evade CoolTime : %f", _coolTime_Skill5);
			LOG(Warning, "Ghost Sword Cool Time : %f", _coolTime_Skill3);
			LOG(Warning, "------------------------------------")
		}
	}

	_currentTargetArmLength = _targetArmLengthOrigin;

}

void ARoxyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	/* 일반 입력 키 */
	//PlayerInputComponent->BindAction("LockOn", IE_Pressed, this, &ARoxyCharacter::TurnOnLockOnMode);
	//PlayerInputComponent->BindAction("LockOn_Off", IE_Pressed, this, &ARoxyCharacter::TurnOffLockOnMode);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ARoxyCharacter::ComboAttack);
	PlayerInputComponent->BindAction("Kick", IE_Pressed, this, &ARoxyCharacter::StartKick);
	PlayerInputComponent->BindAction("Pierce", IE_Pressed, this, &ARoxyCharacter::PierceStart);
	PlayerInputComponent->BindAction("SwiftSlash", IE_Pressed, this, &ARoxyCharacter::SwiftSlashStart);
	PlayerInputComponent->BindAction("Evade", IE_Pressed, this, &ARoxyCharacter::EvadeStart);
	PlayerInputComponent->BindAction("Ultimate", IE_Pressed, this, &ARoxyCharacter::UltimateStart);
	//PlayerInputComponent->BindAction("InstallGhostSword", IE_Pressed, this, &ARoxyCharacter::StartInstallGhostSword);
	//PlayerInputComponent->BindAction("RushTowardGhostSword", IE_Pressed, this, &ARoxyCharacter::RunGhostSwordAttack);
	PlayerInputComponent->BindAction("MouseWheelButton", IE_Pressed, this, &ARoxyCharacter::FixMoveDirection);
	PlayerInputComponent->BindAction("MouseWheelButton", IE_Released, this, &ARoxyCharacter::UnFixMoveDirection);
	PlayerInputComponent->BindAction("Defiance", IE_Pressed, this, &ARoxyCharacter::DefianceFaint);

	/* 커맨드 */
	PlayerInputComponent->BindAction("SetDie", IE_Pressed, this, &ARoxyCharacter::SetDie);
	PlayerInputComponent->BindAction("SetFullHP", IE_Pressed, this, &ARoxyCharacter::SetFullHP);
	PlayerInputComponent->BindAction("CoolClear", IE_Pressed, this, &ARoxyCharacter::SkillCoolClear);


	PlayerInputComponent->BindAxis("WheelUp", this, &ARoxyCharacter::ExpandCamera);
	PlayerInputComponent->BindAxis("MoveForward", this, &ARoxyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARoxyCharacter::MoveRight);
	

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &ARoxyCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARoxyCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &ARoxyCharacter::LookUpAtRate);
}


float ARoxyCharacter::InflictDamage(ESkillType skillType, AEnemyAI* DamageVictim)
{
	ApplyComboWidget();

	// 최종 ad(공격력) 계산
	float DamageAmount = _ad + _addedDamage;
	float stat = 0.f;
	switch (skillType)
	{
	case ESkillType::ESkill0:
		// 계수 계산
		stat = FMath::FRandRange(-_attack_stat, _attack_stat);
		DamageAmount = (_attack_ratio * DamageAmount) + _attack_damage + stat;
		break;
	case ESkillType::ESkill1:
		stat = FMath::FRandRange(- _pierce_stat, _pierce_stat);
		DamageAmount = (_pierce_ratio * DamageAmount) + _pierce_damage + stat;
		break;
	case ESkillType::ESkill2:
		stat = FMath::FRandRange(-_swiftSlash_stat, _swiftSlash_stat);
		DamageAmount = (_swiftSlash_ratio * DamageAmount) + _swiftSlash_damage + stat;
		break;
	case ESkillType::ESkill3:
		stat = FMath::FRandRange(-_ghostSword_stat, _ghostSword_stat);
		DamageAmount = (_ghostSword_ratio * DamageAmount) + _ghostSword_damage+ stat;
		break;
	case ESkillType::ESkill4:
		stat = FMath::FRandRange(-_kick_stat, _kick_stat);
		DamageAmount = (_kick_ratio * DamageAmount) + _kick_damage + stat;
		break;
	default:
		break;
	}
	
	// 치명타 계산
	float rand = FMath::RandRange(0.f, 1.f);
	if (rand <= _criticalHitProbability)
	{
		_bIsCritical = true;
		
		DamageAmount *= _criticalHitMultiply;

		// 크리티컬 터지면 재생될 사운드
		UGameplayStatics::PlaySoundAtLocation(this, SC_Critical, GetActorLocation());
	}

	ARevivalAI* bossMonster = Cast<ARevivalAI>(DamageVictim);

	_finalDamageAmount = DamageAmount;

	LOG(Warning, "Final Damage Amount : %f", _finalDamageAmount);

	FDamageEvent DamageEvent;
	
	return DamageVictim->TakeDamage(DamageAmount, DamageEvent, GetController(), this);
}

float ARoxyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 피격에 대한 피드백. 체력 깍임, 피격 효과(ui, 카메라쉐이크 등)
	// 경직 등 상태 관련 기능은 Stiffen 함수

	if (!_bCanDamaged || !_bCanTakeDamage) return 0;

	float hp = HpComponent->GetHp();

	hp = HpComponent->TakeDamage(DamageAmount);
	LOG(Warning, "Current %s HP : %f", *GetName(), hp);

	WidgetInstance->OnScreenBlood();

	PlayCameraShake(CS_Hit);
	
	// 피격 횟수 카운트
	SetNumOfDamaged();

	return hp;
}

void ARoxyCharacter::ApplyComboWidget()
{
	WidgetInstance->ApplyCombo();
}

void ARoxyCharacter::SetEnableInput(bool canMove /*= true*/, bool canTurn /*= true*/, bool canAttack /*= true*/)
{
	_bCanMove = canMove;
	_bCanTurn = canTurn;
	_bCanAttack = canAttack;
	_bCanEvade = canAttack;
	_bCanPierce = canAttack;
	_bCanSwiftSlash = canAttack;
	_bCanKick = canAttack;
}

void ARoxyCharacter::SetSensitivity (float value)
{
	BaseLookUpRate = LookUpRateOrigin * value;
	BaseTurnRate = TurnRateOrigin * value;
}

void ARoxyCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && _bCanMove)
	{
		//// 이동 키 입력 직후 실행되는 회피기를 위함
		//if (!_bCanEvade && !_bCheckForwardFirstInput)
		//{
		//	if (Value > 0)
		//		_firstInputVector = FVector(1, 0, 0);
		//	else
		//		_firstInputVector = FVector(-1, 0, 0);
		//	_bCanEvade = true;
		//	GetWorldTimerManager().SetTimer(_canEvadeOffTimer, this, &ARoxyCharacter::OffCanEvade, 0.2f, false);
		//}

		// find out which way is forward

		FVector Direction = FVector::ZeroVector;

		if (_bFixMoveDirection)
		{
			if (_fixedRotation == FRotator::ZeroRotator)
			{
				_fixedRotation = Controller->GetControlRotation();
			}
			const FRotator YawRotation(0, _fixedRotation.Yaw, 0);
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		}
		else
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		}
		// get forward vector
		 
		AddMovementInput(Direction, Value);
	}
	else if (Value == 0 && _bCanEvade)
		_bCheckForwardFirstInput = true;

}

void ARoxyCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && _bCanMove)
	{
		//// 이동 키 입력 직후 실행되는 회피기를 위함
		//if (!_bCanEvade && !_bCheckRightFirstInput)
		//{
		//	if (Value > 0)
		//		_firstInputVector = FVector(0, 1, 0);
		//	else
		//		_firstInputVector = FVector(0, -1, 0);
		//	_bCanEvade = true;
		//	GetWorldTimerManager().SetTimer(_canEvadeOffTimer, this, &ARoxyCharacter::OffCanEvade, 0.25f, false);
		//}

		//FRotator newRotation(0, Controller->GetControlRotation().Yaw, 0);
		//// 회전에 대한 입력이 없을 시
		//if (_oldRotation.Equals(newRotation))
		//{
		//	_circlePrajTimer += GetWorld()->GetDeltaSeconds();
		//
		//	// 락온 상태가 아니며, 원 궤적 달리기 카운트 시간 이상이 됐을 시 실행
		//	if (_circlePrajTimer >= 1.2f) {
		//
		//		FQuat DeltaRotaion = (FRotator(0.f, 90.f, 0.f) * Value * GetWorld()->GetDeltaSeconds()).Quaternion();
		//		Controller->SetControlRotation(GetControlRotation() + DeltaRotaion.Rotator());
		//	}
		//}
		//else {// 마우스 입력이 들어오면 원 궤적 달리기 타이머 초기화
		//	_circlePrajTimer = 0.f;
		//}

		// find out which way is right
		FVector Direction = FVector::ZeroVector;

		if (_bFixMoveDirection)
		{
			if (_fixedRotation == FRotator::ZeroRotator)
			{
				_fixedRotation = Controller->GetControlRotation();
			}
			const FRotator YawRotation(0, _fixedRotation.Yaw, 0);
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		}
		else
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		}
		FRotator Rotation = Controller->GetControlRotation();
		
		// add movement in that direction

		AddMovementInput(Direction, Value);

		_oldRotation = FRotator(0, Controller->GetControlRotation().Yaw, 0);
	}
	else if (Value == 0 && _bCanEvade)
		_bCheckRightFirstInput = true;
}

void ARoxyCharacter::TurnAtRate(float Rate)
{
	if (!_bCanTurn) return;
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARoxyCharacter::LookUpAtRate(float Rate)
{
	if (!_bCanTurn) return;
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARoxyCharacter::ExpandCamera(float Rate)
{
	if (Rate == 0)return;
	_currentTargetArmLength = FMath::Clamp(_currentTargetArmLength + Rate * 30, 200.f, 1200.f);
	CameraBoom->SetTargetArmLength(_currentTargetArmLength, 15.f);
}

void ARoxyCharacter::ReduceCamera(float Rate)
{
	if (Rate == 0)return;
	_currentTargetArmLength = _currentTargetArmLength = FMath::Clamp(_currentTargetArmLength * Rate * -5.f, 200.f, 1200.f);
	CameraBoom->SetTargetArmLength(_currentTargetArmLength, 30.f);
}

void ARoxyCharacter::FixMoveDirection()
{
	_bFixMoveDirection = true;
}


void ARoxyCharacter::UnFixMoveDirection()
{
	_bFixMoveDirection = false;
	_fixedRotation = FRotator::ZeroRotator;
}

void ARoxyCharacter::InitializeVelocity()
{
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
}

/* --------------- Die --------------- */
void ARoxyCharacter::OnDie()
{
	// 플레이어 사망 시 일어날 일들

	CHECK(!_bIsDie);

	LOG_SCREEN("Character Die");

	_bIsDie = true;

	_bIsGameOver = true;
	
	APlayerController* playerController = Cast<APlayerController>(Controller);

	if (playerController) {
		DisableInput(playerController);
		CameraBoom->SetEnableCameraWork(false); // 카메라 연출 재생
		PlayActorSequencer_Die();

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));


		RoxyAnim->SetDieAnim();

		_bIsAttacking = false;
		_bIsPierce = false;
		_bIsSwiftianSlash = false;
		_bIsEvasion = false;

		_bCanPierce = false;
		_bCanSwiftSlash = false;
		_bCanEvade = false;
		SetEnableInput(false, false, false);

		GetWorldTimerManager().SetTimer(_dieTimer, this, &ARoxyCharacter::OpenLoseWidget, 4.0f, false);
	}
}

void ARoxyCharacter::OpenGameOverLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), "GameOver");
}

/*------------ 커맨드 --------------*/
void ARoxyCharacter::SetDie()
{
	// 바로 죽음 상태 만들기
	if (_bIsDie) return;

	LOG(Warning, "#####Command##### : Die");

	OnDie();
}

void ARoxyCharacter::SetFullHP()
{
	// 풀피 채우기

	LOG(Warning, "#####Command##### : Set Full HP");

	HpComponent->SetToMaxHp();
}

void ARoxyCharacter::SkillCoolClear()
{
	// 스킬 쿨타임 해제

	LOG(Warning, "#####Command##### : Skill Cool Clear");

	GetWorldTimerManager().ClearTimer(Skill1_CoolTimer);
	GetWorldTimerManager().ClearTimer(Skill2_CoolTimer);
	GetWorldTimerManager().ClearTimer(Skill3_CoolTimer);
	GetWorldTimerManager().ClearTimer(Skill4_CoolTimer);
	GetWorldTimerManager().ClearTimer(Skill5_CoolTimer);

	SetEnableSkill();
}

int32 ARoxyCharacter::GetCurrentCombo()
{
	return _currentCombo;
}

int32 ARoxyCharacter::GetCurrentPierceNum()
{
	return _currentPierceNum;
}

bool ARoxyCharacter::GetCharacterIsDie()
{
	return _bIsDie;
}

bool ARoxyCharacter::GetCharacterIsAttack()
{
	return _bIsAttacking;
}

bool ARoxyCharacter::GetCharacterIsPierce()
{
	return _bIsPierce;
}

bool ARoxyCharacter::GetCharacterIsEvade()
{
	return _bIsEvasion;
}

bool ARoxyCharacter::GetCharacterIsSwift()
{
	return _bIsSwiftianSlash;
}

// Called to bind functionality to input