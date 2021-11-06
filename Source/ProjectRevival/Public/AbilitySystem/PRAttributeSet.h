// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PRAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class PROJECTREVIVAL_API UPRAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UPRAttributeSet();
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category = "Energy", EditAnywhere)
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, Energy)
	
	UPROPERTY(BlueprintReadOnly, Category = "Energy", EditAnywhere)
	FGameplayAttributeData MaxEnergy = 100.f;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, MaxEnergy)

	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed", EditAnywhere)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UPRAttributeSet, MoveSpeed)
protected:
	
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
		const FGameplayAttributeData& MaxAttribute, float NewMaxValue,
		const FGameplayAttribute& AffectedAttributeProperty);

};
