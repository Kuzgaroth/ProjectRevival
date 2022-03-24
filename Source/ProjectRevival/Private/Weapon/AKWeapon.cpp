// Project Revival. All Rights Reserved

#include "AKWeapon.h"
#include "GameFramework/Character.h"

AKWeapon::AKWeapon()
{
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MagazineMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MagazineMeshComponent");
	MagazineClass = AMagazine::StaticClass();
}

void AKWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);

	if(this)
	{    
		const FRotator Rotation = this->GetActorRotation();
		const FVector Location = this->WeaponMesh->GetSocketLocation(MagazineSocketName);
		Magazine = GetWorld()->SpawnActor<AMagazine>(MagazineClass, Location, Rotation);
		if(Magazine)
		{
			MagazineMeshComponent = Magazine->GetMeshComponent();
			MagazineMeshComponent->SetupAttachment(RootComponent);
			check(MagazineMeshComponent);
		}
	}
	else
	{
		UE_LOG(LogCustom,Error,TEXT("AKWeapon::BeginPlay()"));
	}
}

void AKWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKWeapon::MakeShot()
{
	Super::MakeShot();
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
	const FRotator Rotation = this->GetActorRotation();
	const FVector Location = Character->GetMesh()->GetSocketLocation(SocketName);
             
	Magazine = GetWorld()->SpawnActor<AMagazine>(MagazineClass, Location, Rotation);
	if(Magazine)
	{
		if(SocketName == ArmSocketName)
		{
			AttachMagazine(Character->GetMesh(), ArmSocketName);
		}
		else
		{
			AttachMagazine(this->WeaponMesh, MagazineSocketName);
		}
	}
}

void AKWeapon::AttachMagazine(USceneComponent* ParentMesh, FName SocketName)
{
	if (!ParentMesh || !Magazine)
	{
		UE_LOG(LogCustom,Error,TEXT("AKWeapon::AttachMagazine()"));
		return;
	}
	//Magazine->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	Magazine->AttachToComponent(ParentMesh, AttachmentTransformRules, SocketName);
}

void AKWeapon::Remove()
{
	const ACharacter* Character = Cast<ACharacter>(this->GetOwner());
	if(!GetWorld() || !Character)
	{
		UE_LOG(LogCustom,Error,TEXT("AKWeapon::Remove()"));
		return;
	}
	AttachMagazine(Character->GetMesh(), ArmSocketName);
}

void AKWeapon::Drop()
{
	Magazine->GetMeshComponent()->SetVisibility(false);
	
	if(!Cast<ACharacter>(this->GetOwner()) || !GetWorld())
 	{
		UE_LOG(LogCustom,Error,TEXT("AKWeapon::Drop()"));
		return;
	}
	const ACharacter* Character = Cast<ACharacter>(this->GetOwner());
	const FRotator Rotation = Magazine->GetMeshComponent()->GetComponentRotation();
	const FVector Location = Character->GetMesh()->GetSocketLocation(ArmSocketName);
	AMagazine* TempMagazine = GetWorld()->SpawnActor<AMagazine>(MagazineClass, Location, Rotation);
	TempMagazine->DetachMagazine();
}

void AKWeapon::Take()
{
	Magazine->GetMeshComponent()->SetVisibility(true);
}

void AKWeapon::Lock()
{
	if(!GetWorld() || !this)
	{
		UE_LOG(LogCustom,Error,TEXT("AKWeapon::Lock()"));
		return;
	}
	AttachMagazine(this->WeaponMesh, MagazineSocketName);
	Magazine->GetMeshComponent()->SetVisibility(true);
}
