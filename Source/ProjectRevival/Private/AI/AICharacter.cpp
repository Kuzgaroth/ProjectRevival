// Project Revival. All Rights Reserved


#include "AI/AICharacter.h"
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

AAICharacter::AAICharacter(const FObjectInitializer& ObjectInitializer) :Super(
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

	HState = HealthState::FULL_HEALTH;
}



void AAICharacter::OnDeath()
{
	Super::OnDeath();

	const auto PRController = Cast<ABaseAIController>(GetController());
	if (PRController && PRController->BrainComponent)
	{
		PRController->BrainComponent->Cleanup();
	}
	
}

void AAICharacter::Tick(float DeltaSeconds)
{
	UpdateHealthWidgetVisibility();
}

void AAICharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AICon = Cast<ABaseAIController>(GetController());
	UE_LOG(LogTemp, Log, TEXT("After AICon cast"));
	if (AICon)
	{
		BBComp = AICon->GetBlackboardComponent();
		UE_LOG(LogTemp, Log, TEXT("Check if cast was successful"));
	}
	
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	check(HealthWidgetComponent);
}

void AAICharacter::OnHealthChanged(float CurrentHealth, float HealthDelta)
{
	Super::OnHealthChanged(CurrentHealth, HealthDelta);

	if (HealthComponent->GetHealthPercentage() > 0.75)
	{
		HState = HealthState::FULL_HEALTH;
		UE_LOG(LogTemp, Log, TEXT("HState has changed %i"), static_cast<uint8>(HState));
		UpdateHStateBlackboardKey(3);
	}
	else if ((HealthComponent->GetHealthPercentage() <= 0.75) && (HealthComponent->GetHealthPercentage() > 0.5))
	{
		HState = HealthState::SLIGHTLY_INJURED;
		UE_LOG(LogTemp, Log, TEXT("HState has changed %i"), static_cast<uint8>(HState));
		UpdateHStateBlackboardKey(2);
	}
	else if ((HealthComponent->GetHealthPercentage() <= 0.5) && (HealthComponent->GetHealthPercentage() > 0.25))
	{
		HState = HealthState::BADLY_INJURED;
		UE_LOG(LogTemp, Log, TEXT("HState has changed %i"), static_cast<uint8>(HState));
		UpdateHStateBlackboardKey(1);
	}
	else
	{
		HState = HealthState::NEAR_DEATH;
		UE_LOG(LogTemp, Log, TEXT("HState has changed %i"), static_cast<uint8>(HState));
		UpdateHStateBlackboardKey(0);
	}
	
	const auto HealthBarWidget = Cast<UHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());

	if (!HealthBarWidget) return;

	HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercentage());
}

void AAICharacter::UpdateHStateBlackboardKey(uint8 EnumKey)
{
	if(BBComp)
	{
		BBComp->SetValueAsEnum("HState", EnumKey);
		UE_LOG(LogTemp, Log, TEXT("BBComp was updated %i"), EnumKey);
	}
}


void AAICharacter::UpdateHealthWidgetVisibility()
{
	const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
	const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());

	HealthWidgetComponent->SetVisibility(Distance<HealthVisibilityDistance, true);
}
