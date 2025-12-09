#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

UCLASS()
class FINAL_2025_API AMyWeapon : public AActor
{
	GENERATED_BODY()
    
public:    
	AMyWeapon();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float MaxRange;

	// Munición en el cargador actual
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	int32 CurrentClipAmmo;

	// Tamaño máximo del cargador
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	int32 ClipSize;

	// Munición total de reserva
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	int32 TotalAmmoCapacity;

public:
	// Función para disparar. Retorna true si disparó, false si no tenía balas
	UFUNCTION(BlueprintCallable, Category = "Weapon Logic")
	bool Fire(FVector StartLocation, FVector Direction);

	// Getters para UI
	UFUNCTION(BlueprintPure, Category = "Weapon Logic")
	int32 GetCurrentAmmo() const { return CurrentClipAmmo; }

	UFUNCTION(BlueprintCallable, Category = "Weapon Logic")
	void AddAmmo(int32 Amount);
};