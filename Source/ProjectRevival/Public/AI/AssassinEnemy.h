// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "PlayerCharacter.h"
#include "AssassinEnemy.generated.h"

UCLASS()
class PROJECTREVIVAL_API AAssassinEnemy : public AAICharacter
{
	GENERATED_BODY()
public:
	bool IsAttacking() const;
	void SetIsAttacking(bool const Status) { bIsAttacking = Status; };
	void SetMovementMode(EMovementMode Mode);
	
protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsAttacking = false;
	
	virtual void PossessedBy(AController* NewController) override;
	
private:
	bool bWantsToRun = false;
	bool IsMovingForward = false;
	
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void StartRun();
	void StopRun();
	virtual bool IsRunning() const override;
	void LookUp(float Amount);
	void LookAround(float Amount);
	
	UPROPERTY()
	UCharacterMovementComponent* AssassinMovementComponent;
};
