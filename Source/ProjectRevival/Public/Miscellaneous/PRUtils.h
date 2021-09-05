#pragma once
#include "Player/PRPlayerState.h"

class PRUtils
{
public:
	template <typename TComponent>
		static TComponent* GetCharacterComponent(AActor* PlayerPawn)
	{
		if (!PlayerPawn) return nullptr;

		const auto Component = PlayerPawn->GetComponentByClass(TComponent::StaticClass());
		const auto CharacterComponent = Cast<TComponent>(Component);
		return CharacterComponent;
	}

	bool static AreEnemies(AController* Controller1, AController* Controller2)
	{
		if (!Controller1 || !Controller2 || Controller1== Controller2) return false;

		const auto PlayerState1 = Controller1->GetPlayerState<APRPlayerState>();
		const auto PlayerState2 = Controller2->GetPlayerState<APRPlayerState>();
		
		return PlayerState1 && PlayerState2 && PlayerState1->GetTeamId() != PlayerState2->GetTeamId();
	}

	static FText TextFromInt(int32 Number)
	{
		return FText::FromString(FString::FromInt(Number));
	}
};
