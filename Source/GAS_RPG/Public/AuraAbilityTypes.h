#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() {}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WordContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	// 触发几率
	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;

	// 伤害
	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;

	// 持续时间
	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;

	// 触发频率
	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;
	
	// 死亡冲击力
	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	// 死亡冲击方向
	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	// 击退冲击力
	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;

	// 击退几率
	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;

	// 击退力方向
	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;
};


//自定义序列化类
USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
	
public:
	bool IsCriticalHit() const {return bIsCriticalHit;}
	bool IsBlockHit() const {return bIsBlockedHit;}
	bool IsSuccessfulDebuff() const {return bIsSuccessfulDebuff;}
	float GetDebuffDamage() const {return DebuffDamage;}
	float GetDebuffDuration() const {return DebuffDuration;}
	float GetDebuffFrequency() const {return DebuffFrequency;}
	TSharedPtr<FGameplayTag> GetDamageType() const {return DamageType;}
	FVector GetDeathImpulse() const {return DeathImpulse;}
	FVector GetKnockbackForce() const {return KnockbackForce;}

	void SetIsCriticalHit(bool bInIsCriticalHit) {bIsCriticalHit = bInIsCriticalHit;}
	void SetIsBlockedHit(bool bInIsBlockedHit){bIsBlockedHit = bInIsBlockedHit;}
	void SetIsSuccessfulDebuff(bool bInIsDebuff){bIsSuccessfulDebuff = bInIsDebuff;}
	void SetDebuffDamage(float InDamage){DebuffDamage = InDamage;}
	void SetDebuffDuration(float InDuration){DebuffDuration = InDuration;}
	void SetDebuffFrequency(float InFrequency){DebuffFrequency = InFrequency;}
	void SetDamageType(const TSharedPtr<FGameplayTag>& InDamageType){DamageType = InDamageType;}
	void SetDeathImpulse(const FVector& InImpulse){DeathImpulse = InImpulse;}
	void SetKnockbackForce(const FVector& InForce){KnockbackForce = InForce;}
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:

	UPROPERTY()
	bool bIsBlockedHit = false;
	
	UPROPERTY()
	bool bIsCriticalHit = false;

	// 是否为成功的Debuff
	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;
	
	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	// 击退冲击力
	UPROPERTY()
	float KnockbackForceMagnitude = 0.f;

	// 击退力方向
	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;
	
};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : TStructOpsTypeTraitsBase2<FGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true,
	};
};