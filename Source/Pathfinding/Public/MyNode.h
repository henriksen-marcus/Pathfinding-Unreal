// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyNode.generated.h"

class AMyNode;

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

	// The current lowest cost we have found from origin to this node in the algorithm
	float CurrentCost;

	// Ptr to the previous node according to the lowest cost we have found, aka "Path via"
	UPROPERTY()
	AMyNode* PreviousNode;

	// The unique name of the node, should be a char like "A"
	FString Name;

	void InitNode(const FString& _Name);
};
