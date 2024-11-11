// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"


UBTService_PlayerLocation::UBTService_PlayerLocation()
{
    NodeName = "Update Player Location";
}

void UBTService_PlayerLocation::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* FocusPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (FocusPawn == nullptr)
    {
        return;
    }

    OwnerComp.GetBlackboardComponent() -> SetValueAsVector(GetSelectedBlackboardKey(), FocusPawn -> GetActorLocation());
}
