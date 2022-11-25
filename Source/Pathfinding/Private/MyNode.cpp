// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNode.h"
#include "Components/TextRenderComponent.h"
#include "Components/SphereComponent.h"

AMyNode::AMyNode()
{
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	// Custom collision preset/object type for only overlapping with other nodes
	Collision->SetCollisionProfileName(FName("NodeOverlap"));
	SetRootComponent(Collision);

	NameDisplay = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NameDisplay"));
	NameDisplay->SetupAttachment(GetRootComponent());
	NameDisplay->SetRelativeLocation(FVector(0, 0, 30.f));
	NameDisplay->SetRelativeScale3D(FVector(0.8f));
	if (TextMaterial) NameDisplay->SetTextMaterial(TextMaterial);
	
	
	WaitTime = 0.f;
	bVisited = false;
	CurrentCost = FLT_MAX;
	Name = "";
}

void AMyNode::BeginPlay()
{
	Super::BeginPlay();
	// We need to multiply by 1000 to make it have some significance
	WaitTime = FMath::RandRange(1, 5) * 1000.f;
}

void AMyNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyNode::InitNode(const FString& _Name)
{
	Name = _Name;
	FString Num = FString::SanitizeFloat(WaitTime, 0);
	FString NewName = Name + ": " + Num;
	if (NameDisplay) NameDisplay->SetText(FText::FromString(NewName));
}

