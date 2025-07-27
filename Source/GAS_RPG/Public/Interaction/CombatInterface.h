// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

UINTERFACE(MinimalAPI)
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
};
