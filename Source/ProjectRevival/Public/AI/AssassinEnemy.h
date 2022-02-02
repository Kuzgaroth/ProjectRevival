// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "Player/BaseCharacter.h"
#include "AssassinEnemy.generated.h"

UCLASS()
class PROJECTREVIVAL_API AAssassinEnemy : public AAICharacter
{
	GENERATED_BODY()
public:
	//AAssassinEnemy(const FObjectInitializer& ObjectInitializer);
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	//ABaseAIController* AICon;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	//UWeaponComponent* WeaponComponent;
	
	virtual void PossessedBy(AController* NewController) override;

	bool IsAttacking();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsAttacking = false;
private:
	//
};
