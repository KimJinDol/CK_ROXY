// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Item/Item.h"
#include "Item_HP.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API AItem_HP : public AItem
{
	GENERATED_BODY()


private:
	float _addedHp = 9000.f;

protected:
		virtual void OnItemCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
