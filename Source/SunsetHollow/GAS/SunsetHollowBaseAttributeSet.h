// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SunsetHollowBaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class SUNSETHOLLOW_API USunsetHollowBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = Attributes)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USunsetHollowBaseAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = Attributes)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(USunsetHollowBaseAttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, Category = Attributes)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USunsetHollowBaseAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = Attributes)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(USunsetHollowBaseAttributeSet, MaxMana);
};
