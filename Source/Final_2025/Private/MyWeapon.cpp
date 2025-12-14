#include "MyWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AMyWeapon::AMyWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	// Valores por defecto
	BaseDamage = 25.0f;
	MaxRange = 5000.0f; // 50 metros
	ClipSize = 30;
	CurrentClipAmmo = ClipSize;
	TotalAmmoCapacity = 120;
}

bool AMyWeapon::Fire(FVector StartLocation, FVector Direction)
{
    if (CurrentClipAmmo <= 0)
    {
        // Intento de recarga automática
        if (TotalAmmoCapacity > 0)
        {
            Reload();
        }
        return false;
    }

    CurrentClipAmmo--;

    FHitResult Hit;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.AddIgnoredActor(GetOwner());

    FVector EndLocation = StartLocation + (Direction * MaxRange);
    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, QueryParams);

    DrawDebugLine(GetWorld(), StartLocation, bHit ? Hit.Location : EndLocation, FColor::Red, false, 2.0f);

    if (bHit && Hit.GetActor())
    {
        UGameplayStatics::ApplyDamage(
            Hit.GetActor(),
            BaseDamage,
            GetInstigatorController(),
            this,
            UDamageType::StaticClass()
        );
    }

    return true;
}

void AMyWeapon::Reload()
{
    if (TotalAmmoCapacity <= 0 || CurrentClipAmmo == ClipSize) return;

    int32 AmmoNeeded = ClipSize - CurrentClipAmmo;
    int32 AmmoToReload = FMath::Min(AmmoNeeded, TotalAmmoCapacity);

    CurrentClipAmmo += AmmoToReload;
    TotalAmmoCapacity -= AmmoToReload;

    UE_LOG(LogTemp, Log, TEXT("Reloaded: %d bullets. Total left: %d"), AmmoToReload, TotalAmmoCapacity);
}

void AMyWeapon::AddAmmo(int32 Amount)
{
    TotalAmmoCapacity += Amount;
}