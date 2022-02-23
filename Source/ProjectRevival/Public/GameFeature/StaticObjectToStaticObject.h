// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChangeWorld.h"
#include "StaticObjectToStaticObject.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTREVIVAL_API AStaticObjectToStaticObject : public AChangeWorld
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStaticObjectToStaticObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FCollisionResponseContainer OrdinaryWorldCollisionResponseContainer;
	FCollisionResponseContainer OtherWorldCollisionResponseContainer;
	TArray<UMaterialInstanceDynamic*> OrdinaryWMeshesMaterials;
	TArray<UMaterialInstanceDynamic*> OtherWMeshesMaterials;
	void OrdinaryWTimelineFinished();
	void OtherWTimelineFinished();
	void OrdinaryWTimelineFloatReturn(float Value);
	void OtherWTimelineFloatReturn(float Value);
	FTimeline OrdinaryWTimeLine;
	FTimeline OtherWTimeLine;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UCurveFloat* OrdinaryWVisualCurve;
	float MinOrWValue;
	float MaxOrWValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UCurveFloat* OtherWVisualCurve;
	float MinOtWValue;
	float MaxOtValue;
	FOnTimelineFloat OrWInterpFunction;
	FOnTimelineFloat OtWInterpFunction;
	FOnTimelineEvent OrOnTimeLineFinished;
	FOnTimelineEvent OtOnTimeLineFinished;
	virtual void Changing() override;
    

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	
	//UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	//TEnumAsByte<EChangeWorld> World = OrdinaryWorld;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	UStaticMeshComponent* SuperMesh1;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	UStaticMeshComponent* SuperMesh2;

	UFUNCTION()
	void OnOrdinaryMeshCollision(UPrimitiveComponent* OverlappedComponent, 
					  AActor* OtherActor, 
					  UPrimitiveComponent* OtherComp, 
					  int32 OtherBodyIndex, 
					  bool bFromSweep, 
					  const FHitResult &SweepResult);
	UFUNCTION()
	void OnOtherMeshCollision(UPrimitiveComponent* OverlappedComponent, 
					  AActor* OtherActor, 
					  UPrimitiveComponent* OtherComp, 
					  int32 OtherBodyIndex, 
					  bool bFromSweep, 
					  const FHitResult &SweepResult);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComponent;

};
