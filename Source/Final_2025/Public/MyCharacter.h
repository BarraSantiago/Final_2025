#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UCameraComponent;
class AMyWeapon;

UCLASS()
class FINAL_2025_API AMyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMyCharacter();

protected:
    virtual void BeginPlay() override;

    // Componentes
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* FirstPersonCameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USkeletalMeshComponent* Mesh1P;

    // Referencia al arma (Clase para spawnear y Puntero a la instancia)
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AMyWeapon> StartingWeaponClass;

    UPROPERTY(BlueprintReadOnly, Category = "Weapon")
    AMyWeapon* CurrentWeapon;

    // Stats
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    float CurrentHealth;

public:
    // Variables públicas para la lógica del juego
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    int32 KillCount;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    bool bHasUltimateKey;

    // Funciones
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    // Acciones (BlueprintCallable para poder llamarlas desde Enhanced Input en BP)
    UFUNCTION(BlueprintCallable, Category = "Actions")
    void FireWeapon();

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void Interact();

    // Pickups
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void Heal(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void AddAmmoToWeapon(int32 Amount);
};