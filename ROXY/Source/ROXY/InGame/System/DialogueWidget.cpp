// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget.h"

void UDialogueWidget::BindOwner(ADialogSystem* dialogSystem)
{
	CHECK(dialogSystem != nullptr);

	OwnerDialogue = dialogSystem;
}