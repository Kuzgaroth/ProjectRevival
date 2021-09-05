// Project Revival. All Rights Reserved


#include "Menu/MenuGameModeBase.h"
#include "Menu/MenuPlayerController.h"
#include "Menu/MenuHUD.h"
AMenuGameModeBase::AMenuGameModeBase()
{
	HUDClass = AMenuHUD::StaticClass();
	PlayerControllerClass = AMenuPlayerController::StaticClass();
}
