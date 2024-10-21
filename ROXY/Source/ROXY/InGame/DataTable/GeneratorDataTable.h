// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Components/ActorComponent.h"
#include "GeneratorDataTable.generated.h"


USTRUCT(BlueprintType)
struct FGeneratorStatTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Repair)
		float maxRp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Repair)
		float RpIncrementPerSec; // 평소 발전기 초당 수리 증가량

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Repair)
		float RpDecrementPerSec; // 발전기 고장났을 때 초당 수리 감소량

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Repair)
		float breakDownPeriod_Min; // 발전기 고장 최소 주기

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Repair)
		float breakDownPeriod_Max; // 발전기 고장 최대 주기

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Repair)
		float RpDecrementAtDamaged; // 발전기 피격 시 감소될 발전기 수리량

	// Skill Check ----------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkillCheck)
		float skillCheckTime_Max; // 스킬 체크 가능 최대 시간

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkillCheck)
		float skillCheckableTime_Min; // 스킬 체크 지정 가능한 최소 시간

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkillCheck)
		float skillCheckToleranceTime; // 스킬 체크 허용 오차 시간

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkillCheck)
		float repairPauseDuration; // 스킬 체크 실패 시 발전기 수리 일시 정지 시간

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROXY_API UGeneratorDataTable : public UActorComponent
{
	GENERATED_BODY()		
};
