// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyNode.h"
#include "GameFramework/Actor.h"
#include "Dijkstra2.generated.h"

class AMyNode;

UCLASS()
class PATHFINDING_API ADijkstra2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADijkstra2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
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
	void GenerateTree();

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
};


