// Project Revival. All Rights Reserved

#include "AI/SoldierEnemy.h"
#include "BaseAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIWeaponComponent.h"
#include "BrainComponent.h"
#include "HealthBarWidget.h"
#include "HealthComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

ASoldierEnemy::ASoldierEnemy(const FObjectInitializer& ObjectInitializer) :Super(
	ObjectInitializer.SetDefaultSubobjectClass<UAIWeaponComponent>("WeaponComponent"))
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = ABaseAIController::StaticClass();
	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	}

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	HealthWidgetComponent->SetupAttachment(RootComponent);
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->SetDrawAtDesiredSize(true);

	HState = HealthStateSoldier::FULL_HEALTH;
}

void ASoldierEnemy::OnDeath()
{
	Super::OnDeath();

	const auto PRController = Cast<ABaseAIController>(GetController());
	if (PRController && PRController->BrainComponent)
	{
		PRController->BrainComponent->Cleanup();
	}
}

void ASoldierEnemy::Tick(float DeltaSeconds)
{
	UpdateHealthWidgetVisibility();
}

void ASoldierEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AICon = Cast<ABaseAIController>(GetController());
	UE_LOG(LogTemp, Log, TEXT("After AICon cast"));
	if (AICon)
	{
		BBComp = AICon->GetBlackboardComponent();
		UE_LOG(LogTemp, Log, TEXT("Check if cast was successful"));
	}
	UpdateHStateBlackboardKey(3);
	UE_LOG(LogTemp, Log, TEXT("SPECIAL"));
}

void ASoldierEnemy::BeginPlay()
{
	Super::BeginPlay();
	check(HealthWidgetComponent);
}

void ASoldierEnemy::StartCovering()
{
	bWantsToCover=true;
}

void ASoldierEnemy::StopCovering()
{
	bWantsToCover=false;
}

bool ASoldierEnemy::IsCovering() const
{
	return bWantsToCover && IsMovingToCover && !GetVelocity().IsZero();
}

void ASoldierEnemy::OnHealthChanged(float CurrentHealth, float HealthDelta)
{
	Super::OnHealthChanged(CurrentHealth, HealthDelta);

	if (HealthComponent->GetHealthPercentage() > 0.75)
	{
		HState = HealthStateSoldier::FULL_HEALTH;
		UE_LOG(LogTemp, Log, TEXT("HState has changed %i"), static_cast<uint8>(HState));
		UpdateHStateBlackboardKey(3);
	}
	else if ((HealthComponent->GetHealthPercentage() <= 0.75) && (HealthComponent->GetHealthPercentage() > 0.5))
	{
		HState = HealthStateSoldier::SLIGHTLY_INJURED;
		UE_LOG(LogTemp, Log, TEXT("HState has changed %i"), static_cast<uint8>(HState));
		UpdateHStateBlackboardKey(2);
	}
	else if ((HealthComponent->GetHealthPercentage() <= 0.5) && (HealthComponent->GetHealthPercentage() > 0.25))
	{
		HState = HealthStateSoldier::BADLY_INJURED;
		UE_LOG(LogTemp, Log, TEXT("HState has changed %i"), static_cast<uint8>(HState));
		UpdateHStateBlackboardKey(1);
	}
	else
	{
		HState = HealthStateSoldier::NEAR_DEATH;
		UE_LOG(LogTemp, Log, TEXT("HState has changed %i"), static_cast<uint8>(HState));
		UpdateHStateBlackboardKey(0);
	}
	
	const auto HealthBarWidget = Cast<UHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());

	if (!HealthBarWidget) return;

	HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercentage());
}

void ASoldierEnemy::UpdateHStateBlackboardKey(uint8 EnumKey)
{
	if(BBComp)
	{
		BBComp->SetValueAsEnum("HState", EnumKey);
		UE_LOG(LogTemp, Log, TEXT("BBComp was updated %i"), EnumKey);
	}
}

void ASoldierEnemy::UpdateHealthWidgetVisibility()
{
	const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
	const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());

	HealthWidgetComponent->SetVisibility(Distance<HealthVisibilityDistance, true);
}
