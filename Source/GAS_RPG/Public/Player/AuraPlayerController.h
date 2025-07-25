// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h" 
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;

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
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	//缓存光标点击目的地
	FVector CachedDestination = FVector::ZeroVector;
	//玩家跟随光标时间
	float FollowTime = 0.f;
	//短按预值
	float ShortPressThreshold = 0.5f;
	//自动运行
	bool bAutoRunning = false;
	//是否瞄准目标
	bool bTargeting = false;
	//与目标位置的接近程度
	UPROPERTY(EditDefaultsOnly, Category="Input")
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
};
