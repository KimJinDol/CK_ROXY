// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWidget.h"
#include "InGame/Enemy/EnemyAI.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void UEnemyWidget::BindAIStat(class UHpComponent* NewAIStat)
{
	// ����Ʈ ü�¹ٿ����� ���ڴ� �� ��ġ�Ǿ��ִ� ���ʹ� ����...
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
