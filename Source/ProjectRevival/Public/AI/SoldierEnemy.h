// Project Revival. All Rights Reserved
// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "SoldierRifleWeapon.h"
#include "Interfaces/IChangingWorldActor.h"
#include "Interfaces/IGrenadeThrower.h"
#include "Player/BaseCharacter.h"
#include "Soldier/SoldierAIController.h"
#include "SoldierEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FThrowGrenade);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopEnteringCover);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopExitingCover);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopCoverSideMoving);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopFire);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSoldierWorldChange, bool, bCond);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FClearCoverTimer);

UCLASS()
class PROJECTREVIVAL_API ASoldierEnemy : public AAICharacter, public IICoverable, public IIGrenadeThrower, public IIChangingWorldActor
{
	GENERATED_BODY()
public:
	
	virtual void OnDeath() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* NewController) override;
	UFUNCTION(BlueprintCallable)
	bool GetBIsAppearing() { return bIsAppearing; }
	void SetBIsAppearing(bool bCond);
	EBotState GetBotState() const { return BotState; }
	UFUNCTION(BlueprintCallable)
	void SetBotState(EBotState const val);
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FThrowGrenade ThrowGrenadeDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStopEnteringCover StopEnteringCoverDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStopExitingCover StopExitingCoverDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStopCoverSideMoving StopCoverSideMovingDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStopFire StopFireDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FSoldierWorldChange SoldierWorldChangeDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FClearCoverTimer ClearCoverTimerDelegate;

	virtual ECoverType CheckCover() override;
	virtual bool UsesOwnGrenades() override;
	virtual bool SwitchGrenade() override;
	TArray<FAmmoData> GetPlayerWeapons() const;
	
	UFUNCTION(BlueprintCallable)
	virtual TSubclassOf<ABaseGrenade> GetCurrentGrenade() override;
	
	UFUNCTION(BlueprintCallable)
	FCoverData& GetCoverData();

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerCoordinates(const FPlayerPositionData& PlayerPos);

	UFUNCTION()
	void UpdateAimRotator();

	UFUNCTION()
	void StartCoverSoldier(const FVector& CoverPos, AActor* CoverRef);

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
	
	UFUNCTION()
	virtual void StartFiring() override;

	UFUNCTION(BlueprintCallable)
	virtual void StopFiring();

	UFUNCTION(BlueprintCallable)
	virtual void StopFiringImmediately();
	
	UFUNCTION(BlueprintCallable)
	bool GetIsInBattle() { return BotState == EBotState::Battle?true:false; }
	

	virtual void ChangeVisibleWorld(EChangeAllMapEditorVisibility VisibleInEditorWorld) override;
	virtual void Changing() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void ShowChangeWorldObjectByAbility() override;
	virtual void HideChangeWorldObjectByAbility() override;

	
	UFUNCTION()
	virtual void OnMeshComponentCollision(UPrimitiveComponent* OverlappedComponent, 
					  AActor* OtherActor, 
					  UPrimitiveComponent* OtherComp, 
					  int32 OtherBodyIndex, 
					  bool bFromSweep, 
					  const FHitResult &SweepResult) override;

	UFUNCTION()
	virtual void OnMeshComponentEndCollision(UPrimitiveComponent* OverlappedComponent, 
					  AActor* OtherActor, 
					  UPrimitiveComponent* OtherComp, 
					  int32 OtherBodyIndex) override;

	UPROPERTY(BlueprintReadOnly)
	bool bIsInCoverBP;

	UPROPERTY(BlueprintReadOnly)
	bool bIsFiringBP;

	UPROPERTY(BlueprintReadOnly)
	bool bCanFireBP;

	UPROPERTY(BlueprintReadOnly)
	FCoverData CoverData;

	UPROPERTY(BlueprintReadOnly)
	FPlayerPositionData PlayerCoordinates;

	UPROPERTY(BlueprintReadOnly)
	FRotator AimRotator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float IdleMaxSpeed = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BattleMaxSpeed = 400.f;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TEnumAsByte<EChangeWorld> World = OrdinaryWorld;
	TEnumAsByte<EChangeWorld> CurrentWorld = OrdinaryWorld;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComp;

protected:
	
	virtual void BeginPlay() override;
	virtual void OnHealthChanged(float CurrentHealth, float HealthDelta) override;
	
	virtual TEnumAsByte<ECoverSide> CheckSideByNormal(FVector Forward, FVector Normal);
	virtual TEnumAsByte<ECoverPart> GetCoverPart(int8 PartPos);
	virtual void CleanCoverData();
	virtual int8 GetCoverIndex();

	UFUNCTION()
	virtual void TimeLineFinished() override;
	UFUNCTION()
	virtual void TimeLineFloatReturn(float Value) override;
	void SetCollisionResponseToVisible();
	void SetCollisionResponseToInvisible();

	float SideMoveAmount;

	//List of grenades which can be used. If empty than default grenade from GrenadeAbility is used
	UPROPERTY(EditDefaultsOnly, Category="Grenade Ability")
	TArray<TSubclassOf<ABaseGrenade>> Grenades;

	UPROPERTY(EditDefaultsOnly, Category="Grenade Ability")
	TSubclassOf<ABaseGrenade> CurrentGrenade = nullptr;

	//In the future it should be replaced with an AIWeapon class 
	UPROPERTY()
	ASoldierRifleWeapon* RowRifleRef=nullptr;
	
	FDelegateHandle UpdateRowRifleDelegateHandle;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TEnumAsByte<EChangeEditorVisibility> VisibleWorld = BothWorlds;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TEnumAsByte<EChangeAllMapEditorVisibility> AllObjectVisibleWorld=OwnValuesWorld;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float TransparencyLevel=5.0f;
	
	EBotState BotState = EBotState::Idle;

	FTimeline TimeLine;
	FOnTimelineEvent OnTimeLineFinished;
	FOnTimelineFloat InterpFunction;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UCurveFloat* VisualCurve;
	float MinCurveValue;
	float MaxCurveValue;

	FCollisionResponseContainer CollisionResponseContainer;

	TArray<UMaterialInstanceDynamic*> MeshesMaterials;
	
	bool bIsAppearing;

	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	virtual void UpdateHealthWidgetVisibility() override;
	virtual void UpdateHStateBlackboardKey(uint8 EnumKey) override;
	void SetBotVisible();
	void SetBotInvisible();
	void CoverCrouch();

	UFUNCTION()
	void ThrowGrenadeCaller();

	// void MoveForward(float Amount);
	// void MoveRight(float Amount);
	// void StartRun();
	// void StopRun();
	// virtual bool IsRunning() const override;
	// bool bWantsToRun = false;
	// bool IsMovingForward = false;
};
