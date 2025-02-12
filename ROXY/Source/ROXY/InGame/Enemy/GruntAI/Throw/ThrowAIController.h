// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/GruntAIController.h"
#include "ThrowAIController.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API AThrowAIController : public AGruntAIController
{
	GENERATED_BODY()
	
public:
	AThrowAIController();

public:
	static const FName CanSkullShootKey;
};
