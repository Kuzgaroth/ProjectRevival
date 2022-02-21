// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "ChangeWorld.generated.h"

UENUM(BlueprintType, Category = "GameRules")
enum EChangeWorld
{
	OrdinaryWorld=0 UMETA(DisplayName = "Ordinary World"),
	OtherWorld=1 UMETA(DisplayName = "Other World")
};

UENUM(BlueprintType, Category = "GameRules")
enum EChangeEditorVisibility
{
	DefaultWorld=0 UMETA(DisplayName = "Ordinary World"),
    AltirnativeWorld=1 UMETA(DisplayName = "Other World"),
	BothWorlds=2 UMETA(DisplayName = "Both Worlds")
};

UCLASS()
class PROJECTREVIVAL_API AChangeWorld : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChangeWorld();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
    TEnumAsByte<EChangeEditorVisibility> VisibleWorld = DefaultWorld;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	UCurveFloat* VisualCurve;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<UMaterialInstanceDynamic*> MeshesMaterials;

	UFUNCTION()
	void TimelineProgress(float Value);
	FTimeline TimeLine;
	FOnTimelineEvent OnAppearFinished;
	FOnTimelineEvent OnDisappearFinished;
	UFUNCTION()
	virtual void OnTimeLineFinished();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TEnumAsByte<EChangeWorld> CurrentWorld = OrdinaryWorld;
	
    virtual void Changing();
};
