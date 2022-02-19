// Project Revival. All Rights Reserved

#define COVER_TRACE_CHANNEL ECC_GameTraceChannel3

#include "Player/BaseCharacter.h"

#include "DrawDebugHelpers.h"
#include "Components/BaseCharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/HealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"


DEFINE_LOG_CATEGORY(LogPRAbilitySystemBase);

// Sets default values
ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer) :Super(
	ObjectInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
	
	AbilitySystemComponent = CreateDefaultSubobject<UPRAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UPRAttributeSet>(TEXT("AttributeSet"));
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AddStartupGameplayAbilities();
	}
}

void ABaseCharacter::UnPossessed()
{
	
	Super::UnPossessed();
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(HealthComponent);
	
	HealthComponent->OnDeath.AddUObject(this, &ABaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ABaseCharacter::OnHealthChanged);
	OnHealthChanged(HealthComponent->GetHealth(), 1.0f);
	LandedDelegate.AddDynamic(this,&ABaseCharacter::OnGroundLanded);

	SetupDynMaterialsFromMesh(this, DynamicMaterials);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

bool ABaseCharacter::IsRunning() const
{
	return false;
}

float ABaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.0f;
	const FVector VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween =FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	return Degrees*FMath::Sign(CrossProduct.Z);
}

UWeaponComponent* ABaseCharacter::GetWeaponComponent() const
{
	if (WeaponComponent) return WeaponComponent;
	else return nullptr;
}

// Called to bind functionality to input


void ABaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
	/*const auto MateralInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	if (!MateralInst) return;

	MateralInst->SetVectorParameterValue(MaterialColorName, Color);*/
}

TArray<UMaterialInstanceDynamic*> ABaseCharacter::GetDynMaterials()
{
	TArray<UMaterialInstanceDynamic*> FullMaterialArray;
	FullMaterialArray.Append(WeaponComponent->GetCurrentWeaponMaterials());
	FullMaterialArray.Append(DynamicMaterials);
	return FullMaterialArray;
}

bool ABaseCharacter::StartCover(AActor* InstigatorObj)
{
	if (!Cast<AController>(InstigatorObj)) return false;
	FHitResult CoverHit;
	const ECoverType CoverType = CoverTrace(CoverHit);
	if (CoverType==ECoverType::None) return false;;
	return StartCover_Internal(CoverHit);
}

bool ABaseCharacter::StopCover(AActor* InstigatorObj)
{
	if (!Cast<AController>(InstigatorObj)) return false;;
	return StopCover_Internal();
}

void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetEnergyAttribute()).Clear();
	AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().Clear();
	Super::EndPlay(EndPlayReason);
}

void ABaseCharacter::AddStartupGameplayAbilities()
{
	for (const auto& Ability : GameplayAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability.Value, 1, static_cast<int32>(Ability.Key), this));
		UE_LOG(LogPRAbilitySystemBase, Display, TEXT("%s has granted"), *Ability.Value.GetDefaultObject()->GetName());
	}
}

void ABaseCharacter::OnEnergyAttributeChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogPRAbilitySystemBase, Display, TEXT("New value of %s is %f"), *Data.Attribute.GetName(), Data.NewValue);
}

void ABaseCharacter::OnCooldownExpired(const FActiveGameplayEffect& ExpiredEffect)
{
	UE_LOG(LogPRAbilitySystemBase, Display, TEXT("Cooldown effect %s is expired"), *ExpiredEffect.Handle.ToString());
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetEnergyAttribute())
		.AddUObject(this, &ABaseCharacter::OnEnergyAttributeChanged);
	AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ABaseCharacter::OnCooldownExpired);
}

void ABaseCharacter::OnDeath()
{
	//PlayAnimMontage(DeathAnimMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponComponent->StopFire();

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetSimulatePhysics(true);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
}

void ABaseCharacter::OnHealthChanged(float CurrentHealth, float HealthDelta)
{
	//const auto Health = HealthComponent->GetHealth();
	
}

bool ABaseCharacter::StartCover_Internal(FHitResult& CoverHit)
{
	
	GetCharacterMovement()->SetPlaneConstraintEnabled(true);
	GetCharacterMovement()->SetPlaneConstraintNormal(CoverHit.Normal);
	return true;
}

bool ABaseCharacter::StopCover_Internal()
{
	GetCharacterMovement()->SetPlaneConstraintEnabled(false);
	return true;
}

ECoverType ABaseCharacter::CoverTrace(FHitResult& CoverHit)
{
	const FVector Location = GetActorLocation();
	FVector HighLocation;
	HighLocation.Set(Location.X, Location.Y, 200.0f);
	const bool HighTraceResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), HighLocation, GetActorForwardVector()*100.0f+HighLocation,
		UEngineTypes::ConvertToTraceType(COVER_TRACE_CHANNEL),false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, CoverHit, true);
	//DrawDebugLine(GetWorld(), HighLocation, GetActorRotation().Vector()*25.0f, FColor::Red, false, 2,0, 2);
	if (HighTraceResult) return ECoverType::High;

	const bool LowTraceResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Location, GetActorForwardVector()*100.0f+Location,
		UEngineTypes::ConvertToTraceType(COVER_TRACE_CHANNEL),false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, CoverHit, true);
	//DrawDebugLine(GetWorld(), Location, GetActorRotation().Vector()*25.0f, FColor::Red, false, 2,0, 2);
	if (LowTraceResult) return ECoverType::Low;
	return ECoverType::None;
}

void ABaseCharacter::OnGroundLanded(const FHitResult& HitResult)
{
	const auto FallVelocity = -GetCharacterMovement()->Velocity.Z;
	if (FallVelocity<LandedDamageVelocity.X) return;
	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity,LandedDamage, FallVelocity);
	TakeDamage(FinalDamage,FDamageEvent{}, nullptr, nullptr);
}
