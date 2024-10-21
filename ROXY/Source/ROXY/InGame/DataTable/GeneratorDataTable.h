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
		float RpIncrementPerSec; // ��� ������ �ʴ� ���� ������

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Repair)
		float RpDecrementPerSec; // ������ ���峵�� �� �ʴ� ���� ���ҷ�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Repair)
		float breakDownPeriod_Min; // ������ ���� �ּ� �ֱ�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Repair)
		float breakDownPeriod_Max; // ������ ���� �ִ� �ֱ�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Repair)
		float RpDecrementAtDamaged; // ������ �ǰ� �� ���ҵ� ������ ������

	// Skill Check ----------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkillCheck)
		float skillCheckTime_Max; // ��ų üũ ���� �ִ� �ð�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkillCheck)
		float skillCheckableTime_Min; // ��ų üũ ���� ������ �ּ� �ð�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkillCheck)
		float skillCheckToleranceTime; // ��ų üũ ��� ���� �ð�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SkillCheck)
		float repairPauseDuration; // ��ų üũ ���� �� ������ ���� �Ͻ� ���� �ð�

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROXY_API UGeneratorDataTable : public UActorComponent
{
	GENERATED_BODY()		
};
