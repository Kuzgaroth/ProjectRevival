// Project Revival. All Rights Reserved


#include "UI/HealthPBWidget.h"

void UHealthPBWidget::ChangePercent(const float NewPercent)
{
	if (NewPercent == 1 && Percent != 1)
	{
		OnHealthFull();
	}
	else if (Percent == 1 && NewPercent != 1)
	{
		ProgressImage->SetOpacity(1);
		ProgressFull->SetPercent(0);
	}
	Percent = NewPercent;
	
	DMI = UMaterialInstanceDynamic::Create(ImageSegmented, this);
	DMI->SetScalarParameterValue("Percent", Percent);
	ProgressImage->SetBrushFromMaterial(DMI);
}

void UHealthPBWidget::NativeOnInitialized()
{
	DMI = UMaterialInstanceDynamic::Create(ImageSegmented, this);

	Percent = 1;
	DMI->SetScalarParameterValue("Percent", Percent);
	ProgressImage->SetBrushFromMaterial(DMI);
}
