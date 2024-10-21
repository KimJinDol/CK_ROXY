// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowAIProjectile.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAI.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

AThrowAIProjectile::AThrowAIProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereCollision->InitSphereRadius(20.0f);
	SphereCollision->SetEnableGravity(false);
	SphereCollision->SetCollisionProfileName("EnemyProjectile");
	RootComponent = SphereCollision;

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileComponent->SetUpdatedComponent(SphereCollision);
	ProjectileComponent->InitialSpeed = 1.0f;
	ProjectileComponent->MaxSpeed = 2500.0f;
	ProjectileComponent->bShouldBounce = false;
	ProjectileComponent->ProjectileGravityScale = 0.0f;

	PC_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	PC_Particle->SetupAttachment(RootComponent);
	PC_Particle->SetRelativeLocation(FVector(-160.0f, 0.0f, 0.0f));
	PC_Particle->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder <UParticleSystem> PS_BULLET(TEXT("/Game/Resources/Enemy/Particle/fx_attack_L/Fx_P_attack_L.Fx_P_attack_L"));
	if (PS_BULLET.Succeeded())
	{
		PS_Bullet = PS_BULLET.Object;
	}

	static ConstructorHelpers::FObjectFinder <UParticleSystem> PS_SPEC_BULLET(TEXT("/Game/Resources/Enemy/Particle/Fx_Throw_Skull/Fx_P_Throw_Skull.Fx_P_Throw_Skull"));
	if (PS_SPEC_BULLET.Succeeded())
	{
		PS_SpecialBullet = PS_SPEC_BULLET.Object;
	}

	PC_Particle->bAutoActivate = true;

	//SphereMaterial = CreateDefaultSubobject<UMaterial>(TEXT("Material"));
	//static ConstructorHelpers::FObjectFinder<UMaterial> MAT(TEXT("/Game/Resources/Enemy/Grunt/DarkJoy/Materials/Joy_Mat_Red.Joy_Mat_Red"));
	//if (MAT.Succeeded())
	//{
	//	SphereMaterial = MAT.Object;
	//}

	InitialLifeSpan = 3.0f;
}

void AThrowAIProjectile::SetShootDirection(const FVector& direction, float speed)
{
	ProjectileComponent->Velocity = direction * ProjectileComponent->InitialSpeed * speed;
}

void AThrowAIProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		AThrowAI* throwAI = Cast<AThrowAI>(GetOwner());

		ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OtherActor);
		//AGruntAI* grunt = Cast<AGruntAI>(OtherActor);

		if (roxy)
		{
			FDamageEvent DamageEvent;
			
			if (throwAI->GetIsBasicShoot()) /* 기본 샷 */
			{
				roxy->SetStiffen(throwAI->GetProjectileStiffenTime());
				roxy->TakeDamage(throwAI->GetDamage(), DamageEvent, throwAI->GetController(), this);
			}
			else if (throwAI->GetIsSkullShoot()) /* 특수 패턴 */
			{
				roxy->SetStiffen(throwAI->GetProjectileStiffenTime());
				roxy->TakeDamage(throwAI->GetSpecialAttackDamage(), DamageEvent, throwAI->GetController(), this);

				// 특수 패턴인 Skull Shoot 이 록시를 때릴 경우 랜덤으로 록시 스킬 잠금
				_roxySkillNum = FMath::RandRange(1, 3);
				if (!roxy->GetIsSkillLocked())
				{
					roxy->SkillLockApply(_roxySkillNum);
					LOG(Warning, "Roxy Skill Lock : %d", _roxySkillNum);
				}
			}
			//else if (throwAI->GetIsEliteSpecialAttack()) /* 엘리트 패턴 메테오 */
			//{
			//	roxy->SetFaint(0.5f);
			//	roxy->SetAddicted();
			//
			//	FDamageEvent damageEvent;
			//	roxy->TakeDamage(30.0f, damageEvent, throwAI->GetController(), this);
			//}
		}

		Destroy();
	}
}

void AThrowAIProjectile::SetBulletEffect(EBulletType bulletType)
{
	CHECK(PC_Particle);

	if (bulletType == EBulletType::B_ORIGIN)
	{
		// 일반 투사체 이펙트
		PC_Particle->SetTemplate(PS_Bullet);
	}
	else if (bulletType == EBulletType::B_SKULL)
	{
		// 특수패턴 투사체 이펙트
		PC_Particle->SetTemplate(PS_SpecialBullet);
	}
}

void AThrowAIProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AThrowAIProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereCollision->OnComponentHit.AddDynamic(this, &AThrowAIProjectile::OnHit);
}

void AThrowAIProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

