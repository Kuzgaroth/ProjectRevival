// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacterMovementComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerMovement, All,All);

USTRUCT(BlueprintType)
struct FPlayerMovementLogic
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool JumpPressed=false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool JumpStarted=false;
	
};

UCLASS()
class PROJECTREVIVAL_API UBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	FPlayerMovementLogic PlayerMovementLogic;
	
	void PlayTurnAnimation();
	void JumpPressEnded();
	void JumpProcessEnded();
protected:
	UPROPERTY(EditDefaultsOnly, Category="Turns")
	UAnimMontage* TurnAimLeftMontage;

	UPROPERTY(EditDefaultsOnly, Category="Turns")
	UAnimMontage* TurnAimRightMontage;

	UPROPERTY(EditDefaultsOnly, Category="Turns")
	UAnimMontage* TurnLeftMontage;

	UPROPERTY(EditDefaultsOnly, Category="Turns")
	UAnimMontage* TurnRightMontage;

	UPROPERTY(EditDefaultsOnly, Category="Turns")
	UAnimMontage* TurnBackwardMontage;

	UPROPERTY(EditDefaultsOnly, Category="Jump")
	UAnimSequence* JumpAnim;
	
	UPROPERTY(EditDefaultsOnly, Category="Jump")
	UAnimSequence* JumpEndAnim;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement", meta=(ClampMin="1.5", ClampMax="3.0"))
	float RunModifier=1.5;
	virtual float GetMaxSpeed() const override;

	void TurnLeft(bool IsPivotTargeted);
	void TurnRight(bool IsPivotTargeted);
	void TurnBackward(bool IsPivotTargeted);
	void Jump();
	
	UFUNCTION(BlueprintCallable)
	FPlayerMovementLogic GetPlayerMovementLogic();
};
