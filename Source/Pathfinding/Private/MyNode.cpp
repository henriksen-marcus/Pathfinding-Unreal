// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNode.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "Kismet/KismetMathLibrary.h"

AMyNode::AMyNode()
{
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	
	// Custom collision preset/object type for only overlapping with other nodes
	Collision->SetCollisionProfileName(FName("NodeOverlap"));
	SetRootComponent(Collision);
	
	WaitTime = 0.f;
	bVisited = false;
	CurrentCost = 90000000.f;
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
	if (NameDisplay)
	{
		NameDisplay->SetActorRotation(
		UKismetMathLibrary::FindLookAtRotation(
			GetActorLocation(),
			GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()
			));
	}
}

void AMyNode::InitNode(FString _Name)
{
	Name = _Name;
	NameDisplay = GetWorld()->SpawnActor<ATextRenderActor>(GetActorLocation() + FVector(0, 0, 30.f), FRotator());
	NameDisplay->GetTextRender()->SetText(FText::FromString(Name + ": " + FString::SanitizeFloat(WaitTime, 0)));
	NameDisplay->SetActorScale3D(FVector(0.8f));
	if (TextMaterial) NameDisplay->GetTextRender()->SetTextMaterial(TextMaterial);
}

