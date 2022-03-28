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
}

void AKWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
	check(MagazineMeshComponent);
	
	if(!UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) || !GetWorld())
	{
		UE_LOG(LogCustom,Error,TEXT("Unable to spawn magazine"));
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
	UE_LOG(LogCustom, Log,TEXT("Magazines.Num() = %d"), Magazines.Num());
	UE_LOG(LogCustom,Error,TEXT("Scale = %f"), Magazines.Last()->GetActorScale().X);
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
			UE_LOG(LogCustom, Log,TEXT("Character = %s"), *Character->GetName()); 
			Rotation = Character->GetMesh()->GetSocketRotation(SocketName);
			Location = Character->GetMesh()->GetSocketLocation(SocketName);
			Parent = Character->GetMesh();
		}
		else
		{
			UE_LOG(LogCustom,Error,TEXT("AKWeapon::SpawnMagazine(): !Character->GetMesh()"));
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
		SpawnedMagazine->SetOwner(this);
		const FVector TempScale(1, 1, 1);
		SpawnedMagazine->GetMeshComponent()->SetWorldScale3D(TempScale);
		Magazines.Add(SpawnedMagazine);
		UE_LOG(LogCustom, Log,TEXT("Magazines.Num() = %d"), Magazines.Num());
		UE_LOG(LogCustom, Log,TEXT("Magazines.Last = %s"), *Magazines.Last()->GetName()); 
		
		const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepWorld, false);
		SpawnedMagazine->AttachToComponent(Parent, AttachmentTransformRules, SocketName);
	}
	else
	{
		UE_LOG(LogCustom,Error,TEXT("AKWeapon::SpawnMagazine(): !SpawnedMagazine"));
	}
}

void AKWeapon::Take()
{
	if(!UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) || !GetWorld())
 	{
 		UE_LOG(LogCustom,Error,TEXT("Unable to spawn magazine"));
 		return;
 	}
	
	SpawnMagazine(ArmSocketName);
}

void AKWeapon::Drop()
{
	UE_LOG(LogCustom,Log,TEXT("AKWeapon::Drop()"));
	
	if(!GetWorld())
 	{
		UE_LOG(LogCustom,Error,TEXT("AKWeapon::Drop()"));
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
	UE_LOG(LogCustom,Log,TEXT("AKWeapon::Lock()"));
	
	if(!GetWorld() || !this)
	{
		UE_LOG(LogCustom,Error,TEXT("AKWeapon::Lock()"));
		return;
	}
	if(Magazines.Last() != nullptr)
	{
		const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
		Magazines.Last()->AttachToComponent(this->RootComponent, AttachmentTransformRules, MagazineSocketName);
		
		MagazineMeshComponent->SetVisibility(true);
	}
}
