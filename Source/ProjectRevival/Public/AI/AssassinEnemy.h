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
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsAttacking = false;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TSubclassOf<class UGameplayAbility> MeleeAbility;
	virtual void PossessedBy(AController* NewController) override;
	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	//void ActivateAbility();
private:
	//
};
