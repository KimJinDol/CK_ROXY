// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "EnemySpawnerWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UEnemySpawnerWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

private:
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = Owner, meta = (AllowPrivateAccess = true))
		class AFightZoneFinder* OwnerFightZoneFinder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Owner, meta = (AllowPrivateAccess = true))
		class ADialogSystem* DialogueSystem;

public:

	UFUNCTION()
		void BindOwner(AFightZoneFinder* fightZone);

	UFUNCTION(BlueprintImplementableEvent)
		void StartNewWave(int32 waveIndex);

	UFUNCTION(BlueprintImplementableEvent)
		void FinishCurrentWave();
	
};
