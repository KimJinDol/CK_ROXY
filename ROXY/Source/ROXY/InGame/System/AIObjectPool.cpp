// Fill out your copyright notice in the Description page of Project Settings.


#include "AIObjectPool.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"

// Sets default values
AAIObjectPool::AAIObjectPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

AGruntAI* AAIObjectPool::Pop(TSubclassOf<class AGruntAI> ClassKey)
{
	FPoolInfo* Info = PoolInfos.Find(ClassKey);

	if (Info != nullptr)
	{
		if (Info->Pool.Num() == 0)
		{
			Expand(ClassKey);
		}

		AGruntAI* AI = Info->Pool.Pop();
		AI->RevivalAI();

		return AI;
	}

	LOG(Warning, "Not Exist Class In Object Pool");

	return nullptr;
}

void AAIObjectPool::Push(AGruntAI* AI)
{
	if (AI)
	{
		AI->DeactiveAI();

		TSubclassOf<AGruntAI> ClassKey = AI->GetClass();

		FPoolInfo* Info = PoolInfos.Find(ClassKey);

		if (Info)
			Info->Pool.Push(AI);
	}
}

bool AAIObjectPool::Contains(TSubclassOf<class AGruntAI> ClassKey)
{
	return PoolInfos.Find(ClassKey) != nullptr;
}

void AAIObjectPool::Expand(TSubclassOf<class AGruntAI> ClassKey)
{
	FPoolInfo* Info = PoolInfos.Find(ClassKey);
	
	if (Info)
	{
		for (int i = 0; i < Info->ExpandSize ; i++)
		{
			AGruntAI* AI = GetWorld()->SpawnActor<AGruntAI>(ClassKey);

			if (AI)
			{
				AI->DeactiveAI();

				Info->Pool.Push(AI);
			}

		}
	}
}

// Called when the game starts or when spawned
void AAIObjectPool::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIObjectPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

