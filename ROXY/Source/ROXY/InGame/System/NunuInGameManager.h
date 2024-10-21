// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "GameFramework/Actor.h"
#include "NunuInGameManager.generated.h"


namespace FindActorTool
{
	// ���忡 �����ϴ� ã���� �ϴ� ������(������) ���͸� ��ȯ���ִ� �Լ��̴�.
	// ���ø� : ��üȭ�� �Ϲ�ȭ �����ش�
	// ->��ȯ ������ Ư������ �ʾƵ� ��
	// contextWorld ���ڿ��� this�� �ִ´� (GetWorld()�� �� �� �ִ� ��� ����)
	// findClass : ã���� �ϴ� Ŭ���� �̸�::StaticClass
	template<typename T>
	T* FindActor(UWorld* contextWorld, UClass* findClass)
	{
		if (!contextWorld) return NULL;

		TArray<AActor*> actor;
		UGameplayStatics::GetAllActorsOfClass(contextWorld, findClass, actor);

		// ã���� �ϴ� ���Ͱ� ���� �󿡼� �����ϴٸ� �ش� ���͸� ��ȯ�Ѵ�.
		if (actor.Num() == 1) return Cast<T>(actor[0]);

		// ã���� �ϴ� ���Ͱ� ���� �󿡼� �ټ� �����Ѵٸ� ù��°�� �ɸ��� ���͸� ��ȯ�Ѵ�.
		else if (actor.Num() > 1)
		{
			LOG_SCREEN("It's not Only! So Many!!");
			return Cast<T>(actor[0]);
		}

		// ã���� �ϴ� ���Ͱ� ���� �󿡼� �������� �ʴ´ٸ� NULL�� ��ȯ�Ѵ�.
		else
		{
			LOG_SCREEN("Not Exist <%s> In World!", *(findClass->GetName()));
			return NULL;
		}
	}

	// ���忡 �����ϴ� �ش� Ŭ������ ���� ��� ���͸� ��ȯ���ش�
	template<typename T>
	TArray<T*> FindActors(UWorld* contextWorld, UClass* findClass)
	{
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(contextWorld, findClass, actors);

		TArray<T*> returnActors;
		for (int i = 0; i < actors.Num(); i++)
		{
			returnActors.Push(Cast<T>(actors[i]));
		}
		return returnActors;
	}
}

namespace AITool
{
	void AIStopLogic (APawn* ai);
	void AIStopLogic(APawn* ai, FString& reason);
	void AIRestartLogic(APawn* ai);
	void AIBTStopLogic(APawn* ai);
	void AIBTStopLogic(APawn* ai, FString& reason);							// Behavior Tree Stop
	void AIBTRestartLogic(APawn* ai);		// Behavior Tree Restart
}


UCLASS()
class ROXY_API ANunuInGameManager : public AActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		ARoxyCharacter* GetPlayerCharacter();

	UFUNCTION(BlueprintCallable)
		TArray<AEnemyAI*> GetAllEnemys();

	UFUNCTION(BlueprintCallable)
		AActor* FindActor(UClass* findClass);


};
