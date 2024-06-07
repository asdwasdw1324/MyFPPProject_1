// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun_Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "MyFPPProject/MyFPPProjectCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "MyFPPProject/MyFPPProjectProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AGun_Actor::AGun_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));

	BoxCollisionArea = CreateDefaultSubobject<UBoxComponent>(TEXT("PickUpCollision"));
	BoxCollisionArea->SetupAttachment(GunMesh);
	

}

void AGun_Actor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BoxCollisionArea->OnComponentBeginOverlap.AddDynamic(this, &AGun_Actor::OnBoxCollisionBeginOverlap);
}

bool AGun_Actor::AttachWeapon(AMyFPPProjectCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no rifle yet
	if (!Character || Character->GetHasRifle())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("WeaponSocket")));
	
	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AGun_Actor::Fire);
			// Reload
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AGun_Actor::Reload);
		}
	}

	return true;
}

void AGun_Actor::Fire()
{
	if (!Character || !Character->GetController())
	{
		return;
	}

	// Try and fire a projectile
	if (GunParam.ProjectileClass)
	{
		if (UWorld* const World = GetWorld(); World != nullptr)
		{
			GunParam.MuzzleOffset = GunMesh->GetSocketLocation(FName(TEXT("Fire_FX_Slot")));

			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			//FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			FRotator SpawnRotation = GunMesh->GetSocketRotation(FName(TEXT("Fire_FX_Slot")));
			FVector SpawnLocation = GunParam.MuzzleOffset + SpawnRotation.Vector()*2;
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			//FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(GunParam.MuzzleOffset);
			
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
			// Spawn the projectile at the muzzle
			World->SpawnActor<AMyFPPProjectProjectile>(GunParam.ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, GunParam.DE_Fire_VFX, SpawnLocation, SpawnRotation);
		}
		
	}
	
	// Try and play the sound if specified
	if (GunParam.FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GunParam.FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (GunParam.FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		if (UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance(); AnimInstance && GunParam.FireAnimation)
		{
			AnimInstance->Montage_Play(GunParam.FireAnimation, 1.f);
		}

		// Get the animation object for the weapon mesh
		if (UAnimInstance* W_AnimInstance = GunMesh->GetAnimInstance(); W_AnimInstance && GunParam.W_FireAnimation)
		{
			W_AnimInstance->Montage_Play(GunParam.W_FireAnimation);
			
		}
	}
}

void AGun_Actor::Reload()
{
	if (GunParam.ReloadSound != nullptr && GunParam.FireAnimation != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GunParam.ReloadSound, Character->GetActorLocation());

		if (UAnimInstance* ReloadAnimInstance = Character->GetMesh1P()->GetAnimInstance(); ReloadAnimInstance && GunParam.ReloadAnimation )
		{
			ReloadAnimInstance->Montage_Play(GunParam.ReloadAnimation, 1.0f);
		}

		if (UAnimInstance* W_ReloadAnimInstance = GunMesh->GetAnimInstance(); W_ReloadAnimInstance && GunParam.W_ReloadAnimation)
		{
			W_ReloadAnimInstance->Montage_Play(GunParam.W_ReloadAnimation);
		}
	}
}

// Called when the game starts or when spawned
void AGun_Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun_Actor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (!Character)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

void AGun_Actor::OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AMyFPPProjectCharacter* PickupCharacter = Cast<AMyFPPProjectCharacter>(OtherActor);
	if(PickupCharacter != nullptr)
	{
		// Notify that the actor is being picked up
		OnGunPickUp.Broadcast(PickupCharacter);

		AttachWeapon(PickupCharacter);
		
		// Unregister from the Overlap Event, so it is no longer triggered
		BoxCollisionArea->OnComponentBeginOverlap.RemoveAll(this);
	}
}

