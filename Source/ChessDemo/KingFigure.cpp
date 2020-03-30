// Fill out your copyright notice in the Description page of Project Settings.


#include "KingFigure.h"
#include "UObject/ConstructorHelpers.h"
#include "ChessBoard.h"
#include "BoardCell.h" 

AKingFigure::AKingFigure()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DarkFigureAsset(TEXT("/Game/Chess/Models/King/KingDark_SM.KingDark_SM"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WhiteFigureAsset(TEXT("/Game/Chess/Models/King/KingWhite_SM.KingWhite_SM"));

	if (DarkFigureAsset.Succeeded())
	{
		darkInstance = DarkFigureAsset.Object;
	}

	if (WhiteFigureAsset.Succeeded())
	{
		whiteInstance = WhiteFigureAsset.Object;
	}

	mFigureValue = 10.f;
}

bool AKingFigure::GetPossibleMoves(TArray<TPair<int32, int32>>& moves)
{
	if (!ensure(mCurrentCell) && !ensure(mChessBoard))
	{
		return false;
	}

	auto cellPosition = mCurrentCell->GetBoardPosition();
	const uint8 currentRow = cellPosition.Key;
	const uint8 currentColumn = cellPosition.Value;

	auto checkCellPosition = TPair<uint8, uint8>(currentRow, currentColumn);
	UBoardCell* checkCell = nullptr;
	TArray<TPair<int32, int32>> checkDirections;
	checkDirections.Emplace(TPair<int32, int32>(1, 0));
	checkDirections.Emplace(TPair<int32, int32>(-1, 0));
	checkDirections.Emplace(TPair<int32, int32>(0, 1));
	checkDirections.Emplace(TPair<int32, int32>(0, -1));
	checkDirections.Emplace(TPair<int32, int32>(1, 1));
	checkDirections.Emplace(TPair<int32, int32>(-1, 1));
	checkDirections.Emplace(TPair<int32, int32>(1, -1));
	checkDirections.Emplace(TPair<int32, int32>(-1, -1));

	for (const auto& direction : checkDirections)
	{
		checkCellPosition.Key = currentRow + direction.Key;
		checkCellPosition.Value = currentColumn + direction.Value;

		if (CanMoveToCell(checkCellPosition))
		{
			moves.Add(TPair<int32, int32>(checkCellPosition.Key, checkCellPosition.Value));
		}
	}

	return true;
}