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

	mCheckDirections.Reserve(8);
	mCheckDirections.Emplace(2, 1);
	mCheckDirections.Emplace(1, 2);
	mCheckDirections.Emplace(-2, 1);
	mCheckDirections.Emplace(-1, 2);
	mCheckDirections.Emplace(2, -1);
	mCheckDirections.Emplace(1, -2);
	mCheckDirections.Emplace(-2, -1);
	mCheckDirections.Emplace(-1, -2);
}

void AKnightFigure::GetPossibleMoves(TArray<TPair<int32, int32>>& moves)
{
	//check(mChessBoard && mCurrentCell);

	//auto cellPosition = mCurrentCell->GetBoardPosition();
	//const uint8 currentRow = cellPosition.Key;
	//const uint8 currentColumn = cellPosition.Value;

	//for (const auto& direction : mCheckDirections)
	//{
	//	auto checkCellPosition = TPair<uint8, uint8>(currentRow + direction.Key, currentColumn + direction.Value);
	//	bool isThereFigure = false;
	//	if (CanMoveToCell(checkCellPosition, isThereFigure))
	//	{
	//		moves.Add(TPair<int32, int32>(checkCellPosition.Key, checkCellPosition.Value));
	//	}
	//}

	GetMovesBase(moves, 1);
}