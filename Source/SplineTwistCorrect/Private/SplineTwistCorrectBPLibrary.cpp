// Copyright 2018 Ryan Gadz, Inc. All Rights Reserved.

#include "SplineTwistCorrectBPLibrary.h"
#include "SplineTwistCorrect.h"



USplineTwistCorrectBPLibrary::USplineTwistCorrectBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void USplineTwistCorrectBPLibrary::CalcRailLength(
	const class USplineComponent *Spline,
	int &number,
	float &length,
	const float IdealLength)
{
	if (!Spline)
		return;
	float clampedLength = FMath::Clamp(IdealLength, 1.0F, 100000.0F);
	number = (Spline->GetSplineLength() / clampedLength);
	length = (Spline->GetSplineLength() / number);
}

void USplineTwistCorrectBPLibrary::CalcStartEnd(
	const class USplineComponent* Spline, 
	FVector& LocStart, 
	FVector& TanStart, 
	FVector& LocEnd, 
	FVector& TanEnd, 
	const int Index, 
	const float Length)
{
	if (!Spline)
		return;
    ESplineCoordinateSpace::Type L = ESplineCoordinateSpace::Local;

	LocStart = Spline->GetLocationAtDistanceAlongSpline(Index*Length, L);
	LocEnd = Spline->GetLocationAtDistanceAlongSpline((Index+1)*Length, L);

	FVector Tan1 = Spline->GetTangentAtDistanceAlongSpline(Index*Length,L);
	TanStart = Tan1.GetSafeNormal()*Length;

	FVector Tan2 = Spline->GetTangentAtDistanceAlongSpline((Index+1)*Length,L);
	TanEnd = Tan2.GetSafeNormal()*Length;
}

void USplineTwistCorrectBPLibrary::CalcRotFromUp(
	float &Rotation,
	const class USplineComponent *Spline,
	const int Index,
	const float Length)
{
	if (!Spline)
		return;
	ESplineCoordinateSpace::Type W = ESplineCoordinateSpace::World;
	FVector Tan = Spline->GetTangentAtDistanceAlongSpline((Index + 1) * Length, W);
	FVector Crossed1 = FVector::CrossProduct(Tan.GetSafeNormal(), Spline->GetUpVectorAtDistanceAlongSpline(Index * Length, W));
	FVector Crossed2 = FVector::CrossProduct(Tan.GetSafeNormal(), Spline->GetUpVectorAtDistanceAlongSpline((Index + 1) * Length, W));
	FVector Crossed3 = FVector::CrossProduct(Crossed1, Crossed2).GetSafeNormal();
	float Dot1 = FVector::DotProduct(Crossed1.GetSafeNormal(), Crossed2.GetSafeNormal());
	float Dot2 = FVector::DotProduct(Crossed3.GetSafeNormal(), Tan);
	Rotation = ((UKismetMathLibrary::SignOfFloat(Dot2)) * (-1) * (FGenericPlatformMath::Acos(Dot1)));
}

void USplineTwistCorrectBPLibrary::ConfigSplineMesh(
	const int &Index,
	const float &Length,
	const class USplineComponent *SplineFinal,
	class USplineMeshComponent *SplineMesh,
	const class AActor *Actor,
	class UMaterialInterface *Material,
	class UStaticMesh *StaticMesh,
	const FStartEndScale2 StartEndScale,
	const float Roll)
{
	if (!SplineFinal || !SplineMesh || !Actor)
		return;
	FVector locStart = FVector(0, 0, 0),
			locEnd = FVector(100, 0, 0),
			tanStart = FVector(100, 0, 0),
			tanEnd = FVector(100, 0, 0);
	CalcStartEnd(SplineFinal, locStart, tanStart, locEnd, tanEnd, Index, Length);
	SplineMesh->SetStartAndEnd(locStart, tanStart, locEnd, tanEnd, false);

	ESplineCoordinateSpace::Type W = ESplineCoordinateSpace::World;
	FVector upDir = SplineFinal->GetUpVectorAtDistanceAlongSpline(Index * Length, W);
	FTransform transformDir = Actor->GetActorTransform();
	FVector transformed = transformDir.InverseTransformVectorNoScale(upDir);
	SplineMesh->SetSplineUpDir(transformed, true);

	float Rotation = 0;
	CalcRotFromUp(Rotation, SplineFinal, Index, Length);

	Rotation = Rotation + FMath::DegreesToRadians(Roll);
	SplineMesh->SetStartRoll(FMath::DegreesToRadians(Roll), false);
	SplineMesh->SetEndRoll(Rotation, false);
	SplineMesh->SetMaterial(0,Material);

	SplineMesh->SetStartScale(StartEndScale.Start, false);
	SplineMesh->SetEndScale(StartEndScale.End, false);

	SplineMesh->SetStaticMesh(StaticMesh);
}

void USplineTwistCorrectBPLibrary::BuildOffsetSpline(
	const class USplineComponent *SplineUser,
	class USplineComponent *SplineOffset,
	const float RotFromUp,
	const float OffsetDist)
{
	if (!SplineUser || !SplineOffset)
		return;
	ESplineCoordinateSpace::Type W = ESplineCoordinateSpace::World;
	SplineOffset->ClearSplinePoints(true);
	float lastIndex = SplineUser->GetNumberOfSplinePoints() - 1;

	FVector upVectorScaled = FVector(0, 0, 0);
	FVector tanAtPoint = FVector(0, 0, 0);
	FVector offsetVector = FVector(0, 0, 0);
	FVector pointPos = FVector(0, 0, 0);

	for (int i = 0; i <= lastIndex; i++)
	{
		upVectorScaled =  OffsetDist*(SplineUser->GetUpVectorAtSplinePoint(i, W));
		tanAtPoint = (SplineUser->GetTangentAtSplinePoint(i, W));
		offsetVector = upVectorScaled.RotateAngleAxis(RotFromUp, tanAtPoint.GetSafeNormal());
		pointPos = offsetVector + (SplineUser->GetLocationAtSplinePoint(i, W));
		SplineOffset->AddSplinePointAtIndex(pointPos, i, W, false);

		SplineOffset->SetTangentAtSplinePoint(i, tanAtPoint, W, false);

		SplineOffset->SetSplinePointType(i, SplineUser->GetSplinePointType(i), false);
	}

	SplineOffset->UpdateSpline();

	FixTangents(SplineUser, SplineOffset);
}

void USplineTwistCorrectBPLibrary::FixTangents(
	const class USplineComponent *SplineUser,
	class USplineComponent *SplineOffset)
{
	if (!SplineUser || !SplineOffset)
		return;
	ESplineCoordinateSpace::Type W = ESplineCoordinateSpace::World;
	FVector arriveTan = FVector(10, 0, 0);
	FVector leaveTan = FVector(10, 0, 0);
	FVector userTan = FVector(10, 0, 0);
	float lastIndex = SplineUser->GetNumberOfSplinePoints() - 1;
	int32 aindex = 0;  //arrive index
	int32 aindexl = 0; //arrive looping index
	int32 lindex = 0; //leave looping index
	for (int a = 0; a <= 2; a++)
	{
		for (int i = 0; i <= lastIndex; i++)
		{
			userTan = SplineUser->GetTangentAtSplinePoint(i, W);

			aindex = FMath::Clamp(int32(i - 1), 0, int32(lastIndex + 1));
			aindexl = lastIndex * ((i - 1) < 0);
			arriveTan = userTan * ((SplineOffset->GetDistanceAlongSplineAtSplinePoint(i) -
									(SplineOffset->GetDistanceAlongSplineAtSplinePoint(aindex) +
									 SplineOffset->GetDistanceAlongSplineAtSplinePoint(aindexl))) /
								   (SplineUser->GetDistanceAlongSplineAtSplinePoint(i) -
									(SplineUser->GetDistanceAlongSplineAtSplinePoint(aindex) +
									 SplineUser->GetDistanceAlongSplineAtSplinePoint(aindexl))));

			lindex = (lastIndex) == (i);
			leaveTan = userTan * ((SplineOffset->GetDistanceAlongSplineAtSplinePoint(i) -
									(SplineOffset->GetDistanceAlongSplineAtSplinePoint(i + 1) +
									 (SplineOffset->GetSplineLength() * lindex))) /
								   (SplineUser->GetDistanceAlongSplineAtSplinePoint(i) -
									(SplineUser->GetDistanceAlongSplineAtSplinePoint(i + 1) +
									 (SplineUser->GetSplineLength() * lindex))));

			SplineOffset->SetTangentsAtSplinePoint(i, arriveTan, leaveTan, W, false);
		}
		SplineOffset->UpdateSpline();
	}
}

void USplineTwistCorrectBPLibrary::BuildCorrectedSpline(
	const class USplineComponent *SplineUser,
	const class USplineComponent *SplineOffset,
	class USplineComponent *SplineFinal,
	const float IdealLength)
{
	if (!SplineUser || !SplineOffset || !SplineFinal)
		return;
	int32 numSections;
	float length;
	CalcRailLength(SplineUser, numSections, length, IdealLength); 
	SplineFinal->ClearSplinePoints(true);
	float numLoops = numSections+((SplineUser->IsClosedLoop())*-1);
	FVector position;
	FVector posOffset;
	FVector tangent;
	ESplineCoordinateSpace::Type W = ESplineCoordinateSpace::World;
	for (int32 i= 0; i<=numLoops; i++)
	{
		position = SplineUser->GetLocationAtDistanceAlongSpline(i*length,W);
		SplineFinal->AddSplinePointAtIndex(position, i, W, false);
		
		posOffset = SplineOffset->FindLocationClosestToWorldLocation(position,W);
		SplineFinal->SetUpVectorAtSplinePoint(i, (posOffset-position).GetSafeNormal(), W, false);
		
		tangent = length*((SplineUser->GetTangentAtDistanceAlongSpline(i*length,W)).GetSafeNormal());
		SplineFinal->SetTangentAtSplinePoint(i, tangent, W, false);
	//	SplineFinal->SetScaleAtSplinePoint(i, )
	}
	SplineFinal->UpdateSpline();
}