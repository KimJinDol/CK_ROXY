// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Components/ActorComponent.h"
#include "UltimateComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FUltimateComponentDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROXY_API UUltimateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUltimateComponent();

private:
	bool _bActive = true;
	bool _bIsUltimateOn = false;
	float _ultimateGauge; // ���� �ñر� ������
	float _incrementPerSec = 0.5f; // 1�ʴ� �ñر� ������ ������
	const float _maxUltimateGauge = 100.f;

public:
	FUltimateComponentDelegate OnUltimate; // �ñر� On �� ȣ��

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void SetActiveUltimateComponent(bool value);
	
	UFUNCTION()
		void SetInitUltimateGauge(); // �ñر� ��� �� ������ �ʱ�ȭ

	UFUNCTION()
		void SetIncrementPerSec(float incrementPerSec);
		
	UFUNCTION()
		void AddUltimateGauge(float GaugeAmount);

	UFUNCTION(BlueprintPure, Category = Getter)
		bool IsActiveUltimateComponent();

	UFUNCTION(BlueprintPure, Category = Getter)
		bool GetUltimateOn();

	UFUNCTION(BlueprintPure, Category = Getter)
		float GetUltimateGaugeRatio();

	UFUNCTION(BlueprintPure, Category = Getter)
		float GetMaxUltimateGauge();

	UFUNCTION(BlueprintPure, Category = Getter)
		float GetIncrementPerSec();

};
