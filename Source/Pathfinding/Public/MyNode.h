// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "MyNode.generated.h"

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
	USphereComponent* Collision;

	// Time to finish, how long the algorithm needs to wait at this
	// node before proceeding. Adds to the cost.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	int32 WaitTime;

	// If the node has been checked/visited by the algorithm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	bool bVisited = false;

	// Reference to all other nodes that are connected
	UPROPERTY()
	TArray<AMyNode*> Connections;
};
