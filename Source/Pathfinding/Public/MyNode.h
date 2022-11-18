// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
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

	// Collision component, lets nodes overlap and detect each other
	UPROPERTY(EditAnywhere, Category = "Node")
	USphereComponent* Collision;

	// Time to wait at each node
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	int32 waitTime;

	// bool to check if the node has been visited
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
	bool visited = false;

	UPROPERTY()
	TArray<AMyNode*> Connections;
};
