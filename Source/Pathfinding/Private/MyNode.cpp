// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNode.h"


AMyNode::AMyNode()
{
 	// Marcus: Disabled tick. We don't need tick on nodes
	// as they only store values. Save on performance.
	PrimaryActorTick.bCanEverTick = false;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("mesh"); 
}


void AMyNode::BeginPlay()
{
	Super::BeginPlay();
	DrawDebugSphere(GetWorld(), GetActorLocation(), 50, 8, FColor::Red);
}


void AMyNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
