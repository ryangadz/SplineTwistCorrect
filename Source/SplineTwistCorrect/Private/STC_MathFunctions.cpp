// Fill out your copyright notice in the Description page of Project Settings.

#include "STC_MathFunctions.h"

void USTC_MathFunctions::CalcIcosahedron(
    TArray<FVector> &Vertices,
    const float Radius)
{
    float G = ((FMath::Sqrt(5) + 1) / 2);
    float M = FMath::Sqrt((FMath::Pow(G, 2)) + 1);
    float X = (G * Radius) / M;
    float Y = Radius / M;
    Vertices.Add(FVector(Y, X, 0.0f));
    Vertices.Add(FVector(Y, -X, 0.0f));
    Vertices.Add(FVector(-Y, X, 0.0f));
    Vertices.Add(FVector(-Y, -X, 0.0f));

    Vertices.Add(FVector(X, 0.0f, Y));
    Vertices.Add(FVector(-X, 0.0f, Y));
    Vertices.Add(FVector(X, 0.0f, -Y));
    Vertices.Add(FVector(-X, 0.0f, -Y));

    Vertices.Add(FVector(0.0f, Y, X));
    Vertices.Add(FVector(0.0f, Y, -X));
    Vertices.Add(FVector(0.0f, -Y, X));
    Vertices.Add(FVector(0.0f, -Y, -X));
}

bool USTC_MathFunctions::FindNextSplineLocation(
    UObject *WorldContextObject,
    FVector &NextLocation,
    FVector &NextUpVector,
    const float SegmentLength,
    const FVector LastLocation,
    const FVector LastDirection,
    const int32 Seed)
{
    
  //  UKismetMathLibrary::RandomUnitVectorInConeInDegreesFromStream(LastDirection, 45.f, FRandomStream(Seed))
    FVector PreLocation = LastLocation + (UKismetMathLibrary::RandomUnitVectorInConeInDegreesFromStream(LastDirection, 45.f, FRandomStream(Seed)) * SegmentLength);
   // FVector PreLocation = LastLocation + (LastDirection * SegmentLength);
    float ClosestDistance = SegmentLength * 1.5;
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
    TArray<AActor *> ActorsToIgnore;
    ActorsToIgnore.Add(NULL); //none
    FHitResult HitData(ForceInit);
    FVector Start;
    FVector End;
    TArray<FVector> IcoVertices;
    CalcIcosahedron(IcoVertices, ClosestDistance);
    FVector NextLoc;
    FVector NextUpVec;
    bool BlockHit;
    // UObject Object =
    for (int32 i = 0; i < 12; i++)
    {
        float TempLength;
        Start = FVector(PreLocation - IcoVertices[i]);
        End = FVector(PreLocation + IcoVertices[i]);
        if (UKismetSystemLibrary::LineTraceSingleForObjects(
                WorldContextObject,
                Start,
                End,
                ObjectTypes,
                true, //trace complex
                ActorsToIgnore,
                EDrawDebugTrace::None,
                HitData,
                true)) //ignore self
        {
            TempLength = (HitData.Location - PreLocation).Size();
            if (TempLength <= ClosestDistance)
            {
                BlockHit = true;
                ClosestDistance = TempLength;
                NextLoc = HitData.Location;
                NextUpVec = HitData.Normal; 
            }
        }
    }
    if (!BlockHit)
        return false;
    else
    {

        NextLocation = NextLoc;
        NextUpVector = NextUpVec;
        return true;
    }
}