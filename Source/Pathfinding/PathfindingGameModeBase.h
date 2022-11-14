// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PathfindingGameModeBase.generated.h"

/**
 * The class that controls and keeps the top level functions and
 * variables to use in all the pathfinding algorithms.
 */
UCLASS()
class PATHFINDING_API APathfindingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	APathfindingGameModeBase();

	virtual void BeginPlay() override;

	// Spawns nodes with a random location within the specified bounds
	void SpawnNodes();

	// The number of nodes to spawn
	UPROPERTY(EditAnywhere, Category = "Node")
	int32 NumberOfNodes;

	// Array containing pointers to all spawned nodes
	UPROPERTY(EditAnywhere, Category = "Node")
	TArray<class AMyNode*> Nodes;

	UPROPERTY(EditAnywhere, Category = "Node")
	TSubclassOf<AMyNode> BP_MyNode;
	
};
