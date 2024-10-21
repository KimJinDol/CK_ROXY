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
		int32 _currentCombo = 0; // 현재 콤보 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = true))
		int32 _highestCombo = 0; // 최고 콤보 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = true))
		float _comboDuration = 3.f; // 콤보 지속 시간. 해당 시간을 넘어가면 콤보가 초기화 된다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = true))
		bool _bIsHighest = true; // 최고 기록 갱신인지?

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Fiver, Meta = (AllowPrivateAccess = true))
		bool _bIsFiver = false; // 피버 중인가?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Fiver, Meta = (AllowPrivateAccess = true))
		int32 _currentFiverCombo = 0; // 피버 전용 콤보 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Fiver, Meta = (AllowPrivateAccess = true))
		int32 _fiverStartNum = 140; // 피버 적용 시작 콤보 수.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Fiver, Meta = (AllowPrivateAccess = true))
		float _fiverDuration = 10.f;

	FTimerHandle ComboMaintainHandle; // 콤보 유지 타이머
	FTimerHandle FiverEndHandle; // 피버 종료 타이머

public:
	FComboComponent_Delegate OnFiver;
	FComboComponent_Delegate EndFiver;
	FComboComponent_Delegate OffCombo;


public:	
	UFUNCTION()
		void AddComboCount(int32 addedNum); // 콤보 수 추가


	UFUNCTION()
		void InitializeComboCount(); // 콤보 초기화

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
