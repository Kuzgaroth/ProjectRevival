// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "ProjectRevival/Public/AbilitySystem/PRAbilityTypes.h"
#include "BaseGrenade.generated.h"
UCLASS()
class PROJECTREVIVAL_API ABaseGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseGrenade();
	
	virtual void Tick(float DeltaTime) override;

	// This function is both starting and ending function
	UFUNCTION()
	virtual void GrenadeAction();
	
	// Makes visor object destroy itself
	UFUNCTION()
	virtual void SelfDestruction();

	//if set to "true" then Niagara is used, otherwise uses Cascade. By default is set to "true"
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	bool bUseNiagara = true;

	//Niagara effect to display
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	UNiagaraSystem* NiagaraEffect;

	//Cascade particle effect to display
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	UParticleSystem* CascadeEffect;

	//Time before GrenadeAction activation (before explosion)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Action parameters")
	float ActionDelay = 3.0f;

	UPROPERTY(BlueprintReadWrite, Category="Action parameters", Meta = (ExposeOnSpawn = true))
	bool bShouldBlow = true;
	
	FORCEINLINE void SetInitialSpeed(float Speed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SpawnActionEffect();
	void UnbindOverlapEvent();

	UPROPERTY()
	bool bIsFirstHit = true;

	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);

	FTimerHandle GrenadeActionHandler;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	UCapsuleComponent* CollisionComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Default")
	UStaticMeshComponent* GrenadeMesh;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UProjectileMovementComponent* MovementComponent;
	
};
