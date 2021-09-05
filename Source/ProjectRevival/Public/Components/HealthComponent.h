// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "HealthComponent.generated.h"

class UCameraShakeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTREVIVAL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	FORCEINLINE float GetHealth() const {return Health;};
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsDead() const {return Health<=0.0f;};
	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;
	bool TryToAddHealth(float HealthAmount);
	bool IsHealthFull() const;
	
	UFUNCTION(BlueprintCallable, Category="Health")
	FORCEINLINE float GetHealthPercentage() const {return Health/MaxHealth;};
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMin="50", ClampMax="300"))
	float MaxHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Heal")
	bool AutoHeal;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Heal", meta=(EditCondition="AutoHeal"))
	float HealDelay = 3.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Heal", meta=(EditCondition="AutoHeal"))
	float HealModifier = 2.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Heal", meta=(EditCondition="AutoHeal"))
	float HealUpdateTime = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	TSubclassOf<UCameraShakeBase> CameraShake;
	
	virtual void BeginPlay() override;

private:
	float Health;
	UFUNCTION()
	void OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	FTimerHandle HealTimerHandle;
	void HealUpdate();
	void SetHealth(float NewHealth);
	void PlayCameraShake();
	void Killed(AController* KillerController);
};
