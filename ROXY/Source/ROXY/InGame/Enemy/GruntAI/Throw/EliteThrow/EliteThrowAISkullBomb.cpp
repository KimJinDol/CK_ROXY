// Fill out your copyright notice in the Description page of Project Settings.

#include "EliteThrowAISkullBomb.h"
#include "InGame/Enemy/GruntAI/Throw/EliteThrow/EliteThrowAI.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "GameFramework/ProjectileMovementComponent.h"

AEliteThrowAISkullBomb::AEliteThrowAISkullBomb()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComponent = CreateAbstractDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(60.0f);
	SphereComponent->SetEnableGravity(false);
	SphereComponent->SetCollisionProfileName("EnemyBomb");
	RootComponent = SphereComponent;

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileComponent->SetUpdatedComponent(SphereComponent);
	ProjectileComponent->InitialSpeed = 1.0f;
	ProjectileComponent->MaxSpeed = 6000.0f;
	ProjectileComponent->bShouldBounce = false;
	ProjectileComponent->ProjectileGravityScale = 0.8f;

	// ÆÄÆ¼Å¬
	PC_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	PC_Particle->SetupAttachment(RootComponent);
	PC_Particle->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	PC_Particle->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder <UParticleSystem> PS_BOMB(TEXT("/Game/Resources/Enemy/Particle/Fx_Throw_Skull/Fx_P_Throw_Skull.Fx_P_Throw_Skull"));
	if (PS_BOMB.Succeeded())
	{
		// Åõ»çÃ¼(ÆøÅº) ÀÌÆåÆ®
		PS_SkullBomb = PS_BOMB.Object;
		PC_Particle->SetTemplate(PS_SkullBomb);
		PC_Particle->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
		PC_Particle->bAutoActivate = true;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_EXPLOSION(TEXT("/Game/Resources/Enemy/Particle/Fx_Enemy_Poison/bomb/Fx_Enemy_bomb.Fx_Enemy_Bomb"));
	if (PS_EXPLOSION.Succeeded())
	{
		// ÆøÅº ÅÍÁú ½Ã ÀÌÆåÆ®
		PS_BombExplosion = PS_EXPLOSION.Object;
	}

	// »ç¿îµå
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_BOMB(TEXT("/Game/Resources/Sound/Enemy/Elite/SC_EliteThrow_Bomb.SC_EliteThrow_Bomb"));
	if (SC_BOMB.Succeeded())
	{
		SC_Bomb = SC_BOMB.Object;
	}

	InitialLifeSpan = 3.0f;
}

void AEliteThrowAISkullBomb::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEliteThrowAISkullBomb::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComponent->OnComponentHit.AddDynamic(this, &AEliteThrowAISkullBomb::OnHit);
}

void AEliteThrowAISkullBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEliteThrowAISkullBomb::SetDropDirection(const FVector& direction, float speed)
{
	ProjectileComponent->Velocity = direction * ProjectileComponent->InitialSpeed * speed;
}

void AEliteThrowAISkullBomb::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		AEliteThrowAI* eliteThrowAI = Cast<AEliteThrowAI>(GetOwner());

		ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OtherActor);
		AGruntAI* grunt = Cast<AGruntAI>(OtherActor);

		// ÆøÅº ÅÍÁö´Â »ç¿îµå Àç»ý
		UGameplayStatics::PlaySoundAtLocation(this, SC_Bomb, GetActorLocation());

		if (roxy)
		{
			//LOG(Warning, "Skull Bomb Hit Roxy");

			roxy->SetStiffen(eliteThrowAI->GetBombStiffenTime());
			roxy->SetAddicted();

			FDamageEvent damageEvent;
			roxy->TakeDamage(_bombDamageToRoxy, damageEvent, eliteThrowAI->GetController(), this);

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS_BombExplosion, GetTransform(), true);

			DestroyBomb();
		}
		else if (grunt && eliteThrowAI != OtherActor)
		{
			//LOG(Warning, "Skull Bomb Hit Other Grunt");

			FDamageEvent damageEvent;
			grunt->TakeDamage(_bombDamageToEnemy, damageEvent, eliteThrowAI->GetController(), this);

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS_BombExplosion, GetTransform(), true);

			DestroyBomb();
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS_BombExplosion, GetTransform(), true);

			DestroyBomb();
		}
	}	
}

void AEliteThrowAISkullBomb::DestroyBomb()
{
	Destroy();
}
