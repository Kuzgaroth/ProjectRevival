// Project Revival. All Rights Reserved


#include "Interfaces/IChangingWorldActor.h"

// Add default functionality here for any IIChangingWorldObjCoverCheck functions that are not pure virtual.
bool IIChangingWorldActor::CheckIsChangeAbleObjIsCover()
{
	return false;
}

bool IIChangingWorldActor::TryToFindCoverPoint(FVector PlayerPos, FVector& CoverPos)
{
	return false;
}

void IIChangingWorldActor::SetLastCoverPointStatus(bool bIsFree)
{
}

void IIChangingWorldActor::ClearComponentTags(UStaticMeshComponent* supermesh)
{
}

void IIChangingWorldActor::LoadComponentTags(UStaticMeshComponent* supermesh)
{
}

void IIChangingWorldActor::ChangeVisibleWorld(EChangeAllMapEditorVisibility VisibleInEditorWorld)
{
}

void IIChangingWorldActor::Changing()
{
}

void IIChangingWorldActor::TimeLineFinished()
{
}

void IIChangingWorldActor::TimeLineFloatReturn(float Value)
{
}

void IIChangingWorldActor::ShowChangeWorldObjectByAbility()
{
}

void IIChangingWorldActor::HideChangeWorldObjectByAbility()
{
}

void IIChangingWorldActor::OnMeshComponentCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void IIChangingWorldActor::OnMeshComponentEndCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


