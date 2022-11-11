// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "MyNode.generated.h"

UCLASS()
class PATHFINDING_API AMyNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Node mesh
	UPROPERTY(EditAnywhere, Category = "Node")
	UStaticMeshComponent* mesh;

	//distance between nodes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	int distanceValue;

	//time to travel from node to node
	//time to travel will be random
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	int timeToTravel;

	//time spent waiting at each node
	//time to wait will be random
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	int waitTime;

	//bool to check if the node has been visited
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	bool visited = false;



	

};
