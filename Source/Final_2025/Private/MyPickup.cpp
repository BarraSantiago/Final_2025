#include "MyPickup.h"
#include "MyCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AMyPickup::AMyPickup()
{
    PrimaryActorTick.bCanEverTick = false;

    // Componente de colisión para detección
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;
    CollisionSphere->SetSphereRadius(100.0f);
    CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    // Mesh visual
    PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
    PickupMesh->SetupAttachment(CollisionSphere);
    PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Valores por defecto
    PickupValue = 25.0f;
    bAutoPickup = true;
}

void AMyPickup::BeginPlay()
{
    Super::BeginPlay();
    
    if (bAutoPickup)
    {
        CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AMyPickup::OnOverlapBegin);
    }
}



void AMyPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
    if (Character)
    {
        ApplyPickup(Character);
    }
}

void AMyPickup::Interact_Implementation(AMyCharacter* InstigatorPawn)
{
    if (InstigatorPawn)
    {
        ApplyPickup(InstigatorPawn);
    }
}

void AMyPickup::ApplyPickup(AMyCharacter* Character)
{
    // Implementar en clases hijas
    Destroy();
}

// Called every frame
void AMyPickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}
