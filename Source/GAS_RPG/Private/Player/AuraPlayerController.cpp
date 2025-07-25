

#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	//声明一个FHitResult变量，用于存储光标下的命中结果
	FHitResult CursorHit;
	//获取光标下的命中结果，使用可见性通道碰撞，不忽略自身，结果储存在CursorHit中
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	//如果没有阻挡命中则返回
	if(!CursorHit.bBlockingHit)return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/*
	 * line trace from cursor. There are several scenarios;
	 *  A. LastActor is null && ThisActor is null;
	 *		- Do nothing.
	 *	B. LastActor is null && ThisActor is Valid;
	 *		- Highlight ThisActor.
	 *	C. LastActor is Valid && ThisActor is null;
	 *		- UnHighlight LastActor.
	 *	D. Both actors are valid, but LastActor != ThisActor
	 *		- UnHighlight LastActor, and Highlight ThisActor
	 *	E. Both actors are valid, and are the same actor
	 *		- Do nothing
	 */

	if(LastActor == nullptr)
	{
		if(ThisActor != nullptr)
		{
			// Case B
			ThisActor->HighlightActor();
		}
		else
		{
			// Case A - both are null, do nothing
		}
	}
	else // LastActor is valid
	{
		if(ThisActor == nullptr)
		{
			// Case C
			LastActor->UnHighlightActor();
		}
		else // both actors are valid
		{
			if(LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				// Case E - do nothing;	
			}
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor != nullptr;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}
	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	//鼠标左键
	else
	{
		APawn* ControllerPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControllerPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,
				ControllerPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Red, false, 5.f);
				}
				bAutoRunning = true;
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	//鼠标左键
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		FHitResult Hit;
		if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
		{
		/**
		实际碰撞发生时，追踪形状（如盒体、球体、射线等）与被击中对象接触的世界空间位置。
		例如：对于一次球体追踪测试，这里指的是球体表面接触另一个物体的点。
		@note：如果是初始重叠（bStartPenetrating=true）的情况，由于没有明确的单一碰撞点可报告，ImpactPoint 将与 Location 相同。
		*/
		CachedDestination = Hit.ImpactPoint;
		// DrawDebugSphere(
		// 	GetWorld(),
		// 	Hit.ImpactPoint,
		// 	25.0f, // 半径
		// 	12, // 多边形精度
		// 	FColor::Green,
		// 	false,
		// 	2.0f // 显示时间（秒）
		// );
		}

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// 检查 AuraContext 是否有效
	check(AuraContext);

	// 获取本地玩家的增强输入子系统
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		// 添加映射上下文 AuraContext，优先级为 0
		Subsystem->AddMappingContext(AuraContext, 0);
	}
	
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

void AAuraPlayerController::SetupInputComponent()
{
	// 调用父类的 SetupInputComponent 方法
	Super::SetupInputComponent();

	UAuraInputComponent * AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// 从输入值中获取二维向量
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	// 获取控制器的旋转角度
	const FRotator Rotation = GetControlRotation();
	// 仅保留 Yaw（偏航）角度，忽略 Pitch（俯仰）和 Roll（滚转）角度
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	// 获取沿前方向和右方向的单位向量
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 如果有控制的 Pawn，则根据输入值沿前方向和右方向添加移动输入
	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}