// Fill out your copyright notice in the Description page of Project Settings.


#include "Dijkstra.h"

// Sets default values
ADijkstra::ADijkstra()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
}

// Called when the game starts or when spawned
void ADijkstra::BeginPlay()
{
	Super::BeginPlay();

	List.fill(DLL_Base::asc, 0, 100);
	UE_LOG(LogTemp, Warning, TEXT("List elem 0: %d"), List[50]);
	//List.print();
}

// Called every frame
void ADijkstra::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

