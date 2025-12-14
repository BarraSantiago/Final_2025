#pragma once

#include "CoreMinimal.h"
#include "MyPickup.h"
#include "MyAmmoPickup.generated.h"

UCLASS()
class FINAL_2025_API AMyAmmoPickup : public AMyPickup
{
    GENERATED_BODY()

public:
    AMyAmmoPickup();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void ApplyPickup(class AMyCharacter* Character) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    int32 AmmoAmount;
};