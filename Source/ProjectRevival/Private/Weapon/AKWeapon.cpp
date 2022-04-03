// Project Revival. All Rights Reserved

#include "AKWeapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AKWeapon::AKWeapon()
{
	MagazineClass = AMagazine::StaticClass();
	AmmoShell = AShell::StaticClass();
	
	RootComponent = WeaponMesh;
	MagazineMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MagazineMeshComponent");
	MagazineMeshComponent->SetupAttachment(RootComponent);


	MagazineScale.X = 1.0; MagazineScale.Y = 1.0; MagazineScale.Z = 1.0;
}

void AKWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
	if(ShutterMovement != nullptr)
		AnimationRate = this->TimeBetweenShots / ShutterMovement->GetPlayLength();
	else
		UE_LOG(LogActor,Error,TEXT("ShutterMovement = nullptr"));

	check(MagazineMeshComponent);

	
	
}

void AKWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(!UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) || !GetWorld())
	{
		UE_LOG(LogActor,Error,TEXT("Unable to spawn magazine"));
		return;
	}
	SpawnMagazine(MagazineSocketName); 
}

void AKWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKWeapon::MakeShot()
{
	Super::MakeShot();
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}
	SpawnShell(ShutterSocketName);
	Magazines.Last()->GetMeshComponent()->SetRelativeScale3D(MagazineScale);
}

void AKWeapon::SpawnShell(FName SocketName) const
{
	if(!this || !AmmoShell || !GetWorld()) return;
	
	const FRotator Rotation = this->GetActorRotation();
	const FVector Location = WeaponMesh->GetSocketLocation(SocketName);
	GetWorld()->SpawnActor<AShell>(AmmoShell, Location, Rotation);
}

void AKWeapon::SpawnMagazine(FName SocketName)
{
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0); 
	USceneComponent* Parent;
	FRotator Rotation;
	FVector Location;
	
	if(SocketName == ArmSocketName)
	{
		if(Character->GetMesh())
		{
			Rotation = Character->GetMesh()->GetSocketRotation(SocketName);
			Location = Character->GetMesh()->GetSocketLocation(SocketName);
			Parent = Character->GetMesh();
		}
		else
		{
			UE_LOG(LogActor,Error,TEXT("AKWeapon::SpawnMagazine(): !Character->GetMesh()"));
			return;
		}
	}
	else
	{
		Rotation = this->GetActorRotation();
		Location = this->RootComponent->GetSocketLocation(SocketName);
		Parent = this->RootComponent;
	}
	
	AMagazine* SpawnedMagazine = GetWorld()->SpawnActor<AMagazine>(MagazineClass, Location, Rotation);
	if (SpawnedMagazine)
	{
		Magazines.Add(SpawnedMagazine);
		SpawnedMagazine->AttachToComponent(Parent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}
	else
	{
		UE_LOG(LogActor,Error,TEXT("AKWeapon::SpawnMagazine(): !SpawnedMagazine"));
	}
}

void AKWeapon::Take()
{
	if(!UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) || !GetWorld())
 	{
 		UE_LOG(LogActor,Error,TEXT("Unable to spawn magazine"));
 		return;
 	}
	
	SpawnMagazine(ArmSocketName);
}

void AKWeapon::Drop()
{
	if(!GetWorld())
 	{
		UE_LOG(LogActor,Error,TEXT("AKWeapon::Drop()"));
		return;
	}
	
	if(Magazines.Num() > 1)
	{
		Magazines.Last(1)->DetachMagazine();
		MagazineMeshComponent->SetVisibility(false);
	}
}

void AKWeapon::Lock()
{
	if(!GetWorld() || !this)
	{
		UE_LOG(LogActor,Error,TEXT("AKWeapon::Lock()"));
		return;
	}
	if(Magazines.Last() != nullptr)
	{
		//const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
		Magazines.Last()->AttachToComponent(this->RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			MagazineSocketName);
		
		MagazineMeshComponent->SetVisibility(true);

	}
}