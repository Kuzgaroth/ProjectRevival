// Project Revival. All Rights Reserved


#include "UI/CirclePBWidget.h"

#include "Math/UnitConversion.h"

void UCirclePBWidget::NativeOnInitialized()
{
	DMI = UMaterialInstanceDynamic::Create(ExternalCircleMaterial, this);

	Percent = 100;
	bEnable = true;
	DMI->SetScalarParameterValue("Percent", Percent);
	ExternalCircle->SetBrushFromMaterial(DMI);
	InnerCircle->SetBrushFromTexture(InnerCircleFull);
	Icon->SetBrushFromTexture(IconImage);
}

void UCirclePBWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Percent = FMath::FInterpConstantTo(Percent, 100, InDeltaTime, 1/CooldownTime);
	DMI->SetScalarParameterValue("Percent", Percent);
	ExternalCircle->SetBrushFromMaterial(DMI);
}

void UCirclePBWidget::StartCooldown()
{
	InnerCircle->SetBrushFromTexture(InnerCircleEmpty);
	Icon->SetOpacity(0.5);
	Percent = 0;
	bEnable = false;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCirclePBWidget::EndCooldown, CooldownTime, false);
}

void UCirclePBWidget::EndCooldown()
{
	InnerCircle->SetBrushFromTexture(InnerCircleFull);
	Icon->SetOpacity(1);
	bEnable = true;
	OnEndCooldown();
}
