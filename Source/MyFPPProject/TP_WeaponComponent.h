// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "TP_WeaponComponent.generated.h"

class AMyFPPProjectCharacter;

USTRUCT(BlueprintType)
struct FWeaponParam
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

	/** AnimMontage to play each time we reload */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UAnimMontage* ReloadAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FVector MuzzleOffset;
	
};

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYFPPPROJECT_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:

	//Weapon parameters structure
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = Weapon)
	FWeaponParam WeaponParam;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

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
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** The Character holding this weapon*/
	AMyFPPProjectCharacter* Character;
};
