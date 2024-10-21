// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratorWidget.h"

void UGeneratorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}


void UGeneratorWidget::BindOwner(AGenerator* generator)
{
	CHECK(generator);

	OwnerGenerator = generator;
}
