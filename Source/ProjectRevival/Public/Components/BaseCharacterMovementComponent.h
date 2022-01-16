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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsPivotTargeted=false;
	bool IsInJump() const {return JumpStarted || JumpPressed;}
};

UCLASS()
class PROJECTREVIVAL_API UBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	FPlayerMovementLogic PlayerMovementLogic;
	
	void JumpPressEnded();
	void JumpProcessEnded();
	void AimStartFinished();
	void AimEndFininshed();
	void JumpInMoveStarted();
protected:
	UPROPERTY(EditDefaultsOnly, Category="Jump")
	UAnimSequence* JumpAnim;
	
	UPROPERTY(EditDefaultsOnly, Category="Jump")
	UAnimSequence* JumpEndAnim;

	UPROPERTY(EditDefaultsOnly, Category="Jump")
	UAnimSequence* JumpInMove;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement", meta=(ClampMin="1.5", ClampMax="3.0"))
	float RunModifier=1.5;
	virtual float GetMaxSpeed() const override;
	void Jump();
	void MoveRight(float Amount);
	void MoveForward(float Amount);
	void AimStart();
	void AimEnd();
	UFUNCTION(BlueprintCallable)
	FPlayerMovementLogic GetPlayerMovementLogic();
};
