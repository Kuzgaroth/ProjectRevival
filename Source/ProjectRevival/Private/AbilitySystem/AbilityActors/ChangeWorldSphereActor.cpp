// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityActors/ChangeWorldSphereActor.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "DrawDebugHelpers.h"

#include "GameFeature/ChangeWorld.h"
#include "GameFeature/StaticObjectToNothing.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AChangeWorldSphereActor::AChangeWorldSphereActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent=CreateDefaultSubobject<USphereComponent>(TEXT("ChangeWorldSphere"));
	RootComponent=SphereComponent;
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&AChangeWorldSphereActor::OnSphereComponentCollision);
	CurrentRadius=StartRadius;
	SphereComponent->SetSphereRadius(CurrentRadius,true);
	
}

// Called when the game starts or when spawned
void AChangeWorldSphereActor::BeginPlay()
{
	Super::BeginPlay();
	
	
	ChangeWorldFXComponent=SpawnChangeWorldEffect();
	if(ChangeWorldFXComponent)
	{
		auto expSpeed=ExpantionSpeed;
		if(expSpeed<=0.0f)
		{
			expSpeed=1000.0f;
		}
		ChangeWorldFXComponent->SetVariableFloat("User.Life Time",1000.0f/expSpeed);
		ChangeWorldFXComponent->SetVariableVec2("User.Effect Size",FVector2D(2.5f*EndRadius));
		ChangeWorldFXComponent->Activate();
	}
	
}

void AChangeWorldSphereActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void AChangeWorldSphereActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(SphereComponent)
	{
		if(SphereComponent->GetScaledSphereRadius()>EndRadius)
		{
			AbilityEnded.Broadcast();
			Destroy();
		}
		CurrentRadius+=ExpantionSpeed*DeltaTime;
		SphereComponent->SetSphereRadius(CurrentRadius,true);
		DrawDebugSphere(GetWorld(),GetActorLocation(),CurrentRadius,24,FColor::Blue,false,0.1f);
		
	}

}
UNiagaraComponent* AChangeWorldSphereActor::SpawnChangeWorldEffect()
{
	if(ChangeWorldFX)
		return UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ChangeWorldFX,GetActorLocation(),GetActorRotation(),FVector(1),true,false);
	return nullptr;
}
//Обработка колизий
void AChangeWorldSphereActor::OnSphereComponentCollision(UPrimitiveComponent* OverlappedComponent, 
					  AActor* OtherActor, 
					  UPrimitiveComponent* OtherComp, 
					  int32 OtherBodyIndex, 
					  bool bFromSweep, 
					  const FHitResult &SweepResult)
{
	//auto obj =Cast<AChangeWorld>(OtherActor);
	//if(obj)
	//	obj->Changing();
}

