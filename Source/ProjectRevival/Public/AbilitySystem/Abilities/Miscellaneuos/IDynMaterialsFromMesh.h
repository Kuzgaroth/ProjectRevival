// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDynMaterialsFromMesh.generated.h"


UINTERFACE(MinimalAPI)
class UIDynMaterialsFromMesh : public UInterface
{
	GENERATED_BODY()
	
};



class PROJECTREVIVAL_API IIDynMaterialsFromMesh
{
	GENERATED_BODY()
	
public:
	virtual void SetupDynMaterialsFromMesh(AActor* Actor, TArray<UMaterialInstanceDynamic*>& DynamicMaterials);
	virtual TArray<UMaterialInstanceDynamic*>& GetDynMaterials()=0;
};
