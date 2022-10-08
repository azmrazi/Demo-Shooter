// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Target/Target.h"
#include "Components/SkeletalMeshComponent.h"
#include "TestProject_1/TestProject_1Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponPickup, All, All)
DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)
DEFINE_LOG_CATEGORY_STATIC(LogDamageType, All, All)
// Sets default values
AWeapon::AWeapon()
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(MeshComponent);
	CurrentAmmo = DefaultAmmo;
}

void AWeapon::DecreaseAmmo()
{
	if (CurrentAmmo.Bullets == 0)
	{
		return;
	}
	CurrentAmmo.Bullets--;
	LogAmmo();

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		OnClipEmpty.Broadcast();
	}
;}

bool AWeapon::IsAmmoEmpty() const
{
	return CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool AWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}

void AWeapon::ChangeClip()
{
	if (CurrentAmmo.Clips == 0)
	{
		return;
	}
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	CurrentAmmo.Clips--;

	UE_LOG(LogBaseWeapon, Display, TEXT("-----Change Clip -----"));
}

void AWeapon::LogAmmo()
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
	AmmoInfo += FString::FromInt(CurrentAmmo.Clips);

	UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

bool AWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}


void AWeapon::Fire()
{
	MeshComponent->PlayAnimation(FireAnimation, false);

	//const auto Player = Cast<ACharacter>(GetOwner());
	//if (!Player) return;

	//const auto Controller = Player->GetController<APlayerController>();
	//if (!Controller) return;

	//FVector ViewLocation;
	//FRotator ViewRotation;
	//Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

	const FTransform SocketTransform = MeshComponent->GetSocketTransform(MuzzleSocketName);
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);

	if (HitResult.bBlockingHit)
	{
		MakeDamage(HitResult);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
	}
}


void AWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedType = HitResult.GetActor();
	if (ATarget* TargetActor = Cast<ATarget>(DamagedType))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Damaged Target"), true, FVector2D(1.5, 2));
		UE_LOG(LogDamageType, Display, TEXT("Damaged Target"));
		
	}
	
	if (!DamagedType) return;
}

int32 AWeapon::GetCurrentBullets()
{
	if (CurrentAmmo.Bullets)
		return CurrentAmmo.Bullets;
	else
		return CurrentAmmo.Bullets = 0;
}

int32 AWeapon::GetCurrentClips()
{
	if (CurrentAmmo.Clips)
		return CurrentAmmo.Clips;
	else
		return CurrentAmmo.Clips = 0;
	
}



