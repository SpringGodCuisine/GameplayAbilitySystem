
#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	// 创建 Ability System Component（能力系统组件），并将其命名为 "AbilitySystemComponent"
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	// 设置 Ability System Component 为可被网络复制
	AbilitySystemComponent->SetIsReplicated(true);
	// 设置 Ability System Component 的复制模式为混合模式（Mixed）
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// 创建 Attribute Set（属性集），并将其命名为 "AttributeSet"
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	// 设置网络更新频率为 100 次每秒
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
