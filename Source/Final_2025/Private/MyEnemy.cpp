#include "MyEnemy.h"
#include "MyCharacter.h"
#include "MyPickup.h"
#include "MyProjectile.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AMyEnemy::AMyEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    // Stats por defecto
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
    MovementSpeed = 300.0f;
    BaseDamage = 10.0f;

    // Combat defaults
    EnemyType = EEnemyType::Melee;
    AttackRange = 150.0f;
    AttackCooldown = 1.5f;
    DetectionRadius = 1000.0f;
    LastAttackTime = 0.0f;

    // Loot defaults (según documento)
    HealthDropChance = 0.3f; // 30%
    AmmoDropChance = 0.5f;   // 50%

    // Configurar movimiento
    GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;

    // Auto-poseer por AI
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMyEnemy::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHealth;
    AIController = Cast<AAIController>(GetController());
    FindPlayer();
}

void AMyEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TargetPlayer && !TargetPlayer->IsDead())
    {
        float DistanceToPlayer = FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation());

        // Debug visual
        DrawDebugSphere(GetWorld(), GetActorLocation(), DetectionRadius, 12, FColor::Yellow, false, 0.1f);

        if (DistanceToPlayer <= DetectionRadius)
        {
            if (DistanceToPlayer <= AttackRange)
            {
                // Detener movimiento y atacar
                if (AIController)
                {
                    AIController->StopMovement();
                }
                Attack();
            }
            else
            {
                // Moverse hacia el jugador
                MoveTowardsPlayer();
            }
        }
    }
}

float AMyEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, 
    AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    CurrentHealth -= ActualDamage;
    
    UE_LOG(LogTemp, Warning, TEXT("Enemy received %.1f damage. HP left: %.1f"), ActualDamage, CurrentHealth);
    
    if (CurrentHealth <= 0.0f)
    {
        Die();
    }

    return ActualDamage;
}

void AMyEnemy::Attack()
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    
    if (CurrentTime - LastAttackTime < AttackCooldown)
    {
        return;
    }

    LastAttackTime = CurrentTime;

    switch (EnemyType)
    {
        case EEnemyType::Melee:
            PerformMeleeAttack();
            break;
        case EEnemyType::Ranged:
            PerformRangedAttack();
            break;
    }
}

void AMyEnemy::Die()
{
    UE_LOG(LogTemp, Warning, TEXT("Enemigo eliminado"));

    // Notificar al jugador para incrementar KillCount
    if (TargetPlayer)
    {
        TargetPlayer->OnEnemyKilled();
    }

    // Drops según probabilidades del documento
    DropLoot();

    // Destruir enemigo
    Destroy();
}

void AMyEnemy::FindPlayer()
{
    TargetPlayer = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AMyEnemy::MoveTowardsPlayer()
{
    if (AIController && TargetPlayer)
    {
        AIController->MoveToActor(TargetPlayer, AttackRange * 0.9f);
    }
}

void AMyEnemy::PerformMeleeAttack()
{
    if (!TargetPlayer) return;

    float Distance = FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation());
    
    if (Distance <= AttackRange)
    {
        UE_LOG(LogTemp, Log, TEXT("Enemigo Melee ataca"));
        
        UGameplayStatics::ApplyDamage(
            TargetPlayer,
            BaseDamage,
            GetController(),
            this,
            UDamageType::StaticClass()
        );

        // Debug visual
        DrawDebugLine(GetWorld(), GetActorLocation(), TargetPlayer->GetActorLocation(), 
            FColor::Red, false, 0.5f, 0, 3.0f);
    }
}

void AMyEnemy::PerformRangedAttack()
{
    if (!TargetPlayer || !ProjectileClass) return;

    UE_LOG(LogTemp, Log, TEXT("Enemigo Ranged dispara"));

    FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * 100.0f);
    FRotator SpawnRotation = (TargetPlayer->GetActorLocation() - SpawnLocation).Rotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();

    AMyProjectile* Projectile = GetWorld()->SpawnActor<AMyProjectile>(
        ProjectileClass, 
        SpawnLocation, 
        SpawnRotation, 
        SpawnParams
    );

    if (Projectile)
    {
        Projectile->SetDamage(BaseDamage);
    }
}

void AMyEnemy::DropLoot()
{
    FVector DropLocation = GetActorLocation();

    // Health Drop
    if (HealthDropClass && FMath::FRand() <= HealthDropChance)
    {
        GetWorld()->SpawnActor<AMyPickup>(HealthDropClass, DropLocation, FRotator::ZeroRotator);
        UE_LOG(LogTemp, Log, TEXT("Health pickup dropeado"));
    }

    // Ammo Drop
    if (AmmoDropClass && FMath::FRand() <= AmmoDropChance)
    {
        DropLocation.Z += 50.0f; // Offset para evitar overlap
        GetWorld()->SpawnActor<AMyPickup>(AmmoDropClass, DropLocation, FRotator::ZeroRotator);
        UE_LOG(LogTemp, Log, TEXT("Ammo pickup dropeado"));
    }
}