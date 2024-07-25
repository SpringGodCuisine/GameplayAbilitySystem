// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// 检查 AuraContext 是否有效
	check(AuraContext);

	// 获取本地玩家的增强输入子系统
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	// 检查 Subsystem 是否有效
	check(Subsystem);
	// 添加映射上下文 AuraContext，优先级为 0
	Subsystem->AddMappingContext(AuraContext, 0);

	// 显示鼠标光标
	bShowMouseCursor = true;
	// 设置默认的鼠标光标类型
	DefaultMouseCursor = EMouseCursor::Default;

	// 配置游戏和 UI 的输入模式
	FInputModeGameAndUI InputModeData;
	// 设置鼠标不锁定到视口
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// 设置在捕获期间不隐藏光标
	InputModeData.SetHideCursorDuringCapture(false);
	// 设置输入模式
	SetInputMode(InputModeData);
}

