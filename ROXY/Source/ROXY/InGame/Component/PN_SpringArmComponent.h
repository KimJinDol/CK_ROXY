// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/EnemyAI.h"
#include "GameFramework/SpringArmComponent.h"
#include "PN_SpringArmComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UPN_SpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()


public:
	UPN_SpringArmComponent();

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	UFUNCTION(BlueprintCallable)
	void SetEnableCameraWork(bool value);
	UFUNCTION()
	void SetTargetArmLength(float newTargetLength, float speed);
	UFUNCTION()
	void SetSocketOffset(FVector newOffset, float speed);
	UFUNCTION()
	void SetCameraLocation(UCameraComponent* newCamera, FVector newLocation, float speed);
	UFUNCTION()
	void SetCameraRotation(UCameraComponent* newCamera, FRotator newRotation, float speed);


private:
	// ī�޶� ��ũ ��� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraWork, meta = (AllowPrivateAccess = true))
		 bool _bCanUseCameraWork = true;
	// �������� ���� ���� ����
	UPROPERTY()
		float _springArmSpeed;
	UPROPERTY()
		float _newTargetArmLength;

	// ���� ������ ���� ����
	UPROPERTY()
		float _socketOffsetSpeed;
	UPROPERTY()
		FVector _newSocketOffset;


	// ī�޶� ���� ��ġ ���� ����
	UPROPERTY()
		class UCameraComponent* LCamera;
	UPROPERTY()
		FVector _newLocation;
	UPROPERTY()
		float _LcameraSpeed;

	// ī�޶� ���� ȸ�� ���� ����
	UPROPERTY()
		class UCameraComponent* RCamera;
	UPROPERTY()
		FRotator _newRotation;
	UPROPERTY()
		float _RcameraSpeed;

	// �� 
	TArray<AEnemyAI*> _enemys;
};
