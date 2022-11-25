// Copyright (C) 2022 by Marcus Henriksen


#include "Dijkstra2.h"
#include "MyNode.h"

ADijkstra2::ADijkstra2()
{
	PrimaryActorTick.bCanEverTick = false;
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

	// Safety check
	if (!Nodes.Num() || !Origin || !Destination) return false;

	PriorityQueue.Empty();
	CurrentNode = Origin;
	CurrentNode->bVisited = true;
	CurrentNode->CurrentCost = 0;
	CurrentNode->WaitTime = 0;
	OriginNode = Origin;
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

			// Exit algorithm
			if (NextNode == Destination) return HandleFinish(Nodes);

			// Update the node's position in the priority queue, unless it's the destination node
			if (NextNode != Destination) UpdatePriority(NextNode);
		}
		
		// Now that we've checked all the connections from this node, we remove it
		// from the priority queue so we don't get an infinite loop
		PriorityQueue.Remove(CurrentNode);
		PrintQ();

		// Checked everything but no path
		if (PriorityQueue.IsEmpty()) break;
		
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

void ADijkstra2::UpdatePriority(AMyNode* Node)
{
	UE_LOG(LogTemp, Warning, TEXT("		UpdatePriority()"));
	PriorityQueue.Remove(Node);
	for(int32 i{}; i < PriorityQueue.Num(); i++)
	{
		// Find the right position based on the node's cost
		float NodeCost = Node->CurrentCost + Node->WaitTime;
		float ItemCost = PriorityQueue[i]->CurrentCost + PriorityQueue[i]->WaitTime;
		if (NodeCost < ItemCost)
		{
			UE_LOG(LogTemp, Warning, TEXT("		UpdatePriority(): Moved %s before %s in the Priority Queue."), *Node->Name, *PriorityQueue[i]->Name);
			PriorityQueue.Insert(Node, i);
			return;
		}
	}
	// If the node had a higher cost than all other nodes
	UE_LOG(LogTemp, Warning, TEXT("		UpdatePriority(): Added %s to the end of the Priority Queue."), *Node->Name);
	PriorityQueue.Add(Node);
}

bool ADijkstra2::GenerateTree()
{
	UE_LOG(LogTemp, Warning, TEXT("GenerateTree()"));

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

	UE_LOG(LogTemp, Warning, TEXT("			Shortest path:"));
	
	FString STP = "";
	for (const auto n : ShortestPathTree) { STP += n->Name + " "; }
	UE_LOG(LogTemp, Warning, TEXT("			%s"), *STP);
	
	
	return true;
}

float ADijkstra2::Dist(const AActor* n1, const AActor* n2)
{
	return FVector::Dist(n1->GetActorLocation(), n2->GetActorLocation());
}

void ADijkstra2::PrintQ()
{
	UE_LOG(LogTemp, Warning, TEXT("Priority Queue:"));
	
	FString Q = "";
	for (const auto n : PriorityQueue) { Q += n->Name + " "; }
	
	UE_LOG(LogTemp, Warning, TEXT("			%s"), *Q);
}

bool ADijkstra2::HandleFinish(const TArray<AMyNode*>& Nodes)
{
	if (GenerateTree())
	{
		UE_LOG(LogTemp, Warning, TEXT("Dijkstra succeeded."));
		
		int32 NodesVisited = 0;
		for (const auto Node : Nodes) { if (Node->bVisited) NodesVisited++; }
		
		UE_LOG(LogTemp, Warning, TEXT("Number of nodes visited: %d"), NodesVisited);
		return true;
	}
	return false;
}

