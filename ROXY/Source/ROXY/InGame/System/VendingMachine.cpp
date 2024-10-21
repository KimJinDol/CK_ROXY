// Fill out your copyright notice in the Description page of Project Settings.


#include "VendingMachine.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/Item/StatCola.h"
#include "InGame/System/FightZoneFinder.h"
#include "InGame/System/NunuInGameManager.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

AVendingMachine::AVendingMachine()
{
	PrimaryActorTick.bCanEverTick = true;

	/* 콜리전 */
	InteractionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollision"));
	InteractionCollision->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
	InteractionCollision->SetSphereRadius(300.f);
	InteractionCollision->SetupAttachment(RootComponent);
	InteractionCollision->SetRelativeLocation(FVector(0.f, 0.f, -140.f));

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionProfileName(TEXT("BlockAll"));
	BoxCollision->SetBoxExtent(FVector(50.0f, 50.0f, 200.0f));
	RootComponent = BoxCollision;

	/* 메쉬 */
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	Mesh->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOUNDARYZONE(TEXT("/Game/Resources/Background/prop/SM_MERGED_BP_VendingMachine_2.SM_MERGED_BP_VendingMachine_2"));
	if (SM_BOUNDARYZONE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_BOUNDARYZONE.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BROKENMACHIEN(TEXT("/Game/Resources/Background/prop/SM_MERGED_SM_MERGED_drinkMachineBroken_low_machineBody_broken_11.SM_MERGED_SM_MERGED_drinkMachineBroken_low_machineBody_broken_11"));
	if (SM_BROKENMACHIEN.Succeeded())
	{
		// 부서진 자판기 메쉬
		BrokenMesh = SM_BROKENMACHIEN.Object;
	}

	/* 파티클 */
	PC_ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComponent");
	PC_ParticleComponent->SetupAttachment(RootComponent);
	PC_ParticleComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	PC_ParticleComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	PC_ParticleComponent->SetVisibility(false);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_SPARK(TEXT("/Game/Resources/Generator/Particle/Eff_spark/P_spark.P_spark"));
	if (PS_SPARK.Succeeded())
	{
		PS_Spark = PS_SPARK.Object;
		PC_ParticleComponent->SetTemplate(PS_Spark);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_EXPLOSION(TEXT("/Game/Resources/Generator/Particle/Fx_Explosion/Fx_P_Explosion.Fx_P_Explosion"));
	if (PS_EXPLOSION.Succeeded())
	{
		PS_Explosion = PS_EXPLOSION.Object;
	}

	/* 위젯 */
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetVisibility(false);
	WidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_VENDINGMACHINE(TEXT("/Game/Blueprints/ScreenWidget/VendingMachine.VendingMachine_C"));
	if (UI_VENDINGMACHINE.Succeeded())
	{
		// 자판기 위치 표시 UI
		WidgetComponent->SetWidgetClass(UI_VENDINGMACHINE.Class);
		WidgetComponent->SetDrawSize(FVector2D(110.0f, 200.0f));
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_COLA(TEXT("/Game/Blueprints/ScreenWidget/ColaSelect.ColaSelect_C"));
	if (UI_COLA.Succeeded())
	{
		// 발차기 안내 UI(구 콜라선택)
		WidgetTemplate = UI_COLA.Class;
	}

	/* 사운드 */
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_ACTIVE(TEXT("/Game/Resources/Sound/Generator/Effect/SC_VM_Active.SC_VM_Active"));
	if (SC_ACTIVE.Succeeded())
	{
		SC_Active = SC_ACTIVE.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_BROKEN(TEXT("/Game/Resources/Sound/Generator/Effect/SC_VM_Broken.SC_VM_Broken"));
	if (SC_BROKEN.Succeeded())
	{
		SC_Broken = SC_BROKEN.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_STEAM(TEXT("/Game/Resources/Sound/Generator/Effect/SC_Generator_Break_Steam.SC_Generator_Break_Steam"));
	if (SC_STEAM.Succeeded())
	{
		SC_Steam = SC_STEAM.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_SPARK(TEXT("/Game/Resources/Sound/Generator/Effect/SC_VM_Spark.SC_VM_Spark"));
	if (SC_SPARK.Succeeded())
	{
		SC_Spark = SC_SPARK.Object;
	}

	static ConstructorHelpers::FClassFinder<AStatCola> STATCOLA(TEXT("/Game/Blueprints/InGame/System/BP_StatCola.BP_StatCola_C"));
	if (STATCOLA.Succeeded())
	{
		StatColaClass = STATCOLA.Class;
	}
}

void AVendingMachine::BeginPlay()
{
	Super::BeginPlay();

	BindNunuInGameManager(InGameManager);
	
	Roxy = InGameManager->GetPlayerCharacter();
}

void AVendingMachine::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetTemplate);
	//WidgetInstance = Cast<UUserWidget>(WidgetComponent->GetUserWidgetObject());

	InteractionCollision->OnComponentBeginOverlap.AddDynamic(this, &AVendingMachine::OnInteractionCollisionBeginOverlap);
	InteractionCollision->OnComponentEndOverlap.AddDynamic(this, &AVendingMachine::OnInteractionCollisionEndOverlap);
}

void AVendingMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (WidgetComponent->IsVisible())
	{
		RotateWidget();
	}
}

void AVendingMachine::OnInteractionCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OtherActor);

		if (roxy)
		{
			if (!_bIsBroken)
			{
				// 상호작용 시 발차기 안내(구 콜라 선택) 위젯 표시
				WidgetInstance->AddToViewport();

				// 상호작용 콜리전 안에 있어야만 자판기를 찰 수 있음
				_bCanBroken = true;

				roxy->SetIsCanInteractionVM(true);
			}
		}
	}
}

void AVendingMachine::OnInteractionCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OtherActor);

		if (roxy)
		{
			if (!_bIsBroken)
			{
				// 상호작용 범위를 벗어날 시
				WidgetInstance->RemoveFromViewport();

				_bCanBroken = false;

				roxy->SetIsCanInteractionVM(false);
			}
		}
	}
}

void AVendingMachine::ActiveInteractionCollision()
{
	if (_bIsBroken) return;

	InteractionCollision->SetCollisionProfileName(TEXT("RespondOnlyRoxy"));

	// 상호작용 콜리전이 활성화 되면 자판기 위치 표시 아이콘 뜨도록
	WidgetComponent->SetVisibility(true);

	// 활성화 안내 알림음
	PlayActiveSound();
}

void AVendingMachine::SetBrokenMesh()
{
	if (_bIsBroken || !_bCanBroken) return;

	_bIsBroken = true;
	_bCanBroken = false;

	LOG(Warning, "Vending Machine is Broken");

	Roxy->SetIsCanInteractionVM(false);

	// 터지는 이펙트 재생
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS_Explosion, GetTransform(), true);

	// 사운드 재생
	StopActiveSound();
	UGameplayStatics::PlaySoundAtLocation(this, SC_Broken, GetActorLocation()); // 부서지는 소리
	UGameplayStatics::PlaySoundAtLocation(this, SC_Steam, GetActorLocation()); // 스팀 소리
	UGameplayStatics::PlaySoundAtLocation(this, SC_Spark, GetActorLocation()); // 스파크 튀는 소리

	Mesh->SetStaticMesh(BrokenMesh);

	PC_ParticleComponent->SetVisibility(true);

	WidgetInstance->RemoveFromViewport();
	WidgetComponent->SetVisibility(false);

	if(NextFightZoneFinder)
	{
		// 자판기를 부수면 다음 파이트존 활성화
		NextFightZoneFinder->FightZoneActive();
	}
	// 부서진 메쉬로 교체해준 후 상호작용 콜리전 끄기
	InteractionCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 자판기 부수고 n초 후 스탯콜라 드랍
	GetWorldTimerManager().SetTimer(_dropColaTimer, this, &AVendingMachine::DropCola, 0.8f, false);
}

void AVendingMachine::DropCola()
{
	if (StatColaClass)
	{
		_muzzleLocation = GetActorLocation() + (GetActorRightVector() * 100.0f) + (GetActorUpVector() * 30.0f) + (GetActorForwardVector() * -30.0f);
		_muzzleRotation = GetActorRotation() + FRotator(120.0f, 0.0f, 0.0f);

		if (GetWorld())
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.Instigator = Instigator;

			StatCola = GetWorld()->SpawnActor<AStatCola>(StatColaClass, _muzzleLocation, _muzzleRotation, spawnParams);

			if (StatCola)
			{
				StatCola->SetOwnerVendingMachine(this);
				StatCola->SetColaType(_colaType);

				LOG(Warning, "Drop Stat Cola");

				FVector _dropDirection = GetActorRightVector() * 600.0f;
				StatCola->SetDropDirection(_dropDirection);

				OnDropCola.Broadcast();
			}
		}
	}
}

void AVendingMachine::RotateWidget()
{
	if (WidgetComponent != nullptr)
	{
		AActor* roxy = InGameManager->GetPlayerCharacter();

		if (roxy)
		{
			// 위젯 스페이스를 스크린모드로 해두면 거리에 따라 크기가 왔다갔다 하는게 거슬리고 자판기도 가려서
			// 스페이스를 월드모드로 해두되 ui 이미지가 똑바로 보이도록 월드회전값을 캐릭터를 바라보도록
			FVector _targetLoc = roxy->GetActorLocation();
			FVector _originLoc = GetActorLocation();
			FRotator _originRot = GetActorRotation();
			FRotator _tempRot = UKismetMathLibrary::FindLookAtRotation(_originLoc, _targetLoc);
			FRotator _newRot = FRotator(_originRot.Pitch, _tempRot.Yaw, _originRot.Roll);

			WidgetComponent->SetWorldRotation(_newRot);
		}
	}
}

void AVendingMachine::HitableTakeDamage()
{
	SetBrokenMesh();
}

