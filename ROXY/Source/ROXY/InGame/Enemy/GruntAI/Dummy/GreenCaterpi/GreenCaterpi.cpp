// Fill out your copyright notice in the Description page of Project Settings.


#include "GreenCaterpi.h"
#include "InGame/Enemy/GruntAI/Dummy/GreenCaterpi/CaterpiAnimInstance.h"
#include "InGame/System/LockOnSystem.h"
#include "Kismet/KismetMathLibrary.h"

AGreenCaterpi::AGreenCaterpi()
{
	PrimaryActorTick.bCanEverTick = true;

// 	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_BOX(TEXT("/Game/Resources/Enemy/Grunt/GreenCaterpi/Mesh/Box_SK.Box_SK"));
// 	if (SK_BOX.Succeeded())
// 	{
// 		GetMesh()->SetSkeletalMesh(SK_BOX.Object);
// 	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_ORIGIN(TEXT("/Game/Resources/Enemy/Grunt/GreenCaterpi/Materials/GreenCaterpi_Mat.GreenCaterpi_Mat"));
	if (MAT_ORIGIN.Succeeded())
	{
		OriginalMat = MAT_ORIGIN.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_OPACITY(TEXT("/Game/Resources/Enemy/Grunt/GreenCaterpi/Materials/GreenCaterpi_Mat_Inst.GreenCaterpi_Mat_Inst"));
	if (MAT_OPACITY.Succeeded())
	{
		OpacityMat = MAT_OPACITY.Object;
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> BOX_ANIM(TEXT("/Game/Resources/Enemy/Grunt/GreenCaterpi/BP_CaterpiAnim.BP_CaterpiAnim_C"));
	if (BOX_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BOX_ANIM.Class);
	}

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));

}

void AGreenCaterpi::BeginPlay()
{
	Super::BeginPlay();

}

void AGreenCaterpi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGreenCaterpi::Die()
{
	Super::Die();

	Destroy();
}

void AGreenCaterpi::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CaterpiAnim = Cast<UCaterpiAnimInstance>(GetMesh()->GetAnimInstance());
	CHECK(CaterpiAnim != nullptr);

	HpComponent->SetMaxHp(50.f);
	HpComponent->SetHP(50.f);
}

float AGreenCaterpi::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{


	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}