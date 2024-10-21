// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkJoy.h"
#include "InGame/Enemy/GruntAI/Dummy/DarkJoy/DarkJoyAnimInstance.h"
#include "InGame/Enemy/GruntAI/Dummy/DarkJoy/DarkJoyAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/System/Generator.h"
#include "Kismet/KismetMathLibrary.h"

ADarkJoy::ADarkJoy()
{
	PrimaryActorTick.bCanEverTick = true;

// 	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_JOY(TEXT("/Game/Resources/Enemy/Grunt/DarkJoy/Mesh/Joy_SK.Joy_SK"));
// 	if (SK_JOY.Succeeded())
// 	{
// 		GetMesh()->SetSkeletalMesh(SK_JOY.Object);
// 	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> JOY_ANIM(TEXT("/Game/Resources/Enemy/Grunt/DarkJoy/DarkJoy_Anim.DarkJoy_Anim_C"));
	if (JOY_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(JOY_ANIM.Class);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_ORIGIN(TEXT("/Game/Resources/Enemy/Grunt/DarkJoy/Materials/Joy_Mat.Joy_Mat"));
	if (MAT_ORIGIN.Succeeded())
	{
		OriginalMat = MAT_ORIGIN.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_OPACITY(TEXT("/Game/Resources/Enemy/Grunt/DarkJoy/Materials/Joy_Mat_Inst_Opacity.Joy_Mat_Inst_Opacity"));
	if (MAT_OPACITY.Succeeded())
	{
		OpacityMat = MAT_OPACITY.Object;
	}

	if (DataTable)
	{
		FGruntStatTableRow* statTableRow = DataTable->FindRow<FGruntStatTableRow>(TEXT("DarkJoy"), FString("DarkJoy Row is not exist!"));

		if (statTableRow)
		{
			HpComponent->SetMaxHp(statTableRow->maxHp);
			HpComponent->SetHP(HpComponent->GetMaxHp());
			LOG(Warning, "--------------DarkJoy Stat--------------");
			LOG(Warning, "Max Hp : %f", HpComponent->GetMaxHp());
			LOG(Warning, "------------------------------------")
		}
	}

	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>("Weapon");
	WeaponCollision->SetupAttachment(GetMesh(), TEXT("Star"));
	WeaponCollision->SetCapsuleSize(15.0f, 100.0f);
	WeaponCollision->SetRelativeLocationAndRotation(FVector(-80.0f, 0.0f, 0.0f), FRotator(180.0f, -90.0f, 180.0f));
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollision->SetCollisionProfileName("RespondOnlyRoxy");

	AIControllerClass = ADarkJoyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// ºÎºñÀû
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 80.0f;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	_moveSpeed = 150.0f;
	_damage_Attack = 4.0f;
}

void ADarkJoy::Die()
{
	Super::Die();

	_bIsDie = true;
	_bCanMove = false;

	DarkJoyAnim->SetDieAnim(true);

	FCollisionResponseContainer capsuleCollision = GetCapsuleComponent()->GetCollisionResponseToChannels();
	capsuleCollision.GameTraceChannel1 = ECollisionResponse(ECR_Overlap);
	capsuleCollision.GameTraceChannel2 = ECollisionResponse(ECR_Ignore);
	capsuleCollision.Pawn = ECollisionResponse(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannels(capsuleCollision);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Ragdoll"));
}

void ADarkJoy::BeginPlay()
{
	Super::BeginPlay();
}

void ADarkJoy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DarkJoyAnim = Cast<UDarkJoyAnimInstance>(GetMesh()->GetAnimInstance());
	CHECK(nullptr != DarkJoyAnim);

	GetCharacterMovement()->MaxWalkSpeed = _moveSpeed;
}

void ADarkJoy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Die
void ADarkJoy::DestoyActor()
{
	if (!_bIsDie) return;

	Destroy();
}

// Get, Set
bool ADarkJoy::GetMovable()
{
	return _bCanMove;
}

void ADarkJoy::SetMovable(bool value)
{
	_bCanMove = value;
}

bool ADarkJoy::GetCanDetect()
{
	return _bCanDetect;
}