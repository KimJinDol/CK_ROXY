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
	/* ---------- �⺻(����) ���� ---------- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicStat)
		float maxHp; /* ü�·� */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BasicStat)
		float move_speed; /* �̵� �ӵ� */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
		float attack_damage; /* �⺻ ���� ������ */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
		float sttifen_time; /* �������� ���ϴ� ������ ���� �ð� */

	/* ---------- ThrowAI ����ü ���� ---------- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
		float projectile_speed; /* Throw �⺻ �� ����ü �ӵ� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
		float projectile_shoot_range; /* Throw �⺻ �� ����ü ��Ÿ� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
		float projectile_stiffen_time; /* Throw �⺻ ����ü�� ���ϴ� ������ ���� �ð� */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
		float projectile_meteor_speed; /* EliteThrow ���׿� ����ü �ӵ� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
		float projectile_meteor_stiffen_time; /* EliteThrow ���׿��� ���ϴ� ������ ���� �ð� */

	/* ---------- �Ϲ� ���� Ư�� ���� ---------- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpecialAttack)
		float specialAttack_damage; /* �Ϲ� ���� Ư�� ���� ������ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpecialAttack)
		float specialAttack_coolTime; /* �Ϲ� ���� Ư�� ���� ��Ÿ�� */

	/* ---------- ����Ʈ ���� Ư�� ���� ---------- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EliteSpecialAttack)
		float eliteSpecialAttack_damage; /* ����Ʈ ���� ���� ������ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EliteSpecialAttack)
		float eliteSpecialAttack_coolTime; /* ����Ʈ ���� ���� ��Ÿ�� */

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROXY_API UGruntDataTable : public UActorComponent
{
	GENERATED_BODY()
		
};
