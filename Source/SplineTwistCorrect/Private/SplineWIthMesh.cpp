// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineWithMesh.h"


// Sets default values for this component's properties
USplineWithMesh::USplineWithMesh(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}
#if WITH_EDITOR
void USplineWithMesh::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	USplineTwistCorrectBPLibrary::CalcRailLength(this, Number, Length, SegmentLength);
//	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Length %f"), Length));
	for (int32 i= 0; i<Number; i++)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Number %i"), Number));
	}
}



#endif




// Called when the game starts
void USplineWithMesh::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



// Called every frame
void USplineWithMesh::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

