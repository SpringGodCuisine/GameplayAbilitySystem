// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	//初始化原始值
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// 强制转换 AttributeSet 为 UAuraAttributeSet 类型，并确保转换成功
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	// 获取 Health 属性的变化委托，并将 HealthChanged 回调函数绑定到该委托
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributeSet->GetHealthAttribute())
	.AddUObject(this, &UOverlayWidgetController::HealthChanged);

	// 获取 MaxHealth 属性的变化委托，并将 MaxHealthChanged 回调函数绑定到该委托
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributeSet->GetMaxHealthAttribute())
	.AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

	// 获取 Mana 属性的变化委托，并将 MaxHealthChanged 回调函数绑定到该委托
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributeSet->GetManaAttribute())
	.AddUObject(this, &UOverlayWidgetController::ManaChanged);

	// 获取 MaxMana 属性的变化委托，并将 MaxHealthChanged 回调函数绑定到该委托
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	AuraAttributeSet->GetMaxManaAttribute())
	.AddUObject(this, &UOverlayWidgetController::MaxManaChanged);

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				//TODO:: Broadcast the tag to the Widget Controller;
				const FString Msg = FString::Printf(TEXT("GE TAG: %s"), *Tag.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);

				FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
			}
		}	
	);

}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}
