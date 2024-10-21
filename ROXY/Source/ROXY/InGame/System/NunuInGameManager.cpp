// Fill out your copyright notice in the Description page of Project Settings.


#include "NunuInGameManager.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/Enemy/EnemyAI.h"
#include "BehaviorTree/BehaviorTreeComponent.h"


ARoxyCharacter* ANunuInGameManager::GetPlayerCharacter()
{
	ARoxyCharacter* RoxyInst = FindActorTool::FindActor<ARoxyCharacter>(GetWorld(), ARoxyCharacter::StaticClass());
	return RoxyInst;
}


TArray<AEnemyAI*> ANunuInGameManager::GetAllEnemys()
{
	TArray<AEnemyAI*> Enemys = FindActorTool::FindActors<AEnemyAI>(GetWorld(), AEnemyAI::StaticClass());
	return Enemys;
}

AActor* ANunuInGameManager::FindActor(UClass* findClass)
{
	AActor* actor = FindActorTool::FindActor<AActor>(GetWorld(), findClass);
	return actor;
}

void AITool::AIStopLogic(APawn* ai, FString& reason)
{
	AAIController* AIController = Cast<AAIController>(ai->GetController());
	if (AIController)
	{
		if (AIController->GetBrainComponent())
		{
			AIController->BrainComponent->StopLogic(reason);
		}
		else
			LOG_SCREEN("%s AILogic is Don't Stop. Not Exist BrainComponent!", *ai->GetName());
	}
}

void AITool::AIStopLogic (APawn* ai)
{
	AAIController* AIController = Cast<AAIController> (ai->GetController());
	if (AIController)
	{
		if (AIController->GetBrainComponent())
		{
			AIController->BrainComponent->StopLogic("");
		}
		else
			LOG_SCREEN ("%s AILogic is Don't Stop. Not Exist BrainComponent!", *ai->GetName());
	}
}

void AITool::AIRestartLogic(APawn* ai)
{
	AAIController* AIController = Cast<AAIController>(ai->GetController());
	if (AIController)
	{
		if (AIController->GetBrainComponent())
		{
			AIController->BrainComponent->RestartLogic();
		}
		else
			LOG_SCREEN("%s AILogic is Don't Resume. Not Exist BrainComponent!", *ai->GetName());
	}
}

void AITool::AIBTStopLogic(APawn* ai)
{
	AAIController* AIController = Cast<AAIController>(ai->GetController());
	if (AIController)
	{
		UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());

		if (BTComponent)
		{
			BTComponent->StopLogic("");
		}
	}
}

void AITool::AIBTStopLogic(APawn* ai, FString& reason)
{
	AAIController* AIController = Cast<AAIController>(ai->GetController());
	if (AIController)
	{
		UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());

		if (BTComponent)
		{
			BTComponent->StopLogic(reason);
		}
	}
}

void AITool::AIBTRestartLogic(APawn* ai)
{
	AAIController* AIController = Cast<AAIController>(ai->GetController());
	if (AIController)
	{
		UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());

		if (BTComponent)
		{
			BTComponent->RestartLogic();
		}
	}
}
