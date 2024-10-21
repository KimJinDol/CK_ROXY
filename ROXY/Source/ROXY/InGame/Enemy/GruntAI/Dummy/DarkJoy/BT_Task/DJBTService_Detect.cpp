// Fill out your copyright notice in the Description page of Project Settings.


#include "DJBTService_Detect.h"
#include "InGame/Enemy/GruntAI/Dummy/DarkJoy/DarkJoy.h"
#include "InGame/Enemy/GruntAI/Dummy/DarkJoy/DarkJoyAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/System/Generator.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Behaviortree/BlackboardComponent.h"
#include "NavigationSystem.h"

UDJBTService_Detect::UDJBTService_Detect()
{
	NodeName = TEXT("Detect");

	Interval = 0.001f;
	RandomDeviation = 0.0f;
}

void UDJBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ADarkJoy* darkJoy = Cast<ADarkJoy>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == darkJoy) return;

	UWorld* World = darkJoy->GetWorld();
	FVector centerVec = darkJoy->GetActorLocation();
	float detectRadius = 30000.0f;

	if (nullptr == World) return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, darkJoy);

	bool bIsStoneDetected = World->OverlapMultiByChannel(
		OverlapResults,
		centerVec,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(detectRadius),
		CollisionQueryParam);

	if (bIsStoneDetected || darkJoy->GetCanDetect())
	{
		for (auto OverlapResult : OverlapResults)
		{
			AGenerator* generator = Cast<AGenerator>(OverlapResult.GetActor());
			ARoxyCharacter* Roxy = Cast<ARoxyCharacter>(OverlapResult.GetActor());

			if (generator)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ADarkJoyAIController::GeneratorKey, generator);

				// 이동 할 발전기 주변 랜덤위치 찾기
				FVector generatorLoc = generator->GetActorLocation();

				UNavigationSystemV1* NaviSystem = UNavigationSystemV1::GetNavigationSystem(darkJoy->GetWorld());
				if (nullptr == NaviSystem) return;

				FNavLocation targetLoc;

				if (NaviSystem->GetRandomPointInNavigableRadius(generatorLoc, 300.0f, targetLoc))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(ADarkJoyAIController::GeneratorLocKey, targetLoc.Location);
				}
				// ---------------------------------------------

				if (darkJoy->GetIsDie())
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(ADarkJoyAIController::GeneratorKey, nullptr);
				}
			}

			if (Roxy && Roxy->GetController())
			{
				darkJoy->SetMovable(false);

				// HP위젯 표시
				if (darkJoy->GetGruntHUDWidget() != nullptr)
				{
					float dist = FVector::Dist(darkJoy->GetActorLocation(), Roxy->GetActorLocation());

					if (dist < 1000.0f)
					{
						darkJoy->GetGruntHUDWidget()->SetVisibility(true);

						if (dist < 300.0f)
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(ADarkJoyAIController::CharacterKey, Roxy);
						}
						else
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(ADarkJoyAIController::CharacterKey, nullptr);
						}
					}
					else
					{
						darkJoy->GetGruntHUDWidget()->SetVisibility(false);
					}
				}
				if (Roxy->GetCharacterIsDie())
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(ADarkJoyAIController::GeneratorKey, nullptr);
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(ADarkJoyAIController::CharacterKey, nullptr);
				}
			}
			else
			{
				darkJoy->SetMovable(true);
			}
		}
	}

	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ADarkJoyAIController::GeneratorKey, nullptr);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ADarkJoyAIController::CharacterKey, nullptr);
	}

	//DrawDebugSphere(World, centerVec, detectRadius, 16, FColor::Magenta, false, 0.1f);
}
