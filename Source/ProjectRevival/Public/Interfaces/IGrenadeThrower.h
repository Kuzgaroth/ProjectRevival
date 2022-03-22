#pragma once
#include "AbilitySystem/AbilityActors/Grenades/BaseGrenade.h"
#include "ProjectRevival/Public/CoreTypes.h"

#include "IGrenadeThrower.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UIGrenadeThrower : public UInterface
{
	GENERATED_BODY()
};

class IIGrenadeThrower
{
	/* Этот интерфейс определяет, может ли персонаж использовать способность броска гранаты
 * (нужен, чтобы можно было задавать конкретную гранату при броске абилки).
 * Если этот массив пуст, то по дефолту используется HighExplosiveGrenade */
	GENERATED_BODY()
public:
	virtual bool UsesOwnGrenades() {return false;}
	virtual TSubclassOf<ABaseGrenade> GetCurrentGrenade() {return nullptr;}
	virtual bool SwitchGrenade() {return false;}
};