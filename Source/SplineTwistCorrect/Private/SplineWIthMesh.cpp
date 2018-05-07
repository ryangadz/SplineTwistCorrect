// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineWithMesh.h"


// Sets default values for this component's properties
USplineWithMesh::USplineWithMesh(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SplineMeshArray.Empty();

//	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
  //  RootComponent = SphereComponent;
 //  SphereComponent->InitSphereRadius(40.0f);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/SplineTwistCorrect/DebugShape.DebugShape"));
	StaticMesh = DefaultMesh.Object;
	
	
}


void USplineWithMesh::PostInitProperties()
{

	Super::PostInitProperties();
	Actor = this->GetOwner();
	if (Actor != nullptr){
this->SetupAttachment(Actor->GetRootComponent());

	}
	//AddMesh();
}

#if WITH_EDITOR
void USplineWithMesh::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	AddMesh();
//	USplineTwistCorrectBPLibrary::CalcRailLength(this, Number, Length, SubSegmentLength);
//	USplineTwistCorrectBPLibrary::ConfigSplineMesh(0, Length, this, SplineMesh, Actor, StaticMesh);
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

void USplineWithMesh::AddMesh()
{
	
//	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("TEXT %b")+canAttach));
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
		//	USplineMeshComponent* SplineMesh = AddComponent()
		//	USplineMeshComponent *SplineMesh= NewObject<USplineMeshComponent>();
			USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(Actor);
			RemoveMesh();
		//	SplineMesh->CreationMethod = EComponentCreationMethod::Native;
	//		SplineMesh->SetMobility(EComponentMobility::Movable);
			SplineMesh->RegisterComponent();
		//	SplineMesh->AttachTo(this);
		//	SplineMesh->SetupAttachment(Actor->GetRootComponent());
		//	CanAttach= SplineMesh->IsAttachedTo(this);
				SplineMesh->AttachToComponent(Actor->GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
			USplineTwistCorrectBPLibrary::ConfigSplineMesh(i, Length, this, SplineMesh, Actor, StaticMesh);
			//	SplineMesh->RegisterComponentWithWorld(GetWorld());
			//	SplineMesh->RegisterComponent();
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("index %i"),i));
		//	SplineMeshArray[i] = SplineMesh;
			SplineMeshArray.Add(SplineMesh);
		}
	//	RemoveMesh();
		// if (SplineMeshArray[1] != nullptr)
		// {
		// 	SplineMeshArray[1]->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		// 	SplineMeshArray[1]->UnregisterComponent();
		// }
	}


}

void USplineWithMesh::RemoveMesh()
{
	int32 SplineMeshNum = SplineMeshArray.Num();
	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("ArrayLength %i"), SplineMeshNum));
	for (int32 i = 0; i < SplineMeshNum; i++)
	{
		SplineMeshArray[i]->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform );
		SplineMeshArray[i]->UnregisterComponent();
	//	SplineMeshArray[i]->DestroyComponent();
	}
	SplineMeshArray.Empty();
}