// Fill out your copyright notice in the Description page of Project Settings.


#include "GRBTService_Detect.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "InGame/Enemy/GruntAI/GruntAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/System/Generator.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Behaviortree/BlackboardComponent.h"
#include "NavigationSystem.h"

UGRBTService_Detect::UGRBTService_Detect()
{
	NodeName = TEXT("Perception");

	Interval = 0.2f;
	RandomDeviation = 0.0f;
}

void UGRBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AGruntAI* grunt = Cast<AGruntAI>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == grunt) return;

	UWorld* World = grunt->GetWorld();
	FVector gruntLoc = grunt->GetActorLocation();
	float detectRadius = 15000.0f;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, grunt);

	bool bIsDetect = World->OverlapMultiByChannel(
		OverlapResults,
		gruntLoc,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(detectRadius),
		CollisionQueryParam);

	if (bIsDetect)
	{
		for (auto OverlapResult : OverlapResults)
		{
			ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OverlapResult.GetActor());

			if (roxy && roxy->GetController())
			{
				if (!grunt->IsAvoiding())
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(AGruntAIController::CharacterKey, roxy);

					grunt->SetTarget(roxy);
					grunt->SetMoveToTarget(true);

					// 특수 패턴
					//if (grunt->GetIsCanSpecialAttack() && grunt->GetIsCanEliteSpecialAttack())
					//{
					//	// 특수 패턴과 엘리트 특수 패턴이 모두 실행 가능할 때 엘리트 특수 패턴 우선 실행
					//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanSpecialAttackKey, false);
					//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanEliteSpecialAttackKey, true);
					//}
					//else if (!grunt->GetIsCanSpecialAttack() && grunt->GetIsCanEliteSpecialAttack())
					//{
					//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanSpecialAttackKey, false);
					//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanEliteSpecialAttackKey, true);
					//}
					//else if (grunt->GetIsCanSpecialAttack() && !grunt->GetIsCanEliteSpecialAttack())
					//{
					//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanSpecialAttackKey, true);
					//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanEliteSpecialAttackKey, false);
					//}
					//else
					//{
					//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanSpecialAttackKey, false);
					//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanEliteSpecialAttackKey, false);
					//}
					//
					//if (grunt->GetIsCanSpecialAttack())
					//{
					//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanSpecialAttackKey, true);
					//}
					//else
					//{
					//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanSpecialAttackKey, false);
					//}

					if (grunt->GetIsCanEliteSpecialAttack())
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanEliteSpecialAttackKey, true);
					}
					else
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsBool(AGruntAIController::CanEliteSpecialAttackKey, false);
					}

					// 죽었을 경우
					if (grunt->GetIsDie())
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsObject(AGruntAIController::CharacterKey, nullptr);

						grunt->SetTarget(nullptr);
						grunt->SetMoveToTarget(false);
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
