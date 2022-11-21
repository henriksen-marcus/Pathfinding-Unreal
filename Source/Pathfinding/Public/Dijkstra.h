// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyNode.h"
#include "Dijkstra.generated.h"

UCLASS()
class PATHFINDING_API ADijkstra : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADijkstra();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//The start node for the algorithm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra")
	int32 startNode;

	//The end node for the algorithm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra")
	int32 endNode;

	//DLL<int> List;

	//UWorld* world;

	//function to run dijkstra�s algorithm
	void doDijkstra(TArray<AMyNode*>, int32 Origin, int32 Destination);

	class APathfindingGameModeBase* gamemode;
	class AMyNode* mNode;

	//function to get the nearest node 
	int32 getNearest(AMyNode* current);



};


