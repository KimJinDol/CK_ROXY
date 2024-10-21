// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "UObject/Interface.h"
#include "Hitable.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UHitable : public UInterface
{
	GENERATED_BODY()
	
};

class ROXY_API IHitable
{
	GENERATED_BODY()

public:
	UFUNCTION()
		virtual	void HitableTakeDamage() = 0;
};
