#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MyInteractionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UMyInteractionInterface : public UInterface { GENERATED_BODY() };

class FINAL_2025_API IMyInteractionInterface
{
	GENERATED_BODY()

public:
	// Función que llamará el personaje al mirar algo y presionar 'E'
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(class AMyCharacter* InstigatorPawn);
    
	// Para mostrar el hint en UI ("Presiona E para abrir")
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FText GetInteractionHint();
};