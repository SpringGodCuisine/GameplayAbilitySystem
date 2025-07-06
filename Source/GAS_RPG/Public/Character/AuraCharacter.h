// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GAS_RPG_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
public:
	AAuraCharacter();
	//当一个Pawn被Controller占用时，PossessedBy函数会被调用。它通常用于设置和初始化Pawn的一些属性和状态
	virtual void PossessedBy(AController* NewController) override;
	//用于网络复制的一个回调函数，这个函数会在PlayerState变量被更新并同步到客户端时自动调用。
	virtual void OnRep_PlayerState() override;
private:
	virtual void InitAbilityActorInfo() override;
};
