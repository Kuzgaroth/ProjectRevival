// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/PRAbilitySystemComponent.h"
#include "AbilitySystem/PRAttributeSet.h"
#include "AbilitySystem/Abilities/PRGameplayAbility.h"
#include "BaseCharacter.generated.h"

class UHealthComponent;
class UWeaponComponent;
class USoundCue;

DECLARE_LOG_CATEGORY_EXTERN(LogPRAbilitySystemBase, Log, All);

UENUM(BlueprintType)
enum class EGASInputActions : uint8
{
	None,
	Base
};

UCLASS()
class PROJECTREVIVAL_API ABaseCharacter : public ACharacter,  public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UHealthComponent* HealthComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* DeathAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);
	
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category="Team")
	FName MaterialColorName = "Paint Color";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound")
	USoundCue* DeathSound;

	/** Компонент для управления способностями */
	UPROPERTY()
	UPRAbilitySystemComponent* AbilitySystemComponent;

	/** Класс атрибутов персонажа */
	UPROPERTY()
	UPRAttributeSet* AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TMap<EGASInputActions, TSubclassOf<UPRGameplayAbility>> GameplayAbilities;
	
	virtual void AddStartupGameplayAbilities();
	virtual void OnEnergyAttributeChanged(const FOnAttributeChangeData& Data);
	virtual void OnCooldownExpired(const FActiveGameplayEffect& ExpiredEffect); 
	virtual void PostInitializeComponents() override;
	virtual void OnDeath();
	virtual void BeginPlay() override;
	virtual void OnHealthChanged(float CurrentHealth, float HealthDelta);
public:	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category="Movement")
	virtual bool IsRunning() const;
	
	UFUNCTION(BlueprintCallable, Category="Movement")
	float GetMovementDirection() const;
	
	void SetPlayerColor(const FLinearColor& Color);
private:
	
	UFUNCTION()
	void OnGroundLanded(const FHitResult& HitResult);

	friend UPRAttributeSet;
};


