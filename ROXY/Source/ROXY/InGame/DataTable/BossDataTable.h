// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Components/ActorComponent.h"
#include "BossDataTable.generated.h"


USTRUCT(BlueprintType)
struct FBossStatTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicStat)
		float maxHp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicStat)
		float smoothTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Escape)
		float escapeSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Escape)
		float escapeRandRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Escape)
		float escapeMinRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Avoidance)
		float avoidanceSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Avoidance)
		float avoidanceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile_Horizontal)
		int32 ProjNum_Horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile_Horizontal)
		int32 ProjAngleGap_Horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile_Delay)
		int32 recreateProjNum_Delay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile_Delay)
		int32 ProjNum_Delay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile_Delay)
		int32 Proj_Y_AngleGap_Delay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile_Homing)
		int32 recreateProjNum_Homing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile_Fake)
		int32 recreateProjnum_Fake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile_Fake)
		int32 ProjNum_Fake;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROXY_API UBossDataTable : public UActorComponent
{
	GENERATED_BODY()
		
};
