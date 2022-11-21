// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Public/MyNode.h"
#include "PathfindingGameModeBase.generated.h"

class AMyNode;

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

public:
	UPROPERTY(EditAnywhere, Category = "Node")
	TSubclassOf<AMyNode> BP_MyNode;
		
	// The number of nodes to spawn
	UPROPERTY(EditAnywhere, Category = "Node")
	int32 NumberOfNodes;

	// Radius from each node before it connects with another
	UPROPERTY(EditAnywhere, Category = "Node")
	float NodeConnectionRadius;

	// The size of the sphere marking each node
	UPROPERTY(EditAnywhere, Category = "Node")
	float NodeSize;

	// The required distance between each node
	UPROPERTY(EditAnywhere, Category = "Node")
	float NodeDist;

	// Array containing pointers to all spawned nodes
	UPROPERTY(VisibleAnywhere, Category = "Node")
	TArray<AMyNode*> Nodes;

	// Location of the previous node spawned
	FVector PrevLoc;

	// Whether to draw the bounds that the nodes spawn in
	UPROPERTY(EditAnywhere)
	bool DrawBounds;
	
	UPROPERTY(EditAnywhere)
	float ArrowSize;

	TArray<FString> Alphabet;
	

	//---------------- Functions -------------------//

	/**
	 * @brief Spawns nodes with a random location within the specified bounds
	 */
	void SpawnNodes();

	/**
	 * @brief Iterates through each node and makes connections to the closest ones
	 */
	void SetupNodeConnections();

	/**
	 * @brief Add each node to each other's connection arrays,
	 * and draw a line between them.
	 */
	void MakeConnection(AMyNode* n1, AMyNode* n2);

	/**
	 * @brief Draw debug lines showing the shortest path
	 * from the origin node to the destination node
	 * @param SPT Shortest Path Tree
	 */
	void DrawPath(TArray<AMyNode*> SPT);

	/**
	 * @brief Check if item exists in arr
	 * @tparam T Datatype
	 * @param arr The array to search in
	 * @param item The item to search for
	 * @return Whether the item was found in arr.
	 */
	template<class T>
	bool In(TArray<T> arr, T item);
};

template <class T>
bool APathfindingGameModeBase::In(TArray<T> Arr, T Item)
{
	for (const auto obj : Arr)
	{
		if (obj == Item) return true;
	}
	return false;
}
