// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;

UCLASS()
class PROJECTREVIVAL_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USphereComponent* CameraCollisionComponent;

	//The range in which enemies and objects are highlighted 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ablity Higlhlight")
	float HighlightRadius = 2000.f;

	//Trace Channel we use to detect all the stuff
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ablity Higlhlight")
	TEnumAsByte<ECollisionChannel> TraceChannelProvided;

	//Types of collisions by which objects are going to be highlighted
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ablity Higlhlight")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToHighlight;
	
	virtual void OnDeath() override;
	virtual void BeginPlay() override;
	void HighlightAbility();
	
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual bool IsRunning() const override;

	// declare overlap begin function used specially for detecting objects when using highlight function
	UFUNCTION()
	void OnOverlapBeginForHighlight(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function used specially for detecting objects when using highlight function
	UFUNCTION()
	void OnOverlapEndForHighlight(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    

private:
	bool bWantsToRun = false;
	bool IsMovingForward = false;
	bool IsHighlighting = false;
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void StartRun();
	void StopRun();
	
	FTimerHandle THandle;
	void Flip();
	void StopFlip();
	const float FlipTime = 0.5f;
	const float FlipStrength = 2000.f;
	// curve from content manager
	UPROPERTY(EditAnywhere)
	UCurveFloat* FlipCurve = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/ProjectRevival/Core/Player/FlipCurve.FlipCurve"));
	bool IsFlipping = false;
	
	//Array of objects/enemies to highlight
	UPROPERTY()
	TArray<AActor*> ToHighlight;
	//Array of objects/enemies to ignore at highlighting
	UPROPERTY()
	TArray<AActor*> ToIgnore;
	// sphere trigger which is active when we are using highlight
	UPROPERTY()
	class USphereComponent* SphereDetectingHighlightables;

	UFUNCTION()
	void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckCameraOverlap();

	void On_Camera_Move();
	bool Block = false;
	bool IsMoving = false;
	bool CamPos = false;
	float InterpSpeed = 0.0;
	void Camera_Moving();
	void Camera_Stop();
	void Camera_Block();
};
