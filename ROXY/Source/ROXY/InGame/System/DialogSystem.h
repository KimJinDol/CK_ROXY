// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/DataTable/DialogueStruct.h"
#include "GameFramework/Actor.h"
#include "DialogSystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDialogue_Delegate);


UCLASS()
class ROXY_API ADialogSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADialogSystem();

private:
	// 다이얼로그 인스턴스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		class UDataTable* DT_Dialogue_flow_Tutorial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		class UDataTable* DT_Dialogue_flow_Stage1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		class UDataTable* DT_Dialogue_flow_BossRoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		class UDataTable* DataTable_flow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		class UDataTable* DataTable_event;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
		class UDialogueWidget* WidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, Meta = (AllowPrivateAccess = true))
		TSubclassOf<UDialogueWidget> WidgetTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, Meta = (AllowPrivateAccess = true))
		class AGenerator* Generator;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Dialogue, Meta = (AllowPrivateAccess = true))
		int _currentFlowDialougueRow = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Dialogue, Meta = (AllowPrivateAccess = true))
		FString _currentFlowDialogueText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Dialogue, Meta = (AllowPrivateAccess = true))
		bool _bIsKillQuest;
public:
	// 하나의 대화 구성이 끝났을 때 실행된다.
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = Delegate)
		FDialogue_Delegate OnFinishConversation;
	

public:
	UFUNCTION(BlueprintCallable)
		void SetDialogueInfo(int mapCode);
	// 다이얼로그 이벤트 시작 함수
	// 지정한 행에 대한 다이얼로그 이벤트를 시작한다.
	UFUNCTION(BlueprintCallable)
		void StartDialogueEvent_flow(FName rowName);
	
	// 위젯에 다이얼로그를 그린다.
	UFUNCTION(BlueprintCallable)
		void DrawDialogue_flow(FName rowName);

	// 다음으로 이어질 대화가 있는지 확인하는 함수
	UFUNCTION(BlueprintCallable)
		void CheckNextDialogue_flow();

	// 현재 다이얼로그 행을 구함
	UFUNCTION(BlueprintGetter)
		int GetCurrentDialogueRow_flow();

	// 현재 다이얼로그 텍스트를 구함
	UFUNCTION(BlueprintGetter)
		FString GetCurrentDialogueText_flow();

	UFUNCTION(BlueprintPure)
		bool GetIsKillQuest() {return _bIsKillQuest;}

	UFUNCTION(BlueprintCallable)
		void SetCurrentDialogueRow_flow(int row) {_currentFlowDialougueRow = row;}

	// 다이얼로그 텍스트를 화면에 그릴 함수
	UFUNCTION(BlueprintCallable)
		void DrawDialogue_event(FName rowName);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	virtual void PostInitializeComponents() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
