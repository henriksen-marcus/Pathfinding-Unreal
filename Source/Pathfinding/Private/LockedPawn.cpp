// Copyright (C) 2022 by Marcus Henriksen


#include "LockedPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"

ALockedPawn::ALockedPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(Collision);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->TargetArmLength = 3000.f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->bEnableCameraRotationLag = true;
    SpringArm->CameraRotationLagSpeed = 15.f;
	SpringArm->bDoCollisionTest = false;
    SpringArm->SetupAttachment(GetRootComponent());

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("MyCamera"));
    //Camera->bUsePawnControlRotation = false;
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeLocation(FVector());

	TargetLength = SpringArm->TargetArmLength;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ALockedPawn::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(FVector());
}

void ALockedPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Interp the arm length for the spring arm, for smoother movement
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetLength, DeltaTime, 15.f);

	
	FVector MovementDelta = Camera->GetComponentLocation() - PreviousFrameLocation;
	PreviousFrameLocation = Camera->GetComponentLocation();

	// If this is positive, we are moving
	if (MovementDelta.Size())
	{
		OnMoveDelegate.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("Moving. %f"), MovementDelta.Size());
	}
}

void ALockedPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ScrollUp", IE_Pressed, this, &ALockedPawn::ZoomIn);
	PlayerInputComponent->BindAction("ScrollDown", IE_Pressed, this, &ALockedPawn::ZoomOut);

	PlayerInputComponent->BindAxis("Up", this, &ALockedPawn::RotateUp);
	PlayerInputComponent->BindAxis("Right", this, &ALockedPawn::RotateRight);
}

void ALockedPawn::RotateRight(const float Value)
{
	SpringArm->AddRelativeRotation(FRotator(0, Value * GetWorld()->GetDeltaSeconds() * 110 * -1, 0));
}

void ALockedPawn::RotateUp(const float Value)
{
	FRotator NewRot = SpringArm->GetRelativeRotation();
	NewRot.Pitch = FMath::Clamp(SpringArm->GetRelativeRotation().Pitch + Value * GetWorld()->GetDeltaSeconds() * 90 * -1, -80.f, 80.f);
	SpringArm->SetRelativeRotation(NewRot);
}

void ALockedPawn::ZoomIn()
{
	TargetLength = FMath::Clamp(TargetLength - 300.f, 500.f, 5000.f);
}

void ALockedPawn::ZoomOut()
{
	TargetLength = FMath::Clamp(TargetLength + 300.f, 500.f, 5000.f);
}

