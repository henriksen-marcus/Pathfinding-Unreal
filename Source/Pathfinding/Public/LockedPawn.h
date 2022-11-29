// Copyright (C) 2022 by Marcus Henriksen

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LockedPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoveSignature);

/**
 * Spectator pawn that rotates around the center
 * of the map with simple controls.
 */
UCLASS()
class PATHFINDING_API ALockedPawn : public APawn
{
	GENERATED_BODY()

public:
	ALockedPawn();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FOnMoveSignature OnMoveDelegate;

	UPROPERTY(EditAnywhere, Category = "Node")
	class USphereComponent* Collision;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	// The target arm length for the spring arm, used for interpolation
	float TargetLength;

	FVector PreviousFrameLocation = FVector::ZeroVector;

	void RotateRight(const float Value);
	void RotateUp(const float Value);
	void ZoomIn();
	void ZoomOut();
};
