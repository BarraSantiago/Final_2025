#include "MyProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AMyProjectile::AMyProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    // Colisión
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;
    CollisionSphere->SetSphereRadius(15.0f);
    CollisionSphere->SetCollisionProfileName(TEXT("BlockAll"));
    CollisionSphere->OnComponentHit.AddDynamic(this, &AMyProjectile::OnHit);

    // Mesh
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(CollisionSphere);
    ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Movimiento
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 2000.0f;
    ProjectileMovement->MaxSpeed = 2000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;

    // Defaults
    Damage = 15.0f;
    ProjectileSpeed = 2000.0f;
    LifeSpan = 5.0f;

    InitialLifeSpan = LifeSpan;
}

void AMyProjectile::BeginPlay()
{
    Super::BeginPlay();
    ProjectileMovement->InitialSpeed = ProjectileSpeed;
    ProjectileMovement->MaxSpeed = ProjectileSpeed;
}

void AMyProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMyProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this && OtherActor != GetOwner())
    {
        UGameplayStatics::ApplyDamage(
            OtherActor,
            Damage,
            GetInstigatorController(),
            this,
            UDamageType::StaticClass()
        );

        UE_LOG(LogTemp, Log, TEXT("Projectile hit: %s"), *OtherActor->GetName());
    }

    Destroy();
}