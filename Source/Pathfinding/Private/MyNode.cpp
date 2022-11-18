// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNode.h"


AMyNode::AMyNode()
{
 	// Marcus: Disabled tick. We don't need tick on nodes
	// as they only store values. Save on performance.
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	// Custom collision channel for only overlapping with other nodes
	//Collision->SetCollisionProfileName(FName("NodeOverlap"));
	Collision->SetCollisionProfileName(FName("OverlapAll"));
	SetRootComponent(Collision); 

	distanceValue = 0;
	timeToTravel = 0;
	waitTime = 0;
	visited = false; 

	
}


void AMyNode::BeginPlay()
{
	Super::BeginPlay();
	DrawDebugSphere(GetWorld(), GetActorLocation(), 20, 8, FColor::Red, true);
	UE_LOG(LogTemp, Warning, TEXT("MyNode.cpp: Spawned Node"))
}


void AMyNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

