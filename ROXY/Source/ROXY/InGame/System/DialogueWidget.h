// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Owner, meta = (AllowPrivateAccess = true))
	class ADialogSystem* OwnerDialogue;


public:	
	UFUNCTION()
	void BindOwner(ADialogSystem* dialogSystem);

	UFUNCTION(BlueprintImplementableEvent)
	void SetDialogueInfo_flow(const FString& name, const FString& dialogue, int32 lineNum, float lifeTime, bool bIsKillQuest);

	UFUNCTION(BlueprintImplementableEvent)
	void SetDialogueInfo_event(const FString& name, const FString& dialogue, int32 lineNum, float lifeTime);
};
