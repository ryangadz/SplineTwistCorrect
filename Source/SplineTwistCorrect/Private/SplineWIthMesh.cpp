// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineWithMesh.h"

// Sets default values for this component's properties
USplineWithMesh::USplineWithMesh(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UObject> DefaultMesh(TEXT("StaticMesh'/SplineTwistCorrect/DebugShape.DebugShape'"));
	if (DefaultMesh.Object != NULL)                             
	StaticMeshDefault = (UStaticMesh*)DefaultMesh.Object; 

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DefaultMaterial(TEXT("/SplineTwistCorrect/M_Track.M_Track"));
	if (MaterialDefault.Object != NULL)   
	MaterialDefault = DefaultMaterial.Object;

	MaterialArray.Empty();
	//OffsetSpline = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, TEXT("OffsetSpline"), true);
	
	//CorrectedSpline = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, TEXT("CorrectedSpline"), true);
	//CorrectedSpline->SetVisibility()

	FAttachmentTransformRules AttachRule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);

	USplineTwistCorrectBPLibrary::CalcRailLength(this, Number, Length, SubSegmentLength);
	
}

// class FActorComponentInstanceData* GetComponentInstanceData()
// {}

void USplineWithMesh::OnRegister()
{
	AddRootToParent();
	if (Root != nullptr)
	{
		if (bAddDirectionArrows)
			AddDirectionArrows();

		if (OffsetSpline == nullptr)
		{
			OffsetSpline = NewObject<USplineComponent>(this);
			OffsetSpline->AttachToComponent(Root, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		}

		if (CorrectedSpline == nullptr)
		{
			CorrectedSpline = NewObject<USplineComponent>(this);
			CorrectedSpline->AttachToComponent(Root, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		}

		AddMesh(Actor);
		if (CorrectedSpline != nullptr)
			CorrectedSpline->DestroyComponent();
	}
	Super::OnRegister();
}

void USplineWithMesh::PostLoad()
{
	// AddRootToParent();
	// AddMesh(Actor);
	Super::PostLoad();

}

void USplineWithMesh::PostInitProperties()
{
//	RemoveMesh();
//	AddRootToParent();
	Super::PostInitProperties();
USplineTwistCorrectBPLibrary::CalcRailLength(this, Number, Length, SubSegmentLength);
//	AddMesh(Actor);
//AddRootToParent();
}

void USplineWithMesh::PostLoadSubobjects( FObjectInstancingGraph* OuterInstanceGraph )
{
//		 AddRootToParent();
	Super::PostLoadSubobjects(OuterInstanceGraph);
//	AddMesh(Actor);
	// AddRootToParent();
	// AddMesh();

}

#if WITH_EDITOR
void USplineWithMesh::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{

	//AddRootToParent();
	//AddMesh(Actor);
	Super::PostEditChangeProperty(PropertyChangedEvent);


}

void USplineWithMesh::PreEditChange(UProperty* PropertyThatWillChange)
{
	USplineTwistCorrectBPLibrary::CalcRailLength(this, Number, Length, SubSegmentLength);
	Super::PreEditChange(PropertyThatWillChange);
//	RemoveMesh();
	// AddRootToParent();
	// AddMesh();
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

void USplineWithMesh::AddMesh(class AActor *PActor)
{
	//http://www.mike-purvis.com/?p=288 How to make Component with Billboard SubComponent
	Actor = this->GetOwner();
	//https://answers.unrealengine.com/questions/345109/create-and-attach-custom-actor-component.html
	//https://forums.unrealengine.com/development-discussion/c-gameplay-programming/71644-c-spline-creates-hundreds-of-meshes
	//AActor *Actor = this->GetOwner();

	RemoveMesh();
	USplineTwistCorrectBPLibrary::CalcRailLength(this, Number, Length, SubSegmentLength);
	USplineTwistCorrectBPLibrary::BuildOffsetSpline(this, OffsetSpline, 0.f, 30.f);
	USplineTwistCorrectBPLibrary::BuildCorrectedSpline(this, OffsetSpline, CorrectedSpline, Length);
	//	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("number %i"), Number));
	//	SplineMeshArray.SetNum(Number, true);
	//	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("SetArray %i"), SplineMeshArray.Num()));
	for (int32 i = 0; i < Number; i++)
	{

		SplineMesh = NewObject<USplineMeshComponent>(this);
		SplineMesh->AttachToComponent(Root, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		if (MaterialArray.Num() > i)
		{
			Material = (MaterialArray[i]) ? MaterialArray[i] : MaterialDefault;
		}
		else
			Material = MaterialDefault;

		if (MeshToUse == EMeshToUse::E_Array)
		{
			if (StaticMeshArray.Num() > i)
			{
				StaticMesh = (StaticMeshArray[i]) ? StaticMeshArray[i] : StaticMeshDefault;
			}
			else StaticMesh = StaticMeshDefault;
		}
		else if (MeshToUse == EMeshToUse::E_Random)
		{
			if (StaticMeshArray.Num() > i)
			{
				//TODO clean this part up
				//if (StaticMeshArray.Num()  0 )
				int32 random = FMath::RandRange(0, FMath::Clamp(StaticMeshArray.Num() - 1, 0, 10000));
				StaticMesh = (StaticMeshArray[random]) ? StaticMeshArray[random] : StaticMeshDefault;
			}
			else StaticMesh = StaticMeshDefault;
		}
		else StaticMesh = StaticMeshDefault;
		USplineTwistCorrectBPLibrary::ConfigSplineMesh(i, Length, CorrectedSpline, SplineMesh, Actor, Material, StaticMesh);

		SplineMesh->RegisterComponent();
		SplineMeshArray.Add(SplineMesh);
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

void USplineWithMesh::AddRootToParent()
{
	Actor = this->GetOwner();
	if (Actor != nullptr)
	{

			FTransform ActorTransform = Actor->GetTransform();
			Root = NewObject<USceneComponent>(this);
			//Root = NewObject<USceneComponent>(Actor);
			Root->SetMobility(EComponentMobility::Static);
			Root->SetWorldTransform(ActorTransform);


	}
}
#if WITH_EDITORONLY_DATA
void USplineWithMesh::Serialize(FArchive& Ar)
{
	
	// AddRootToParent();
	// AddMesh();
Super::Serialize(Ar);
}
#endif

void USplineWithMesh::AddDirectionArrows()
{
	int32 numSplinePoints = this->GetNumberOfSplinePoints();
	RemoveDirectionArrows();
	for(int32 i = 0; i < numSplinePoints; i++ )
	{
		Arrow = NewObject<UArrowComponent>(this);
		Arrow->AttachToComponent(Root, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Arrow->SetRelativeLocation(this->GetLocationAtSplinePoint(i,CoordSpace));
		FVector upVector = this->GetUpVectorAtSplinePoint(i, CoordSpace);
		Arrow->SetRelativeRotation(UKismetMathLibrary::MakeRotFromX(upVector));
		Arrow->RegisterComponent();
		DirectionArrows.Add(Arrow);
	
	}
}

void USplineWithMesh::RemoveDirectionArrows()
{
	int32 arrowNum = DirectionArrows.Num();
	for (int32 i = 0; i < arrowNum; i++)
	{
		if (DirectionArrows[i] != nullptr)
		DirectionArrows[i]->DestroyComponent(false);
	}
	DirectionArrows.Empty();
}