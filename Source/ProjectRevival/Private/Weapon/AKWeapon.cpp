// Project Revival. All Rights Reserved

#include "AKWeapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AKWeapon::AKWeapon()
{
	MagazineClass = AMagazine::StaticClass();
	
	RootComponent = WeaponMesh;
	
	MagazineMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MagazineMeshComponent");
	MagazineMeshComponent->SetupAttachment(RootComponent);
}

void AKWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
	check(MagazineMeshComponent);
	
	AMagazine* Magazine = NewObject<AMagazine>();
	Magazines.Add(Magazine);
	//MagazineMeshComponent = Magazines.Last()->GetMeshComponent();
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
	if(ShutterMovement)
	{
		this->WeaponMesh->PlayAnimation(ShutterMovement, false);
	}
	SpawnShell(ShutterSocketName);
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
	if(!Cast<ACharacter>(this->GetOwner()) || !GetWorld())
	{
		UE_LOG(LogCustom,Error,TEXT("AKWeapon::SpawnMagazine()"));
		return;
	}
	
    const ACharacter* Character = Cast<ACharacter>(this->GetOwner());         
	FRotator Rotation = this->GetActorRotation(); //Rotation.Yaw-=90.f;
	//const FVector Location = Character->GetMesh()->GetSocketLocation(SocketName);
	AMagazine* TempMagazine = GetWorld()->SpawnActor<AMagazine>(MagazineClass,
		Character->GetMesh()->GetSocketLocation(SocketName), Rotation);
	Magazines.Add(TempMagazine);
	if(Magazines.Last())
	{
		if(SocketName == ArmSocketName)
		{
			AttachMagazine(Magazines.Last(), Character->GetMesh(), ArmSocketName);
		}
		else
		{
			AttachMagazine(Magazines.Last(),this->RootComponent, MagazineSocketName);
		}
	}
}

void AKWeapon::AttachMagazine(AMagazine* Magazine, USceneComponent* ParentMesh, FName SocketName)
{
	if (!ParentMesh || !Magazine)
	{
		UE_LOG(LogCustom, Error, TEXT("AKWeapon::AttachMagazine()"));
		return;
	}
	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	Magazine->AttachToComponent(ParentMesh, AttachmentTransformRules, SocketName);
}

void AKWeapon::Take()
{
	UE_LOG(LogCustom, Error, TEXT("AKWeapon::Take()"));
	SpawnMagazine(ArmSocketName);
}

void AKWeapon::Drop()
{
	UE_LOG(LogCustom, Error, TEXT("AKWeapon::Drop()"));
	if(!Cast<ACharacter>(this->GetOwner()) || !GetWorld())
 	{
		UE_LOG(LogCustom,Error,TEXT("AKWeapon::Drop()"));
		return;
	}
	if(Magazines.Num() > 1)
	{
		Magazines[Magazines.Num() - 2]->DetachMagazine();
		MagazineMeshComponent = Magazines.Last()->GetMeshComponent();
	}
	else
	{
		UE_LOG(LogCustom,Error,TEXT("Magazines.Num() = %f"), Magazines.Num());
	}
}

void AKWeapon::Lock()
{
	UE_LOG(LogCustom, Error, TEXT("AKWeapon::Lock()"));
	if(!GetWorld() || !this)
	{
		UE_LOG(LogCustom,Error,TEXT("AKWeapon::Lock()"));
		return;
	}
	if(Magazines.Last() != nullptr)
	{
		AttachMagazine(Magazines.Last(), this->WeaponMesh, MagazineSocketName);
	}
	UE_LOG(LogCustom,Error,TEXT("Magazines.Num() = %f"), Magazines.Num());
}
