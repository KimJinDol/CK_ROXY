// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Components/ActorComponent.h"
#include "GruntDataTable.generated.h"


USTRUCT(BlueprintType)
struct FGruntStatTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	/* ---------- 기본(공통) 스탯 ---------- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicStat)
		float maxHp; /* 체력량 */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicStat)
		float move_speed; /* 이동 속도 */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
		float attack_damage; /* 기본 공격 데미지 */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
		float sttifen_time; /* 공격으로 가하는 경직의 지속 시간 */

	/* ---------- ThrowAI 투사체 관련 ---------- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
		float projectile_speed; /* Throw 기본 슛 투사체 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
		float projectile_shoot_range; /* Throw 기본 슛 투사체 사거리 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
		float projectile_stiffen_time; /* Throw 기본 투사체로 가하는 경직의 지속 시간 */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
		float projectile_meteor_speed; /* EliteThrow 메테오 투사체 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
		float projectile_meteor_stiffen_time; /* EliteThrow 메테오로 가하는 경직의 지속 시간 */

	/* ---------- 일반 몬스터 특수 패턴 ---------- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpecialAttack)
		float specialAttack_damage; /* 일반 몬스터 특수 패턴 데미지 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpecialAttack)
		float specialAttack_coolTime; /* 일반 몬스터 특수 패턴 쿨타임 */

	/* ---------- 엘리트 몬스터 특수 패턴 ---------- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EliteSpecialAttack)
		float eliteSpecialAttack_damage; /* 엘리트 몬스터 패턴 데미지 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EliteSpecialAttack)
		float eliteSpecialAttack_coolTime; /* 엘리트 몬스터 패턴 쿨타임 */

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROXY_API UGruntDataTable : public UActorComponent
{
	GENERATED_BODY()
		
};
