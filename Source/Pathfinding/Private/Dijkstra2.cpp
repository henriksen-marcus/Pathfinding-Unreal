// Fill out your copyright notice in the Description page of Project Settings.


#include "Dijkstra2.h"
#include "MyNode.h"

ADijkstra2::ADijkstra2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADijkstra2::BeginPlay()
{
	Super::BeginPlay();
}

void ADijkstra2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ADijkstra2::Start(const TArray<AMyNode*> Nodes, AMyNode* Origin, AMyNode* Destination)
{
	UE_LOG(LogTemp, Warning, TEXT("Start()"));

	CurrentNode = Origin;
	CurrentNode->bVisited = true;
	CurrentNode->CurrentCost = 0;
	CurrentNode->WaitTime = 0;
	DestinationNode = Destination;

	// Add each node to the priority queue
	// Each node comes pre-init with max distance cost
	for (const auto Node : Nodes)
	{
		// Don't add the destination node as we don't want
		// to check any path going FROM the destination
		if (Node != Destination)
		{
			PriorityQueue.Add(Node);
			UE_LOG(LogTemp, Warning, TEXT("Added %s to Priority Queue"), *Node->Name);
		}
	}
	PrintQ();

	// Algorithm runs as long as there are items left in the priority queue.
	// This means that we will check all nodes before concluding the shortest path.
	int32 loops = 0;
	while (loops++ < 500)
	{
		// Step 1: Update estimates
		// Go through each connected node and update the nodes CurrentCost and PreviousNode
		
		UE_LOG(LogTemp, Warning, TEXT("CurrentNode: %s"), *CurrentNode->Name);
		for (int32 i{}; i < CurrentNode->Connections.Num(); i++)
		{
			AMyNode* NextNode = CurrentNode->Connections[i];

			// Avoid infinite loop of checking visited nodes
			if (NextNode == PreviousNode || NextNode == CurrentNode->PreviousNode || NextNode == CurrentNode) continue;
			
			float TotalCost = Dist(CurrentNode, NextNode) + NextNode->WaitTime + CurrentNode->CurrentCost;
			
			// Add cost of previous nodes back to origin
			/*AMyNode* Previous = CurrentNode->PreviousNode;
			while (Previous && Previous != Origin) // We don't add the cost of the first node
			{
				TotalCost += Previous->CurrentCost;
				Previous = Previous->PreviousNode;
			}*/
			
			UE_LOG(LogTemp, Warning, TEXT("		Checking connection to %s"), *NextNode->Name);
			UE_LOG(LogTemp, Warning, TEXT("		Cost to node:"));
			UE_LOG(LogTemp, Warning, TEXT("			Distance: %f"), Dist(CurrentNode, NextNode));
			UE_LOG(LogTemp, Warning, TEXT("			WaitTime NextNode: %f"), NextNode->WaitTime);
			UE_LOG(LogTemp, Warning, TEXT("			WaitTime CurrentNode %f"), CurrentNode->WaitTime);
			UE_LOG(LogTemp, Warning, TEXT("			Previous Nodes: %f"), CurrentNode->CurrentCost - CurrentNode->WaitTime);
			UE_LOG(LogTemp, Warning, TEXT("			Total: %f"), TotalCost);
			UE_LOG(LogTemp, Warning, TEXT("		Node visited: %s"), NextNode->bVisited ? TEXT("true") : TEXT("false"));
		
			// Skip check if node already has a shorter path!
			if (NextNode->bVisited && NextNode->CurrentCost <= TotalCost)
			{
				UE_LOG(LogTemp, Warning, TEXT("		Skipped updating %s, shorter path already exists"), *NextNode->Name);
				continue;
			}
			// We don't want to update this if there is already a shorter path to this node
			NextNode->CurrentCost = TotalCost;
			NextNode->PreviousNode = CurrentNode; // Just like in linked list
			NextNode->bVisited = true;

			// Update the node's position in the priority queue, unless it's the destination node
			if (NextNode != Destination) UpdatePriority(NextNode);
		}
		
		// Now that we've checked all the connections from this node, we remove it
		// from the priority queue so we don't get an infinite loop
		PriorityQueue.Remove(CurrentNode);
		PrintQ();
		
		// Step 2: Choose next node
		// We always choose the node at the top of the priority queue,
		// meaning that we can jump across the map between nodes depending
		// on what is the current shortest, unexplored path we can check

		if (!PriorityQueue.Num())
		{
			GenerateTree();
			UE_LOG(LogTemp, Warning, TEXT("Dijkstra succeeded."));
			return true;
		}
		PreviousNode = CurrentNode;
		CurrentNode = PriorityQueue[0];
	}
	UE_LOG(LogTemp, Warning, TEXT("Not possible to get to destination!"));
	return false;
}

void ADijkstra2::UpdatePriority(AMyNode* Node)
{
	UE_LOG(LogTemp, Warning, TEXT("		UpdatePriority()"));
	PriorityQueue.Remove(Node);
	for(int32 i{}; i < PriorityQueue.Num(); i++)
	{	// Find the right position based on the node's cost
		if (Node->CurrentCost + Node->WaitTime < PriorityQueue[i]->CurrentCost + PriorityQueue[i]->WaitTime)
		{
			UE_LOG(LogTemp, Warning, TEXT("		UpdatePriority(): Moved %s before %s in the Priority Queue."), *Node->Name, *PriorityQueue[i]->Name);
			PriorityQueue.Insert(Node, i);
			return;
		}
	}
	// If the node had a higher cost than all other nodes
	UE_LOG(LogTemp, Warning, TEXT("		UpdatePriority(): Updated %s in the Priority Queue."), *Node->Name);
	PriorityQueue.Add(Node);
}

void ADijkstra2::GenerateTree()
{
	UE_LOG(LogTemp, Warning, TEXT("GenerateTree(): Shortest path:"));

	ShortestPathTree.Empty();
	ShortestPathTree.Add(DestinationNode);
	AMyNode* Previous = DestinationNode->PreviousNode;
	while (Previous)
	{
		ShortestPathTree.Add(Previous);
		Previous = Previous->PreviousNode;
	}

	Algo::Reverse(ShortestPathTree);
	
	for (int32 i{}; i < ShortestPathTree.Num(); i++)
	{
		if (ShortestPathTree.Num() >= i + 2)
		{
			FVector Loc1 = ShortestPathTree[i]->GetActorLocation();
			FVector Loc2 = ShortestPathTree[i+1]->GetActorLocation();
			DrawDebugDirectionalArrow(GetWorld(), Loc1, Loc2, 1000, FColor::Red, true, -1, 0, 5);
		}
	}

	for (const auto n : ShortestPathTree)
	{
		UE_LOG(LogTemp, Warning, TEXT("		%s"), *n->Name);
	}
}

float ADijkstra2::Dist(const AActor* n1, const AActor* n2)
{
	return FVector::Dist(n1->GetActorLocation(), n2->GetActorLocation());
}

void ADijkstra2::PrintQ()
{
	UE_LOG(LogTemp, Warning, TEXT("Priority Queue:"));
	for(const auto Node : PriorityQueue)
	{
		UE_LOG(LogTemp, Warning, TEXT("		%s"), *Node->Name);
	}
}

