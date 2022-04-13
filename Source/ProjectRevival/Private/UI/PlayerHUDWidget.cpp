// Project Revival. All Rights Reserved


#include "UI/PlayerHUDWidget.h"

#include "HealthPBWidget.h"
#include "PlayerCharacter.h"
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

// void UPlayerHUDWidget::OnEnergyValueChanged(float Energy)
// {
// 	EnergyPB->ChangePercent(Energy);
// }

UCirclePBWidget* UPlayerHUDWidget::GetWidgetByAction(EGASInputActions AbilityAction) const
{
	for (auto CircleWidget : CirclePBWidgets)
	{
		if (CircleWidget->GetAbilityAction() == AbilityAction)
		{
			return CircleWidget;
		}
	}
	UE_LOG(LogPRAbilitySystemBase, Error,
                                    TEXT("No such ability named %s"), *UEnum::GetDisplayValueAsText(AbilityAction).ToString());

	return nullptr;
}

UCrosshairWidget* UPlayerHUDWidget::GetCrosshairWidget() const
{
	if (Crosshair)
	{
		return Crosshair;
	}

	return nullptr;
}

void UPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (GetOwningPlayer())
	{
		GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this,&UPlayerHUDWidget::OnNewPawn);
		OnNewPawn(GetOwningPlayerPawn());
	}

	CirclePBWidgets.Add(GhostPB);
	CirclePBWidgets.Add(VisorPB);
	CirclePBWidgets.Add(FlipPB);
	CirclePBWidgets.Add(ChangeWorldPB);
}

void UPlayerHUDWidget::OnHealthChanged(float Health, float DeltaHealth)
{
	if (DeltaHealth<0) OnTakeDamage(Health);
	HealthPB->ChangePercent(GetHealthPercentage());
}


void UPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
	const auto HealthComponent = PRUtils::GetCharacterComponent<UHealthComponent>(NewPawn);
	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddUObject(this, &UPlayerHUDWidget::OnHealthChanged);
	}
	if (NewPawn)
	{
		APlayerCharacter* const Character = Cast<APlayerCharacter>(NewPawn);
		Character->OnEnergyValueChangedHandle.BindUFunction(this, FName("OnEnergyValueChanged"));
	}
	HealthPB->ChangePercent(GetHealthPercentage());
	// EnergyPB->ChangePercent(1);
}
