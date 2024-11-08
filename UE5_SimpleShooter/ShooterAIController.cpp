// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    AActor* FocusPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    SetFocus(FocusPawn, 2);

}

void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    AActor* FocusPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    MoveToActor(FocusPawn, 500);
}