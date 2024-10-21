// Fill out your copyright notice in the Description page of Project Settings.


#include "GruntWidget.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "InGame/Component/HpComponent.h"
#include "Components/ProgressBar.h"

void UGruntWidget::BindAIStat(class UHpComponent* NewAIStat)
{
	CHECK(nullptr != NewAIStat);

	CurrentAIStat = NewAIStat;
}

void UGruntWidget::BindDamage(float damage)
{
	DamageAmount = damage;
}

void UGruntWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	//CHECK(nullptr != HPProgressBar);
}

float UGruntWidget::GetHpPercent()
{
	return CurrentAIStat.IsValid() ? CurrentAIStat->GetHpRatio() : NULL;
}

float UGruntWidget::GetDamage()
{
	return DamageAmount;
}
