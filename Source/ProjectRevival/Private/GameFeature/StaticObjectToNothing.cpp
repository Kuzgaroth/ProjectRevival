// Project Revival. All Rights Reserved


#include "GameFeature/StaticObjectToNothing.h"
#include <string>
#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "PRGameModeBase.h"
#include "AbilitySystem/AbilityActors/ChangeWorldSphereActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY(LogPRStaticObject);

// Sets default values
AStaticObjectToNothing::AStaticObjectToNothing()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	


	InterpFunction.BindUFunction(this,FName("TimeLineFloatReturn"));
	OnTimeLineFinished.BindUFunction(this,FName("TimeLineFinished"));
	
	SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SuperMesh"));
	RootComponent = SuperMesh;
	SuperMesh->SetMobility(EComponentMobility::Movable);
	SuperMesh->SetVisibility(true);
}

// Called when the game starts or when spawned
void AStaticObjectToNothing::BeginPlay()
{
	Super::BeginPlay();
	CollisionResponseContainer=SuperMesh->GetCollisionResponseToChannels();
	for(int i=0;i<this->Tags.Num();i++)
	{
		MeshTags.Add(this->Tags[i]);
	}
	SuperMesh->SetVisibility(true);
	int32 num=SuperMesh->GetNumMaterials();
	for (int32 i=0;i<num;i++)
	{
		const auto Material = SuperMesh->CreateDynamicMaterialInstance(i);
		MeshesMaterials.Add(Material);
	}
	if(VisualCurve)
	{
		VisualCurve->GetValueRange(MinCurveValue,MaxCurveValue);
		TimeLine.AddInterpFloat(VisualCurve,InterpFunction);
		TimeLine.SetTimelineFinishedFunc(OnTimeLineFinished);
		TimeLine.SetLooping(false);
	}
	auto GameMode=Cast<APRGameModeBase>(GetWorld()->GetAuthGameMode());
	if (World==OrdinaryWorld)
	{
		if(VisualCurve&&MeshesMaterials.Num()>0)
		{
			for (const auto Material : MeshesMaterials)
			{
				if (Material!=nullptr)
				{
					Material->SetVectorParameterValue("Color",FLinearColor::Red);
				}
			}
		}
	}
	else
	{
		if(VisualCurve&&MeshesMaterials.Num()>0)
		{
			for (const auto Material : MeshesMaterials)
			{
				if (Material!=nullptr)
				{
					Material->SetVectorParameterValue("Color",FLinearColor::Blue);
				}
			}
		}
	}
	if (GameMode && GameMode->GetCurrentWorld()==World)
	{
		isApearing=true;
		if(VisualCurve&&MeshesMaterials.Num()>0)
		{
			for (const auto Material : MeshesMaterials)
			{
				if (Material!=nullptr)
				{
					Material->SetScalarParameterValue("Amount",MinCurveValue);
				}
			}
		}
		else
		{
			SuperMesh->SetVisibility(true);
		}
		SuperMesh->SetCollisionProfileName("OverlapAll");
		SuperMesh->SetCollisionResponseToChannels(CollisionResponseContainer);
	}
	else
	{
		if(VisualCurve&&MeshesMaterials.Num()>0)
		{
			for (const auto Material : MeshesMaterials)
			{
				if (Material!=nullptr)
				{
					Material->SetScalarParameterValue("Amount",MaxCurveValue);
				}
			}
		}
		SuperMesh->SetVisibility(false);
		
		SuperMesh->SetCollisionProfileName("OverlapAll");
		ClearComponentTags(SuperMesh);
	}
	SuperMesh->OnComponentBeginOverlap.AddDynamic(this,&AStaticObjectToNothing::OnMeshComponentCollision);

	SuperMesh->GetChildrenComponents(true,CoverStruct.CoverPositions);
	
	for(auto covpoint:CoverStruct.CoverPositions)
	{
		auto boxcomp=Cast<UBoxComponent>(covpoint);
		if(boxcomp)
		{
			CoverStruct.PointIsNotTaken.Add(boxcomp,true);
			boxcomp->OnComponentBeginOverlap.AddDynamic(this,&AStaticObjectToNothing::OnCoverPointComponentCollision);
			boxcomp->OnComponentEndOverlap.AddDynamic(this,&AStaticObjectToNothing::OnCoverPointComponentExit);
			
		}
	}
	SuperMesh->OnComponentEndOverlap.AddDynamic(this,&AStaticObjectToNothing::OnMeshComponentEndCollision);
	
}

// Called every frame
void AStaticObjectToNothing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(TimeLine.IsPlaying())
	{
		TimeLine.TickTimeline(DeltaTime);
	}
}
#if WITH_EDITOR
void AStaticObjectToNothing::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if(PropertyChangedEvent.Property->GetName()=="VisibleWorld")
	{
		if(VisibleWorld==World || VisibleWorld==BothWorlds)
		{
			SuperMesh->SetVisibility(true);
		}
		else
		{
			SuperMesh->SetVisibility(false);
		}
	}
	if(PropertyChangedEvent.Property->GetName()=="AllObjectVisibleWorld")
	{
		TArray<AActor*> ChangeAbleObjs;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),AChangeWorld::StaticClass(),ChangeAbleObjs);
		for(auto obj:ChangeAbleObjs)
		{
			Cast<AStaticObjectToNothing>(obj)->ChangeVisibleWorld(AllObjectVisibleWorld);
		}
	}
}
#endif


void AStaticObjectToNothing::Changing()
{
	if(TimeLine.IsPlaying()) return;
	auto GameMode=Cast<APRGameModeBase>(GetWorld()->GetAuthGameMode());
	if(GameMode)
	{
		if(GameMode->GetCurrentWorld()==CurrentWorld)
		{
			return;
		}
		CurrentWorld=GameMode->GetCurrentWorld();
	}
	if (World == CurrentWorld)
	{
		if(VisualCurve)
		{
			isApearing=true;
			SuperMesh->SetVisibility(true);
			SuperMesh->SetCollisionResponseToChannels(CollisionResponseContainer);
			LoadComponentTags(SuperMesh);
			TimeLine.PlayFromStart();
			
		}
		else
		{
			SuperMesh->SetCollisionResponseToChannels(CollisionResponseContainer);
			LoadComponentTags(SuperMesh);
			SuperMesh->SetVisibility(true);
		}
	}
	else
	{
		if(VisualCurve)
		{
			isApearing=false;
			TimeLine.PlayFromStart();
		}
		else
		{
			ClearComponentTags(SuperMesh);
			SuperMesh->SetCollisionProfileName("OverlapAll");
			SuperMesh->SetVisibility(false);
		}
	}
}

void AStaticObjectToNothing::ChangeVisibleWorld(EChangeAllMapEditorVisibility VisibleInEditorWorld)
{
	if(VisibleInEditorWorld!=OwnValuesWorld)
	{
		switch (VisibleInEditorWorld)
		{
		case DefaultVisibleWorld:
			VisibleWorld=DefaultWorld;
			break;
		case OtherVisibleWorld:
			VisibleWorld=AltirnativeWorld;
			break;
		case BothVisibleWorlds:
			VisibleWorld=BothWorlds;
			break;
		default:
			break;
		}
		if(VisibleWorld==World || VisibleWorld==BothWorlds)
		{
			SuperMesh->SetVisibility(true);
		}
		else
		{
			SuperMesh->SetVisibility(false);
		}
	}
	else
	{
		VisibleWorld=DefaultWorld;
		if(VisibleWorld==World || VisibleWorld==BothWorlds)
		{
			SuperMesh->SetVisibility(true);
		}
		else
		{
			SuperMesh->SetVisibility(false);
		}
	}
}

void AStaticObjectToNothing::ShowChangeWorldObjectByAbility()
{

	SuperMesh->SetVisibility(true);
	GLog->Log("ShowingObject");
	if(MeshesMaterials.Num()!=0)
		for (const auto Material : MeshesMaterials)
		{
			auto reqwar=(MaxCurveValue-MinCurveValue)/TransparencyLevel;
			reqwar=MaxCurveValue-reqwar;
			if (Material!=nullptr) Material->SetScalarParameterValue("Amount",reqwar);
		}
	
}

void AStaticObjectToNothing::HideChangeWorldObjectByAbility()
{
	if(SuperMesh->GetCollisionProfileName()=="OverlapAll")
	{
		SuperMesh->SetVisibility(false);
		if(MeshesMaterials.Num()!=0)
			for (const auto Material : MeshesMaterials)
			{
				if (Material!=nullptr) Material->SetScalarParameterValue("Amount",MaxCurveValue);
			}
	}
}

void AStaticObjectToNothing::OnMeshComponentCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogPRStaticObject, Warning, TEXT("StaticObject OnMeshComponentCollision"))
	//UE_LOG(LogPRStaticObject, Warning, TEXT("%s"), *FString(OtherActor->GetName()))
	if(Cast<AChangeWorldSphereActor>(OtherActor))
	{
		UE_LOG(LogPRStaticObject, Warning, TEXT("StaticObject Changing"))
		Changing();
	}
	auto Player=Cast<APlayerCharacter>(OtherActor);
	auto GameMode=Cast<APRGameModeBase>(GetWorld()->GetAuthGameMode());
	if(Player&& GameMode && CurrentWorld==GameMode->GetCurrentWorld())
	{
		UE_LOG(LogPRStaticObject, Warning, TEXT("%s"), *FString(OtherActor->GetName()))
		Player->SetChangeWorldPossibility(this);
	}
}

void AStaticObjectToNothing::OnMeshComponentEndCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto Player=Cast<APlayerCharacter>(OtherActor);
	if(Player)
	{
		Player->RemoveOverlappedChangeWActor(this);
		HideChangeWorldObjectByAbility();
	}
}

void AStaticObjectToNothing::OnCoverPointComponentCollision(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,
															UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult)
{
	if(Cast<APawn>(OtherActor))
	{
		UE_LOG(LogPRStaticObject, Log, TEXT("Point BeginOverlap()"))
		BlockCoverPoint(Cast<UBoxComponent>(OverlappedComponent));
	}
}

void AStaticObjectToNothing::OnCoverPointComponentExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(Cast<APawn>(OtherActor))
	{
		UE_LOG(LogPRStaticObject, Log, TEXT("Point EndOverlap()"))
		FreeCoverPoint(Cast<UBoxComponent>(OverlappedComponent));
	}
}

void AStaticObjectToNothing::SetLastCoverPointStatus(bool bIsFree)
{
	CoverStruct.PointIsNotTaken[Cast<UBoxComponent>(CoverStruct.LastCoverPosition)] = bIsFree;
}

void AStaticObjectToNothing::BlockCoverPoint(const UBoxComponent* CoverPoint)
{
	UE_LOG(LogPRStaticObject, Log, TEXT("StaticToNothing: BlockCoverPoint() CoverPoint is %s"), *CoverPoint->GetName())
	CoverStruct.PointIsNotTaken[CoverPoint]=false;
}

void AStaticObjectToNothing::FreeCoverPoint(const UBoxComponent* CoverPoint)
{
	UE_LOG(LogPRStaticObject, Log, TEXT("StaticToNothing: FreeCoverPoint() CoverPoint is %s"), *CoverPoint->GetName())
	CoverStruct.PointIsNotTaken[CoverPoint]=true;
}

void AStaticObjectToNothing::TimeLineFinished()
{
	if(!isApearing)
	{
		SuperMesh->SetCollisionProfileName("OverlapAll");
		ClearComponentTags(SuperMesh);
		SuperMesh->SetVisibility(false);
	}
}

void AStaticObjectToNothing::TimeLineFloatReturn(float Value)
{
	for (const auto Material : MeshesMaterials)
	{
		if(isApearing)
		{
			if (Material!=nullptr) Material->SetScalarParameterValue("Amount",Value);
		}
		else
		{
			float val=MinCurveValue-Value;
			val=MaxCurveValue+val;
			if (Material!=nullptr) Material->SetScalarParameterValue("Amount",val);
		}
	}
}


void AStaticObjectToNothing::ClearComponentTags(UStaticMeshComponent* supermesh)
{
	Tags.Empty();
}

void AStaticObjectToNothing::LoadComponentTags(UStaticMeshComponent* supermesh)
{
	for(int i=0;i<MeshTags.Num();i++)
	{
		this->Tags.AddUnique(MeshTags[i]);
	}
}

bool AStaticObjectToNothing::CheckIsChangeAbleObjIsCover()
{
	return CoverStruct.CanBeTakenAsCover&&this->ActorHasTag("Cover");
}

bool AStaticObjectToNothing::TryToFindCoverPoint(FVector PlayerPos, FVector& CoverPos)
{
	//UE_LOG(LogPRAISoldier, Log, TEXT("StaticToNothing: Input PlayerPos is %s"), *PlayerPos.ToString())
	//UE_LOG(LogPRAISoldier, Log, TEXT("StaticToNothing: Input CoverPos  is %s"), *CoverPos.ToString())
	if(CoverStruct.CoverPositions.Num()==0) return false;
	for(USceneComponent* covpos:CoverStruct.CoverPositions)
	{
		FVector TraceStart=covpos->GetComponentLocation();
		FVector TraceEnd=PlayerPos;
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		GetWorld()->LineTraceSingleByChannel(HitResult,TraceStart,TraceEnd,ECollisionChannel::ECC_Visibility,CollisionParams);
		UBoxComponent* box = Cast<UBoxComponent>(covpos);
		if(HitResult.bBlockingHit)
		{
			DrawDebugLine(GetWorld(),TraceStart,HitResult.ImpactPoint,FColor::Blue,false,3.0f,0,3.0f);
			if(HitResult.Actor==this && CoverStruct.PointIsNotTaken.Contains(box) && CoverStruct.PointIsNotTaken[box])
			{
				//UE_LOG(LogPRAISoldier, Log, TEXT("StaticToNothing: covpos set to %s"), *covpos->GetComponentLocation().ToString())
				CoverPos = TraceStart;
				CoverStruct.LastCoverPosition = covpos;
				return true;
			}
		}
	}
	return false;
}
