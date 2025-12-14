#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyEnemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
    Melee UMETA(DisplayName = "Melee"),
    Ranged UMETA(DisplayName = "Ranged")
};

UCLASS()
class FINAL_2025_API AMyEnemy : public ACharacter
{
    GENERATED_BODY()

public:
    AMyEnemy();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Stats según documento
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    float CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MovementSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float BaseDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    EEnemyType EnemyType;

    // Configuración de ataque
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float AttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float AttackCooldown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float DetectionRadius;

    // Drops según documento
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
    TSubclassOf<class AMyPickup> HealthDropClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
    TSubclassOf<class AMyPickup> AmmoDropClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float HealthDropChance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float AmmoDropChance;

    // Projectile para enemigos Ranged
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TSubclassOf<class AMyProjectile> ProjectileClass;

    // Funciones
    UFUNCTION()
    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, 
        AController* EventInstigator, AActor* DamageCauser) override;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Attack();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void Die();

private:
    UPROPERTY()
    class AAIController* AIController;

    UPROPERTY()
    class AMyCharacter* TargetPlayer;

    float LastAttackTime;

    void FindPlayer();
    void MoveTowardsPlayer();
    void PerformMeleeAttack();
    void PerformRangedAttack();
    void DropLoot();
};