// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacterMovementComponent.h"
#include "Player/BaseCharacter.h"
#include "Components/TimelineComponent.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;
class UCurveFloat;

UCLASS()
class PROJECTREVIVAL_API APlayerCharacter : public ABaseCharacter
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
	FPlayerAimZoom PlayerAimZoom;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
	FLeftSideView LeftSideView;
	
	FTimeline CurveTimeline;
	FTimeline LeftSideViewCurveTimeline;

	
	
	USpringArmComponent* GetPlayerSpringArmComponent(){return SpringArmComponent;}
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USphereComponent* CameraCollisionComponent;

	virtual void OnDeath() override;
	virtual void BeginPlay() override;
	
	void OnCameraMove();
	void CameraStop();
	void CameraBlock();
	
	void CameraZoomIn();
	void CameraZoomOut();

	void OnWorldChanged();
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//The range in which enemies and objects are highlighted 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ablity Higlhlight")
	float HighlightRadius = 2000.f;

	//Trace Channel we use to detect all the stuff
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ablity Higlhlight")
	TEnumAsByte<ECollisionChannel> TraceChannelProvided;

	//Types of collisions by which objects are going to be highlighted
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ablity Higlhlight")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToHighlight;
	
	void HighlightAbility();

	UFUNCTION()
	void OnOverlapBeginForHighlight(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function used specially for detecting objects when using highlight function
	UFUNCTION()
	void OnOverlapEndForHighlight(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	FRotator GetAimDelta() const;
	
	virtual bool IsRunning() const override;
private:

	UPROPERTY()
	UBaseCharacterMovementComponent* PlayerMovementComponent;
	
	bool bWantsToRun = false;
	bool IsMovingForward = false;
	void MoveForward(float Amount);
	void StartRun();
	void StopRun();
	void StartFire();
	void LookUp(float Amount);
	void LookAround(float Amount);
	UPROPERTY()
	class USphereComponent* SphereDetectingHighlightables;
	
	bool IsHighlighting = false;

	FTimerHandle THandle;

	const float FlipTime = 0.5f;
	const float FlipStrength = 2000.f;
	// curve from content manager
	UPROPERTY(EditAnywhere)
	UCurveFloat* FlipCurve = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/ProjectRevival/Core/Player/FlipCurve.FlipCurve"));
	bool IsFlipping = false;
	
	UPROPERTY()
	TArray<AActor*> ToHighlight;
	//Array of objects/enemies to ignore at highlighting
	UPROPERTY()
	TArray<AActor*> ToIgnore;
	UFUNCTION()
	void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckCameraOverlap();
	
};