// Fill out your copyright notice in the Description page of Project Settings.


#include "QueenFigure.h"
#include "UObject/ConstructorHelpers.h"
#include "ChessBoard.h"
#include "BoardCell.h" 

AQueenFigure::AQueenFigure()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DarkFigureAsset(TEXT("/Game/Chess/Models/Queen/QueenDark_SM.QueenDark_SM"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WhiteFigureAsset(TEXT("/Game/Chess/Models/Queen/QueenWhite_SM.QueenWhite_SM"));

	if (DarkFigureAsset.Succeeded())
	{
		darkInstance = DarkFigureAsset.Object;
	}

	if (WhiteFigureAsset.Succeeded())
	{
		whiteInstance = WhiteFigureAsset.Object;
	}

	mFigureValue = 9.f;

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