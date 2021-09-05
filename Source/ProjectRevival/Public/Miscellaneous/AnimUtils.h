// Project Revival. All Rights Reserved

#pragma once

class AnimUtils
{
public:
	template <typename TNotify>
	static TNotify* FindNotifyByClass(UAnimSequenceBase* Animation)
	{
		if (!Animation) return nullptr;
		const auto Notifies = Animation->Notifies;
		for (auto NotifyEvent : Notifies)
		{
			auto AnimNotify = Cast<TNotify>(NotifyEvent.Notify);
			if (AnimNotify)
			{
				return AnimNotify;
			}
		
		}
		return nullptr;
	}
	
};
