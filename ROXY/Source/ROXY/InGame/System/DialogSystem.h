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
	// ���̾�α� �ν��Ͻ�
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
	// �ϳ��� ��ȭ ������ ������ �� ����ȴ�.
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = Delegate)
		FDialogue_Delegate OnFinishConversation;
	

public:
	UFUNCTION(BlueprintCallable)
		void SetDialogueInfo(int mapCode);
	// ���̾�α� �̺�Ʈ ���� �Լ�
	// ������ �࿡ ���� ���̾�α� �̺�Ʈ�� �����Ѵ�.
	UFUNCTION(BlueprintCallable)
		void StartDialogueEvent_flow(FName rowName);
	
	// ������ ���̾�α׸� �׸���.
	UFUNCTION(BlueprintCallable)
		void DrawDialogue_flow(FName rowName);

	// �������� �̾��� ��ȭ�� �ִ��� Ȯ���ϴ� �Լ�
	UFUNCTION(BlueprintCallable)
		void CheckNextDialogue_flow();

	// ���� ���̾�α� ���� ����
	UFUNCTION(BlueprintGetter)
		int GetCurrentDialogueRow_flow();

	// ���� ���̾�α� �ؽ�Ʈ�� ����
	UFUNCTION(BlueprintGetter)
		FString GetCurrentDialogueText_flow();

	UFUNCTION(BlueprintPure)
		bool GetIsKillQuest() {return _bIsKillQuest;}

	UFUNCTION(BlueprintCallable)
		void SetCurrentDialogueRow_flow(int row) {_currentFlowDialougueRow = row;}

	// ���̾�α� �ؽ�Ʈ�� ȭ�鿡 �׸� �Լ�
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
