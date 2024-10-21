// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWidget.h"
#include "InGame/Enemy/EnemyAI.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void UEnemyWidget::BindAIStat(class UHpComponent* NewAIStat)
{
	// 엘리트 체력바용으로 쓰겠다 이 방치되어있던 에너미 위젯...
	CHECK(nullptr != NewAIStat);

	CurrentAIStat = NewAIStat;
}

void UEnemyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	CHECK(nullptr != HPProgressBar);
}

float UEnemyWidget::GetHpPercent()
{
	return CurrentAIStat.IsValid() ? CurrentAIStat->GetHpRatio() : NULL;
}
