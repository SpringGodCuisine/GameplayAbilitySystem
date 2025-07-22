
#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h" 
#include "AuraInputConfig.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAS_RPG_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldfuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldfuncType HeldFunc);
};

/**
 * 
 * @tparam UserClass 
 * @tparam PressedFuncType 
 * @tparam ReleasedFuncType 
 * @tparam HeldfuncType 
 * @param InputConfig 
 * @param Object 
 * @param PressedFunc 
 * @param ReleasedFunc 
 * @param HeldFunc 
 *
 * 它是一个模板类型，能够处理我们传入的任何内容。前提是BindAction的操作支持这些类型，它支持函数类型，甚至支持带有输出参数的函数
 * BindAction的工作方式是接收一个函数，然后在该函数之后，我们可以传入输入参数，这些参数将被传递到哪些回调函数中
 * 当输入分别被按下，释放，或按住时
 */
template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldfuncType>
void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldfuncType HeldFunc)
{
	check(InputConfig);

	for (const FAuraInputAction Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction,ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);	
			}
			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}
