// Fill out your copyright notice in the Description page of Project Settings.


#include "Dijkstra.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "MyNode.h"
#include "DrawDebugHelpers.h"

// Sets default values
ADijkstra::ADijkstra()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");

	NumberOfNodes = 10;

}

// Called when the game starts or when spawned
void ADijkstra::BeginPlay()
{
	Super::BeginPlay();
	spawnNodes();
	Dijkstra();

	List.fill(DLL_Base::asc, 0, 100);
	UE_LOG(LogTemp, Warning, TEXT("List elem 0: %d"), List[50]);
	//List.print();
}

// Called every frame
void ADijkstra::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADijkstra::spawnNodes()
{

	if (nodes.Num() > 0)
	{
		return; 
	}

	world = GetWorld();
	
	if (world != nullptr)
	{
		float x;
		float y;
		float z;

		for (int i{}; i < NumberOfNodes; i++)
		{
			x = FMath::FRandRange(-1000.f, 1000.f);
			y = FMath::FRandRange(-1000.f, 1000.f);
			z = FMath::FRandRange(-1000.f, 1000.f);

			AMyNode* newNode = world->SpawnActor<AMyNode>(BP_MyNode, FVector(x, y, z), FRotator(0, 0, 0));
			nodes.Add(newNode);  
			
		}
		UE_LOG(LogTemp, Warning, TEXT("Nodes spawned"));
	}


}

void ADijkstra::Dijkstra()
{
	startNode = FMath::RandRange(0, NumberOfNodes - 1);
	int tempstart = startNode;
	endNode = FMath::RandRange(0, NumberOfNodes - 1);
	int tempend = endNode;
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Green, FString::Printf(TEXT("StartNode position: %d"), tempstart));
	GEngine->AddOnScreenDebugMessage(1, 12.f, FColor::Red, FString::Printf(TEXT("EndNode position: %d"), tempend));



}



