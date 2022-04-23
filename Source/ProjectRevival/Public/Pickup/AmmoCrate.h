// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "BaseWeapon.h"
#include "HealthPBWidget.h"
#include "AmmoCrate.generated.h"

class USphereComponent;
class UStaticMeshComponent;

//DECLARE_LOG_CATEGORY_EXTERN(LogPickup, Log, All);

UCLASS()
class PROJECTREVIVAL_API AAmmoCrate : public AActor
{
	GENERATED_BODY()

public:
	AAmmoCrate();

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visibility distance")
	float VisibilityDistance = 1000.0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DistanceToAmmo;

	UFUNCTION()
	float GetAmmoCratePercent() const;
	UFUNCTION()
	int32 GetCurrentClipsAmount() const { return CurrentClipsAmount; }
	UFUNCTION()
	void SetCurrentClipsAmount(int32 Amount) { CurrentClipsAmount = Amount; }
	
protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void UpdateWidgetVisibility();
	void UpdateWidgetPercent();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Components")
	UWidgetComponent* WidgetComponent;
	
	UPROPERTY(EditAnywhere, Category="Components")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, Category="Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	int32 ClipSize = 30.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup", meta=(ClampMin="1.0", ClampMax="10.0"))
	int32 MaxClipsAmount = 4.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	int32 MaxBulletsAmount = 120.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Pickup")
	int32 CurrentClipsAmount;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup")
    int32 CurrentBulletsAmount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
	TSubclassOf<ABaseWeapon> WeaponType;

	UPROPERTY(EditInstanceOnly)
	TEnumAsByte<EChangeWorld> AmmoWorld;
	EChangeWorld CurrentWorld;
private:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool IsInvisible = false;
	void SetCrateVisibility(bool NewVisibility);
	UFUNCTION()
	virtual  bool GivePickUpTo(APawn* PlayerPawn);
	UFUNCTION()
	void ChangeWorld(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	FCollisionResponseContainer CollisionContainer;
};


