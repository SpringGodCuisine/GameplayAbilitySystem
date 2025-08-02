// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAnimMontage;

UINTERFACE(MinimalAPI, Blueprintable)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 战斗接口
 */
class GAS_RPG_API ICombatInterface
{
	GENERATED_BODY()

public:
	//获取玩家等级
	virtual int32 GetPlayerLevel();
	virtual FVector GetCombatSocketLocation();

	//在蓝图中可以直接实现
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);

	//BlueprintNativeEvent 是 Unreal Engine 中 UFUNCTION 宏的一种修饰符，用于让函数既可以：
	//可以在 C++ 中写默认实现，
	//又可以在蓝图中重写覆盖该实现。
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	virtual void Die() = 0;
};
