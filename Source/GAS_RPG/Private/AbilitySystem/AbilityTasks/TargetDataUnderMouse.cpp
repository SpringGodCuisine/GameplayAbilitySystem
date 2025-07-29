// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	//如果是本地控制
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredicate = GetActivationPredictionKey();
		
		//Server, Listen for the target Data
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle,
			ActivationPredicate).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		//如果太晚，并且目标数据已经被发送了
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle,
			ActivationPredicate);
		//数据尚未送达服务器
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
	

}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	//下面函数内的一切都应该被预测
	//请允许我们在客户端进行该操作
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;

	FGameplayAbilityTargetDataHandle DataHandle;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	FGameplayTag ApplicationTag;
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle, FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	//复制只发生在从服务端到客户端
	//但在GAS中，你会看到很多函数以两种方式使用这个术语。
	//通过RPCs从客户端发送到服务器的复制数据，有时GAS系统将其称为复制数据
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
