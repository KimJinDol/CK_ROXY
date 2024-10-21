// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAI.h"
#include "InGame/Enemy/GruntAI/Combat/CombatAnimInstance.h"
#include "InGame/Enemy/GruntAI/Combat/CombatAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/System/Generator.h"
#include "Components/WidgetComponent.h"

ACombatAI::ACombatAI()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_COMBAT(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Mesh/SK_Cactus_02.SK_Cactus_02"));
	if (SK_COMBAT.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_COMBAT.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> COMBAT_ANIM(TEXT("/Game/Resources/Enemy/Grunt/Cactus/BP_Combat_Anim.BP_Combat_Anim_C"));
	if (COMBAT_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(COMBAT_ANIM.Class);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_ORIGIN(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Materials/M_Combat_Base.M_Combat_Base"));
	if (MAT_ORIGIN.Succeeded())
	{
		OriginalMat = MAT_ORIGIN.Object;
		GetMesh()->SetMaterial(0, OriginalMat);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_OPACITY(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Materials/M_Combat_Alpha_Inst.M_Combat_Alpha_Inst"));
	if (MAT_OPACITY.Succeeded())
	{
		OpacityMat = MAT_OPACITY.Object;
	}

	AIControllerClass = ACombatAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// ºÎºñÀû
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 150.0f;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetMesh()->SetRenderCustomDepth(true);
}

void ACombatAI::Die()
{
	Super::Die();

}

void ACombatAI::BeginPlay()
{
	Super::BeginPlay();

	SetTarget(InGameManager->FindActor(ARoxyCharacter::StaticClass()));
}

void ACombatAI::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (DataTable)
	{
		FGruntStatTableRow* statTableRow = DataTable->FindRow<FGruntStatTableRow> (TEXT ("Combat"), FString ("Combat Row is not exist!"));

		if (statTableRow)
		{
			HpComponent->SetMaxHp (statTableRow->maxHp);
			HpComponent->SetHP (HpComponent->GetMaxHp ());
			_moveSpeed = statTableRow->move_speed;
			_damage_Attack = statTableRow->attack_damage;
			_stiffenTime = statTableRow->sttifen_time;
			LOG (Warning, "--------------CombatAI Stat--------------");
			LOG (Warning, "Max Hp : %f", HpComponent->GetMaxHp ());
			LOG (Warning, "Move Speed : %f", _moveSpeed);
			LOG (Warning, "Attack Damage : %f", _damage_Attack);
			LOG (Warning, "Attack Stiffen Time : %f", _stiffenTime);
			LOG (Warning, "------------------------------------")
		}
	}

	CombatAnim = Cast<UCombatAnimInstance>(GetMesh()->GetAnimInstance());
	CHECK(nullptr != CombatAnim);

	GetCharacterMovement()->MaxWalkSpeed = _moveSpeed;
}

void ACombatAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
