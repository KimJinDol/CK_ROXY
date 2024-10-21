// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "GameFramework/Actor.h"
#include "InGame/Enemy/GruntAI/Combat/CombatAI.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAI.h"
#include "InGame/Enemy/GruntAI/Charge/ChargeAI.h"
#include "AIObjectPool.generated.h"

USTRUCT(BlueprintType)
struct FPoolInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI_ObjectPool, Meta = (AllowPrivateAccess = true))
		TArray<class AGruntAI*> Pool;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_ObjectPool, Meta = (AllowPrivateAccess = true))
		int32 PoolSize = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI_ObjectPool, Meta = (AllowPrivateAccess = true))
		int32 ExpandSize = 10;
};


UCLASS()
class ROXY_API AAIObjectPool : public AActor
{
	GENERATED_BODY()
	
public:	
	AAIObjectPool();

public:
	// Pop : Ǯ���� <T> ������ ���� �Ѹ��� Pop���ִ� �Լ�. Ǯ�� ���ٸ� nullptr ��ȯ
	class AGruntAI* Pop(TSubclassOf<class AGruntAI> ClassKey);

	// Push : Ǯ�� ���� ��ȯ���ִ� �Լ�
	void Push(class AGruntAI* AI);

	// Contains : �� �����̳ʿ� �ش� Ŭ���� ������ Ǯ�� �ִ��� Ȯ�����ִ� �Լ�.
	bool Contains(TSubclassOf<class AGruntAI> ClassKey);

private:
	// Expand : Key�� ���� ������ ai Object Pool Size Ȯ��
	void Expand(TSubclassOf<class AGruntAI> ClassKey);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI_ObjectPool, Meta = (AllowPrivateAccess = true))
		TMap<class TSubclassOf<class AGruntAI>, FPoolInfo> PoolInfos;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};