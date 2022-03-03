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

UENUM(BlueprintType, Category = "GameRules")
enum EChangeAllMapEditorVisibility
{
	DefaultVisibleWorld=0 UMETA(DisplayName = "Ordinary World"),
    OtherVisibleWorld=1 UMETA(DisplayName = "Other World"),
	BothVisibleWorlds=2 UMETA(DisplayName = "Both Worlds"),
	OwnValuesWorld=3 UMETA(DisplayName = "None World")

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
    TEnumAsByte<EChangeEditorVisibility> VisibleWorld = BothWorlds;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TEnumAsByte<EChangeAllMapEditorVisibility> AllObjectVisibleWorld=OwnValuesWorld;
	virtual void ClearComponentTags(UStaticMeshComponent* supermesh);
	virtual void LoadComponentTags(UStaticMeshComponent* supermesh);
	
	




	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TEnumAsByte<EChangeWorld> CurrentWorld = OrdinaryWorld;

	virtual void ChangeVisibleWorld(EChangeAllMapEditorVisibility VisibleInEditorWorld);
    virtual void Changing();
};
