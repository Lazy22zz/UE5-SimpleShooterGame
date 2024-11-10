// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Shoot.h"
#include "ShooterCharacter.h"
#include "AIController.h"

UBT_Shoot::UBT_Shoot()
{
    NodeName = "Shoot";
}

EBTNodeResult::Type UBT_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AShooterCharacter* AICharacter = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (AICharacter)
    {
        AICharacter -> Shoot();
    }
    return EBTNodeResult::Succeeded;
}