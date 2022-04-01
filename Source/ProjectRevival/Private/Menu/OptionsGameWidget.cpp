// Project Revival. All Rights Reserved


#include "Menu/OptionsGameWidget.h"

#include "Components/Button.h"

void UOptionsGameWidget::NativeOnInitialized()
{
	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddDynamic(this, &UOptionsGameWidget::ApplyChanges);	
	}
	if (ApplyDefaultButton)
	{
		ApplyDefaultButton->OnClicked.AddDynamic(this, &UOptionsGameWidget::ApplyDefaultChanges);
	}

	SetView();
}

bool UOptionsGameWidget::HasUnsavedChanges()
{
	// Write your check code here
	
	return false;
}

void UOptionsGameWidget::ApplyChanges()
{
	// Apply your changes here
}

void UOptionsGameWidget::SetView()
{
	// Set the check box value here
}

void UOptionsGameWidget::ApplyDefaultChanges()
{
	// Apply default changes here
	SetView();
}
