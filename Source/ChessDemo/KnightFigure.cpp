// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightFigure.h"
#include "Engine/StaticMesh.h"
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

void AKnightFigure::GetPossibleMoves(TArray<TPair<int32, int32>>& moves, bool checkUpdate)
{
	GetMovesBase(moves, checkUpdate, 1);
}

FigureType AKnightFigure::GetFigureType() const
{
	return FigureType::Knight;
}