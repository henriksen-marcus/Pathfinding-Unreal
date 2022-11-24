// Copyright Epic Games, Inc. All Rights Reserved.


#include "PathfindingGameModeBase.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Public/MyNode.h"
#include "Public/LockedPawn.h"
#include "Public/PlayerPawn.h"
#include "Engine/World.h"
#include "Public/Dijkstra2.h"
#include "Kismet/KismetMathLibrary.h"

// todo: Make node spawning mode consistent
// todo: Make animation?
// todo: Make nodes spawn in one after one visibly
// todo: Make UI

APathfindingGameModeBase::APathfindingGameModeBase() {}

void APathfindingGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	// We know that the initial pawn is a LockedPawn because it is set in the game mode settings
	PlayerCurrentPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	LockedPawn = Cast<ALockedPawn>(PlayerCurrentPawn);
	PlayerPawn = GetWorld()->SpawnActor<APlayerPawn>(FVector(), FRotator());
	PlayerPawn->GameModeBase = this;
	
	if (NumberOfNodes < 2) NumberOfNodes = 2;
}

void APathfindingGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DrawDebugSphere(GetWorld(), FVector(), 200, 32, FColor::Red);
	if (bDrawBounds) DrawDebugBox(GetWorld(), FVector(), FVector(1000), FQuat(), FColor::White, false, -1, 0, 4.f);
	
	for (const auto Node : Nodes)
	{
		// Draw node spheres
		if (Node == OriginNode)
			DrawDebugSphere(GetWorld(), Node->GetActorLocation(), NodeSize, 12, FColor::Green, false, -1, 0, 3);
		else if (Node == DestinationNode)
			DrawDebugSphere(GetWorld(), Node->GetActorLocation(), NodeSize, 12, FColor::Red, false, -1, 0, 3);
		else if (bDrawIrrelevantNodes || !bIsPathGenerated)
		{
			DrawDebugSphere(GetWorld(), Node->GetActorLocation(), NodeSize, 12, FColor::White);
			for (const auto Connection : Node->Connections)
			{
				DrawDebugLine(GetWorld(), Node->GetActorLocation(), Connection->GetActorLocation(), FColor::White);
			}
		}
	}
	DrawPath();
	UpdateNodeNameRotations();
}

void APathfindingGameModeBase::SpawnNodes(ESpawnMethod SpawnMethod)
{
	// Reset
	DeleteNodes();
	
	switch (SpawnMethod)
	{
		case ESpawnMethod::Random: SpawnRandom(); break;
		case ESpawnMethod::Grid: SpawnGrid(); break;
		case ESpawnMethod::RandomGrid: SpawnRandomGrid(); break;
	}
	SetStartFinish();
}

void APathfindingGameModeBase::SpawnRandom()
{
	const FBox Box = FBox(FVector3d(-1000), FVector3d(1000));
	FVector RandPoint, PrevLoc;

	for (int32 i{}; i < NumberOfNodes; i++)
	{
		// Continue generating new random position as long as we
		// don't meet the distance between each node requirement
		int32 loops = 0;
		do
		{
			RandPoint = FMath::RandPointInBox(Box);
		}
		while (FVector::Dist(PrevLoc, RandPoint) < NodeDist && loops++ < 10);
		
		AMyNode* NewNode = GetWorld()->SpawnActor<AMyNode>(BP_MyNode, RandPoint, FRotator());

		int32 Index{}, TempIndex{i};
		while (TempIndex - Alphabet.Num() >= 0)
		{
			Index += Alphabet.Num();
			TempIndex -= Alphabet.Num();
		}
		FString Name = Alphabet[i - Index] + ((Index / Alphabet.Num() > 0) ? FString::FromInt(Index / Alphabet.Num()) : TEXT(""));
		NewNode->InitNode(Name);
		
		PrevLoc = RandPoint;
		Nodes.Add(NewNode);
	}
	SetupNodeConnections();
}

void APathfindingGameModeBase::SpawnGrid()
{
}

void APathfindingGameModeBase::SpawnRandomGrid()
{
}

void APathfindingGameModeBase::DeleteNodes()
{
	for (auto Node : Nodes)
	{
		Node->Destroy();
	}
	// Clear nullptr references
	Nodes.Empty();
	CurrentPathTree.Empty();
	bIsPathGenerated = false;
}

void APathfindingGameModeBase::Pathfind(EAlgorithm Algorithm)
{
	UE_LOG(LogTemp, Warning, TEXT("Algorithm chosen: %s"), *UEnum::GetValueAsString(Algorithm));
	switch (Algorithm)
	{
	case EAlgorithm::Dijkstra:
		{
			if (ADijkstra2* Dijkstra = Cast<ADijkstra2>(GetWorld()->SpawnActor(ADijkstra2::StaticClass())))
			{
				// If the Dijkstra algorithm succeeded, enable a variable used in tick to draw the shortest path
				if (Dijkstra->Start(Nodes, OriginNode, DestinationNode))
				{
					CurrentPathTree = Dijkstra->ShortestPathTree;
					bIsPathGenerated = true;
				}
				else UE_LOG(LogTemp, Warning, TEXT("Dijkstra failed."));
			}
			else { UE_LOG(LogTemp, Warning, TEXT("Could not cast!")); }
		}
		break;
	case EAlgorithm::AStar:
		break;
	case EAlgorithm::TSM:
		break;
	}
}

void APathfindingGameModeBase::SetupNodeConnections()
{
	for (const auto Node : Nodes)
	{
		auto NodesToCheck(Nodes);
		NodesToCheck.Remove(Node); // Remove self so we don't make a connection to self

		TArray<AMyNode*> Result;
		if (!FindNearestNodes(Node->GetActorLocation(), NodesToCheck, NumberOfNodes, Result)) continue;
		
		const int32 MaxLoops = FMath::Min(Result.Num(), MaxConnections);
		int32 ConnectionsMade = 0;

		// Iterate through closest nodes
		for (int32 i{}; i < Result.Num() && ConnectionsMade < MaxLoops; i++)
		{	// Check if other node has available connection slots
			if (Result[i]->Connections.Num() < MaxConnections)
			{
				MakeConnection(Node, Result[i]);
				ConnectionsMade++;
			}
		}
	}
}

void APathfindingGameModeBase::MakeConnection(AMyNode* n1, AMyNode* n2)
{
	if (n1->Connections.Find(n2) == INDEX_NONE) n1->Connections.Add(n2);
	if (n2->Connections.Find(n1) == INDEX_NONE) n2->Connections.Add(n1);
}

void APathfindingGameModeBase::SetStartFinish()
{
	// Check nodes from the corner of the bounding box
	OriginNode = FindNearestNode(FVector(1000.f), Nodes);
	if (!OriginNode)
	{	// Fallback
		OriginNode = Nodes[0];
		UE_LOG(LogTemp, Warning, TEXT("Was not able to set smart origin node."));
	}
	
	DestinationNode = FindNearestNode(FVector(-1000.f), Nodes);
	if (!DestinationNode)
	{	// Fallback
		if (DestinationNode == OriginNode)
		{
			DestinationNode = Nodes[1];
			UE_LOG(LogTemp, Warning, TEXT("Was not able to set smart destination node."));	
		}
	}
}

void APathfindingGameModeBase::DrawNodes()
{
	for (const auto Node : Nodes)
	{
		if (Node == OriginNode)
			DrawDebugSphere(GetWorld(), Node->GetActorLocation(), NodeSize, 12, FColor::Green, true, -1, 0, 3);
		else if (Node == DestinationNode)
			DrawDebugSphere(GetWorld(), Node->GetActorLocation(), NodeSize, 12, FColor::Red, true, -1, 0, 3);
		else
			DrawDebugSphere(GetWorld(), Node->GetActorLocation(), NodeSize, 12, FColor::White, true);
	}
}

void APathfindingGameModeBase::DrawPath()
{
	for (int32 i{}; i < CurrentPathTree.Num(); i++)
	{
		if (CurrentPathTree.Num() >= i + 2)
		{
			FVector Loc1 = CurrentPathTree[i]->GetActorLocation();
			FVector Loc2 = CurrentPathTree[i+1]->GetActorLocation();
			//DrawDebugDirectionalArrow(GetWorld(), Loc1, Loc2, 1000, FColor::Red, true, -1, 0, 5);
			DrawDebugLine(GetWorld(), Loc1, Loc2, FColor::Orange, false, -1, 0, 10);
		}
	}
}

TArray<FOverlapResult> APathfindingGameModeBase::SphereOverlap(const FVector& Location, float Radius)
{
	TArray<FOverlapResult> Result;
	// NOTE: This specific function
	// does NOT return true with overlaps! Only blocking hits!
	GetWorld()->OverlapMultiByProfile(
			Result,
			Location,
			FQuat(),
			FName("NodeOverlap"),
			FCollisionShape::MakeSphere(Radius),
			FCollisionQueryParams::DefaultQueryParam);
	return Result;
}

AMyNode* APathfindingGameModeBase::FindNearestNode(const FVector& Origin, const TArray<AMyNode*>& NodesToCheck)
{
	AMyNode* NearestNode = nullptr;
	float DistanceFromNearestNode = TNumericLimits<float>::Max();

	for (AMyNode* NodeToCheck : NodesToCheck)
	{
		if (NodeToCheck)
		{
			const float DistanceFromActorToCheck = (Origin - NodeToCheck->GetActorLocation()).SizeSquared();
			if (DistanceFromActorToCheck < DistanceFromNearestNode)
			{
				NearestNode = NodeToCheck;
				DistanceFromNearestNode = DistanceFromActorToCheck;
			}
		}
	}
	return NearestNode;
}

bool APathfindingGameModeBase::FindNearestNodes(const FVector& Origin, const TArray<AMyNode*>& NodesToCheck, int32 MaxNodes,
	TArray<AMyNode*>& Result)
{
	// Use copy constructor, we need a modifiable array
	TArray NodesToCheckCopy(NodesToCheck);

	// Make sure we check through everything
	while(NodesToCheckCopy.Num())
	{
		AMyNode* NearestNode = nullptr;
		float DistanceFromNearestNode = TNumericLimits<float>::Max();

		for (AMyNode* NodeToCheck : NodesToCheckCopy)
		{
			if (NodeToCheck)
			{
				const float DistanceFromActorToCheck = (Origin - NodeToCheck->GetActorLocation()).SizeSquared();
				if (DistanceFromActorToCheck < DistanceFromNearestNode)
				{
					NearestNode = NodeToCheck;
					DistanceFromNearestNode = DistanceFromActorToCheck;
				}
			}
		}
		if (NearestNode) Result.Add(NearestNode);
		if (Result.Num() >= MaxConnections) return bool(Result.Num());
		NodesToCheckCopy.Remove(NearestNode);
	}
	return bool(Result.Num());
}

void APathfindingGameModeBase::ToggleNodeNames(bool Visible)
{
	Visible = !Visible;
	for (const auto Node : Nodes)
	{
		Node->NameDisplay->ToggleVisibility();
	}
}

void APathfindingGameModeBase::SwitchPawn()
{
	if (PlayerCurrentPawn == LockedPawn)
	{
		// Make a smooth transition
		PlayerPawn->SetActorRotation(LockedPawn->Camera->GetComponentRotation());
		PlayerPawn->SetActorLocation(LockedPawn->Camera->GetComponentLocation());
		GetWorld()->GetFirstPlayerController()->Possess(PlayerPawn);
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
		PlayerCurrentPawn = PlayerPawn;
	}
	else
	{
		GetWorld()->GetFirstPlayerController()->Possess(LockedPawn);
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		PlayerCurrentPawn = LockedPawn;
	}
	PawnSwitched();
}

void APathfindingGameModeBase::UpdateNodeNameRotations()
{
	FVector PlayerLocation;
	if (PlayerCurrentPawn == LockedPawn)
	{
		PlayerLocation = LockedPawn->Camera->GetComponentLocation();
	}
	else
	{
		PlayerLocation = PlayerPawn->GetActorLocation();
	}
	
	for (const auto Node : Nodes)
	{
		Node->NameDisplay->SetWorldRotation(
		UKismetMathLibrary::FindLookAtRotation(
			Node->NameDisplay->GetComponentLocation(),
			PlayerLocation
			));
		
		Node->NameDisplay->SetRelativeScale3D(
			FVector(FMath::Lerp(0.3f, 5.f,
				FVector::Dist(Node->NameDisplay->GetComponentLocation(), PlayerLocation) / 5000)));
	}
}

