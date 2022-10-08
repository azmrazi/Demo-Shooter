// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp/PickUpWeapon.h"
#include "Components/SphereComponent.h"
#include "TestProject_1/TestProject_1Character.h"


DEFINE_LOG_CATEGORY_STATIC(LogWeaponPickup, All, All)
// Sets default values
APickUpWeapon::APickUpWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(CollisionComponent);
}

// Called when the game starts or when spawned
void APickUpWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUpWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickUpWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	UE_LOG(LogWeaponPickup, Display, TEXT("Pickup was taken"));

	auto* Character = Cast<ATestProject_1Character>(OtherActor);
	if (Character != nullptr)
		Character->WeaponInteract();
	Destroy();

}