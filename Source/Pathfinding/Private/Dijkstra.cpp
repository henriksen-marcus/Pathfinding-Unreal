// Fill out your copyright notice in the Description page of Project Settings.


#include "Dijkstra.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "MyNode.h"
#include "DrawDebugHelpers.h"


ADijkstra::ADijkstra()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");

	NumberOfNodes = 10;

}


void ADijkstra::BeginPlay()
{
	Super::BeginPlay();
	spawnNodes();
	doDijkstra();

	//List.fill(DLL_Base::asc, 0, 100);
	//UE_LOG(LogTemp, Warning, TEXT("List elem 0: %d"), List[50]);
	//List.print();
}


void ADijkstra::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADijkstra::spawnNodes()
{
	
}

void ADijkstra::doDijkstra()
{
	startNode = FMath::RandRange(0, NumberOfNodes - 1);
	int tempstart = startNode;
	endNode = FMath::RandRange(0, NumberOfNodes - 1);
	int tempend = endNode;
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Green, FString::Printf(TEXT("StartNode position: %d"), tempstart));
	GEngine->AddOnScreenDebugMessage(1, 12.f, FColor::Red, FString::Printf(TEXT("EndNode position: %d"), tempend));
}



