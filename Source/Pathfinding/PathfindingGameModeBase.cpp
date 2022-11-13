// Copyright Epic Games, Inc. All Rights Reserved.


#include "PathfindingGameModeBase.h"
#include "Public/MyNode.h"
#include "DrawDebugHelpers.h"

void APathfindingGameModeBase::BeginPlay()
{

}

void APathfindingGameModeBase::SpawnNodes(int32 Amount)
{
	if (Nodes.Num())
	{
		// Are you sure we don't want to be able to call spawnNodes()
		// to delete the current nodes and spawn new ones (reset)?
		return;
	}

	if (GetWorld())
	{
		/*float x;
		float y;
		float z;*/

		for (int i{}; i < NumberOfNodes; i++)
		{
			/*x = FMath::FRandRange(-1000.f, 1000.f);
			y = FMath::FRandRange(-1000.f, 1000.f);
			z = FMath::FRandRange(-1000.f, 1000.f);*/

			FBox Box = FBox(FVector3d(-1000), FVector3d(1000));
			FVector RandPoint = FMath::RandPointInBox(Box);

			DrawDebugBox(GetWorld(), FVector(0), FVector(1000), FQuat(0), FColor::White);

			//AMyNode* newNode = world->SpawnActor<AMyNode>(BP_MyNode, FVector(x, y, z), FRotator(0.f));
			AMyNode* newNode = GetWorld()->SpawnActor<AMyNode>(BP_MyNode, RandPoint, FRotator(0.f));

			Nodes.Add(newNode);
		}
		UE_LOG(LogTemp, Warning, TEXT("Nodes added to array."));
	}
}

