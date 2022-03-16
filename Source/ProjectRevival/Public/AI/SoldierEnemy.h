// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "SoldierRifleWeapon.h"
#include "Interfaces/IGrenadeThrower.h"
#include "Player/BaseCharacter.h"
#include "Soldier/SoldierAIController.h"
#include "SoldierEnemy.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartEnteringCoverForAnim);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartExitingCoverForAnim);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartCoverSideMovingForAnim);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartCoverToFireForAnim);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartCoverFromFireForAnim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FThrowGrenade);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopEnteringCover);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopExitingCover);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopCoverSideMoving);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartFire);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopFire);

UCLASS()
class PROJECTREVIVAL_API ASoldierEnemy : public AAICharacter, public IICoverable, public IIGrenadeThrower
{
	GENERATED_BODY()
public:
	
	virtual void OnDeath() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* NewController) override;
	
	// UPROPERTY(BlueprintAssignable, BlueprintCallable)
	// FStartEnteringCoverForAnim StartEnteringCoverForAnimDelegate;
	// UPROPERTY(BlueprintAssignable, BlueprintCallable)
	// FStartExitingCoverForAnim StartExitingCoverForAnimDelegate;
	// UPROPERTY(BlueprintAssignable, BlueprintCallable)
	// FStartCoverSideMovingForAnim StartCoverSideMovingForAnimDelegate;
	// UPROPERTY(BlueprintAssignable, BlueprintCallable)
	// FStartCoverToFireForAnim StartCoverToFireForAnimDelegate;
	// UPROPERTY(BlueprintAssignable, BlueprintCallable)
	// FStartCoverToFireForAnim StartCoverFromFireForAnimDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FThrowGrenade ThrowGrenadeDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStopEnteringCover StopEnteringCoverDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStopExitingCover StopExitingCoverDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStopCoverSideMoving StopCoverSideMovingDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStartFire StartFireDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStopFire StopFireDelegate;

	virtual ECoverType CheckCover() override;
	virtual bool UsesOwnGrenades() override;
	virtual bool SwitchGrenade() override; 
	virtual TSubclassOf<ABaseGrenade> GetCurrentGrenade() override;
	
	UFUNCTION(BlueprintCallable)
	FCoverData& GetCoverData();

	UFUNCTION()
	void StartCoverSoldier(const FVector& CoverPos);

	UFUNCTION(BlueprintCallable)
	void StartCoverSoldierFinish();

	UFUNCTION()
	void StopCoverSoldier();

	UFUNCTION(BlueprintCallable)
	void StopCoverSoldierFinish();

	UFUNCTION()
	void ChangeCoverSide(float Amount);

	UFUNCTION(BlueprintCallable)
	void ChangeCoverSideFinish();

	UFUNCTION(BlueprintCallable)
	void ChangeCoverTypeFinish();

	UFUNCTION()
	void StartCoverToFire();

	UFUNCTION(BlueprintCallable)
	void StartCoverToFireFinish();
	
	UFUNCTION()
	void StartCoverFromFire();

	UFUNCTION(BlueprintCallable)
	void StartCoverFromFireFinish();

	UFUNCTION(BlueprintCallable)
	void ThrowGrenade();

	UPROPERTY(BlueprintReadOnly)
	bool bIsInCoverBP;

	UPROPERTY(BlueprintReadOnly)
	bool bIsFiringBP;

	UPROPERTY(BlueprintReadOnly)
	FCoverData CoverData;
	
	UFUNCTION()
	virtual void StartFiring(const FVector& PlayerPos) override;

	UFUNCTION(BlueprintCallable)
	virtual void StopFiring();
	
protected:
	
	virtual void BeginPlay() override;
	virtual void OnHealthChanged(float CurrentHealth, float HealthDelta) override;

	virtual TEnumAsByte<ECoverSide> CheckSideByNormal(FVector Forward, FVector Normal);
	virtual TEnumAsByte<ECoverPart> GetCoverPart(int8 PartPos);
	virtual void CleanCoverData();
	virtual int8 GetCoverIndex();

	float SideMoveAmount;

	//List of grenades which can be used. If empty than default grenade from GrenadeAbility is used
	UPROPERTY(EditDefaultsOnly, Category="Grenade Ability")
	TArray<TSubclassOf<ABaseGrenade>> Grenades;

	UPROPERTY(EditDefaultsOnly, Category="Grenade Ability")
	TSubclassOf<ABaseGrenade> CurrentGrenade = nullptr;

	UPROPERTY()
	ASoldierRifleWeapon* RifleRef=nullptr;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	virtual void UpdateHealthWidgetVisibility() override;
	virtual void UpdateHStateBlackboardKey(uint8 EnumKey) override;
	void CoverCrouch();
	FVector PlayerPosition; //Used when we need to call StartFireBP(It is needed when we start firing while covering)

	UFUNCTION()
	void ThrowGrenadeCaller();

	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void StartRun();
	void StopRun();
	virtual bool IsRunning() const override;
	bool bWantsToRun = false;
	bool IsMovingForward = false;
};
