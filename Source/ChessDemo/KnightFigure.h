// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FigureBase.h"
#include "KnightFigure.generated.h"

/**
 * 
 */
UCLASS()
class CHESSDEMO_API AKnightFigure : public AFigureBase
{
	GENERATED_BODY()
	
public:
	AKnightFigure();

	bool GetPossibleMoves(TArray<TPair<int32, int32>>& moves) override;
};
