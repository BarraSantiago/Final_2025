#include "MyAmmoPickup.h"
#include "MyCharacter.h"

AMyAmmoPickup::AMyAmmoPickup()
{
    PrimaryActorTick.bCanEverTick = false;
    AmmoAmount = 30;
}

void AMyAmmoPickup::BeginPlay()
{
    Super::BeginPlay();
}

void AMyAmmoPickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMyAmmoPickup::ApplyPickup(AMyCharacter* Character)
{
    if (Character)
    {
        Character->AddAmmoToWeapon(AmmoAmount);
        UE_LOG(LogTemp, Log, TEXT("Add ammo: %d"), AmmoAmount);
    }
    Super::ApplyPickup(Character);
}