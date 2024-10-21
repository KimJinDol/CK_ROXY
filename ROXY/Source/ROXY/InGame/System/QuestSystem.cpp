// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystem.h"

// Sets default values
AQuestSystem::AQuestSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AQuestSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQuestSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

