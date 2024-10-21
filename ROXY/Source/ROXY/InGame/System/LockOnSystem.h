// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/EnemyAI.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/Component/PN_SpringArmComponent.h"
#include "Components/ActorComponent.h"
#include "LockOnSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROXY_API ULockOnSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ULockOnSystem();


private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = FightZoneFinder, Meta = (AllowPrivateAccess = true))
		class AFightZoneFinder* _currentFightZone;

	/*----------------락온 시스템을 소유 중인 owner 에 관한 것들------------------*/

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = OwnerInfo, Meta = (AllowPrivateAccess = true))
		class ARoxyCharacter* _ownerCharacter; // 락온 시스템을 사용중인 owner Character
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = OwnerInfo, Meta = (AllowPrivateAccess = true))
		class APlayerController* _ownerController; // 락온 시스템을 사용중인 owner Player controller
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = OwnerInfo, Meta = (AllowPrivateAccess = true))
		class UPN_SpringArmComponent* _ownerSpringArm; // 락온 시스템을 사용중인 owner Character의 springArm Component
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = OwnerInfo, Meta = (AllowPrivateAccess = true))
		class UCameraComponent* _ownerCamera; // 락온 시스템을 사용중인 owner Character의 Camera Component

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = LockOnSystem, Meta = (AllowPrivateAccess = true))
		FTimerHandle _targetingTimer; // 새로운 타겟팅 대상을 시각적으로 확인하기 위한 auto targetinag의 타이머


	/*----------------락온 시스템과 연관되는 Enemy 들에 관한 것들---------------------*/

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = EnemyInfo, Meta = (AllowPrivateAccess = true))
		TArray<AEnemyAI*> _enemysArray; // 공간에 존재하는 모든 적들 정보를 담을 배열
	UPROPERTY()
		class AEnemyAI* _newTarget; // 새로운 락온 타겟이 될 대상
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = EnemyInfo, Meta = (AllowPrivateAccess = true))
		class AEnemyAI* _currentTarget; // 현재 룩온 대상
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = EnemyInfo, Meta = (AllowPrivateAccess = true))
		int32 _targetIndex; // 락온 타겟 대상의 인덱스 번호
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = EnemyInfo, Meta = (AllowPrivateAccess = true))
		int32 _enemysArrayMaxNum; // 현재 락온 싸이클에서의 락온 후보들의 총 숫자.


	/*------------------락온 시스템과 관련된 Getter 정보들---------------------*/

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = LockOnSystem, Meta = (AllowPrivateAccess = true))
		bool _bOnLockOnMode = false; // 락온 모드가 활성화 됐는가?
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = LockOnSystem, Meta = (AllowPrivateAccess = true))
		bool _bOnFightZone = false; // 전투 존에 있는지


	/*-------------------락온상태에서 활성화 할 기능들--------------------------*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = LockOn_Ability, Meta = (AllowPrivateAccess = true))
		bool _bOnAutoTargeting = false; // 카메라 오토 타겟팅을 활성화 할 것인가?
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = LockOn_Ability, Meta = (AllowPrivateAccess = true))
		bool _bOnLookAtTarget = false; // 락온 대상을 플레이어가 바라볼 것인가?

	/*----------------오토 타겟팅-------------------*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = AutoTargeting, Meta = (AllowPrivateAccess = true))
		float _targetingDuration;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = AutoTargeting, Meta = (AllowPrivateAccess = true))
		float _targetingSpeed;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		// 현재 플레이어가 들어가있는 fightZoneFinder를 _currentFightZone포인터에 할당
		void SetCurrentFightZone(AFightZoneFinder* fightZone);

	UFUNCTION()
		// 전투 존에 있는 적들의 정보를 LockOnSystem클래스의 enemy 배열에 복사해준다
		// 한 번만 호출되야 함
		void SetEnemysInformation(TArray<AEnemyAI*> enemys);

	UFUNCTION()
		// 전투 존에 새로 스폰되거나 입장한 적의 정보를 enemys 배열에 추가
		void AddEnemyInformation(AEnemyAI* enemy);

	UFUNCTION()
		// 전투 존에서 삭제된 적의 정보를 enemys 배열에서 삭제
		void RemoveEnemyInformation(AEnemyAI* enemy);

	UFUNCTION()
		// 화면에 렌더링 된 적들의 목록을 가져오는 함수
		TArray<AEnemyAI*> GetRenderedEnemys(UWorld* contextWorld);

	UFUNCTION()
		// 현재 enemys 배열에 있는 적을 가까운 순서대로 정렬하는 함수
		void ArrangeEnemysArray();

	UFUNCTION()
		// 락온이 입력되었을 때 실행
		AEnemyAI* InputLockOnSystem();


	UFUNCTION()
		void SetLookAtTargetMode(bool value = true) { _bOnLookAtTarget = value; }

	UFUNCTION()
		void SetAutoTargeting(float duration, float speed) { _targetingDuration = duration; _targetingSpeed = speed; }

	UFUNCTION()
	void TurnOffAutoTargeting();

	// 락온 활성화 함수
	void ActiveLockOnMode(); 

	// 락온 비활성화 함수
	void UnActiveLockOnMode();

	// 락온 모드가 활성화 여부 반환
	bool IsLockOnModeOn() { return _bOnLockOnMode; }

	// 오토 타겟팅이 활성화 여부 반환
	bool IsAutoTargetingOn() { return _bOnAutoTargeting; }

	// 룩온 대상 주시 여부 반환
	bool IsLookAtTargetOn() { return _bOnLookAtTarget; }


	AEnemyAI* GetTarget() { return _currentTarget ? _currentTarget : NULL; }


	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
