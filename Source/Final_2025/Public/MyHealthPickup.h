#pragma once

#include "CoreMinimal.h"
#include "MyPickup.h"
#include "MyHealthPickup.generated.h"

UCLASS()
class FINAL_2025_API AMyHealthPickup : public AMyPickup
{
    GENERATED_BODY()

public:
    AMyHealthPickup();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void ApplyPickup(class AMyCharacter* Character) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    float HealthAmount;
};