// Project Revival. All Rights Reserved

#include "Miscellaneous/FuncActors/CutsceneTriggerActor.h"
#include "Components/BoxComponent.h"
#include "Components/WeaponComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectRevival/Public/Miscellaneous/PRUtils.h"
//#include "Sequencer/Public/SequencerChannelTraits.h"

ACutsceneTriggerActor::ACutsceneTriggerActor()
{
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>("CollisionComponent");
	RootComponent = CollisionComponent;

	CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionComponent->SetupAttachment(RootComponent);
	
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACutsceneTriggerActor::OnOverlapBegin);
}

void ACutsceneTriggerActor::BeginPlay()
{
 	Super::BeginPlay();
 	check(CollisionComponent);
	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), SequenceAsset, FMovieSceneSequencePlaybackSettings(), LevelSequenceActor);
} 


void ACutsceneTriggerActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}

void ACutsceneTriggerActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		const auto Pawn = Cast<APawn>(OtherActor);
		if (Pawn)
		{
			if (SequencePlayer)
			{
				SequencePlayer->OnPlay.AddDynamic(this,&ACutsceneTriggerActor::OnSequencePlay);
				SequencePlayer->OnFinished.AddDynamic(this,&ACutsceneTriggerActor::OnSequenceEnd);
				SequencePlayer->Play();
			}
			else
			{
				UE_LOG(LogActor, Error, TEXT("OnOverlapBegin: !SequencePlayer"));
			}
			UE_LOG(LogActor, Warning, TEXT("OnOverlapBegin"));
		}
		else UE_LOG(LogActor, Warning, TEXT("OnOverlapBegin: !Pawn"));
	}
}

void ACutsceneTriggerActor::OnSequencePlay()
{
	UE_LOG(LogActor, Warning, TEXT("OnSequencePlay()"));
	if(GetWorld() != nullptr && Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)) != nullptr)
	{
		APlayerCharacter* const Character = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
		UWeaponComponent* Weapon = Cast<UWeaponComponent>(Character->GetWeaponComponent());
		APlayerController* Controller = Cast<APlayerController>(Character->GetController());
                  		
		Character->CameraZoomOut();
		Character->DisableInput(Controller);
		if(Weapon->IsShooting())
		{
			Weapon->StopFire();
		}
	}
}

void ACutsceneTriggerActor::OnSequenceEnd()
{
	UE_LOG(LogActor, Warning, TEXT("OnSequenceEnd()"));
	if(GetWorld() != nullptr && Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)) != nullptr)
	{
		if(GetWorld() != nullptr && Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)) != nullptr)
		{
			APlayerCharacter* const Character = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
			APlayerController* Controller = Cast<APlayerController>(Character->GetController());
			Character->EnableInput(Controller);
		}
	}
}
