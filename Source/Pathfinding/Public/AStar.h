// Copyright (C) 2022 by Marcus Henriksen

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AStar.generated.h"

class AMyNode;

UCLASS()
class PATHFINDING_API AAStar : public AActor
{
	GENERATED_BODY()
	
public:	
	AAStar();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Contains the chronological path to get from the origin node
	// to the destination node.
	// The SPT is not used until we find the destination node, where
	// we backtrack to origin using Node->Previous to get the shortest
	// path. Then the array is inverted to get the correct order.
	UPROPERTY()
	TArray<AMyNode*> ShortestPathTree;

	// Tells us which node to check next
	UPROPERTY()
	TArray<AMyNode*> PriorityQueue;

	UPROPERTY()
	AMyNode* OriginNode;

	UPROPERTY()
	AMyNode* CurrentNode;

	UPROPERTY()
	AMyNode* PreviousNode;

	UPROPERTY()
	AMyNode* DestinationNode;

	/**
	 * @brief Starts the pathfinding algorithm
	 * @param Nodes Array of pointers to the spawned nodes
	 * @param Origin The starting node
	 * @param Destination The destination node
	 */
	bool Start(const TArray<AMyNode*> Nodes, AMyNode* Origin, AMyNode* Destination);

	/**
	 * @brief Moves a node into the correct position in the
	 * Priority Queue, based on the node's cost
	 * @param Node The node to update
	 */
	void UpdatePriority(AMyNode* Node);

	/**
	 * @brief Iterates through the nodes and finds the path back to
	 * the origin node. Adds the path to ShortestPathTree in the
	 * correct order.
	 */
	bool GenerateTree();

	/**
	 * @param n1 The first node
	 * @param n2 The second node
	 * @return The distance between the two nodes
	 * @remarks This function is just to shorten down
	 * some lines of code.
	 */
	static float Dist(const AActor* n1, const AActor* n2);

	/**
	* @brief Logs each item currently in the Priority Queue
	*/
	void PrintQ();

	/**
	 * @brief Handle extra actions to be done before returning.
	 * @param Nodes The array of nodes we have processed.
	 * @return If the algorithm result is valid.
	 */
	bool HandleFinish(const TArray<AMyNode*>& Nodes);
};
