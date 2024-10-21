// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogSystem.h"
#include "InGame/System/DialogueWidget.h"
#include "InGame/System/Generator.h"
#include "Kismet/KismetTextLibrary.h"

// Sets default values
ADialogSystem::ADialogSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_DIALOGUE_FLOW_TUTORIAL(TEXT("/Game/DataTable/Dialogue/DT_Dialogue_flow_Tutorial.DT_Dialogue_flow_Tutorial"));
	if (DT_DIALOGUE_FLOW_TUTORIAL.Succeeded())
	{
		DT_Dialogue_flow_Tutorial = DT_DIALOGUE_FLOW_TUTORIAL.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_DIALOGUE_FLOW_STAGE1(TEXT("/Game/DataTable/Dialogue/DT_Dialogue_flow_Stage.DT_Dialogue_flow_Stage"));
	if (DT_DIALOGUE_FLOW_STAGE1.Succeeded())
	{
		DataTable_flow  = DT_DIALOGUE_FLOW_STAGE1.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_DIALOGUE_FLOW_BOSSROOM(TEXT("/Game/DataTable/Dialogue/DT_Dialogue_flow_BossRoom.DT_Dialogue_flow_BossRoom"));
	if(DT_DIALOGUE_FLOW_BOSSROOM.Succeeded())
	{
		DT_Dialogue_flow_BossRoom = DT_DIALOGUE_FLOW_BOSSROOM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_DIALOGUE_EVENT_STAGE1(TEXT("/Game/DataTable/Dialogue/DT_Dialogue_event_Stage.DT_Dialogue_event_Stage"));
	if (DT_DIALOGUE_EVENT_STAGE1.Succeeded())
	{
		DataTable_event = DT_DIALOGUE_EVENT_STAGE1.Object;
	}

	static ConstructorHelpers::FClassFinder<UDialogueWidget> UI_DIALOGUE(TEXT("/Game/Blueprints/InGame/System/BP_DialogueWidget.BP_DialogueWidget_C"));
	if (UI_DIALOGUE.Succeeded())
	{
		WidgetTemplate = UI_DIALOGUE.Class;
	}
}

void ADialogSystem::SetDialogueInfo(int mapCode)
{
	switch (mapCode)
	{
	case 0:
		DataTable_flow = DT_Dialogue_flow_Tutorial;
		break;
	case 1:
		DataTable_flow = DT_Dialogue_flow_Stage1;
		break;
	case 2:
		DataTable_flow = DT_Dialogue_flow_BossRoom;
	}
}

void ADialogSystem::StartDialogueEvent_flow(FName rowName)
{
	if (DataTable_flow)
		DrawDialogue_flow(rowName);
	else
		LOG(Warning, "Dialogue information is doesn't set.");
}


void ADialogSystem::DrawDialogue_flow(FName rowName)
{
	if (DataTable_flow)
	{
		FDialogueColumn* tableRow = DataTable_flow->FindRow<FDialogueColumn>(rowName, FString(("DialogSystem : <%s> row is not exist!", *rowName.ToString())));
		_currentFlowDialougueRow = FCString::Atoi(*rowName.ToString());
		_currentFlowDialogueText = tableRow->dialog_text;
		_bIsKillQuest = tableRow->killQuest;
			if (tableRow)
			{
				WidgetInstance->SetDialogueInfo_flow(tableRow->char_name, tableRow->dialog_text, tableRow->int_lineNum, tableRow->text_duration, tableRow->killQuest);
			}
	}
}

void ADialogSystem::CheckNextDialogue_flow()
{
	FName currentName = FName(*FString::FromInt(_currentFlowDialougueRow));
	FName nextName = FName(*FString::FromInt(_currentFlowDialougueRow + 1));

	FDialogueColumn* currentTableRow = DataTable_flow->FindRow<FDialogueColumn>(currentName, FString(("DialogSystem : <%s> row is not exist!", *currentName.ToString())));
	FDialogueColumn* nextTableRow = DataTable_flow->FindRow<FDialogueColumn>(nextName, FString(("DialogSystem : <%s> row is not exist!", *nextName.ToString())));
	if (currentTableRow && nextTableRow)
	{
		// 다음으로 이어질 대화가 존재한다면, 또 위젯에 그려준다.
		if (nextTableRow->dialog_code == currentTableRow->dialog_code)
		{
			DrawDialogue_flow(nextName);
		}

		// 대화가 끝났다면 대화의 끝을 알리는 델리게이트를 실행한다.
		else
		{
			OnFinishConversation.Broadcast();
		
		}
	}
	else if (currentTableRow && !nextTableRow)
	{
		OnFinishConversation.Broadcast();
	}
}

int ADialogSystem::GetCurrentDialogueRow_flow()
{
	return _currentFlowDialougueRow;
}

FString ADialogSystem::GetCurrentDialogueText_flow()
{
	return _currentFlowDialogueText;
}

void ADialogSystem::DrawDialogue_event(FName rowName)
{
	LOG_SCREEN("row Name : %s", *rowName.ToString());
	if (DataTable_event)
	{
		FDialogueColumn* tableRow = DataTable_event->FindRow<FDialogueColumn>(rowName, FString(("DialogSystem : <%s> row is not exist!", *rowName.ToString())));
		
		if (tableRow)
		{
			WidgetInstance->SetDialogueInfo_event(tableRow->char_name, tableRow->dialog_text, tableRow->int_lineNum, tableRow->text_duration);
		}
	}
}

// Called when the game starts or when spawned
void ADialogSystem::BeginPlay()
{
	Super::BeginPlay();

	WidgetInstance = CreateWidget<UDialogueWidget>(GetWorld(), WidgetTemplate);
	WidgetInstance->BindOwner(this);
	WidgetInstance->AddToViewport();
	
}

void ADialogSystem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if(Generator)
		Generator->DangerousGaugeNotify.AddDynamic(this, &ADialogSystem::DrawDialogue_event);
}

// Called every frame
void ADialogSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

