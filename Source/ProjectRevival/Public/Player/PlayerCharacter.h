// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/BaseCharacter.h"
#include "Components/TimelineComponent.h"
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
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USphereComponent* CameraCollisionComponent;

	virtual void OnDeath() override;
	virtual void BeginPlay() override;

	void CameraZoomIn();
	void CameraZoomOut();

	FTimeline CurveTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveVector* CurveVector;
	UPROPERTY()
	FVector StartLoc;
	UPROPERTY()
	FVector EndLoc;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	FVector Offset;
	FVector Start_StartPos = FVector(0.0, 0.0, 0.0);
	bool IsZooming = false;
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


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