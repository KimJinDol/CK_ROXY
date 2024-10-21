// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Components/ActorComponent.h"
#include "DialogueStruct.generated.h"


USTRUCT(BlueprintType)
struct FDialogueColumn : public FTableRowBase
{
	GENERATED_BODY()

	// 대화 상황 고유 코드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	int dialog_code;
	// 대화 회차
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	int32 text_num;
	// 텍스트 유지 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	float text_duration;
	// 화자 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	FString char_name;
	// 대화 내용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	FString dialog_text;
	
	// 줄 개수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	int32 int_lineNum;
	// 사운드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	FString sound;
	// UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	FString ui;
	// 킬 퀘스트인지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dialogue)
	bool killQuest;
};


UCLASS()
class ROXY_API UDialogueStruct : public UActorComponent
{
	GENERATED_BODY()
		
};
