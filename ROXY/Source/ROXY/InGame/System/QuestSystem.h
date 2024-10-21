// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "GameFramework/Actor.h"
#include "QuestSystem.generated.h"


USTRUCT(BlueprintType)
struct FQuestKill
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = KillInfo)
		int _amount;


};

USTRUCT(BlueprintType)
struct FQuestMove
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MoveInfo)
		FVector _location; // 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MoveInfo)
		bool _isReached;
};

USTRUCT(BlueprintType)
struct FQuestUseSkill
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MoveInfo)
		FString _name; // 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MoveInfo)
		bool _isUsed;
};

USTRUCT(BlueprintType)
struct FQuestTask
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TaskInfo)
		FQuestKill kills;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TaskInfo)
		FQuestMove move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TaskInfo)
		FQuestUseSkill skill;
};

UCLASS()
class ROXY_API AQuestSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestSystem();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = QuestInfo, meta = (AllowPrivateAccess = true))
		FQuestTask _currentQuest;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
