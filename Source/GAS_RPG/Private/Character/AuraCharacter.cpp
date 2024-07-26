// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

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

