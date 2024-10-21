// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkJoyAnimInstance.h"
#include "InGame/Enemy/GruntAI/Dummy/DarkJoy/DarkJoy.h"

UDarkJoyAnimInstance::UDarkJoyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_HIT(TEXT("/Game/Resources/Enemy/Grunt/DarkJoy/Mon_Hit_Joy.Mon_Hit_Joy"));
	if (MON_HIT.Succeeded())
	{
		HitMontage = MON_HIT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_KNOCKBACK(TEXT("/Game/Resources/Enemy/Grunt/DarkJoy/Mon_KnockBack_Joy.Mon_KnockBack_Joy"));
	if (MON_KNOCKBACK.Succeeded())
	{
		KnockBackMontage = MON_KNOCKBACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_ATTACK(TEXT("/Game/Resources/Enemy/Grunt/DarkJoy/Mon_Attack_Joy.Mon_Attack_Joy"));
	if (MON_ATTACK.Succeeded())
	{
		AttackMontage = MON_ATTACK.Object;
	}
}

void UDarkJoyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	DarkJoy = Cast<ADarkJoy>(TryGetPawnOwner());
	_currentSpeedRate = 0.0f;
}

void UDarkJoyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (DarkJoy)
	{
		_currentSpeedRate = DarkJoy->GetVelocity().Size();
	}
}
