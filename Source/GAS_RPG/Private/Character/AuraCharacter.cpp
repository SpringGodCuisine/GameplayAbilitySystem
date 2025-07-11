// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	// 设置角色的移动方向与其朝向一致
	GetCharacterMovement()->bOrientRotationToMovement = true;
    
	// 设置角色旋转速率，只有 Yaw 轴（水平面上的旋转）为 400 度/秒
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
    
	// 启用角色平面运动约束
	GetCharacterMovement()->bConstrainToPlane = true;
    
	// 在游戏开始时将角色对齐到平面
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 禁用控制器控制角色的俯仰角度（Pitch）
	bUseControllerRotationPitch = false;
	// 禁用控制器控制角色的滚转角度（Roll）
	bUseControllerRotationRoll = false;
	// 禁用控制器控制角色的偏航角度（Yaw）
	bUseControllerRotationYaw = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//Init ability actor info for the Server
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//Init ability actor info for the Client
	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	// 获取当前玩家的状态，并强制转换为 AAuraPlayerState 类型
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	// 检查 AuraPlayerState 是否有效（非空），如果无效则会导致程序崩溃
	check(AuraPlayerState);
	// 初始化 Ability System Component，将其与 AuraPlayerState 和当前对象（this）关联
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);

	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet() ;
	// 将 Ability System Component 指向 AuraPlayerState 的 Ability System Component
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	// 将 Attribute Set 指向 AuraPlayerState 的 Attribute Set
	AttributeSet = AuraPlayerState->GetAttributeSet();

	//客户端中只有自己的Controller才会Vaild
	if(AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		// 只有在客户端中，并且当前控制器为玩家自己的控制器时才会有效
		// 获取当前控制器的 HUD 并强制转换为 AAuraHUD 类型
		if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			// 初始化 HUD 的 Overlay，将其与当前控制器、玩家状态、Ability System Component 和 Attribute Set 关联
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

