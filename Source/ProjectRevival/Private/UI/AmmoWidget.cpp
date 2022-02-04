// Project Revival. All Rights Reserved


#include "UI/AmmoWidget.h"

void UAmmoWidget::NativeOnInitialized()
{
	if (AmmoPB)
	{
		AmmoPB->SetPercent(1);
	}
}

void UAmmoWidget::SetAmmoAmount(float NewAmmoAmount)
{
	if (AmmoPB)
	{
		AmmoPB->SetPercent(NewAmmoAmount);
	}
}
