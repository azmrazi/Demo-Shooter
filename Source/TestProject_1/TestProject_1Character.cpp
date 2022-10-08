// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestProject_1Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Animation/SkeletalMeshActor.h"
#include "GameFramework/Controller.h"
#include "Public/Weapon/Weapon.h"
#include "GameFramework/SpringArmComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogChangeAmmo, All, All)
//////////////////////////////////////////////////////////////////////////
// ATestProject_1Character

ATestProject_1Character::ATestProject_1Character()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetSphereRadius(100.0f);
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATestProject_1Character::BeginPlay()
{
	Super::BeginPlay();
	/*FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (WeaponClass)
	{
		FTransform WeaponTransform;
		WeaponTransform.SetLocation(FVector::ZeroVector);
		WeaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));

		Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, WeaponTransform, SpawnParams);
		if (Weapon)
		{
			Weapon->NotifyActorBeginOverlap(this);
			Weapon->SetActorEnableCollision(false);
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Weapon_r_hand"));

	if (Weapon)
	{
		Weapon->SetActorEnableCollision(false);
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Weapon_r_hand"));
	}
	else
	{

	}
		}
	}*/
	
}

void ATestProject_1Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ATestProject_1Character::Attack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ATestProject_1Character::StopFire);

	
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ATestProject_1Character::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ATestProject_1Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ATestProject_1Character::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ATestProject_1Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATestProject_1Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ATestProject_1Character::TouchStopped);
}

void ATestProject_1Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ATestProject_1Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ATestProject_1Character::Attack()
{
	
	if (Weapon)
	{
		if (!GetWorld() || Weapon->IsAmmoEmpty())
		{
			StopFire();
			return;
		}
		Weapon->Fire();
		
		GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ATestProject_1Character::Attack, TimeBetweenShots, true);


		Weapon->DecreaseAmmo();
	}
	FVector Start = GetMesh()->GetBoneLocation(FName("head"));
	FVector End = Start + FollowCamera->GetForwardVector() * 1500.0f;
	

}
void ATestProject_1Character::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ATestProject_1Character::OnEmptyClip()
{
	ChangeClip();
}

void ATestProject_1Character::ChangeClip()
{
	if (!CanReload()) return;
	StopFire();
	Weapon->ChangeClip();
	ReloadAnimInprogress = true;
	PlayAnimMontage(ReloadAnimMontage);
	UE_LOG(LogChangeAmmo, Display, TEXT("Changed Ammo"));
}

bool ATestProject_1Character::CanReload() const
{
	return Weapon && !EquipAnimInProgress && !ReloadAnimInprogress && Weapon->CanReload();
}

/*bool ATestProject_1Character::GetWeaponAmmoData(FAmmoData& AmmoData) const
{
	if (Weapon)
	{
		AmmoData = Weapon->GetAmmoData();
		return true;
	}
	return false;

}
*/

FString ATestProject_1Character::GetPlayerStats()
{
	if (Weapon)
	{
		FString String = "Bullets: " + FString::FromInt(Weapon->GetCurrentBullets()) + "    Clips: " + FString::FromInt(Weapon->GetCurrentClips());
		return String;
	}
	else 
	{
		FString String = "Empty";
		return String;
	}
	
}

void ATestProject_1Character::WeaponInteract()
{
	//ACharacter* Character = Cast<ACharacter>(GetOwner());
	//if (!Character) return;
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform WeaponTransform;
	WeaponTransform.SetLocation(FVector::ZeroVector);
	WeaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));
	Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, WeaponTransform, SpawnParams);
	Weapon->OnClipEmpty.AddUObject(this, &ATestProject_1Character::OnEmptyClip);
	if (Weapon)
	{

		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Weapon_r_hand"));
	
	}
}

void ATestProject_1Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ATestProject_1Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ATestProject_1Character::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATestProject_1Character::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
