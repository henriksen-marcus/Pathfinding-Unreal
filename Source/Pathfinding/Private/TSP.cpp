// Fill out your copyright notice in the Description page of Project Settings.


#include "TSP.h"
#include "MyNode.h"
#include "algorithm"

// Sets default values
ATSP::ATSP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATSP::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATSP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ATSP::doTSP(TArray<AMyNode*> Nodes, AMyNode* Origin)
{
	//store all nodes except for the start node/Origin
	for (auto Node : Nodes)
	{
		if (Node != Origin)
		{
			Nodes.Push(Node);
		}
	}

	int32 minDistance = INT_MAX; 
	/*do
	{
		//current cost of the total distance
		int32 currentDistCost = 0;

		//goes through the nodes to get currentDistCost
		for (auto Node : Nodes)
		{
			//currentDistCost += Nodes[Origin][Node];
			Origin = Node;
		}
		//currentDistCost += Nodes[Origin][Origin];

		//update the min value of minDistance
		minDistance = std::min(minDistance, currentDistCost);

	} while (std::next_permutation(Nodes.begin(), Nodes.end()));*/

	return minDistance;
}

