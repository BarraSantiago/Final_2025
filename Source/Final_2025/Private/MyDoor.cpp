#include "MyDoor.h"
#include "MyCharacter.h"

AMyDoor::AMyDoor()
{
    PrimaryActorTick.bCanEverTick = false;

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    RootComponent = DoorMesh;
}

void AMyDoor::Interact_Implementation(AMyCharacter* InstigatorPawn)
{
    if (!InstigatorPawn) return;

    if (InstigatorPawn->bHasUltimateKey)
    {
        int32 Kills = InstigatorPawn->KillCount;
        FString EndingResult;

        // Rangos corregidos según documento
        if (Kills < 20)
        {
            EndingResult = "Final A - Fish Particle";
        }
        else if (Kills >= 21 && Kills <= 50)
        {
            EndingResult = "Final B - Random NPC";
        }
        else // Más de 100
        {
            EndingResult = "Final C - Main Character";
        }

        UE_LOG(LogTemp, Warning, TEXT("PUERTA ABIERTA. Final: %s (Kills: %d)"), *EndingResult, Kills);
        OnDoorOpen(EndingResult);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PUERTA BLOQUEADA: Falta Key_asset_0001"));
        if (GEngine) 
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Locked! Find the Key."));
        }
    }
}