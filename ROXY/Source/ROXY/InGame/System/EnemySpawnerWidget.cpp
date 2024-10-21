// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnerWidget.h"

void UEnemySpawnerWidget::BindOwner(AFightZoneFinder* fightZone)
{
	CHECK(fightZone);

	OwnerFightZoneFinder = fightZone;
}
