// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_HP.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/Component/HpComponent.h"

void AItem_HP::OnItemCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ARoxyCharacter::StaticClass()))
	{
		ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OtherActor);
		if (roxy)
		{
			if(FMath::IsNearlyEqual(roxy->GetHpComponent()->GetHp(), roxy->GetHpComponent()->GetMaxHp(), KINDA_SMALL_NUMBER))
				return;
			roxy->GetHpComponent()->AddHp(_addedHp);
			DestroyItem();
		}
	}
}
