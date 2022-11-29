// Copyright (c) 2022 by Marcus Henriksen\nPublished under BSL 1.0


#include "PlayerPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Pathfinding/PathfindingGameModeBase.h"

APlayerPawn::APlayerPawn()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetFieldOfView(110.f);
	Camera->SetupAttachment(GetRootComponent());
	Camera->bUsePawnControlRotation = true;

	// lulz
	GetMeshComponent()->SetStaticMesh(nullptr);
	GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(FVector(3000.f, 0.f, 0.f));
	// I'm not doing math rn ok?
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector()));
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyZero(GetMovementComponent()->Velocity.Size())) OnMoveDelegate.Broadcast();
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Escape"), EInputEvent::IE_Pressed, this, &APlayerPawn::SwitchPawn);
}

void APlayerPawn::SwitchPawn()
{
	GameModeBase->SwitchPawn();
}

