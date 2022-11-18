// Fill out your copyright notice in the Description page of Project Settings.


#include "Dijkstra.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "MyNode.h"
#include "../PathfindingGameModeBase.h"
#include "DrawDebugHelpers.h"
#include "Math/Vector.h"


ADijkstra::ADijkstra()
{
	PrimaryActorTick.bCanEverTick = true;


}


void ADijkstra::BeginPlay()
{
	Super::BeginPlay();
	gamemode = Cast<APathfindingGameModeBase>(GetWorld()->GetAuthGameMode());

	//List.fill(DLL_Base::asc, 0, 100);
	//UE_LOG(LogTemp, Warning, TEXT("List elem 0: %d"), List[50]);
	//List.print();
}


void ADijkstra::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADijkstra::doDijkstra(TArray<AMyNode*> Nodes, int32 Origin, int32 Destination)
{
	// nearest gives the index to the nearest node from current node
	int32 nearest = Origin;


	for (int32 i = 0; i < gamemode->NumberOfNodes; i++)
	{
		//add to array of visited nodes with the shortest path
		//
		nearest = getNearest(Nodes[nearest]);

	}
	
	UE_LOG(LogTemp, Warning, TEXT("doDijkstra is running"));
}


int32 ADijkstra::getNearest(AMyNode* current)
{
	if (!gamemode) return -1;

	int32 minDistVal = MAX_int32;

	//for the number of connections each node has with other nodes
	//current->Connections.Num();

	for (auto node : gamemode->Nodes)
	{
		int32 dist = FVector::Dist(current->GetActorLocation(), node->GetActorLocation());
		if (!node->bVisited && dist < minDistVal)
		{
			minDistVal = dist;
		}
		
	}

	return minDistVal;

}



