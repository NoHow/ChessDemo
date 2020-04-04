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

	mCheckDirections.Reserve(8);
	mCheckDirections.Emplace(1, 0);
	mCheckDirections.Emplace(-1, 0);
	mCheckDirections.Emplace(0, 1);
	mCheckDirections.Emplace(0, -1);
	mCheckDirections.Emplace(1, 1);
	mCheckDirections.Emplace(-1, 1);
	mCheckDirections.Emplace(1, -1);
	mCheckDirections.Emplace(-1, -1);
}

void AKingFigure::GetPossibleMoves(TArray<TPair<int32, int32>>& moves)
{
	GetMovesBase(moves, 1);
}