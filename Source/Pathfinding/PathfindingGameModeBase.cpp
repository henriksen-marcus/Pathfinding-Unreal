// Copyright Epic Games, Inc. All Rights Reserved.


#include "PathfindingGameModeBase.h"

#include "Engine/TextRenderActor.h"
#include "Kismet/GameplayStatics.h"
#include "Public/MyNode.h"
#include "Engine/World.h"
#include "Materials/MaterialExpressionChannelMaskParameter.h"
#include "Public/Dijkstra2.h"

APathfindingGameModeBase::APathfindingGameModeBase()
{
	NumberOfNodes = 20;
	NodeConnectionRadius = 500.f;
	NodeDist = 100.f;
	DrawBounds = false;
	ArrowSize = 1000.f;
	NodeSize = 15.f;
	Alphabet = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
}

void APathfindingGameModeBase::BeginPlay()
{
	if (NumberOfNodes < 2) NumberOfNodes = 2;
	
	SpawnNodes();

	for (auto ni : Nodes[0]->Connections)
	{
		UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *ni->Name);
	}
	/*Nodes.Add(GetWorld()->SpawnActor<AMyNode>(BP_MyNode, FVector(500, 0, 0), FRotator(0.f)));
	Nodes.Add(GetWorld()->SpawnActor<AMyNode>(BP_MyNode, FVector(0, 0, 0), FRotator(0.f)));
	Nodes.Add(GetWorld()->SpawnActor<AMyNode>(BP_MyNode, FVector(0, 500, 0), FRotator(0.f)));
	Nodes.Add(GetWorld()->SpawnActor<AMyNode>(BP_MyNode, FVector(500, 500, 0), FRotator(0.f)));

	Nodes[0]->InitNode("A");
	Nodes[1]->InitNode("B");
	Nodes[2]->InitNode("C");
	Nodes[3]->InitNode("D");

	DrawDebugSphere(GetWorld(), Nodes[0]->GetActorLocation(), 20, 8, FColor::Green, true, -1, 0, 2);
	DrawDebugSphere(GetWorld(), Nodes[1]->GetActorLocation(), 20, 8, FColor::Blue, true);
	DrawDebugSphere(GetWorld(), Nodes[2]->GetActorLocation(), 20, 8, FColor::Red, true, -1, 0, 2);
	DrawDebugSphere(GetWorld(), Nodes[3]->GetActorLocation(), 20, 8, FColor::Blue, true);

	
	Nodes[0]->Connections.Add(Nodes[1]);
	Nodes[1]->Connections.Add(Nodes[0]);
	DrawLine(Nodes[0], Nodes[1]);
	
	Nodes[0]->Connections.Add(Nodes[3]);
	Nodes[3]->Connections.Add(Nodes[0]);
	DrawLine(Nodes[0], Nodes[3]);

	
	Nodes[1]->Connections.Add(Nodes[2]);
	Nodes[2]->Connections.Add(Nodes[1]);
	DrawLine(Nodes[1], Nodes[2]);

	Nodes[2]->Connections.Add(Nodes[3]);
	Nodes[3]->Connections.Add(Nodes[2]);
	DrawLine(Nodes[2], Nodes[3]);

	UpdateNodeOverlapSpheres();*/
	
	ADijkstra2* Dijkstra = Cast<ADijkstra2>(GetWorld()->SpawnActor(ADijkstra2::StaticClass()));
	if (Dijkstra)
	{
		AMyNode* OriginNode = Nodes[0];
		AMyNode* DestinationNode = Nodes[1];
		
		bool success = Dijkstra->Start(Nodes, OriginNode, DestinationNode);
		//if (success) DrawPath(Dijkstra->ShortestPathTree);
		//else UE_LOG(LogTemp, Warning, TEXT("Dijkstra failed."));
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Could not cast!")); }

	
}

void APathfindingGameModeBase::SpawnNodes()
{
	// Reset
	if (Nodes.Num())
	{
		for (auto Node : Nodes)
		{
			Node->NameDisplay->Destroy();
			Node->Destroy();
		}
		Nodes.Empty();
		return;
	}
	
	const FBox Box = FBox(FVector3d(-1000), FVector3d(1000));
	FVector RandPoint;

	if (DrawBounds)
	{
		DrawDebugBox(GetWorld(), FVector(0), FVector(1000), FQuat(0), FColor::White, true);
	}

	for (int32 i{}; i < NumberOfNodes; i++)
	{
		// Continue generating new random position as long as we
		// don't meet the distance between each node requirement
		int32 loops = 0;
		do
		{
			RandPoint = FMath::RandPointInBox(Box);
		}
		while (FVector::Dist(PrevLoc, RandPoint) < NodeDist && loops++ < 50);
		
		AMyNode* NewNode = GetWorld()->SpawnActor<AMyNode>(BP_MyNode, RandPoint, FRotator());
		NewNode->Collision->SetSphereRadius(NodeConnectionRadius);
		FString Name = (i > Alphabet.Num() - 1) ? Alphabet[i - 26] + "1" : Alphabet[i];
		NewNode->InitNode(Name);
		
		switch (i)
		{
		case 0: // Starting node
			DrawDebugSphere(GetWorld(), RandPoint, NodeSize, 12, FColor::Green, true, -1, 0, 2);
			break;
		case 1: // Destination node
			DrawDebugSphere(GetWorld(), RandPoint, NodeSize, 12, FColor::Red, true, -1, 0, 2);
			break;
		default:
			DrawDebugSphere(GetWorld(), RandPoint, NodeSize, 12, FColor::White, true);
			break;
		}
		
		PrevLoc = RandPoint;
		Nodes.Add(NewNode);
	}
	SetupNodeConnections();
}

void APathfindingGameModeBase::SetupNodeConnections()
{
	for (const auto node : Nodes)
	{
		TArray<FOverlapResult> Result;

		// Check if we overlap with any nodes within the
		// NodeConnectionRadius. NOTE: This specific function
		// does NOT return true with overlaps! Only blocking hits!
		GetWorld()->OverlapMultiByProfile(
			Result,
			node->GetActorLocation(),
			FQuat(),
			FName("NodeOverlap"),
			FCollisionShape::MakeSphere(NodeConnectionRadius),
			FCollisionQueryParams::DefaultQueryParam);

		// If there were any overlaps
		if (Result.Num() > 1)
		{
			for (int32 i{}; i < Result.Num(); i++)
			{
				// We might have collided with something else, in which case the cast will fail
				if (AMyNode* OverlappedNode = Cast<AMyNode>(Result[i].GetActor()))
				{
					// Skip self
					if (OverlappedNode == node) continue;
					MakeConnection(node, OverlappedNode);
				}
			}
		}
	}
}

void APathfindingGameModeBase::MakeConnection(AMyNode* n1, AMyNode* n2)
{
	if (n1->Connections.Find(n2) == INDEX_NONE) n1->Connections.Add(n2);
	if (n2->Connections.Find(n1) == INDEX_NONE) n2->Connections.Add(n1);
	DrawDebugLine(GetWorld(), n1->GetActorLocation(), n2->GetActorLocation(), FColor::White, true);
}

void APathfindingGameModeBase::DrawPath(TArray<AMyNode*> SPT)
{
	UE_LOG(LogTemp, Warning, TEXT("DrawPath()"));
	for (int32 i{}; i < SPT.Num(); i++)
	{
		if (SPT.Num() >= i + 2)
		{
			FVector Loc1 = SPT[i]->GetActorLocation();
			FVector Loc2 = SPT[i+1]->GetActorLocation();
			//DrawDebugDirectionalArrow(GetWorld(), Loc1, Loc2, 1000, FColor::Red, true, -1, 0, 5);
			DrawDebugLine(GetWorld(), Loc1, Loc2, FColor::Orange, true, -1, 0, 10);
		}
	}
}
