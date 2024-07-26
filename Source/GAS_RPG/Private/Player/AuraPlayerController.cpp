

#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

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

void AAuraPlayerController::SetupInputComponent()
{
	// 调用父类的 SetupInputComponent 方法
	Super::SetupInputComponent();

	// 将 InputComponent 强制转换为 UEnhancedInputComponent 类型
	UEnhancedInputComponent * EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	// 绑定移动操作到 Move 函数，在触发事件发生时调用
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
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
