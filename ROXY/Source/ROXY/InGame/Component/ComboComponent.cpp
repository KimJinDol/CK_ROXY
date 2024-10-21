// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboComponent.h"

// Sets default values for this component's properties
UComboComponent::UComboComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



void UComboComponent::AddComboCount(int32 addedNum)
{
	_currentCombo += addedNum;
	
	if (_currentCombo > _highestCombo)
	{
		_bIsHighest = true;
		_highestCombo = _currentCombo;
	}

	if(!_bIsFiver)
		_currentFiverCombo++;

	if (_currentFiverCombo >= _fiverStartNum)
	{
		_currentFiverCombo = 0;

		FiverStart();
		
		GetWorld()->GetTimerManager().SetTimer(FiverEndHandle, this, &UComboComponent::FiverEnd, _fiverDuration, false);
	}

	// 콤보 지속 타이머 재설정
	if (GetWorld()->GetTimerManager().IsTimerActive(ComboMaintainHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(ComboMaintainHandle);
	}
	GetWorld()->GetTimerManager().SetTimer(ComboMaintainHandle, this, &UComboComponent::InitializeComboCount, _comboDuration, false);

}


void UComboComponent::InitializeComboCount()
{
	_currentCombo = 0;
	/*_currentFiverCombo = 0;*/
	_bIsHighest = false;

	OffCombo.Broadcast();
}

void UComboComponent::FiverStart()
{
	_bIsFiver = true;
	ACharacter* owner = Cast<ACharacter>(GetOwner());

	if(owner)
	{ 
		OnFiver.Broadcast();
	}
}

void UComboComponent::FiverEnd()
{
	_bIsFiver = false;
	EndFiver.Broadcast();
}

int32 UComboComponent::GetCurrentCombo()
{
	return _currentCombo;
}

bool UComboComponent::IsFiver()
{
	return _bIsFiver;
}

int32 UComboComponent::GetCurrentFiverCombo()
{
	return _currentFiverCombo;
}

float UComboComponent::GetFiverGaugeRatio()
{
	return FMath::Clamp((float)_currentFiverCombo / _fiverStartNum, 0.f, 1.f);
}

float UComboComponent::GetFiverRemainingTime ()
{
	return GetWorld()->GetTimerManager().GetTimerRemaining(FiverEndHandle);
}

float UComboComponent::GetFiverDuration ()
{
	return _fiverDuration;
}

int32 UComboComponent::GethighestCombo()
{
	return _highestCombo;
}

int32 UComboComponent::GetFiverStartNum()
{
	return _fiverStartNum;
}
