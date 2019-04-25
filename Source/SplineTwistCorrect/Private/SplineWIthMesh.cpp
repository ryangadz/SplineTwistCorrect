// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineWithMesh.h"

// Sets default values for this component's properties
USplineWithMesh::USplineWithMesh(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UObject> DefaultMesh(TEXT("StaticMesh'/SplineTwistCorrect/SM_DebugSpline.SM_DebugSpline'"));
	if (DefaultMesh.Object != NULL)                             
	StaticMeshDefault = (UStaticMesh*)DefaultMesh.Object; 

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DefaultMaterial(TEXT("Material'/SplineTwistCorrect/M_DebugSpline.M_DebugSpline'"));
	if (DefaultMaterial.Object != NULL)   
	MaterialDefault = DefaultMaterial.Object;

	MaterialArray.Empty();
	//OffsetSpline = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, TEXT("OffsetSpline"), true);
	
	//CorrectedSpline = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, TEXT("CorrectedSpline"), true);
	//CorrectedSpline->SetVisibility()

	static ConstructorHelpers::FObjectFinder<UObject> DefaultScaleCurve(TEXT("CurveVector'/SplineTwistCorrect/C_SplineMeshScale.C_SplineMeshScale'"));
	if (DefaultScaleCurve.Object != NULL)                             
	ScaleCurve = (UCurveVector*)DefaultScaleCurve.Object; 
		


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
		// if (CorrectedSpline != nullptr)
		// 	CorrectedSpline->DestroyComponent();
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
bool USplineWithMesh::CanEditChange(const UProperty *InProperty) const
{

	
	if (InProperty)
	{
		//grey out uproperty if scaling type does not use it
		FString PropertyName = InProperty->GetName();
		if (FCString::Strcmp(*PropertyName, TEXT("Scale")) == 0)
		{
			return MeshScalingType == EMeshScalingType::E_Numerical;
		}
		else if (FCString::Strcmp(*PropertyName, TEXT("ScaleCurve")) == 0)
		{
			return (MeshScalingType == EMeshScalingType::E_UniformCurve || MeshScalingType == EMeshScalingType::E_NonUniformCurve);
		}

		if(FCString::Strcmp(*PropertyName, TEXT("RollIncrement")) == 0)
		{
			return (MeshRollType == EMeshRollType::E_Incremental || MeshRollType == EMeshRollType::E_Random);
		}



	}
	return Super::CanEditChange(InProperty);
}
#endif

#if WITH_EDITOR
void USplineWithMesh::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FName MemberPropertyChanged = (PropertyChangedEvent.MemberProperty ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None);

	// if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(USplineWithMesh, bClosedLoop)))
	// {
		// OffsetSpline->SetClosedLoop(this->IsClosedLoop(), true);
		// CorrectedSpline->SetClosedLoop(this->IsClosedLoop(), true);
	// }
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
	//AddMesh(Actor);
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
	OffsetSpline->SetClosedLoop(this->IsClosedLoop(), true);
	CorrectedSpline->SetClosedLoop(this->IsClosedLoop(), true);
	USplineTwistCorrectBPLibrary::BuildOffsetSpline(this, OffsetSpline, 0.f, 1.f);
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

		if ( MeshToUse != EMeshToUse::E_Default)
		{
			if (StaticMeshArray.Num() > i && MeshToUse == EMeshToUse::E_Array)
			{
				StaticMesh = (StaticMeshArray[i]) ? StaticMeshArray[i] : StaticMeshDefault;
			}
			else if (StaticMeshArray.Num() >= 1 && MeshToUse == EMeshToUse::E_Random)
			{

				int32 random = FMath::RandRange(0, StaticMeshArray.Num() - 1);
				StaticMesh = (StaticMeshArray[random]) ? StaticMeshArray[random] : StaticMeshDefault;
			}
		}
		else
			StaticMesh = StaticMeshDefault;
		USplineTwistCorrectBPLibrary::ConfigSplineMesh(i, Length, CorrectedSpline, SplineMesh, Actor, Material, StaticMesh, GetScaleMesh(i), GetRollMesh(i));

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
		Arrow->SetHiddenInGame(false);
	
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

// FStartEndScale USplineWithMesh::GetScaleMesh()
// {
// 	struct FStartEndScale StartEndScale;
// 	if (MeshScalingType == EMeshScalingType::E_Numerical)
// 		{

			
// 				StartEndScale.Start = FVector2D(Scale, Scale);
// 				StartEndScale.End = Vector2D(Scale, Scale);
			 
			
// 		}
// 	return StartEndScale;
// }

FStartEndScale2 USplineWithMesh::GetScaleMesh(int i)
{
	float fi = float(i);
	struct FStartEndScale2 StartEndScale;
	if (MeshScalingType == EMeshScalingType::E_Numerical)
	{
		StartEndScale.Start = FVector2D(Scale, Scale);
		StartEndScale.End = FVector2D(Scale, Scale);
	}
	else if (MeshScalingType == EMeshScalingType::E_UniformCurve)
	{
		FVector scaleS;
		FVector scaleE;
		scaleS = ScaleCurve->GetVectorValue(fi/(CorrectedSpline->GetNumberOfSplinePoints()-1));
		scaleE = ScaleCurve->GetVectorValue((fi+1)/(CorrectedSpline->GetNumberOfSplinePoints()-1));
		StartEndScale.Start = FVector2D(scaleS.X, scaleS.X);
		StartEndScale.End = FVector2D(scaleE.X, scaleE.X);
	}
	else if (MeshScalingType == EMeshScalingType::E_NonUniformCurve)
	{
		FVector scaleS;
		FVector scaleE;
		scaleS = ScaleCurve->GetVectorValue(fi/(CorrectedSpline->GetNumberOfSplinePoints()-1));
		scaleE = ScaleCurve->GetVectorValue((fi+1)/(CorrectedSpline->GetNumberOfSplinePoints()-1));
		StartEndScale.Start = FVector2D(scaleS.X, scaleS.Y);
		StartEndScale.End = FVector2D(scaleE.X, scaleE.Y);
	}
	else if (MeshScalingType == EMeshScalingType::E_SplineScale)
	{
		FVector scaleS;
		FVector scaleE;
		float t = fi/(CorrectedSpline->GetNumberOfSplinePoints()-1);
		float t2 = (fi+1)/(CorrectedSpline->GetNumberOfSplinePoints()-1);
		scaleS = this->GetScaleAtTime(fi/float(CorrectedSpline->GetNumberOfSplinePoints()-1), true);
		scaleE = this->GetScaleAtTime(t2, true);
		StartEndScale.Start = FVector2D(scaleS.Y, scaleS.Z);
		StartEndScale.End = FVector2D(scaleE.Y, scaleE.Z);
	}

	return StartEndScale;
}

float USplineWithMesh::GetRollMesh(int i)
{

	if (MeshRollType == EMeshRollType::E_Incremental)
	{
		return i*RollIncrement;
	}
	else if (MeshRollType == EMeshRollType::E_Random)
	{
		return UKismetMathLibrary::RandomInteger(360)*RollIncrement;
	}
	else return 0.f;
}