// Fill out your copyright notice in the Description page of Project Settings.


#include "HpComponent.h"

// Sets default values for this component's properties
UHpComponent::UHpComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


float UHpComponent::SetHP(float newHp)
{
	return _hp = FMath::Clamp(newHp, 0.0f, _maxHp);
}

void UHpComponent::AddHp(float addedHp)
{
	_hp = FMath::Clamp(_hp + addedHp, 0.0f, _maxHp);
}

void UHpComponent::SetMaxHp(float maxHp)
{
	_maxHp = maxHp;
	_hp = _maxHp;
}

void UHpComponent::AddMaxHp(float addedMaxHp)
{
	_maxHp += addedMaxHp;
	_hp += addedMaxHp;
}

void UHpComponent::SetToMaxHp()
{
	_hp = _maxHp;
	OnChangeHp.Broadcast();
}

float UHpComponent::GetHp()
{
	return _hp;
}

float UHpComponent::GetMaxHp()
{
	return _maxHp;
}

float UHpComponent::GetHpRatio()
{
	return (_hp <= KINDA_SMALL_NUMBER) ? 0.0f : (_hp / _maxHp);
}

float UHpComponent::IsDie()
{
	return _bIsDie;
}

float UHpComponent::TakeDamage(float damage)
{
	_hp = FMath::Clamp(_hp - damage, 0.0f, _maxHp);
	OnChangeHp.Broadcast();
	if (_hp <= KINDA_SMALL_NUMBER)
	{ 
		_bIsDie = true;
		OnDie.Broadcast();
	}
	return _hp;
}

