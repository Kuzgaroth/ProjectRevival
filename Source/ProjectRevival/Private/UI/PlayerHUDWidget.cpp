// Project Revival. All Rights Reserved


#include "UI/PlayerHUDWidget.h"
#include "Components/HealthComponent.h"
#include "Components/WeaponComponent.h"
#include "ProjectRevival/Public/Miscellaneous/PRUtils.h"

float UPlayerHUDWidget::GetHealthPercentage() const
{
	const auto HealthComponent = PRUtils::GetCharacterComponent<UHealthComponent>(GetOwningPlayerPawn());
	if (!HealthComponent) return 0.0f;

	return HealthComponent->GetHealthPercentage();
}

bool UPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	const auto WeaponComponent = PRUtils::GetCharacterComponent<UWeaponComponent>(GetOwningPlayerPawn());
	if (!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool UPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	const auto WeaponComponent = PRUtils::GetCharacterComponent<UWeaponComponent>(GetOwningPlayerPawn());
	if (!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool UPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
}

bool UPlayerHUDWidget::IsPlayerAlive() const
{
	const auto HealthComponent = PRUtils::GetCharacterComponent<UHealthComponent>(GetOwningPlayerPawn());
	return HealthComponent && !HealthComponent->IsDead();
}

void UPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (GetOwningPlayer())
	{
		GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this,&UPlayerHUDWidget::OnNewPawn);
		OnNewPawn(GetOwningPlayerPawn());
	}
}

void UPlayerHUDWidget::OnHealthChanged(float Health, float DeltaHealth)
{
	if (DeltaHealth<0) OnTakeDamage(Health);
}

void UPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
	const auto HealthComponent = PRUtils::GetCharacterComponent<UHealthComponent>(NewPawn);
	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddUObject(this, &UPlayerHUDWidget::OnHealthChanged);
	}
}
