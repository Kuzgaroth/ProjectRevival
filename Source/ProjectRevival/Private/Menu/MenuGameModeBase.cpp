// Project Revival. All Rights Reserved


#include "Menu/MenuGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "Menu/MenuPlayerController.h"
#include "Menu/MenuHUD.h"
AMenuGameModeBase::AMenuGameModeBase()
{
	HUDClass = AMenuHUD::StaticClass();
	PlayerControllerClass = AMenuPlayerController::StaticClass();
}

void AMenuGameModeBase::ClearSaveData()
{
	UGameplayStatics::DeleteGameInSlot("SaveSlot",0);
}
