

#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h" 
#include "AbilitySystem/AuraAbilitySystemComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Target);
	// if(ASCInterface)
	// {
	// 	ASCInterface->GetAbilitySystemComponent();
	// }
	
	// 使用蓝图库函数获取目标的 Ability System Component (ASC)
	UAbilitySystemComponent* TargetASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(TargetASC == nullptr)return;
	check(GameplayEffectClass);
	
	// 创建一个效果上下文句柄
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	// 将当前对象添加为效果的来源对象
	EffectContextHandle.AddSourceObject(this);
	// 创建一个效果规格句柄，指定效果类、等级和上下文
	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	// 将效果应用到自身
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

