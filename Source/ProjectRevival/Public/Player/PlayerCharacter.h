// Project Revival. All Rights Reserved

#pragma once

#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "Player/BaseCharacter.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timeline")
	FPlayerAimZoom PlayerAimZoom;
	
	FTimeline CurveTimeline;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USphereComponent* CameraCollisionComponent;
	
	virtual void OnDeath() override;
	virtual void BeginPlay() override;

	void CameraZoomIn();
	void CameraZoomOut();

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USpringArmComponent* SpringArmComponent;
	virtual bool IsRunning() const override;
	
private:
	bool bWantsToRun = false;
	bool IsMovingForward = false;
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void StartRun();
	void StopRun();

	UFUNCTION()
	void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckCameraOverlap();

	void On_Camera_Move();
	bool Block = false;
	bool IsMoving = false;
	bool CamPos = false;
	float InterpSpeed = 0.0;
	void Camera_Moving();
	void Camera_Stop();
	void Camera_Block();

	UPROPERTY(EditAnywhere, Category = "Interp")
	FVector DeltaPosition;
};