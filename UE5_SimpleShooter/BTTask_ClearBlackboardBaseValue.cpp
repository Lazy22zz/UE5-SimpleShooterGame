// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearBlackboardBaseValue.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearBlackboardBaseValue::UBTTask_ClearBlackboardBaseValue()
{
    NodeName = "Clear Blackboard Value";
}

EBTNodeResult::Type UBTTask_ClearBlackboardBaseValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

    return EBTNodeResult::Succeeded;

}