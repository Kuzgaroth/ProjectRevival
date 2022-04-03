// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChangeWorld.h"
#include "Interfaces/IChangingWorldActor.h"
#include "UObject/UObjectGlobals.h"
#include "CoreTypes.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "StaticObjectToNothing.generated.h"

class UBoxComponent;

UCLASS()

class PROJECTREVIVAL_API AStaticObjectToNothing : public AChangeWorld, public IIChangingWorldActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStaticObjectToNothing();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FCollisionResponseContainer CollisionResponseContainer;
	TArray<UMaterialInstanceDynamic*> MeshesMaterials;
	
    FTimeline TimeLine;
    FOnTimelineEvent OnTimeLineFinished;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UCurveFloat* VisualCurve;
    float MinCurveValue;
    float MaxCurveValue;
    bool isApearing=false;
    UFUNCTION()
    virtual void TimeLineFinished();
    UFUNCTION()
    virtual void TimeLineFloatReturn(float Value);
    FOnTimelineFloat InterpFunction;


	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FCoverPointsAndPossibility CoverStruct;


	
	virtual void LoadComponentTags(UStaticMeshComponent* supermesh) override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float TransparencyLevel=5.0f;
	



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	UStaticMeshComponent* SuperMesh;

	TArray<FName> MeshTags;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TEnumAsByte<EChangeWorld> World = OrdinaryWorld;
	
	void ShowChangeWorldObjectByAbility();
	void HideChangeWorldObjectByAbility();


	UFUNCTION()
	void OnMeshComponentCollision(UPrimitiveComponent* OverlappedComponent, 
					  AActor* OtherActor, 
					  UPrimitiveComponent* OtherComp, 
					  int32 OtherBodyIndex, 
					  bool bFromSweep, 
					  const FHitResult &SweepResult);

	UFUNCTION()
	void OnMeshComponentEndCollision(UPrimitiveComponent* OverlappedComponent, 
					  AActor* OtherActor, 
					  UPrimitiveComponent* OtherComp, 
					  int32 OtherBodyIndex);

	UFUNCTION()
	void OnCoverPointComponentCollision(UPrimitiveComponent* OverlappedComponent, 
					  AActor* OtherActor, 
					  UPrimitiveComponent* OtherComp, 
					  int32 OtherBodyIndex, 
					  bool bFromSweep, 
					  const FHitResult &SweepResult);

	UFUNCTION()
	void OnCoverPointComponentExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void BlockCoverPoint(const UBoxComponent* CoverPoint);

	UFUNCTION()
	void FreeCoverPoint(const UBoxComponent* CoverPoint);
	
	void Changing() override;
	
	virtual void ChangeVisibleWorld(EChangeAllMapEditorVisibility VisibleInEditorWorld) override;
	
	virtual bool CheckIsChangeAbleObjIsCover() override;
	virtual bool TryToFindCoverPoint(FVector PlayerPos, FVector& CoverPos) override;
};
