// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"

#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/Widgets/AuraUserWidget.h"

// 获取 Overlay Widget Controller，如果尚未创建则进行创建和初始化
UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if(OverlayWidgetController == nullptr)
	{
		// 创建新的 Overlay Widget Controller 实例
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		// 设置 Widget Controller 的参数
		OverlayWidgetController->SetWidgetControllerParams(WCParams);

		// 返回创建的 Overlay Widget Controller
		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// 确保 Overlay Widget Class 已初始化，否则打印错误信息并中止运行
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
	// 确保 Overlay Widget Controller Class 已初始化，否则打印错误信息并中止运行
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"));

	// 创建 Overlay Widget 实例
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	// 将 Widget 强制转换为 UAuraUserWidget 类型
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	// 设置 Widget Controller 的参数
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	// 获取（或创建） Overlay Widget Controller
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	// 将 Widget Controller 赋给 Overlay Widget
	OverlayWidget->SetWidgetController(WidgetController);

	// 将 Widget 添加到视口中显示
	Widget->AddToViewport();
}