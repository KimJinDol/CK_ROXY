// Fill out your copyright notice in the Description page of Project Settings.


#include "BossProjectile.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABossProjectile::ABossProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereCollision->SetCollisionProfileName("EnemyProjectile");
	SphereCollision->InitSphereRadius(60.f);
	SphereCollision->SetEnableGravity(false);
	SphereCollision->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f)); // 스케일은 임의로 키운 것. 나중에 고치자
	SphereCollision->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	RootComponent = SphereCollision;

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileComponent->MaxSpeed = 10000.f;
	ProjectileComponent->InitialSpeed = 1.f;
	ProjectileComponent->bShouldBounce = false;
	ProjectileComponent->ProjectileGravityScale = 0.f;
	
	PC_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	PC_Particle->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder <UParticleSystem> PS_PARTICLE(TEXT("/Game/Resources/Enemy/Particle/Fx_Boss_Charge/Fx_P_Boss_Charge.Fx_P_Boss_Charge"));
	if (PS_PARTICLE.Succeeded())
	{
		PS_Particle = PS_PARTICLE.Object;
		PC_Particle->SetTemplate(PS_Particle);
		PC_Particle->bAutoActivate = true;
	}

	static ConstructorHelpers::FObjectFinder <UParticleSystem> PS_HIT(TEXT("/Game/Resources/Enemy/Particle/Fx_Boss_Explosion+Hit/Fx_P_Boss_Hit.Fx_P_Boss_Hit"));
	if (PS_HIT.Succeeded())
	{
		PS_Hit = PS_HIT.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC_SHOOT(TEXT("/Game/Resources/Sound/Enemy/Elite/SC_Undead_Shoot_Projectile.SC_Undead_Shoot_Projectile"));
	if (SC_SHOOT.Succeeded())
	{
		SC_Shoot = SC_SHOOT.Object;
	}

	InitialLifeSpan = 3.f;
}

// Called when the game starts or when spawned
void ABossProjectile::BeginPlay()
{
	Super::BeginPlay();

}

void ABossProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereCollision->OnComponentHit.AddDynamic(this, &ABossProjectile::OnHit);
}

// Called every frame
void ABossProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_bIsAccelerate)
		ProjectileComponent->ComputeVelocity(ProjectileComponent->Velocity *= 1.11f, DeltaTime);
	
}

void ABossProjectile::SetDamageInstigatorInfo(AController* controller, AActor* actor)
{
	DamageInstigator = controller;
	DamageCauser = actor;
}

void ABossProjectile::SetActiveAfterDelay(float delay, float speed)
{
	FTimerHandle delayHandle;
	FTimerDelegate delayDelegate;
	SetCollisionEnable(false);
	SetVelocity(0.f);
	delayDelegate.BindLambda([this](float speed) {
		EndDelayActive.ExecuteIfBound();
		SetVelocity(speed);
		SetCollisionEnable(true);
	}, speed);
	GetWorld()->GetTimerManager().SetTimer(delayHandle, delayDelegate, delay, false);
}

void ABossProjectile::SetCollisionEnable(bool value)
{
	if (value)
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	else
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABossProjectile::SetCollisionRadius(float radius)
{
	SphereCollision->InitSphereRadius(radius);
}

void ABossProjectile::SetVelocity(float velocity)
{
	velocity = FMath::Clamp(velocity, 0.f, ProjectileComponent->MaxSpeed);
	ProjectileComponent->Velocity = GetActorForwardVector() * velocity;

	PlayProjectileSound();
}


void ABossProjectile::SetAccelerate(bool value)
{
	_bIsAccelerate = value;
}

void ABossProjectile::SetRotation(FRotator rotation)
{
	SetActorRotation(rotation);
}

void ABossProjectile::PlayProjectileSound()
{
	UGameplayStatics::PlaySoundAtLocation(this, SC_Shoot, GetActorLocation());
}

void ABossProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	CHECK(DamageInstigator);
	CHECK(DamageCauser);
	ARoxyCharacter* Roxy = Cast<ARoxyCharacter>(OtherActor);
	if (Roxy != nullptr ) {
		FDamageEvent  DamageEvent;
		Roxy->TakeDamage(_damage, DamageEvent, DamageInstigator, DamageCauser);
#if ENABLE_DRAW_DEBUG
		LOG_SCREEN("Boss Projectile On Hit Player");
#endif
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS_Hit, GetTransform(), true);

	Destroy();
}