#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPickup.generated.h"

class AMyCharacter;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class FINAL_2025_API AMyPickup : public AActor
{
    GENERATED_BODY()

public:
    AMyPickup();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    void Interact(AMyCharacter* InstigatorPawn);
    virtual void Interact_Implementation(AMyCharacter* InstigatorPawn);

    virtual void ApplyPickup(AMyCharacter* Character);

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* CollisionSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* PickupMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    float PickupValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    bool bAutoPickup;
};