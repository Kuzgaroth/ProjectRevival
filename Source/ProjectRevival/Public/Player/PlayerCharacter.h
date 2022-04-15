// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterMovementComponent.h"
#include "SoldierEnemy.h"
#include "Player/BaseCharacter.h"
#include "Components/TimelineComponent.h"
#include "GameFeature/StaticObjectToNothing.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVisorPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVisorReleased);

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;
class UCurveFloat;

UCLASS()
class PROJECTREVIVAL_API APlayerCharacter : public ABaseCharacter, public IICoverable
{
	GENERATED_BODY()
public:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);
	UFUNCTION()
	void TimelineProgress(float Value);

	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void TimelineFieldOfView(float Value);

	UFUNCTION()
	void TimelineLeftSideView(float Value);

	UFUNCTION()
	void TimelineCover(float Value);

	UFUNCTION()
	void TimelineCoverFieldOfView(float Value);

	UFUNCTION()
	void TimelineCoverYShift(float Value);

	UFUNCTION()
	void TimelineCoverLow(float Value);
	
	void SetChangeWorldPossibility(bool newValue, AStaticObjectToNothing* overlappedAct);
	void SetChangeWorldPossibility(bool newValue, ASoldierEnemy* overlappedAct);

	UFUNCTION()
	bool CheckIfWorldCanBeChanged() const;

	UPROPERTY(BlueprintAssignable)
	FVisorPressed VisorPressedDelegate;

	UPROPERTY(BlueprintAssignable)
	FVisorReleased VisorReleasedDelegate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
	FPlayerAimZoomBlueprint PlayerAimZoom;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
	FLeftSideViewBlueprint LeftSideView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cover")
	FCameraCover CameraCover;

	UPROPERTY()
	UPlayerAimZoomFunctions* PlayerAimZoomFunctions;

	UPROPERTY()
	ULeftSideViewFunctions* LeftSideViewFunctions;

	UPROPERTY()
	UCameraCoverFunctions* CameraCoverFunctions;
	
	USpringArmComponent* GetPlayerSpringArmComponent(){ return SpringArmComponent; }
	void CameraZoomIn();
	void CameraZoomOut();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USphereComponent* CameraCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USceneComponent* CameraSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Phrases")
	USoundCue* WorldCantBeChangedPhrase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Death")
	UParticleSystem* DeathVFX;
	
	bool WorldCanBeChanged=true;
	AStaticObjectToNothing* OverlappedChangeWActor;
	ASoldierEnemy* OverlappedChangeWEnemy;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnEnergyAttributeChanged(const FOnAttributeChangeData& Data) override;
	virtual void OnCooldownExpired(const FActiveGameplayEffect& ExpiredEffect) override;
	
	virtual void OnDeath() override;
	virtual void BeginPlay() override;
	
	void OnCameraMove();
	
	
	void OnWorldChanged();
	virtual bool StartCover_Internal(FHitResult& CoverHit) override;
	virtual bool StopCover_Internal() override;
	virtual void EnterDeathWorld();
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FOnEnergyValueChanged OnEnergyValueChangedHandle;

	//The range in which enemies and objects are highlighted 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ablity Higlhlight")
	float HighlightRadius = 2000.f;

	//Time for effect to remain after turn off 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ablity Higlhlight")
	float DestroyDelay = 1.0f;

	//Trace Channel we use to detect all the stuff
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ablity Higlhlight")
	TEnumAsByte<ECollisionChannel> TraceChannelProvided;

	//Types of collisions by which objects are going to be highlighted
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ablity Higlhlight")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToHighlight;
	
	UFUNCTION(BlueprintCallable)
	FRotator GetAimDelta() const;
	virtual bool IsRunning() const override;
	virtual ECoverType CheckCover() override;
	virtual void OnTurn() override;
	virtual void Falling() override;
	virtual void Landed(const FHitResult& Hit) override;
	
	TArray<FAmmoData> GetPlayerWeapons() const;
	UFUNCTION(BlueprintCallable)
	FCoverData& GetCoverData();
	
	bool IsMovingRight() const { return bIsMovingRight; }
	bool IsMovingLeft() const { return bIsMovingLeft; }
	bool IsMovingBackward() const { return bIsMovingBackward; }
	bool IsMovingForward() const { return bIsMovingForward; }
private:
	UPROPERTY()
	UBaseCharacterMovementComponent* PlayerMovementComponent;
	UPROPERTY()
	FCoverData CoverData;
	FTimerHandle DeathTimerHandle;
	
	bool bWantsToRun = false;
	bool bIsMovingForward = false;
	bool bIsMovingRight = false;
	bool bIsMovingBackward = false;
	bool bIsMovingLeft = false;
	
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void StartRun();
	void StopRun();
	void Cover();
	void StartFire();
	void LookUp(float Amount);
	void LookAround(float Amount);
	void CoverCrouch();
	UPROPERTY()
	class USphereComponent* SphereDetectingHighlightables;
	
	FTimerHandle THandle;
	const float FlipTime = 0.5f;
	const float FlipStrength = 2000.f;
	// curve from content manager
	UPROPERTY(EditAnywhere)
	UCurveFloat* FlipCurve = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/ProjectRevival/Core/Player/FlipCurve.FlipCurve"));
	
	
	UFUNCTION()
	void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckCameraOverlap();
};