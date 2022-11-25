// Copyright (C) 2022 by Marcus Henriksen


#include "AStar.h"
#include "MyNode.h"
#include "Kismet/GameplayStatics.h"

AAStar::AAStar()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAStar::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAStar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AAStar::Start(const TArray<AMyNode*> Nodes, AMyNode* Origin, AMyNode* Destination)
{
	UE_LOG(LogTemp, Warning, TEXT("Start()"));

	// Safety check
	if (!Nodes.Num() || !Origin || !Destination) return false;

	// Add each node to the priority queue
	// Each node comes pre-init with max distance cost
	for (const auto Node : Nodes)
	{
		Node->Heuristic = Dist(Node, Destination);
		//if (Node != Destination) PriorityQueue.Add(Node);
	}

	CurrentNode = Origin;
	CurrentNode->bVisited = true;
	CurrentNode->CurrentCost = 0;
	CurrentNode->WaitTime = 0;
	OriginNode = Origin;
	DestinationNode = Destination;
	PrintQ();

	int32 loops = 0;
	while (loops++ < 500)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentNode: %s"), *CurrentNode->Name);
		for (int32 i{}; i < CurrentNode->Connections.Num(); i++)
		{
			AMyNode* NextNode = CurrentNode->Connections[i];

			// Avoid infinite loop of checking visited nodes. Don't check parent
			if (NextNode == PreviousNode || NextNode == CurrentNode->PreviousNode || NextNode == CurrentNode) continue;

			// Used to determine if we should update the node we are checking, does not include heuristic value
			// Distance from current node + the wait time + the previous costs, not including heuristic
			float TotalCost = Dist(CurrentNode, NextNode) + NextNode->WaitTime + CurrentNode->CurrentCost;

			UE_LOG(LogTemp, Warning, TEXT("		Checking connection to %s"), *NextNode->Name);
			UE_LOG(LogTemp, Warning, TEXT("		Cost to node:"));
			UE_LOG(LogTemp, Warning, TEXT("			Distance: %f"), Dist(CurrentNode, NextNode));
			UE_LOG(LogTemp, Warning, TEXT("			WaitTime NextNode: %f"), NextNode->WaitTime);
			UE_LOG(LogTemp, Warning, TEXT("			WaitTime CurrentNode %f"), CurrentNode->WaitTime);
			UE_LOG(LogTemp, Warning, TEXT("			Previous Nodes: %f"), CurrentNode->CurrentCost - CurrentNode->WaitTime);
			UE_LOG(LogTemp, Warning, TEXT("			Heuristic: NextNode: %f"), NextNode->Heuristic);
			UE_LOG(LogTemp, Warning, TEXT("			Heuristic CurrentNode: %f"), CurrentNode->Heuristic);
			UE_LOG(LogTemp, Warning, TEXT("			Total: %f"), TotalCost + NextNode->Heuristic);
			UE_LOG(LogTemp, Warning, TEXT("		Node visited: %s"), NextNode->bVisited ? TEXT("true") : TEXT("false"));

			// Skip check if node already has a shorter path!
			if (NextNode->bVisited && NextNode->CurrentCost <= TotalCost)
			{
				UE_LOG(LogTemp, Warning, TEXT("		Skipped updating %s, shorter path already exists"), *NextNode->Name);
				continue;
			}

			// We don't want to update this if there is already a shorter path to this node
			NextNode->CurrentCost = TotalCost;
			NextNode->PreviousNode = CurrentNode; // Update parent
			NextNode->bVisited = true;

			// Update the node's position/Add it to the priority queue, unless it's the destination node
			if (NextNode != Destination) UpdatePriority(NextNode);
		}

		// Now that we've checked all the connections from this node, we remove it
		// from the priority queue so we don't get an infinite loop
		PriorityQueue.Remove(CurrentNode);
		PrintQ();

		/*bool AreWeDone = true;
		for (const auto Node : Nodes)
		{
			if (Node->CurrentCost < CurrentNode->CurrentCost)
			{
				AreWeDone = false;
				break;
			}
		}
		
		if (AreWeDone)
		{
			GenerateTree();
			return true;
		}*/

		if (!PriorityQueue.Num())
		{
			if (GenerateTree())
			{
				UE_LOG(LogTemp, Warning, TEXT("AStar succeeded."));
				return true;
			}
			return false;
		}
		
		// Step 2: Choose next node
		// We always choose the node at the top of the priority queue,
		// meaning that we can jump across the map between nodes depending
		// on what is the current shortest, unexplored path we can check
		
		PreviousNode = CurrentNode;
		CurrentNode = PriorityQueue[0];
	}
	UE_LOG(LogTemp, Warning, TEXT("Not possible to get to destination!"));
	return false;
}

void AAStar::UpdatePriority(AMyNode* Node)
{
	UE_LOG(LogTemp, Warning, TEXT("		UpdatePriority()"));
	PriorityQueue.Remove(Node);
	for(int32 i{}; i < PriorityQueue.Num(); i++)
	{
		// Find the right position based on the node's cost, NOTE: This is where we use the heuristic value!
		float NodeCost = Node->CurrentCost + Node->WaitTime + Node->Heuristic;
		float ItemCost = PriorityQueue[i]->CurrentCost + PriorityQueue[i]->WaitTime + PriorityQueue[i]->Heuristic;
		if (NodeCost < ItemCost)
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

bool AAStar::GenerateTree()
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
	// Check if we actually found a complete path
	if (ShortestPathTree.Find(OriginNode) == INDEX_NONE) return false;

	Algo::Reverse(ShortestPathTree);
	/*
	for (int32 i{}; i < ShortestPathTree.Num(); i++)
	{
		if (ShortestPathTree.Num() >= i + 2)
		{
			FVector Loc1 = ShortestPathTree[i]->GetActorLocation();
			FVector Loc2 = ShortestPathTree[i+1]->GetActorLocation();
			DrawDebugDirectionalArrow(GetWorld(), Loc1, Loc2, 1000, FColor::Red, true, -1, 0, 5);
		}
	}
	*/
	for (const auto n : ShortestPathTree)
	{
		UE_LOG(LogTemp, Warning, TEXT("		%s"), *n->Name);
	}
	return true;
}

float AAStar::Dist(const AActor* n1, const AActor* n2)
{
	return FVector::Dist(n1->GetActorLocation(), n2->GetActorLocation());
}

void AAStar::PrintQ()
{
	UE_LOG(LogTemp, Warning, TEXT("Priority Queue:"));
	for(const auto Node : PriorityQueue)
	{
		UE_LOG(LogTemp, Warning, TEXT("		%s"), *Node->Name);
	}
}

