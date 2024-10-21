// Fill out your copyright notice in the Description page of Project Settings.


#include "RoxyWidget.h"
#include "Core/NunuGameInstance.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/Component/ComboComponent.h"


void URoxyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	EnemysInfoForMinimap = FindActorTool::FindActors<AEnemyAI>(GetWorld(), AEnemyAI::StaticClass());

	if (EnemysInfoForMinimap.Num() > 0)
	{

	}
}

void URoxyWidget::BindOwner(class ARoxyCharacter* _playerCharacter)
{
	CHECK(_playerCharacter);

	PlayerCharacter = _playerCharacter;

	/*BindNunuInGameManager(InGameManager);*/
}

