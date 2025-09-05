// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class GAS_RPG_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	UFUNCTION(BlueprintPure, Category = "Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();
	
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	int32 NumMinions = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MinSpawnDistance = 75.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MaxSpawnDistance = 300.f;

	//生成角度
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnSpread = 90.f;
	
};
