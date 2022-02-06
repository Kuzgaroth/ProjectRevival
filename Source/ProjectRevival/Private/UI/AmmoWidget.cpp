// Project Revival. All Rights Reserved


#include "UI/AmmoWidget.h"

void UAmmoWidget::NativeOnInitialized()
{
	if (AmmoPB)
	{
		AmmoPB->SetPercent(1);
	}
}

void UAmmoWidget::SetAmmoAmount(int NewAmmoAmount)
{
	if (NewAmmoAmount >= 4)
	{
		AmmoPB->SetPercent(1);
	}
	else if(NewAmmoAmount == 3)
	{
		AmmoPB->SetPercent(0.75);
	}
	else if(NewAmmoAmount == 2)
	{
		AmmoPB->SetPercent(0.5);
	}
	else if(NewAmmoAmount == 1)
	{
		AmmoPB->SetPercent(0.25);
	}
	else
	{
		AmmoPB->SetPercent(0);
	}
	// if (AmmoPB)
	// {
	// 	AmmoPB->SetPercent(NewAmmoAmount);
	// }
}
