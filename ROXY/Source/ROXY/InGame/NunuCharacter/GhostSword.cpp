// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostSword.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AGhostSword::AGhostSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));

	CapsuleComponent->SetCollisionProfileName("OverlapOnlyPawn");
	CapsuleComponent->SetCapsuleSize(150.f, 200.f);
	RootComponent = CapsuleComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	Mesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
// 	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GHOSTSWORD(TEXT("/Game/Resources/Roxy/Mesh/SM_Ghostsword.SM_Ghostsword"));
// 	if (SM_GHOSTSWORD.Succeeded())
// 	{
// 		Mesh->SetStaticMesh(SM_GHOSTSWORD.Object);
// 	}
}

// Called when the game starts or when spawned
void AGhostSword::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGhostSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGhostSword::DestroySword()
{
	Destroy();
}

