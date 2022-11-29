// Copyright (C) 2022 by Marcus Henriksen

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "PlayerPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoveSignaturePP);

/**
 * Derived from UE's default pawn. Able to float
 * around in the world freely.
 */
UCLASS()
class PATHFINDING_API APlayerPawn : public ADefaultPawn
{
	GENERATED_BODY()

	APlayerPawn();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	FOnMoveSignaturePP OnMoveDelegate;

	UPROPERTY()
	class UCameraComponent* Camera;

	UPROPERTY()
	class APathfindingGameModeBase* GameModeBase;

	void SwitchPawn();
};


