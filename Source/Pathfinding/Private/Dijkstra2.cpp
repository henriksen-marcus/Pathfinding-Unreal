// Fill out your copyright notice in the Description page of Project Settings.


#include "Dijkstra2.h"

// Sets default values
ADijkstra2::ADijkstra2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADijkstra2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADijkstra2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADijkstra2::doDijkstra(TArray<AMyNode*>, int32 Origin, int32 Destination)
{
	
}

