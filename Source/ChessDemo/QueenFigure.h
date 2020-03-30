// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FigureBase.h"
#include "QueenFigure.generated.h"

/**
 * 
 */
UCLASS()
class CHESSDEMO_API AQueenFigure : public AFigureBase
{
	GENERATED_BODY()
	
public:
	AQueenFigure();

	bool GetPossibleMoves(TArray<TPair<int32, int32>>& moves) override;
};
