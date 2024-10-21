// Fill out your copyright notice in the Description page of Project Settings.


#include "UltimateComponent.h"

// Sets default values for this component's properties
UUltimateComponent::UUltimateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called every frame
void UUltimateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!_bActive) return;

	AddUltimateGauge(_incrementPerSec * DeltaTime);

	if (_ultimateGauge >= _maxUltimateGauge)
		OnUltimate.Broadcast();

}

void UUltimateComponent::SetActiveUltimateComponent(bool value)
{
	_bActive = value;
}

void UUltimateComponent::SetInitUltimateGauge()
{
	_ultimateGauge = 0.f;
}

void UUltimateComponent::SetIncrementPerSec(float incrementPerSec)
{
	_incrementPerSec = FMath::Clamp(incrementPerSec, 0.0f, _maxUltimateGauge);
}

void UUltimateComponent::AddUltimateGauge(float GaugeAmount)
{
	_ultimateGauge += FMath::Clamp(GaugeAmount, 0.0f, _maxUltimateGauge);

	if (_ultimateGauge == _maxUltimateGauge)
		_bIsUltimateOn = true;
}

bool UUltimateComponent::IsActiveUltimateComponent()
{
	return _bActive;
}

bool UUltimateComponent::GetUltimateOn()
{
	return _bIsUltimateOn;
}

float UUltimateComponent::GetUltimateGaugeRatio()
{
	return (_ultimateGauge < KINDA_SMALL_NUMBER) ? 0.0f : (_ultimateGauge / _maxUltimateGauge);
}

float UUltimateComponent::GetMaxUltimateGauge()
{
	return _maxUltimateGauge;
}

float UUltimateComponent::GetIncrementPerSec()
{
	return _incrementPerSec;
}
