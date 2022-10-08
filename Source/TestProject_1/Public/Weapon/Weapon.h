// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Bullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Clips;
	
};

UCLASS()
class TESTPROJECT_1_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	FOnClipEmptySignature OnClipEmpty;

	FAmmoData GetAmmoData() const { return CurrentAmmo; }
protected:
	// Called when the game starts or when spawn ed
	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* MeshComponent;
		

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		FAmmoData DefaultAmmo {30, 10};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
		FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float TraceMaxDistance = 1500.0f;

	UPROPERTY(EditAnywhere)
		class UAnimationAsset* FireAnimation;
	virtual void BeginPlay() override;

	
	
public:
	UFUNCTION(BlueprintCallable, Category = "Message")
	void Fire();
	void DecreaseAmmo();
	bool IsAmmoEmpty() const;
	bool IsClipEmpty() const;
	void ChangeClip();
	void LogAmmo();
	bool CanReload() const;
	UFUNCTION(BlueprintCallable, Category = "Message")
	void MakeDamage(const FHitResult& HitResult);

	int32 GetCurrentBullets();
	int32 GetCurrentClips();
private:
	FAmmoData CurrentAmmo;

};
