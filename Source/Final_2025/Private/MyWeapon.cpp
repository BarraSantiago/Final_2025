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
	if (CurrentClipAmmo <= 0) return false;

	CurrentClipAmmo--;

	// Configuración del Raycast (Trace)
	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner()); // Ignorar al personaje que sostiene el arma

	FVector EndLocation = StartLocation + (Direction * MaxRange);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, QueryParams);

	// Dibujar línea de depuración (Visible por 2 segundos)
	DrawDebugLine(GetWorld(), StartLocation, bHit ? Hit.Location : EndLocation, FColor::Red, false, 2.0f);

	if (bHit && Hit.GetActor())
	{
		// Aplicar daño genérico
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

void AMyWeapon::AddAmmo(int32 Amount)
{
	TotalAmmoCapacity += Amount;
}