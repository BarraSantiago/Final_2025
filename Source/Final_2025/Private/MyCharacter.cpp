#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyWeapon.h"
#include "MyInteractionInterface.h"

AMyCharacter::AMyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Configuración básica de cámara FPS
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
    FirstPersonCameraComponent->SetRelativeLocation(FVector(0, 0, 60.f)); // Altura de ojos
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    // Mesh para brazos (Solo lo ve el dueño)
    Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
    Mesh1P->SetupAttachment(FirstPersonCameraComponent);
    Mesh1P->SetOnlyOwnerSee(true);
    Mesh1P->bCastDynamicShadow = false;
    Mesh1P->CastShadow = false;

    // Valores iniciales
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
    KillCount = 0;
    bHasUltimateKey = false;
}

void AMyCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Spawneamos el arma y la pegamos a la mano
    if (StartingWeaponClass)
    {
        CurrentWeapon = GetWorld()->SpawnActor<AMyWeapon>(StartingWeaponClass);
        if (CurrentWeapon)
        {
            // OJO: "GripPoint" debe ser un Socket existente en el esqueleto de tus brazos (Mesh1P)
            CurrentWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("GripPoint"));
            CurrentWeapon->SetOwner(this);
        }
    }
}

void AMyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bindings Clásicos (Legacy). Si usas Enhanced Input, ignora esto y llama a las funciones desde BP.
    //PlayerInputComponent->BindAxis("MoveForward", this, AMyCharacter::AddMovementInput(GetActorForwardVector())); // Pseudocódigo para axis
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::FireWeapon);
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMyCharacter::Interact);
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
    CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.0f, MaxHealth);
    
    if (CurrentHealth <= 0)
    {
        // Lógica de muerte (Reiniciar nivel o mostrar Game Over)
        UE_LOG(LogTemp, Warning, TEXT("PLAYER MUERTO"));
    }
    return ActualDamage;
}

void AMyCharacter::FireWeapon()
{
    if (CurrentWeapon)
    {
        FVector CameraLoc;
        FRotator CameraRot;
        GetController()->GetPlayerViewPoint(CameraLoc, CameraRot);

        CurrentWeapon->Fire(CameraLoc, CameraRot.Vector());
    }
}

void AMyCharacter::Interact()
{
    FVector Start = FirstPersonCameraComponent->GetComponentLocation();
    FVector End = Start + (FirstPersonCameraComponent->GetForwardVector() * 300.0f); // 3 metros

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    if (CurrentWeapon) Params.AddIgnoredActor(CurrentWeapon);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
    {
        AActor* HitActor = HitResult.GetActor();
        // Verificamos si el actor tiene la interfaz implementada
        if (HitActor && HitActor->Implements<UMyInteractionInterface>())
        {
            IMyInteractionInterface::Execute_Interact(HitActor, this);
        }
    }
}

void AMyCharacter::Heal(float Amount)
{
    CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);
}

void AMyCharacter::AddAmmoToWeapon(int32 Amount)
{
    if (CurrentWeapon)
    {
        CurrentWeapon->AddAmmo(Amount);
    }
}