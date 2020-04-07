// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FigureBase.h"
#include "PawnFigure.generated.h"

class UBoardCell;

UCLASS()
class CHESSDEMO_API APawnFigure : public AFigureBase
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnFigure();

	bool MoveTo(UBoardCell* newCell) override;
	void GetPossibleMoves(TArray<TPair<int32, int32>>& moves, bool checkUpdate = false) override;
	FigureType GetFigureType() const override;

	void OnReplacementSelect(FigureType type);
	
private:
	bool CheckCellForEnemy(TPair<uint8, uint8> cellPosition) const;
	bool CheckCellForFigure(TPair<uint8, uint8> cellPosition) const;

private:
	bool mFirstMove = true;
};
