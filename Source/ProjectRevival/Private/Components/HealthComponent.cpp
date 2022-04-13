// Project Revival. All Rights Reserved


#include "Components/HealthComponent.h"

#include "PlayerCharacter.h"
#include "Camera/CameraShakeBase.h"
#include "PRGameModeBase.h"


UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

bool UHealthComponent::TryToAddHealth(float HealthAmount)
{
	if (IsDead() || IsHealthFull()) return false;
	SetHealth(Health+HealthAmount);
	return true;
}

bool UHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogPRSaveSystem, Display, TEXT("----Loading player health----"))
	const auto SaveLoader = Cast<IISaveLoader>(GetWorld()->GetAuthGameMode());
	if (SaveLoader)
	{
		const auto SaveGame = SaveLoader->GetSaveFromLoader();
		if (SaveGame)
		{
			SetHealth(SaveGame->PlayerSaveData.HP);
		}
		else SetHealth(MaxHealth);
	}
	;
	
	const auto Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this,&UHealthComponent::OnTakeAnyDamageHandle);
	}
}

void UHealthComponent::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage<=0.0f || IsDead()) return;
	SetHealth(Health-Damage);
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	if (IsDead())
	{
		Killed(InstigatedBy);
		OnDeath.Broadcast();
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}
	PlayCameraShake();
}

void UHealthComponent::HealUpdate()
{
	SetHealth(Health+HealModifier);
	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void UHealthComponent::SetHealth(float NewHealth)
{
	const auto NextHealth=FMath::Clamp<float>(NewHealth, 0.0f, MaxHealth);
	const auto HealthDelta = NextHealth- Health;
	Health = NextHealth;
	if (OnHealthChanged.IsBound())
		OnHealthChanged.Broadcast(Health, HealthDelta);
}

void UHealthComponent::PlayerFinallyDied()
{
	const auto GameMode = GetWorld()->GetAuthGameMode<APRGameModeBase>();
	if (!GameMode) return;
	GameMode->GameOver();
}

void UHealthComponent::PlayCameraShake()
{
	if (IsDead()) return;

	const auto Player = Cast<APawn>(GetOwner());
	if (!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void UHealthComponent::Killed(AController* KillerController)
{
	if (!GetWorld()) return;
	
	const auto GameMode = GetWorld()->GetAuthGameMode<APRGameModeBase>();
	if (!GameMode) return;

	const auto Player = Cast<APawn>(GetOwner());
	const auto VictimController = Player ? Player->Controller : nullptr;
}

