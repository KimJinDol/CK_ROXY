
#include "RevivalAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "InGame/Enemy/GruntAI/Revival/RevivalAI.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"

ARevivalAIController::ARevivalAIController()
{

}

void ARevivalAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwningPawn = Cast<ARevivalAI>(InPawn);
	CHECK(OwningPawn);

	OwningPawn->OnSpawnEnd.AddUFunction(this, "SetBossState", ERevivalState::RS_REVIVAL); // 처음 부활 몹의 스폰이 끝난 후 호출
	OwningPawn->OnEndShootProj.AddUFunction(this, "SetBossState", ERevivalState::RS_IDLE); // RS_IDLE은 대기 상태임 (무적)
	OwningPawn->OnEndRevival.AddUFunction(this, "SetBossState", ERevivalState::RS_IDLE);
	OwningPawn->OnAttackTaskEnd.AddUFunction(this, "SetBossState", ERevivalState::RS_IDLE); // OnAttackTakeEnd 델리게이트는 GruntAI 에 선언돼있음
	OwningPawn->OnEndTired.AddUFunction(this, "SetBossState", ERevivalState::RS_REVIVAL); // 쳐맞기 상태가 끝난 후 호출
}

void ARevivalAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	EndStandByDelegate.BindLambda([this]()->void {_bIsStandBy = false; });
}

void ARevivalAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(OwningPawn == nullptr) return;
	
	/*------------------------- FSM --------------------------*/
	if(_currentBossState == ERevivalState::RS_IDLE)
	{
		if(_bIsStandBy && OwningPawn->GetRestEnemysNum() == 0) // idle 상태이고 적을 다 처치시 쳐맞기 상태
		{
			_bIsStandBy = false;
			SetBossState(ERevivalState::RS_CANTAKEDAMAGE);
		}
		else if(!_bIsStandBy) 
		{
			if(FVector::Distance(Target->GetActorLocation(), OwningPawn->GetActorLocation()) < 500.f)
			{
				SetBossState(ERevivalState::RS_SHORT_RANGE_ATTACK);
			}
			else
			{
				SetBossState(ERevivalState::RS_LONG_RANGE_ATTACK);		
			}
		}
	}

	
	/*-----------------------------------------------------------*/
}

void ARevivalAIController::BeginPlay()
{
	Super::BeginPlay();

	BindNunuInGameManager(InGameManager);
	Target = InGameManager->GetPlayerCharacter();

}

void ARevivalAIController::SetBossState(ERevivalState newState /*= ERevivalState::RS_IDLE*/)
{
	switch(newState) {
	case ERevivalState::RS_IDLE:
		_bIsStandBy = true;
		_currentBossState = ERevivalState::RS_IDLE;
		if(OwningPawn->GetHpComponent()->GetHpRatio() <= 0.8f && OwningPawn->GetHpComponent()->GetHpRatio() > 0.6f)
			GetWorldTimerManager().SetTimer(StandByTimer, EndStandByDelegate, _standByDuration_hp80below60over, false);
		else if(OwningPawn->GetHpComponent()->GetHpRatio() <= 0.6f && OwningPawn->GetHpComponent()->GetHpRatio() > 0.3f)
			GetWorldTimerManager().SetTimer(StandByTimer, EndStandByDelegate, _standByDuration_hp60below30over, false);
		else if(OwningPawn->GetHpComponent()->GetHpRatio() <= 0.3f)
			GetWorldTimerManager().SetTimer(StandByTimer, EndStandByDelegate, _standByDuration_hp30below, false);
		break;
	case ERevivalState::RS_CANTAKEDAMAGE:
		GetWorldTimerManager().ClearTimer(StandByTimer);
		_currentBossState = ERevivalState::RS_CANTAKEDAMAGE;
		OwningPawn->StartTired();
		break;
	case ERevivalState::RS_LONG_RANGE_ATTACK:
		_currentBossState = ERevivalState::RS_LONG_RANGE_ATTACK;
		OwningPawn->BeginShootProj();
		break;
	case ERevivalState::RS_SHORT_RANGE_ATTACK:
		_currentBossState = ERevivalState::RS_SHORT_RANGE_ATTACK;
		OwningPawn->StartAttack();
		break;
	case ERevivalState::RS_REVIVAL:
		_currentBossState = ERevivalState::RS_REVIVAL;
		OwningPawn->StartRevival();
		break;
	}
}
