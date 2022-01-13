// Project Revival. All Rights Reserved


#include "UI/GameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"
#include "ProjectRevival/Public/PRGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameHUD, All, All)

void AGameHUD::DrawHUD()
{
	Super::DrawHUD();

	//DrawCrossHair();
}

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);

	GameWidgets.Add(EMatchState::InProgress, PlayerHUDWidget);
	GameWidgets.Add(EMatchState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass));
	GameWidgets.Add(EMatchState::GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass));

	for (auto WidgetPair : GameWidgets)
	{
		const auto GameWidget = WidgetPair.Value;
		if (!GameWidget) continue;

		GameWidget->AddToViewport();
		GameWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (GetWorld())
	{
		const auto GameMode = Cast<APRGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &AGameHUD::OnMatchStateChanged);
		}
	}
	
}

void AGameHUD::DrawCrossHair()
{
	const TInterval<float> Center(Canvas->SizeX*0.5, Canvas->SizeY*0.5);
	const float HalfLineSize = 10.0f;
	const float LineThickness = 2.0f;
	const FLinearColor LinearColor = FLinearColor::Green;

	DrawLine(Center.Min-HalfLineSize, Center.Max,Center.Min+HalfLineSize,Center.Max, LinearColor, LineThickness);
	DrawLine(Center.Min, Center.Max-HalfLineSize,Center.Min,Center.Max+HalfLineSize, LinearColor, LineThickness);
}

void AGameHUD::OnMatchStateChanged(EMatchState NewMatchState)
{
	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (GameWidgets.Contains(NewMatchState))
	{
		CurrentWidget = GameWidgets[NewMatchState];
	}

	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
	}
	UE_LOG(LogGameHUD, Display,  TEXT("Match state changed: %s"), *UEnum::GetValueAsString(NewMatchState))
}
