// Copyright Epic Games, Inc. All Rights Reserved.


#include "PathfindingGameModeBase.h"
#include "MeshPassProcessor.h"
#include "Kismet/GameplayStatics.h"
#include "Public/MyNode.h"
#include "Engine/World.h"

APathfindingGameModeBase::APathfindingGameModeBase()
{
	NumberOfNodes = 0;
}

void APathfindingGameModeBase::BeginPlay()
{
	SpawnNodes();
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
		UE_LOG(LogTemp, Warning, TEXT("Nodes added to array."));
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
			FCollisionShape::MakeSphere(100)
			);

		if (success)
		{
			for (auto item : Result)
			{
				MakeConnection(node, Cast<AMyNode>(item.GetActor()));
			}
		}
	}
}

void APathfindingGameModeBase::MakeConnection(AMyNode* n1, AMyNode* n2)
{
	// Check that we aren't already connected
	//if (in(n1->Connections, n2));
}
