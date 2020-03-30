// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightFigure.h"
#include "UObject/ConstructorHelpers.h"
#include "ChessBoard.h"
#include "BoardCell.h" 

AKnightFigure::AKnightFigure()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DarkFigureAsset(TEXT("/Game/Chess/Models/Knight/KnightDark_SM.KnightDark_SM"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WhiteFigureAsset(TEXT("/Game/Chess/Models/Knight/KnightWhite_SM.KnightWhite_SM"));

	if (DarkFigureAsset.Succeeded())
	{
		darkInstance = DarkFigureAsset.Object;
	}

	if (WhiteFigureAsset.Succeeded())
	{
		whiteInstance = WhiteFigureAsset.Object;
	}

	mFigureValue = 3.f;
}

bool AKnightFigure::GetPossibleMoves(TArray<TPair<int32, int32>>& moves)
{
	if (!ensure(mCurrentCell) && !ensure(mChessBoard))
	{
		return false;
	}

	auto cellPosition = mCurrentCell->GetBoardPosition();
	const uint8 currentRow = cellPosition.Key;
	const uint8 currentColumn = cellPosition.Value;

	TArray<TPair<int32, int32>> checkDirections;
	checkDirections.Emplace(TPair<int32, int32>(2, 1));
	checkDirections.Emplace(TPair<int32, int32>(1, 2));
	checkDirections.Emplace(TPair<int32, int32>(-2, 1));
	checkDirections.Emplace(TPair<int32, int32>(-1, 2));
	checkDirections.Emplace(TPair<int32, int32>(2, -1));
	checkDirections.Emplace(TPair<int32, int32>(1, -2));
	checkDirections.Emplace(TPair<int32, int32>(-2, -1));
	checkDirections.Emplace(TPair<int32, int32>(-1, -2));

	for (const auto& direction : checkDirections)
	{
		auto checkCellPosition = TPair<uint8, uint8>(currentRow + direction.Key, currentColumn + direction.Value);
		if (CanMoveToCell(checkCellPosition))
		{
			moves.Add(TPair<int32, int32>(checkCellPosition.Key, checkCellPosition.Value));
		}
	}

	return true;
}