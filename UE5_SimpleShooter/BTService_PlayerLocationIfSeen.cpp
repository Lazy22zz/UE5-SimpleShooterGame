// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
    NodeName = "Player Location If Seen";
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* FocusPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (FocusPawn == nullptr)
    {
        return;
    }

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return;
    }

    if (AIController -> LineOfSightTo(FocusPawn))
    {
        OwnerComp.GetBlackboardComponent() -> SetValueAsVector(GetSelectedBlackboardKey(), FocusPawn -> GetActorLocation());
    }
    else
    {
        OwnerComp.GetBlackboardComponent() -> ClearValue(GetSelectedBlackboardKey());
    }
}
