// Copyright Epic Games, Inc. All Rights Reserved.


#include "PathfindingGameModeBase.h"
#include "MeshPassProcessor.h"
#include "Kismet/GameplayStatics.h"
#include "Public/MyNode.h"
#include "Engine/World.h"
#include "Public/Dijkstra2.h"

APathfindingGameModeBase::APathfindingGameModeBase()
{
	NumberOfNodes = 20;
	NodeConnectionRadius = 150;
}

void APathfindingGameModeBase::BeginPlay()
{
	SpawnNodes();
	ADijkstra2* Alg = Cast<ADijkstra2>(GetWorld()->SpawnActor(ADijkstra2::StaticClass()));
	if (Alg) Alg->doDijkstra(Nodes, 0, FMath::RandRange(1, Nodes.Num() - 1));
	else {UE_LOG(LogTemp, Warning, TEXT("Could not cast!"));}
}

void APathfindingGameModeBase::SpawnNodes()
{
	if (Nodes.Num())
	{
		// Are you sure we don't want to be able to call spawnNodes()
		// to delete the current nodes and spawn new ones (reset)?
		return;
	}

	if (GetWorld())
	{
		FBox Box = FBox(FVector3d(-1000), FVector3d(1000));
		FVector RandPoint;
		
		DrawDebugBox(GetWorld(), FVector(0), FVector(1000), FQuat(0), FColor::White, true);
		
		for (int i{}; i < NumberOfNodes; i++)
		{
			// Continue generating new random position as long as we
			// don't meet the distance between each node requirement
			do
			{
				RandPoint = FMath::RandPointInBox(Box);
			}
			while (FVector::Dist(PrevLoc, RandPoint) < 50);
			
			AMyNode* NewNode = GetWorld()->SpawnActor<AMyNode>(BP_MyNode, RandPoint, FRotator(0.f));
			PrevLoc = RandPoint;
			Nodes.Add(NewNode);
		}
		UE_LOG(LogTemp, Warning, TEXT("PathfindingGameModeBase.cpp: Nodes added to array."));
		SetupNodeConnections();
	}
}

void APathfindingGameModeBase::SetupNodeConnections()
{
	for (auto node : Nodes)
	{
		TArray<FOverlapResult> Result;
		bool success = GetWorld()->OverlapMultiByChannel(
			Result,
			node->GetActorLocation(),
			FQuat(),
			ECollisionChannel::ECC_Visibility,
			FCollisionShape::MakeSphere(NodeConnectionRadius)
			);
		
		if (success)
		{
			
			for (auto item : Result)
			{
				AMyNode* OverlappedNode = Cast<AMyNode>(item.GetActor());
				UE_LOG(LogTemp, Warning, TEXT("Collided with: %s"), *item.GetActor()->GetName());
				// We might have collided with something else, in which case the cast will fail
				if (OverlappedNode)
				{
					DrawDebugSphere(GetWorld(), node->GetActorLocation(), NodeConnectionRadius, 16, FColor::Green, true);
					MakeConnection(node, OverlappedNode);
				}
			}
		}
		else
		{
			DrawDebugSphere(GetWorld(), node->GetActorLocation(), NodeConnectionRadius, 16, FColor::Blue, true);
		}
	}
}

void APathfindingGameModeBase::MakeConnection(AMyNode* n1, AMyNode* n2)
{
	// Check that we aren't already connected
	//if (in(n1->Connections, n2) || in(n2->Connections, n1))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Object is already connected."));
	//	return;
	//}
	n1->Connections.Add(n2);
	n2->Connections.Add(n1);
	DrawLine(n1, n2);
}

void APathfindingGameModeBase::DrawLine(AMyNode* n1, AMyNode* n2, FColor Color)
{
	DrawDebugLine(GetWorld(), n1->GetActorLocation(), n2->GetActorLocation(), Color, true);
}
