// Copyright (C) 2022 by Marcus Henriksen

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LockedPawn.generated.h"

UCLASS()
class PATHFINDING_API ALockedPawn : public APawn
{
	GENERATED_BODY()

public:
	ALockedPawn();

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Node")
	class USphereComponent* Collision;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	// The target arm length for the spring arm
	float TargetLength;

	void RotateRight(const float Value);
	void RotateUp(const float Value);
	void ZoomIn();
	void ZoomOut();

};
