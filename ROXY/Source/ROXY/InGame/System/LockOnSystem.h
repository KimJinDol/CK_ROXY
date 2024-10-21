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

	/*----------------���� �ý����� ���� ���� owner �� ���� �͵�------------------*/

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = OwnerInfo, Meta = (AllowPrivateAccess = true))
		class ARoxyCharacter* _ownerCharacter; // ���� �ý����� ������� owner Character
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = OwnerInfo, Meta = (AllowPrivateAccess = true))
		class APlayerController* _ownerController; // ���� �ý����� ������� owner Player controller
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = OwnerInfo, Meta = (AllowPrivateAccess = true))
		class UPN_SpringArmComponent* _ownerSpringArm; // ���� �ý����� ������� owner Character�� springArm Component
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = OwnerInfo, Meta = (AllowPrivateAccess = true))
		class UCameraComponent* _ownerCamera; // ���� �ý����� ������� owner Character�� Camera Component

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = LockOnSystem, Meta = (AllowPrivateAccess = true))
		FTimerHandle _targetingTimer; // ���ο� Ÿ���� ����� �ð������� Ȯ���ϱ� ���� auto targetinag�� Ÿ�̸�


	/*----------------���� �ý��۰� �����Ǵ� Enemy �鿡 ���� �͵�---------------------*/

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = EnemyInfo, Meta = (AllowPrivateAccess = true))
		TArray<AEnemyAI*> _enemysArray; // ������ �����ϴ� ��� ���� ������ ���� �迭
	UPROPERTY()
		class AEnemyAI* _newTarget; // ���ο� ���� Ÿ���� �� ���
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = EnemyInfo, Meta = (AllowPrivateAccess = true))
		class AEnemyAI* _currentTarget; // ���� ��� ���
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = EnemyInfo, Meta = (AllowPrivateAccess = true))
		int32 _targetIndex; // ���� Ÿ�� ����� �ε��� ��ȣ
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = EnemyInfo, Meta = (AllowPrivateAccess = true))
		int32 _enemysArrayMaxNum; // ���� ���� ����Ŭ������ ���� �ĺ����� �� ����.


	/*------------------���� �ý��۰� ���õ� Getter ������---------------------*/

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = LockOnSystem, Meta = (AllowPrivateAccess = true))
		bool _bOnLockOnMode = false; // ���� ��尡 Ȱ��ȭ �ƴ°�?
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = LockOnSystem, Meta = (AllowPrivateAccess = true))
		bool _bOnFightZone = false; // ���� ���� �ִ���


	/*-------------------���»��¿��� Ȱ��ȭ �� ��ɵ�--------------------------*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = LockOn_Ability, Meta = (AllowPrivateAccess = true))
		bool _bOnAutoTargeting = false; // ī�޶� ���� Ÿ������ Ȱ��ȭ �� ���ΰ�?
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = LockOn_Ability, Meta = (AllowPrivateAccess = true))
		bool _bOnLookAtTarget = false; // ���� ����� �÷��̾ �ٶ� ���ΰ�?

	/*----------------���� Ÿ����-------------------*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = AutoTargeting, Meta = (AllowPrivateAccess = true))
		float _targetingDuration;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = AutoTargeting, Meta = (AllowPrivateAccess = true))
		float _targetingSpeed;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		// ���� �÷��̾ ���ִ� fightZoneFinder�� _currentFightZone�����Ϳ� �Ҵ�
		void SetCurrentFightZone(AFightZoneFinder* fightZone);

	UFUNCTION()
		// ���� ���� �ִ� ������ ������ LockOnSystemŬ������ enemy �迭�� �������ش�
		// �� ���� ȣ��Ǿ� ��
		void SetEnemysInformation(TArray<AEnemyAI*> enemys);

	UFUNCTION()
		// ���� ���� ���� �����ǰų� ������ ���� ������ enemys �迭�� �߰�
		void AddEnemyInformation(AEnemyAI* enemy);

	UFUNCTION()
		// ���� ������ ������ ���� ������ enemys �迭���� ����
		void RemoveEnemyInformation(AEnemyAI* enemy);

	UFUNCTION()
		// ȭ�鿡 ������ �� ������ ����� �������� �Լ�
		TArray<AEnemyAI*> GetRenderedEnemys(UWorld* contextWorld);

	UFUNCTION()
		// ���� enemys �迭�� �ִ� ���� ����� ������� �����ϴ� �Լ�
		void ArrangeEnemysArray();

	UFUNCTION()
		// ������ �ԷµǾ��� �� ����
		AEnemyAI* InputLockOnSystem();


	UFUNCTION()
		void SetLookAtTargetMode(bool value = true) { _bOnLookAtTarget = value; }

	UFUNCTION()
		void SetAutoTargeting(float duration, float speed) { _targetingDuration = duration; _targetingSpeed = speed; }

	UFUNCTION()
	void TurnOffAutoTargeting();

	// ���� Ȱ��ȭ �Լ�
	void ActiveLockOnMode(); 

	// ���� ��Ȱ��ȭ �Լ�
	void UnActiveLockOnMode();

	// ���� ��尡 Ȱ��ȭ ���� ��ȯ
	bool IsLockOnModeOn() { return _bOnLockOnMode; }

	// ���� Ÿ������ Ȱ��ȭ ���� ��ȯ
	bool IsAutoTargetingOn() { return _bOnAutoTargeting; }

	// ��� ��� �ֽ� ���� ��ȯ
	bool IsLookAtTargetOn() { return _bOnLookAtTarget; }


	AEnemyAI* GetTarget() { return _currentTarget ? _currentTarget : NULL; }


	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
