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

	// 1. Verificar si tiene la llave
	if (InstigatorPawn->bHasUltimateKey)
	{
		// 2. Calcular Final basado en Kills
		int32 Kills = InstigatorPawn->KillCount;
		FString EndingResult;

		if (Kills < 20)
		{
			EndingResult = "Final A - Fish Particle";
		}
		else if (Kills >= 21 && Kills <= 50)
		{
			EndingResult = "Final B - Random NPC";
		}
		else // Mas de 50 (o 100 según consigna)
		{
			EndingResult = "Final C - Main Character";
		}

		// Log para debug
		UE_LOG(LogTemp, Warning, TEXT("PUERTA ABIERTA. Final: %s"), *EndingResult);
        
		// Llamamos al evento de Blueprint para animación y UI
		OnDoorOpen(EndingResult);
	}
	else
	{
		// Feedback visual de "Puerta Bloqueada"
		UE_LOG(LogTemp, Warning, TEXT("PUERTA BLOQUEADA: Falta Key_asset_0001"));
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Locked! Find the Key."));
	}
}