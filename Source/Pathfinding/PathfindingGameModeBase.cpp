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
#include "Public/AStar.h"
#include "Kismet/KismetMathLibrary.h"


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
	
	if (bDrawBounds) DrawDebugBox(GetWorld(), FVector(), FVector(1000), FQuat(), FColor::White, false, -1, 0, 7.f);
	
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

	if (!bUseWaitTime)
	{
		for (const auto Node : Nodes)
		{
			Node->NameDisplay->SetText(FText::FromString(Node->Name));
			Node->WaitTime = 0;
		}
	}

	if (!bShowNodeNames) ToggleNodeNames(false);
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
		
		NewNode->InitNode(GenerateName(i));
		
		PrevLoc = RandPoint;
		Nodes.Add(NewNode);
	}
	SetupNodeConnections();
}

void APathfindingGameModeBase::SpawnGrid()
{
	int Loops = FMath::Pow(NumberOfNodes, 1.f / 3.f);
	const float DistanceBetweenNodes = 2000 / Loops;
	float Offset = -1000.f;
	int32 TotIndex = 0;
	
	for (int32 x{}; x <= Loops; x++)
	{
		for (int32 y{}; y <= Loops; y++)
		{
			for (int32 z{}; z <= Loops; z++)
			{
				FVector Pos = FVector(DistanceBetweenNodes * x + Offset, DistanceBetweenNodes * y + Offset, DistanceBetweenNodes * z + Offset);
				AMyNode* NewNode = GetWorld()->SpawnActor<AMyNode>(BP_MyNode, Pos, FRotator());
				NewNode->InitNode(GenerateName(TotIndex));
				Nodes.Add(NewNode);
				TotIndex++;
			}
		}
	}

	SetupNodeConnections();
	
	UE_LOG(LogTemp, Warning, TEXT("NumNodes: %d"), Nodes.Num());
}

void APathfindingGameModeBase::SpawnRandomGrid()
{
}

FString APathfindingGameModeBase::GenerateName(int32 i)
{
	const int32 Number = i / Alphabet.Num();
	return Number ? Alphabet[i - Alphabet.Num() * Number] + FString::FromInt(Number) : Alphabet[i];
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

void APathfindingGameModeBase::ResetNodes()
{
	for (auto Node : Nodes)
	{
		Node->PreviousNode = nullptr;
		Node->bVisited = false;
		Node->Heuristic = FLT_MAX;
		Node->CurrentCost = FLT_MAX;
	}
}

void APathfindingGameModeBase::Pathfind(EAlgorithm Algorithm)
{
	UE_LOG(LogTemp, Warning, TEXT("Algorithm chosen: %s"), *UEnum::GetValueAsString(Algorithm));

	// In case we want to run another algorithm on the same node set
	ResetNodes();
	
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
			else { UE_LOG(LogTemp, Warning, TEXT("Could not cast Dijkstra!")); }
		}
		break;
	case EAlgorithm::AStar:
		{
			if (auto* AStar = Cast<AAStar>(GetWorld()->SpawnActor(AAStar::StaticClass())))
			{
				// If the AStar algorithm succeeded, enable a variable used in tick to draw the shortest path
				if (AStar->Start(Nodes, OriginNode, DestinationNode))
				{
					CurrentPathTree = AStar->ShortestPathTree;
					bIsPathGenerated = true;
				}
				else UE_LOG(LogTemp, Warning, TEXT("AStar failed."));
			}
			else { UE_LOG(LogTemp, Warning, TEXT("Could not cast AStar!")); }
		}
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
	if (Nodes.IsEmpty()) return;
	
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

	OriginNode->NameDisplay->SetText(FText::FromString(TEXT("Start")));
	DestinationNode->InitNode(TEXT("End"));
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
	for (const auto Node : Nodes)
	{
		Node->NameDisplay->ToggleVisibility();
	}
	bShowNodeNames = !bShowNodeNames;
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

