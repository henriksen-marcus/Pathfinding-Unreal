// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../DLL.h"
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

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BaseMesh;

	//The start node for the algorithm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra")
	int startNode;

	//The end node for the algorithm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dijkstra")
	int endNode;

	DLL<int> List;

	UWorld* world;

	//function to spawn the nodes
	void spawnNodes();

	//function to run dijkstra´s algorithm
	void Dijkstra();


	//number of nodes to be spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	int NumberOfNodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	TSubclassOf<AMyNode> BP_MyNode;

	TArray<AMyNode*> nodes;



};


