// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAI.h"
//#include "InGame/Enemy/GruntAI/Throw/EliteThrow/EliteThrowAISkullBomb.h"
#include "EliteThrowAI.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API AEliteThrowAI : public AThrowAI
{
	GENERATED_BODY()
	
public:
	AEliteThrowAI();

	FThrowAI_Delegate OnMeteorTaskEnd;

private:
	UPROPERTY()
		class UEliteThrowAnimInstance* EliteThrowAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Meteor, Meta = (AllowPrivateAccess = true))
		TSubclassOf<class AEliteThrowAISkullBomb> SkullBombClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Meteor, Meta = (AllowPrivateAccess = true))
		AEliteThrowAISkullBomb* EliteThrowSkullBomb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_Jangpan;

private:
	// ¿¤¸®Æ® Æ¯¼ö ÆÐÅÏ Meteor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Meteor, Meta = (AllowPrivateAccess = true))
		bool _bIsMeteor = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Meteor, Meta = (AllowPrivateAccess = true))
		bool _bCanMeteor = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Meteor, Meta = (AllowPrivateAccess = true))
		float _meteorSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Meteor, Meta = (AllowPrivateAccess = true))
		float _meteorStiffenTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Meteor, Meta = (AllowPrivateAccess = true))
		int32 _maxBobmNum = 3; // ÇÑ ¹øÀÇ ÆÐÅÏ¿¡¼­ ¶³±¼ ¼ö ÀÖ´Â ÆøÅºÀÇ ÃÑ °¹¼ö
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Meteor, Meta = (AllowPrivateAccess = true))
		int32 _curBombNum = 0; // ¶³±º ÆøÅºÀÇ °¹¼ö
	UPROPERTY()
		FTimerHandle _meteorTimer;

public:
	UFUNCTION()
		void StartEliteShoot();
	UFUNCTION()
		void EliteShoot();
	UFUNCTION()
		void EndEliteShoot();

	UFUNCTION()
		void StartMeteor();
	UFUNCTION()
		void Meteor();
	UFUNCTION()
		void DropBomb();
	UFUNCTION()
		void EndMeteor();

protected:
	virtual void Die() override;
	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	UFUNCTION()
		float GetBombStiffenTime() { return _meteorStiffenTime; }

	UFUNCTION()
		int32 GetMaxBombNum() { return _maxBobmNum; }
	UFUNCTION()
		int32 GetCurrentBombNum() { return _curBombNum; }



};
