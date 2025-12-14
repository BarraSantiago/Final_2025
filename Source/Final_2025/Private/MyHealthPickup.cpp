#include "MyHealthPickup.h"
#include "MyCharacter.h"

AMyHealthPickup::AMyHealthPickup()
{
    PrimaryActorTick.bCanEverTick = false;
    HealthAmount = 25.0f;
}

void AMyHealthPickup::BeginPlay()
{
    Super::BeginPlay();
}

void AMyHealthPickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMyHealthPickup::ApplyPickup(AMyCharacter* Character)
{
    if (Character)
    {
        Character->Heal(HealthAmount);
        UE_LOG(LogTemp, Log, TEXT("HP healed: %.1f"), HealthAmount);
    }
    Super::ApplyPickup(Character);
}