// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyNode.h"
#include "TSP.generated.h"

UCLASS()
class PATHFINDING_API ATSP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATSP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//function to run the TSP algorithm
	int32 doTSP(TArray<AMyNode*> Nodes, AMyNode* Origin);


};
