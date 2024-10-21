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
	// Pop : 풀에서 <T> 유형의 몹을 한마리 Pop해주는 함수. 풀이 없다면 nullptr 반환
	class AGruntAI* Pop(TSubclassOf<class AGruntAI> ClassKey);

	// Push : 풀에 몹을 반환해주는 함수
	void Push(class AGruntAI* AI);

	// Contains : 맵 컨테이너에 해당 클래스 유형의 풀이 있는지 확인해주는 함수.
	bool Contains(TSubclassOf<class AGruntAI> ClassKey);

private:
	// Expand : Key로 들어온 종류의 ai Object Pool Size 확장
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