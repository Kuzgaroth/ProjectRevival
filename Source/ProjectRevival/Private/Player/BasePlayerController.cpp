// Project Revival. All Rights Reserved


#include "Player/BasePlayerController.h"

#include "CourtLairGameMode.h"
#include "RespawnComponent.h"
#include "PRGameModeBase.h"

ABasePlayerController::ABasePlayerController()
{
	RespawnComponent = CreateDefaultSubobject<URespawnComponent>("RespawnController");
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!InputComponent) return;

	InputComponent->BindAction("Pause", IE_Pressed, this, &ABasePlayerController::OnPauseGame);
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		const auto GameMode = Cast<APRGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ABasePlayerController::OnMatchStateChanged);
		}
		else
		{
			const auto CourtGameMode = Cast<ACourtLairGameMode>(GetWorld()->GetAuthGameMode());
			if (CourtGameMode)
			{
				CourtGameMode->OnMatchStateChanged.AddUObject(this, &ABasePlayerController::OnMatchStateChanged);
			}
		}
	}
}

void ABasePlayerController::OnPauseGame()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	GetWorld()->GetAuthGameMode()->SetPause(this);
}

void ABasePlayerController::OnMatchStateChanged(EMatchState NewMatchState)
{
	if (NewMatchState == EMatchState::InProgress)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}
