// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BT_Shoot.generated.h"

/**
 * 
 */
UCLASS()
class UE5_SIMPLESHOOTER_API UBT_Shoot : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBT_Shoot();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
