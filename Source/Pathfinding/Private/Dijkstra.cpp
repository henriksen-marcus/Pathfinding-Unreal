// Fill out your copyright notice in the Description page of Project Settings.


#include "Dijkstra.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "MyNode.h"
#include "../PathfindingGameModeBase.h"
#include "DrawDebugHelpers.h"


ADijkstra::ADijkstra()
{
	PrimaryActorTick.bCanEverTick = true;


}


void ADijkstra::BeginPlay()
{
	Super::BeginPlay();
	doDijkstra();

	//List.fill(DLL_Base::asc, 0, 100);
	//UE_LOG(LogTemp, Warning, TEXT("List elem 0: %d"), List[50]);
	//List.print();
}


void ADijkstra::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADijkstra::doDijkstra()
{
	gamemode = Cast<APathfindingGameModeBase>(GetWorld()->GetAuthGameMode()); 
	int32 numNodes = gamemode->NumberOfNodes;
	startNode = FMath::RandRange(0, numNodes - 1);
	endNode = FMath::RandRange(0, numNodes - 1);
	
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Green, FString::Printf(TEXT("StartNode position: %d"), startNode));
	GEngine->AddOnScreenDebugMessage(1, 12.f, FColor::Red, FString::Printf(TEXT("EndNode position: %d"), endNode));

	for (int32 i = 0; i < numNodes; i++)
	{
		int32 nearest = getNearest();

	}

	UE_LOG(LogTemp, Warning, TEXT("doDijkstra is running"));
}

void ADijkstra::initialize()
{
	gamemode = Cast<APathfindingGameModeBase>(GetWorld()->GetAuthGameMode());
	int32 numNodes = gamemode->NumberOfNodes;
	TArray<AMyNode*> nodeArray = gamemode->Nodes;

	//mNode = Cast<AMyNode>(GetWorld()->GetAuthGameMode());
	//int32 dist = mNode->distanceValue;
	int32 dist[10]; //array to hold distance values for the nodes

	//to set all the nodes distance to infinity and the startnode distance to 0
	for (int32 i = 0; i < numNodes; i++) 
	{
			 nodeArray[i] = i;
			 dist[i] = INF; //set all nodes distance to infinity
	}

	dist[startNode] = 0; // set the distance of the startnode to 0
}

int ADijkstra::getNearest()
{
	gamemode = Cast<APathfindingGameModeBase>(GetWorld()->GetAuthGameMode());
	int32 numNodes = gamemode->NumberOfNodes;

	mNode = Cast<AMyNode>(GetWorld()->GetAuthGameMode());
	bool visited = mNode->visited;
	int32 dist = mNode->distanceValue;

	int32 minDistVal = INF; //the shortest distance to the nearest node
	int32 minNode = 0; //minimum value node

	//goes through the number of nodes and checks if the nodes has
	//been visited and the distance to the nodes is lower than the minimum distance  
	for (int32 i = 0; i < numNodes; i++)
	{

		if (!visited[i] && dist[i] < minDistVal)
		{
			minDistVal = dist[i]; //update the minimum distance 
			minNode = i; //update which node we are on
		}
	}
	return minNode; //return the nearest node 
}



