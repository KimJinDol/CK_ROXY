// Fill out your copyright notice in the Description page of Project Settings.


#include "GRBTService_DetectThrow.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "InGame/Enemy/GruntAI/GruntAIController.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAI.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAIController.h"
#include "InGame/Enemy/GruntAI/Throw/EliteThrow/EliteThrowAI.h"
#include "InGame/Enemy/GruntAI/Throw/EliteThrow/EliteThrowAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Behaviortree/BlackboardComponent.h"
#include "NavigationSystem.h"

UGRBTService_DetectThrow::UGRBTService_DetectThrow()
{
	NodeName = TEXT("Detect_Throw");

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UGRBTService_DetectThrow::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AGruntAI* throwAI = Cast<AGruntAI>(OwnerComp.GetAIOwner()->GetPawn());
	AEliteThrowAI* EliteThrowAI = Cast<AEliteThrowAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == throwAI) return;

	UWorld* World = throwAI->GetWorld();
	FVector throwLoc = throwAI->GetActorLocation();
	float detectRadius = 15000.0f;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, throwAI);

	bool bIsGenerator = World->OverlapMultiByChannel(
		OverlapResults,
		throwLoc,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(detectRadius),
		CollisionQueryParam);

	if (bIsGenerator)
	{
		for (auto OverlapResult : OverlapResults)
		{
			ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OverlapResult.GetActor());

			if (roxy && roxy->GetController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AGruntAIController::CharacterKey, roxy);

				if (throwAI)
				{
					throwAI->SetTarget(roxy);
					throwAI->SetMoveToTarget(true);

					if (throwAI->GetIsCanSpecialAttack())
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanSpecialAttackKey, true);
					}
					else
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanSpecialAttackKey, false);
					}

					// 죽었을 경우
					if (throwAI->GetIsDie())
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsObject(AGruntAIController::CharacterKey, nullptr);

						throwAI->SetTarget(nullptr);
						throwAI->SetMoveToTarget(false);
					}

					if (EliteThrowAI)
					{
						EliteThrowAI->SetTarget(roxy);
						EliteThrowAI->SetMoveToTarget(true);
					
						// 특수 패턴
						if (EliteThrowAI->GetIsCanSpecialAttack() && EliteThrowAI->GetIsCanEliteSpecialAttack())
						{
							// 특수 패턴과 엘리트 특수 패턴이 모두 실행 가능할 때 엘리트 특수 패턴 우선 실행
							OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanSpecialAttackKey, false);
							OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanEliteSpecialAttackKey, true);
						}
						else if (!EliteThrowAI->GetIsCanSpecialAttack() && EliteThrowAI->GetIsCanEliteSpecialAttack())
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanSpecialAttackKey, false);
							OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanEliteSpecialAttackKey, true);
						}
						else if (EliteThrowAI->GetIsCanSpecialAttack() && !EliteThrowAI->GetIsCanEliteSpecialAttack())
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanSpecialAttackKey, true);
							OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanEliteSpecialAttackKey, false);
						}
						else
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanSpecialAttackKey, false);
							OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanEliteSpecialAttackKey, false);
						}
					
						// 죽었을 경우
						if (EliteThrowAI->GetIsDie())
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(AGruntAIController::CharacterKey, nullptr);
					
							EliteThrowAI->SetTarget(nullptr);
							EliteThrowAI->SetMoveToTarget(false);
						}
					}
				}
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AGruntAIController::CharacterKey, nullptr);
	}
}
