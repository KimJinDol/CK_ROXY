// Fill out your copyright notice in the Description page of Project Settings.


#include "PN_SpringArmComponent.h"
#include "InGame/Enemy/EnemyAI.h"
#include "InGame/Enemy/Boss/NunuBossAI.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/Component/PN_CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"


UPN_SpringArmComponent::UPN_SpringArmComponent()
{
	bTickInEditor = false;
	
}

void UPN_SpringArmComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!_bCanUseCameraWork) return;

	// Ÿ�پϷ��� ���� ����
	if (!FMath::IsNearlyEqual(TargetArmLength, _newTargetArmLength, SMALL_NUMBER))
	{
		TargetArmLength = FMath::FInterpTo(TargetArmLength, _newTargetArmLength, DeltaTime, _springArmSpeed);
	}

	// ���� ������ ����
	if (FVector::Distance(SocketOffset, _newSocketOffset) > SMALL_NUMBER)
	{
		SocketOffset = FMath::VInterpTo(SocketOffset, _newSocketOffset, DeltaTime, _socketOffsetSpeed);
	}

	// ī�޶� ��ġ ����
	if (LCamera != nullptr)
	{
		FVector CurrentCameraLocation = LCamera->GetRelativeTransform().GetLocation();

		if (FVector::Distance(CurrentCameraLocation, _newLocation) > SMALL_NUMBER)
		{
			FVector newLocation = FMath::VInterpTo(CurrentCameraLocation, _newLocation, DeltaTime, _LcameraSpeed);
			LCamera->SetRelativeLocation(newLocation);
		}
	}

	// ī�޶� ȸ�� ����
	if (RCamera != nullptr)
	{
		FRotator CurrentCameraRotation = RCamera->GetRelativeTransform().GetRotation().Rotator();

		if (CurrentCameraRotation.Equals(_newRotation))
		{
			FRotator newRotation = FMath::RInterpTo(CurrentCameraRotation, _newRotation, DeltaTime, _RcameraSpeed);
			RCamera->SetRelativeRotation(newRotation);
		}
	}


	ARoxyCharacter* Roxy = Cast<ARoxyCharacter>(GetOwner());

	if (Roxy)
	{
		// ���İ� ����
		TArray<FHitResult> hitResults;
		FVector ownerVec = GetOwner()->GetActorLocation();
		FVector cameraVec = Roxy->GetFollowCamera()->GetComponentLocation();

		float capsuleRadius = Roxy->GetCapsuleComponent()->GetScaledCapsuleRadius() * 0.6f;
		float capsuleHalfHeight = FVector::Distance(ownerVec, cameraVec) / 2;
		FQuat quat = UKismetMathLibrary::FindLookAtRotation(ownerVec, cameraVec).Quaternion();
		FCollisionQueryParams Params(NAME_None, false, GetOwner());
		FCollisionObjectQueryParams objectParams;
		objectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1); // enemy �ݸ��� ä��
		bool bResult = GetWorld()->SweepMultiByObjectType(
			hitResults,
			cameraVec,
			ownerVec,
			quat,
			objectParams,
			FCollisionShape::MakeCapsule(capsuleRadius, capsuleHalfHeight),
			Params
		);
// 		bool bResult = GetWorld()->LineTraceMultiByObjectType(
// 			hitResults,
// 			cameraVec,
// 			ownerVec,
// 			objectParams,
// 			Params
// 		);

		//TArray<AEnemyAI*> enemyTemp;
		//if (bResult) {
		//	for (int i = 0; i < hitResults.Num(); i++) {
		//		AEnemyAI* enemy = Cast<AEnemyAI>(hitResults[i].GetActor());
		//		if (enemy)
		//		{
		//			if (!enemy->IsA(ANunuBossAI::StaticClass())) {
		//
		//				// ���͸��� ���� ���ߴ� �� ����
		//				if (enemy->GetCurrentMatType() != ECurrentMatType::MT_DAMAGED) {
		//					enemyTemp.Push(enemy);
		//
		//					if (!_enemys.Contains(enemy)) {
		//						enemy->SetOpacityMat();
		//						_enemys.Push(enemy);
		//					}
		//				}
		//				else {
		//					_enemys.Remove(enemy);
		//				}
		//			}
		//		}
		//	}
		//	for (int i = 0; i < _enemys.Num(); i++) {
		//		if (!enemyTemp.Contains(_enemys[i]))
		//		{
		//			_enemys[i]->SetOriginMat();
		//			_enemys.RemoveAt(i);
		//		}
		//	}
		//}
		//else {
		//	for (int i = 0; i < _enemys.Num(); i++) {
		//		_enemys[i]->SetOriginMat();
		//		_enemys.RemoveAt(i);
		//	}
		//}
	}
}

void UPN_SpringArmComponent::SetEnableCameraWork(bool value)
{
	_bCanUseCameraWork = value;
}

void UPN_SpringArmComponent::SetTargetArmLength(float newTargetLength, float speed)
{
	_newTargetArmLength = newTargetLength;
	_springArmSpeed = speed;
}

void UPN_SpringArmComponent::SetSocketOffset(FVector newOffset, float speed)
{
	_newSocketOffset = newOffset;
	_socketOffsetSpeed = speed;
}

void UPN_SpringArmComponent::SetCameraLocation(UCameraComponent* newCamera, FVector newLocation, float speed)
{
	LCamera = newCamera;
	_newLocation = newLocation;
	_LcameraSpeed = speed;
}

void UPN_SpringArmComponent::SetCameraRotation(UCameraComponent* newCamera, FRotator newRotation, float speed)
{
	RCamera = newCamera;
	_newRotation = newRotation;
	_RcameraSpeed = speed;
}