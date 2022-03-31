// Project Revival. All Rights Reserved


#include "AbilitySystem/Abilities/Miscellaneuos/IDynMaterialsFromMesh.h"

// Add default functionality here for any IIDynMaterialsFromMesh functions that are not pure virtual.
void IIDynMaterialsFromMesh::SetupDynMaterialsFromMesh(AActor* Actor, TArray<UMaterialInstanceDynamic*>& DynamicMaterials)
{
	for (auto Component: Actor->GetComponents())
	{
		const auto Mesh = Cast<UMeshComponent>(Component);
		if (Mesh)
		{
			const int32 MatNum = Mesh->GetNumMaterials();
			for (int32 i=0;i<MatNum;i++)
			{
				const auto Material = Mesh->CreateDynamicMaterialInstance(i);
				DynamicMaterials.Add(Material);
			}
			
		}
	}
	TArray<AActor*> AttachedActors;
	Actor->GetAttachedActors(AttachedActors, false);
	if (AttachedActors.Num()>0)
		for (AActor* AttachedActor : AttachedActors)
			SetupDynMaterialsFromMesh(AttachedActor, DynamicMaterials);
}
