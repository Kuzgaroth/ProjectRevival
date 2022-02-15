// Project Revival. All Rights Reserved


#include "AbilitySystem/AbilityActors/ChangeWorldSphereActor.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "ComponentVisualizer.h"
#include "DrawDebugHelpers.h"
// Sets default values
AChangeWorldSphereActor::AChangeWorldSphereActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent=CreateDefaultSubobject<USphereComponent>(TEXT("ChangeWorldSphere"));
	RootComponent=SphereComponent;
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&AChangeWorldSphereActor::OnSphereComponentCollision);
	SphereComponent->SetSphereRadius(StartRadius,true);
}

// Called when the game starts or when spawned
void AChangeWorldSphereActor::BeginPlay()
{
	Super::BeginPlay();

	ChangeWorldFXComponent=SpawnChangeWorldEffect();
	ChangeWorldFXComponent->SetPaused(false);
	ChangeWorldFXComponent->SetVisibility(true,true);
	
	
}

// Called every frame
void AChangeWorldSphereActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(SphereComponent)
	{
		if(SphereComponent->GetScaledSphereRadius()>EndRadius)
			Destroy();
		StartRadius+=ExpantionSpeed*DeltaTime;
		SphereComponent->SetSphereRadius(StartRadius,true);
		DrawDebugSphere(GetWorld(),GetActorLocation(),StartRadius,24,FColor::Blue,false,0.2f);
		
	}

}
UNiagaraComponent* AChangeWorldSphereActor::SpawnChangeWorldEffect()
{
	
	return UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ChangeWorldFX,GetActorLocation(),GetActorRotation());
	
}
//Обработка колизий
void AChangeWorldSphereActor::OnSphereComponentCollision(UPrimitiveComponent* OverlappedComponent, 
					  AActor* OtherActor, 
					  UPrimitiveComponent* OtherComp, 
					  int32 OtherBodyIndex, 
					  bool bFromSweep, 
					  const FHitResult &SweepResult)
{
	//UE_LOG(LogTemp,Warning,TEXT("Collision with: %s"),*OtherActor->GetName());
}

