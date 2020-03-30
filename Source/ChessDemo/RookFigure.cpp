// Fill out your copyright notice in the Description page of Project Settings.


#include "RookFigure.h"
#include "UObject/ConstructorHelpers.h"
#include "ChessBoard.h"
#include "BoardCell.h" 

ARookFigure::ARookFigure()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DarkFigureAsset(TEXT("/Game/Chess/Models/Rook/RookDark_SM.RookDark_SM"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WhiteFigureAsset(TEXT("/Game/Chess/Models/Rook/RookWhite_SM.RookWhite_SM"));

	if (DarkFigureAsset.Succeeded())
	{
		darkInstance = DarkFigureAsset.Object;
	}

	if (WhiteFigureAsset.Succeeded())
	{
		whiteInstance = WhiteFigureAsset.Object;
	}

	SetActorScale3D(FVector{ 0.05 });
	mFigureValue = 5.f;
}

bool ARookFigure::GetPossibleMoves(TArray<TPair<int32, int32>>& moves)
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

	for (const auto& direction : checkDirections)
	{
		checkCellPosition.Key = currentRow + direction.Key;
		checkCellPosition.Value = currentColumn + direction.Value;

		checkCell = mChessBoard->GetCell(checkCellPosition);
		while (checkCell)
		{
			AFigureBase* checkCellFigure = checkCell->GetFigure();
			if (!checkCellFigure)
			{
				moves.Add(TPair<int32, int32>(checkCellPosition.Key, checkCellPosition.Value));
			}
			else
			{
				if (checkCellFigure->GetTeam() == mCurrentTeam)
				{
					break;
				}
				else
				{
					moves.Add(TPair<int32, int32>(checkCellPosition.Key, checkCellPosition.Value));
					break;
				}
			}

			checkCellPosition.Key += direction.Key;
			checkCellPosition.Value += direction.Value;

			checkCell = mChessBoard->GetCell(checkCellPosition);
		}
	}

	return true;
}