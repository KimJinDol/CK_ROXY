// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Components/ActorComponent.h"
#include "HpComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FHpComponent_Delegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROXY_API UHpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHpComponent();

private:
	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = "Hp", Meta = (AllowPrivateAccess = true))
		float _hp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hp", Meta = (AllowPrivateAccess = true))
		float _maxHp;
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "Hp", Meta = (AllowPrivateAccess = true))
		bool _bIsDie;

public:
	FHpComponent_Delegate OnChangeHp;
	FHpComponent_Delegate OnDie;

public:

	UFUNCTION(BlueprintCallable)
		float SetHP(float newHp);

	UFUNCTION()
		void AddHp(float addedHp);

	UFUNCTION(BlueprintCallable)
		void SetMaxHp(float maxHp);

	UFUNCTION()
		void AddMaxHp(float addedMaxHp);

	UFUNCTION()
		void SetToMaxHp(); // 부활 프로세스에서 사용

	// BlueprintPure 는 블루프린트 내에서 Getter 류의 함수에 지정하는 매크로이다
	UFUNCTION(BlueprintPure, Category = Getter)
		float GetHp();

	UFUNCTION(BlueprintPure, Category = Getter)
		float GetMaxHp();

	UFUNCTION(BlueprintPure, Category = Getter)
		float GetHpRatio();

	UFUNCTION(BlueprintPure, Category = Getter)
		float IsDie();

	float TakeDamage(float damage);

		
};
