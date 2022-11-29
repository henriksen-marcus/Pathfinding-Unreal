// Copyright (C) 2022 by Marcus Henriksen

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyNode.generated.h"


/**
 * Used as a position placeholder, used for
 * pathfinding algorithms.
 */
UCLASS()
class PATHFINDING_API AMyNode : public AActor
{
	GENERATED_BODY()
	
public:
	AMyNode();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Collision component, lets nodes overlap and detect each other
	UPROPERTY(EditAnywhere, Category = "Node")
	class USphereComponent* Collision;

	// Rendered text to display the name of the node
	UPROPERTY()
	class UTextRenderComponent* NameDisplay;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* TextMaterial;

	// Time to finish, adds to the cost
	UPROPERTY(VisibleAnywhere, Category = "Node")
	float WaitTime;

	// If the node has been checked/visited by the algorithm
	UPROPERTY(VisibleAnywhere, Category = "Node")
	bool bVisited = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Node")
	TArray<AMyNode*> Connections;

	// The current lowest cost we have found from origin to this node in the algorithm.
	// NOT heuristic.
	float CurrentCost;

	// Used in A* algorithm
	float Heuristic = FLT_MAX;

	// Ptr to the previous node according to the lowest cost we have found, aka "Path via"
	UPROPERTY()
	AMyNode* PreviousNode;

	// The unique name of the node, should be a char like "A"
	FString Name;

	// Numbers gotten from the task
	TArray<float> TimeToFinishArr = {5, 8, 10, 12, 15, 18, 22, 26};

	/**
	 * @brief Set the name of the node and update it's overhead text component
	 * @param _Name The name of the node
	 */
	void InitNode(const FString& _Name);
};
