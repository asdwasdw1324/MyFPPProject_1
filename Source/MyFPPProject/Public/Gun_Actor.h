// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InputMappingContext.h"
#include "Gun_Actor.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;
class AMyFPPProjectCharacter;
class UInputMappingContext;
class UInputAction;
class UNiagaraSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGunPickUp, AMyFPPProjectCharacter*, PickUpCharacter);

USTRUCT(BlueprintType)
struct FGunParam
{
	GENERATED_BODY()
	
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<class AMyFPPProjectProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	USoundBase* FireSound;

	/** Sound to play each time we reload */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	USoundBase* ReloadSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UAnimMontage* FireAnimation;

    /** Weapon AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UAnimMontage* W_FireAnimation;

	/** AnimMontage to play each time we reload */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UAnimMontage* ReloadAnimation;

    /** Weapon AnimMontage to play each time we reload */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UAnimMontage* W_ReloadAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FVector MuzzleOffset;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UNiagaraSystem* DE_Fire_VFX;
	
};

UCLASS()
class MYFPPPROJECT_API AGun_Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun_Actor();

	UPROPERTY(BlueprintAssignable)
	FOnGunPickUp OnGunPickUp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<USkeletalMeshComponent> GunMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<UBoxComponent> BoxCollisionArea;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Weapon")
	FGunParam GunParam;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool AttachWeapon(AMyFPPProjectCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	/** Reload the weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	/** The Character holding this weapon*/
	TObjectPtr<AMyFPPProjectCharacter> Character;

};

