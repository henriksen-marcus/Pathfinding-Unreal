// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Public/MyNode.h"
#include "PathfindingGameModeBase.generated.h"

class AMyNode;
class ADijkstra2;
class AAStar;
class APlayerPawn;
class ALockedPawn;

UENUM(BlueprintType)
enum class EAlgorithm : uint8
{
	Dijkstra	UMETA(DisplayName = "Dijkstra"),
	AStar		UMETA(DisplayName = "A*"),
	TSM			UMETA(DisplayName = "Traveling Salesman")
};

UENUM(BlueprintType)
enum class ESpawnMethod : uint8
{
	Random		UMETA(DisplayName = "Random"),
	Grid		UMETA(DisplayName = "Grid"),
	RandomGrid	UMETA(DisplayName = "Random Grid")
};

struct FNodeInfo
{
	AMyNode* Node;
	float Dist;
	FNodeInfo(AMyNode* _Node, float _Dist) : Node(_Node), Dist(_Dist) {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnError, FString, Message);

/**
 * The class that controls and keeps the top level functions and
 * variables to use in all the pathfinding algorithms.
 */
UCLASS()
class PATHFINDING_API APathfindingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	APathfindingGameModeBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintAssignable)
	FKOnError OnError;

	// Choose the BP class based on MyNode in this slot.
	UPROPERTY(EditAnywhere, Category = "Node")
	TSubclassOf<AMyNode> BP_MyNode;

	// Ref to the current possessed pawn.
	UPROPERTY()
	AActor* PlayerCurrentPawn;
	
	UPROPERTY()
	ALockedPawn* LockedPawn;

	// Choose the BP class based on PlayerPawn in this slot.
	UPROPERTY(EditAnywhere, Category = "Player")
	TSubclassOf<APlayerPawn> BP_PlayerPawn;

	UPROPERTY()
	APlayerPawn* PlayerPawn;
	
	// The size of the sphere marking each node
	UPROPERTY(EditAnywhere, Category = "Node")
	float NodeSize = 20.f;

	// Array containing pointers to all spawned nodes
	UPROPERTY(VisibleAnywhere, Category = "Node")
	TArray<AMyNode*> Nodes;

	// Array containing the current path displayed on the screen.
	UPROPERTY()
	TArray<AMyNode*> CurrentPathTree;
	
	UPROPERTY(EditAnywhere)
	float ArrowSize = 1000.f;

	TArray<FString> Alphabet = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};

	// The node we start pathfinding from
	UPROPERTY()
	AMyNode* OriginNode;

	// The node we are trying to pathfind to
	UPROPERTY()
	AMyNode* DestinationNode;

	// If we have a successful shortest path supplied by
	// one of the algorithms. Decides if we should display
	// that path on screen.
	bool bIsPathGenerated = false;

	UPROPERTY()
	ADijkstra2* Dijkstra;

	UPROPERTY()
	AAStar* AStar;
	

	//---------------- Runtime editable variables ----------------//
	
	// The number of nodes to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|Runtime")
	int32 NumberOfNodes = 20;

	// How many connections a node can have. Minimum = 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|Runtime")
	int32 MaxConnections = 3;

	// The required distance between each node
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|Runtime")
	float NodeDist = 200.f;

	// Radius from each node before it connects with another
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|Runtime")
	float NodeConnectionRadius = 500.f;

	// If we should display a sphere marking the NodeConnectionRadius
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|Runtime")
	bool bShowConnectionRadius = false;

	// If the text above each node should be visible
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Node|Runtime")
	bool bShowNodeNames = true;

	// Whether to draw nodes that are not part of the path
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|Runtime")
	bool bDrawIrrelevantNodes = true;

	// If we should add a random wait time to each node, increasing the complexity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|Runtime")
	bool bUseWaitTime = true;

	// Whether to draw the bounds that the nodes spawn in
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Runtime")
	bool bDrawBounds = true;
	

	//---------------- Functions -------------------//

	/**
	 * @brief Spawns nodes with a random location within the specified bounds.
	 * @param SpawnMethod  What algorithm to use.
	 */
	UFUNCTION(BlueprintCallable)
	void SpawnNodes(ESpawnMethod SpawnMethod);

	void SpawnRandom();
	void SpawnGrid();
	void SpawnRandomGrid();

	FString GenerateName(int32 TempIndex);

	/**
	 * @brief Deletes all the nodes in the world.
	 */
	UFUNCTION(BlueprintCallable)
	void DeleteNodes();

	/**
	 * @brief Resets the variables of each node, used when
	 * running different algorithms multiple times on the
	 * same set of nodes.
	 */
	void ResetNodes();

	/**
	 * @brief The branch function that runs the individual
	 * pathfinding algorithms.
	 * @param Algorithm Which pathfinding algorithm to use.
	 */
	UFUNCTION(BlueprintCallable)
	void Pathfind(EAlgorithm Algorithm);

	/**
	 * @brief Iterates through each node and makes connections to the closest ones
	 */
	void SetupNodeConnections();

	/**
	 * @brief Add each node to each other's connection arrays,
	 * and draw a line between them.
	 */
	void MakeConnection(AMyNode* n1, AMyNode* n2);

	/**
	 * @brief Draws the debug spheres around the nodes
	 */
	void DrawNodes();

	/**
	 * @brief Sets the Origin and Destination node
	 */
	void SetStartFinish();

	/**
	 * @brief Draw debug lines showing the shortest path
	 * from the origin node to the destination node
	 */
	void DrawPath();

	/**
	 * @brief Simplified OverlapMultiByProfile() function
	 * for more readable code.
	 * @param Location The position to check from
	 * @param Radius How big the overlap sphere should span
	 * @return Array over overlapped nodes
	 * @remarks Returning the array like this is suboptimal,
	 * I know, but it works for this scenario. I also wanted
	 * a shorter function call.
	 */
	TArray<FOverlapResult> SphereOverlap(const FVector& Location, float Radius);

	/**
	 *	Returns a Node nearest to Origin from ActorsToCheck array.
	 *	@param	Origin			World Location from which the distance is measured.
	 *	@param	NodesToCheck	Array of Actors to examine and return Actor nearest to Origin.
	 *	@return				Nearest Node.
	 *	@remarks This function is copied and modified from UGameplayStatics.
	 */
	AMyNode* FindNearestNode(const FVector& Origin, const TArray<AMyNode*>& NodesToCheck);

	/**
	 *	Returns a Node nearest to Origin from ActorsToCheck array.
	 *	@param	Origin			World Location from which the distance is measured.
	 *	@param	NodesToCheck	Array of Actors to examine and return Actor nearest to Origin.
	 *	@param  MaxNodes		The max number of returned Nodes in Result.
	 *	@param	Result			Output array of the closest Nodes found.
	 *	@return					If there were any results.
	 *	@remarks				Guarantees that the contents of Result are not null.
	 *	@remarks				This function is copied and modified from UGameplayStatics.
	 */
	bool FindNearestNodes(const FVector& Origin, const TArray<AMyNode*>& NodesToCheck, int32 MaxNodes, TArray<AMyNode*>& Result);

	UFUNCTION(BlueprintCallable)
	void ToggleNodeNames(bool Visible);

	/**
	 * @brief Swap between locked and free movement mode.
	 * Changes which pawn the player controller possesses.
	 */
	UFUNCTION(BlueprintCallable)
	void SwitchPawn();

	UFUNCTION()
	void UpdateNodeNameRotations();

	UFUNCTION(BlueprintImplementableEvent)
	void PawnSwitched();
};
