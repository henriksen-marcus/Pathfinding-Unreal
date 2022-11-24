// Copyright (c) 2022 by Marcus Henriksen\nPublished under BSL 1.0

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "PlayerPawn.generated.h"

/**
 * 
 */
UCLASS()
class PATHFINDING_API APlayerPawn : public ADefaultPawn
{
	GENERATED_BODY()

	APlayerPawn();

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	class UCameraComponent* Camera;

	UPROPERTY()
	class APathfindingGameModeBase* GameModeBase;

	void SwitchPawn();
	
};


