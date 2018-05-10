// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineWithMesh.h"

// Sets default values for this component's properties
USplineWithMesh::USplineWithMesh(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/SplineTwistCorrect/DebugShape.DebugShape"));
	StaticMesh = DefaultMesh.Object;
	Actor = this->GetOwner();

	FAttachmentTransformRules AttachRule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	// if (Actor != nullptr) // && StaticMesh != nullptr
	// {
	// 	RemoveMesh();
	// 	// FTransform ActorTransform = Actor->GetTransform();
	// 	// Root = NewObject<USceneComponent>(Actor);
	// 	// Root->SetMobility(EComponentMobility::Static);
	// 	// Root->SetWorldTransform(ActorTransform);
	// 	USplineTwistCorrectBPLibrary::CalcRailLength(this, Number, Length, SubSegmentLength);

	// 	for (int32 i = 0; i < Number; i++)
	// 	{
	// 		SplineMesh = ObjectInitializer.CreateDefaultSubobject<USplineMeshComponent>(Actor, TEXT("SplineMesh"), false);

	// 	//	SplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	// 		SplineMesh->AttachToComponent(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	// 		USplineTwistCorrectBPLibrary::ConfigSplineMesh(i, Length, this, SplineMesh, Actor, StaticMesh);
	// 	//	Actor->AddOwnedComponent(SplineMesh);
	// 		SplineMesh->RegisterComponent();
	// 		SplineMeshArray.Add(SplineMesh);
	// 	}
	// }
	
}

void USplineWithMesh::PostInitProperties()
{
	//RemoveMesh();

	Super::PostInitProperties();
	Actor = this->GetOwner();
	if (Actor != nullptr) // && Actor->GetRootComponent() == nullptr ){
		{
//this->SetupAttachment(Actor->GetRootComponent());
//	AddMesh();
	FTransform ActorTransform = Actor->GetTransform();	
	Root = NewObject<USceneComponent>(Actor);
	Root->SetMobility(EComponentMobility::Static);
	Root->SetWorldTransform(ActorTransform);
	
	}

}

void USplineWithMesh::PostLoadSubobjects( FObjectInstancingGraph* OuterInstanceGraph )
{
	Super::PostLoadSubobjects(OuterInstanceGraph);
	AddMesh();
}

#if WITH_EDITOR
void USplineWithMesh::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	AddMesh();

}

void USplineWithMesh::PreEditChange(UProperty* PropertyThatWillChange)
{
	Super::PreEditChange(PropertyThatWillChange);
//	RemoveMesh();
}
#endif

// Called when the game starts
void USplineWithMesh::BeginPlay()
{
	Super::BeginPlay();
//AddMesh();
	// ...
	
}



// Called every frame
void USplineWithMesh::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USplineWithMesh::AddMesh()
{
	//http://www.mike-purvis.com/?p=288 How to make Component with Billboard SubComponent

	//https://answers.unrealengine.com/questions/345109/create-and-attach-custom-actor-component.html
	//https://forums.unrealengine.com/development-discussion/c-gameplay-programming/71644-c-spline-creates-hundreds-of-meshes
	//AActor *Actor = this->GetOwner()
	if (Actor != nullptr) // && StaticMesh != nullptr
	{
	    RemoveMesh();
		USplineTwistCorrectBPLibrary::CalcRailLength(this, Number, Length, SubSegmentLength);
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("number %i"), Number));
	//	SplineMeshArray.SetNum(Number, true);
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("SetArray %i"), SplineMeshArray.Num()));
		for (int32 i = 0; i < Number; i++)
		{

			SplineMesh = NewObject<USplineMeshComponent>(Actor);
			SplineMesh->AttachToComponent(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
			USplineTwistCorrectBPLibrary::ConfigSplineMesh(i, Length, this, SplineMesh, Actor, StaticMesh);

			SplineMesh->RegisterComponent();
			SplineMeshArray.Add(SplineMesh);
		}

	}


}

void USplineWithMesh::RemoveMesh()
{
	int32 SplineMeshNum = SplineMeshArray.Num();
	for (int32 i = 0; i < SplineMeshNum; i++)
	{
		if (SplineMeshArray[i] != nullptr)
		SplineMeshArray[i]->DestroyComponent(false);
	}
	SplineMeshArray.Empty();

}