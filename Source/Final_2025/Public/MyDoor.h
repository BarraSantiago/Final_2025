#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInteractionInterface.h" // Importante incluir la interfaz
#include "MyDoor.generated.h"

UCLASS()
class FINAL_2025_API AMyDoor : public AActor, public IMyInteractionInterface
{
	GENERATED_BODY()
    
public:    
	AMyDoor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* DoorMesh;

public:
	// Implementación de la Interfaz
	virtual void Interact_Implementation(class AMyCharacter* InstigatorPawn) override;

	// Evento Blueprint para hacer la animación de abrir (Timeline)
	UFUNCTION(BlueprintImplementableEvent, Category = "Door Logic")
	void OnDoorOpen(const FString& EndingType);
};