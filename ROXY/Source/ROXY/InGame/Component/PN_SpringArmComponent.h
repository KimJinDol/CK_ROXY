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
	// 카메라 워크 사용 가능 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraWork, meta = (AllowPrivateAccess = true))
		 bool _bCanUseCameraWork = true;
	// 스프링암 길이 조절 변수
	UPROPERTY()
		float _springArmSpeed;
	UPROPERTY()
		float _newTargetArmLength;

	// 소켓 오프셋 조절 변수
	UPROPERTY()
		float _socketOffsetSpeed;
	UPROPERTY()
		FVector _newSocketOffset;


	// 카메라 지역 위치 조절 변수
	UPROPERTY()
		class UCameraComponent* LCamera;
	UPROPERTY()
		FVector _newLocation;
	UPROPERTY()
		float _LcameraSpeed;

	// 카메라 지역 회전 조절 변수
	UPROPERTY()
		class UCameraComponent* RCamera;
	UPROPERTY()
		FRotator _newRotation;
	UPROPERTY()
		float _RcameraSpeed;

	// 적 
	TArray<AEnemyAI*> _enemys;
};
