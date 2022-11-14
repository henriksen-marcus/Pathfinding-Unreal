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


	UE_LOG(LogTemp, Warning, TEXT("doDijkstra is running"));
}



