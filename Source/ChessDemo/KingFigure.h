// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FigureBase.h"
#include "KingFigure.generated.h"

/**
 * 
 */
UCLASS()
class CHESSDEMO_API AKingFigure : public AFigureBase
{
	GENERATED_BODY()
	
public:
	AKingFigure();

	void GetPossibleMoves(TArray<TPair<int32, int32>>& moves, bool checkUpdate = false) override;
	FigureType GetFigureType() const override;

	bool GetCheckStatus();

private:
	bool GetCheckMateStatus();
};
