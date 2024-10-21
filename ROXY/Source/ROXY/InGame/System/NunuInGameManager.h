// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "GameFramework/Actor.h"
#include "NunuInGameManager.generated.h"


namespace FindActorTool
{
	// 월드에 존재하는 찾고자 하는 유일한(단일의) 액터를 반환해주는 함수이다.
	// 템플릿 : 구체화를 일반화 시켜준다
	// ->반환 형식을 특정하지 않아도 됨
	// contextWorld 인자에는 this를 넣는다 (GetWorld()를 쓸 수 있는 모든 액터)
	// findClass : 찾고자 하는 클래스 이름::StaticClass
	template<typename T>
	T* FindActor(UWorld* contextWorld, UClass* findClass)
	{
		if (!contextWorld) return NULL;

		TArray<AActor*> actor;
		UGameplayStatics::GetAllActorsOfClass(contextWorld, findClass, actor);

		// 찾고자 하는 액터가 월드 상에서 유일하다면 해당 액터를 반환한다.
		if (actor.Num() == 1) return Cast<T>(actor[0]);

		// 찾고자 하는 액터가 월드 상에서 다수 존재한다면 첫번째로 걸리는 액터를 반환한다.
		else if (actor.Num() > 1)
		{
			LOG_SCREEN("It's not Only! So Many!!");
			return Cast<T>(actor[0]);
		}

		// 찾고자 하는 액터가 월드 상에서 존재하지 않는다면 NULL을 반환한다.
		else
		{
			LOG_SCREEN("Not Exist <%s> In World!", *(findClass->GetName()));
			return NULL;
		}
	}

	// 월드에 존재하는 해당 클래스를 가진 모든 액터를 반환해준다
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
