// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

   if(AIEnemyBehaviour)
   {
    RunBehaviorTree(AIEnemyBehaviour);
    APawn* FocusPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    GetBlackboardComponent() -> SetValueAsVector(TEXT("PlayerLocation"), FocusPawn -> GetActorLocation());
    GetBlackboardComponent() -> SetValueAsVector(TEXT("StartLocation"), FocusPawn -> GetActorLocation());
   }
}

void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    APawn* FocusPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (LineOfSightTo(FocusPawn))
    {
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), FocusPawn -> GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), FocusPawn -> GetActorLocation());
    }
    else
    {
        GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    }

}

bool AShooterAIController::Is_Dead() const
{
    AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
    if (ControlledCharacter != nullptr)
    {
        return ControlledCharacter->IsDead();
    }

    return false;
}
