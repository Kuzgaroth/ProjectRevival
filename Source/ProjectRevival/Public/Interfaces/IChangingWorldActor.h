// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "IChangingWorldActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIChangingWorldActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTREVIVAL_API IIChangingWorldActor
{
	GENERATED_BODY()

public:
	virtual bool CheckIsChangeAbleObjIsCover();

	virtual bool TryToFindCoverPoint(FVector PlayerPos, FVector& CoverPos);

	/**
	 * Changes a status (whether it's taken or not) of a CoverPoint (BoxComponent).
	 * @param bIsFree if false than there is already someone how wants to take it (i.e. other bots can't use it). if true than it is free and one can use it.
	 */
	virtual void SetLastCoverPointStatus(bool bIsFree);

	virtual void ClearComponentTags(UStaticMeshComponent* supermesh);
	virtual void LoadComponentTags(UStaticMeshComponent* supermesh);
	virtual void ChangeVisibleWorld(EChangeAllMapEditorVisibility VisibleInEditorWorld);
	virtual void Changing();
	virtual void TimeLineFinished();
	virtual void TimeLineFloatReturn(float Value);
	virtual void ShowChangeWorldObjectByAbility();
	virtual void HideChangeWorldObjectByAbility();
	virtual void OnMeshComponentCollision(UPrimitiveComponent* OverlappedComponent, 
					  AActor* OtherActor, 
					  UPrimitiveComponent* OtherComp, 
					  int32 OtherBodyIndex, 
					  bool bFromSweep, 
					  const FHitResult &SweepResult);
	virtual void OnMeshComponentEndCollision(UPrimitiveComponent* OverlappedComponent, 
					  AActor* OtherActor, 
					  UPrimitiveComponent* OtherComp, 
					  int32 OtherBodyIndex);
};
