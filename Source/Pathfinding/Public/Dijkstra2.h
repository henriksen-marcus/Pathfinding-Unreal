// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	// Stores the sequence of nodes to get to the destination fastest
	TArray<AMyNode*> ShortestPathTree;

	void doDijkstra(TArray<AMyNode*>, int32 Origin, int32 Destination);

};
