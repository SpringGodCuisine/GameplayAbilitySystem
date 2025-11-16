// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 *
 * Singleton containing native Gameplay Tags
 * 单例类
 */

struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get(){return GameplayTags;}
	//静态函数 初始化原生玩法标签
	static void InitializeNativeGameplayTags();

	//属性标签
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_HealthRegeneration;
	FGameplayTag Attributes_ManaRegeneration;

	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	//经验
	FGameplayTag Attributes_Meta_IncomingXP;

	//输入标签
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	FGameplayTag Damage;

	//伤害类型
	//火焰伤害类型
	FGameplayTag Damage_Fire;
	//闪电伤害类型
	FGameplayTag Damage_Lightning;
	//奥术伤害类型
	FGameplayTag Damage_Arcane;
	//物理伤害类型
	FGameplayTag Damage_Physical;

	//抗性
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;

	//Debuff
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Arcane;
	FGameplayTag Debuff_Physical;

	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Frequency;

	FGameplayTag Abilities_None;

	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;

	FGameplayTag Abilities_HitReact;

	//锁定的
	FGameplayTag Abilities_Status_Locked;
	//合格，符合条件的，可解锁
	FGameplayTag Abilities_Status_Eligible;
	//已解锁
	FGameplayTag Abilities_Status_Unlocked;
	//装备中
	FGameplayTag Abilities_Status_Equipped;

	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;
	
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Lightning_Electrocute;
	
	FGameplayTag Cooldown_Fire_FireBolt;

	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	
	//伤害类型和抗性映射关系
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	//伤害类型对应Debuff标签
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;
	
	FGameplayTag Effects_HitReact;

	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_CursorTrace;


private:
	static FAuraGameplayTags GameplayTags;
};