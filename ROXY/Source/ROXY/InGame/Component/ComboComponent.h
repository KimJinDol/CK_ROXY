// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Components/ActorComponent.h"
#include "ComboComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FComboComponent_Delegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROXY_API UComboComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComboComponent();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = true))
		int32 _currentCombo = 0; // ���� �޺� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = true))
		int32 _highestCombo = 0; // �ְ� �޺� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = true))
		float _comboDuration = 3.f; // �޺� ���� �ð�. �ش� �ð��� �Ѿ�� �޺��� �ʱ�ȭ �ȴ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = true))
		bool _bIsHighest = true; // �ְ� ��� ��������?

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Fiver, Meta = (AllowPrivateAccess = true))
		bool _bIsFiver = false; // �ǹ� ���ΰ�?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Fiver, Meta = (AllowPrivateAccess = true))
		int32 _currentFiverCombo = 0; // �ǹ� ���� �޺� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Fiver, Meta = (AllowPrivateAccess = true))
		int32 _fiverStartNum = 140; // �ǹ� ���� ���� �޺� ��.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Fiver, Meta = (AllowPrivateAccess = true))
		float _fiverDuration = 10.f;

	FTimerHandle ComboMaintainHandle; // �޺� ���� Ÿ�̸�
	FTimerHandle FiverEndHandle; // �ǹ� ���� Ÿ�̸�

public:
	FComboComponent_Delegate OnFiver;
	FComboComponent_Delegate EndFiver;
	FComboComponent_Delegate OffCombo;


public:	
	UFUNCTION()
		void AddComboCount(int32 addedNum); // �޺� �� �߰�


	UFUNCTION()
		void InitializeComboCount(); // �޺� �ʱ�ȭ

	UFUNCTION()
		void FiverStart();

	UFUNCTION()
		void FiverEnd();

	UFUNCTION(BlueprintPure)
		int32 GetCurrentCombo();

	UFUNCTION(BlueprintPure)
		bool IsFiver();

	UFUNCTION(BlueprintPure)
		int32 GetCurrentFiverCombo();

	UFUNCTION(BlueprintPure)
		float GetFiverGaugeRatio();

	UFUNCTION(BlueprintPure)
		float GetFiverRemainingTime();

	UFUNCTION(BlueprintPure)
		float GetFiverDuration();


	UFUNCTION(BlueprintPure)
		int32 GethighestCombo();

	UFUNCTION(BlueprintPure)
		int32 GetFiverStartNum();

		
};
