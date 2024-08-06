// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;

/**
 * 
 */
UCLASS()
class GAS_RPG_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();
	//PlayerTick是APlayerController特有的，每帧更新时用来处理与玩家控制相关的逻辑
	virtual void PlayerTick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	//初始化并绑定输入组件，用于处理玩家输入。
	virtual void SetupInputComponent() override;
	
private:
	//输入映射上下文，用于定义一组输入绑定及其对应的输入动作
	//它允许开发者将一组相关的输入操作组织在一起，方便载不动的游戏状态下或模式下切换
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	//输入行为的抽象
	//定义游戏中可以执行的具体操作，例如跳跃，移动，射击等
	//每个ACTION文件可以包含触发时间（如按下丶释放丶保持）和触发条件（如双击丶长按）
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	//移动回调
	void Move(const FInputActionValue& InputActionValue);

	//光标追踪
	void CursorTrace();

	//控制多个高亮物体显示隐藏描边
	TObjectPtr<IEnemyInterface> LastActor;
	TObjectPtr<IEnemyInterface> ThisActor;
};
