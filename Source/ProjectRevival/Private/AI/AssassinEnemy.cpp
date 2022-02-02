// Project Revival. All Rights Reserved

// ReSharper disable CppMemberFunctionMayBeConst
#include "AI/AssassinEnemy.h"
#include "BaseAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/WidgetComponent.h"

/*AAssassinEnemy::AAssassinEnemy(const FObjectInitializer& ObjectInitializer) :Super(
	ObjectInitializer.SetDefaultSubobjectClass<UAIWeaponComponent>("WeaponComponent"))
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = AAIController::StaticClass();
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
}*/

void AAssassinEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AICon = Cast<ASoldierAIController>(GetController());
	if (AICon)
	{
		BBComp = AICon->GetBlackboardComponent();}
	//UpdateHStateBlackboardKey(0);
}

void AAssassinEnemy::BeginPlay()
{
	Super::BeginPlay();
	//check(HealthWidgetComponent);
}

bool AAssassinEnemy::IsAttacking()
{
	return bIsAttacking;
}