// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "GameFramework/Actor.h"
#include "RoxyDataTable.generated.h"


USTRUCT(BlueprintType)
struct FRoxyStatTableRow : public FTableRowBase
{
	GENERATED_BODY()

	// 스탯 ---------------------------------

	/*기본 최대 hp*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
		float maxHp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
		float ultimateGaugeIncrementPerSec;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
		float moveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
		float TurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat)
		float LookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
		float cameraDistanceOrigin;

	// attack & skill ---------------

	/*기본 공격력*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = stat)
		float ad;
	/*치명타 (곱해지는 딜량)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = stat)
		float criticalHitMultiply;
	/*치명타가 발생할 기본 확률*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = stat)
		float criticalHitProbability;

	// attack -----------------------

	/*기본 공격 데미지*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
		float attack_damage;
	/*기본 공격 계수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
		float attack_ratio;
	/*기본 공격 스탯(플마값)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
		float attack_stat;
	/*1타 불릿타임_시간 팽창 정도*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
		float attack1_bulletTimeDilation;
	/*1타 불릿타임_유지시간*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
		float attack1_bulletTimeDuration;

	/*2타 불릿타임_시간 팽창 정도*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
		float attack2_bulletTimeDilation;
	/*2타 불릿타임_유지시간*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
		float attack2_bulletTimeDuration;

	/*3타 불릿타임_시간 팽창 정도*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
		float attack3_bulletTimeDilation;
	/*3타 불릿타임_유지시간*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
		float attack3_bulletTimeDuration;

	/*막타 불릿타임_시간 팽창 정도*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
		float lastAttack_bulletTimeDilation;
	/*막타 불릿타임_유지시간*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
		float lastAttack_bulletTimeDuration;

	// kick --------------------------

	/*발차기 데미지*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Kick)
		float kick_damage;
	/*발차기 계수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Kick)
		float kick_ratio;
	/*발차기 스탯(플마값)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Kick)
		float kick_stat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Kick)
		float kick_coolTime;
	/*4타(발차기) 불릿타임_시간 팽창 정도*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Kick)
		float kick_bulletTimeDilation;
	/*4타(발차기) 불릿타임_유지시간*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Kick)
		float kick_bulletTimeDuration;

	// pierce -------------------------
	/*일섬 스킬 데미지*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pierce)
		float pierce_damage;
	/*일섬 계수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pierce)
		float pierce_ratio;
	/*일섬 스탯(플마값)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pierce)
		float pierce_stat;
	/*일섬 기본 쿨타임*/
	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = Pierce)
		float pierce_coolTime;
	/*일섬 시 속도 증가량*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pierce)
		float pierce_speed;
	/*일섬 시 카메라가 얼마나 멀어질 지*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pierce)
		float pierce_targetArmLength;

	// SwiftSlash ----------------------
	
	/*연참 스킬 데미지*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SwiftSlash)
		float swiftSlash_damage;
	/*기본 공격 계수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SwiftSlash)
		float swiftSlash_ratio;
	/*일섬 스탯(플마값)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SwiftSlash)
		float swiftSlash_stat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SwiftSlash)
		float swiftSlash_coolTime;

	// evade ---------------------------

	/*대쉬 시 속도 증가량(스피드)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Evade)
		float evade_speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Evade)
		float evade_coolTime;

	// Ghost Sword -------------------

	/*환영검이 충전되는 쿨타임*/
	
	/*환영검 스킬 데미지*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GhostSword)
		float ghostSword_damage;
	/*환영검 계수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GhostSword)
		float ghostSword_ratio;
	/*환영검 스탯(플마값)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GhostSword)
		float ghostSword_stat;
	/*환영검 고정 데미지(표식이 있는 적에게 또 표식을 남겼을 때 적용되는 데미지)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GhostSword)
		float ghostSword_fixedDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GhostSword)
		float ghostSword_coolTime;
};


UCLASS()
class ROXY_API ARoxyDataTable : public AActor
{
	GENERATED_BODY()

};
