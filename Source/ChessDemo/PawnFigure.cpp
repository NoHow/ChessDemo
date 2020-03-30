// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnFigure.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h" 
#include "UObject/ConstructorHelpers.h"
#include "BoardCell.h" 
#include "Containers/Array.h"
#include "ChessBoard.h"

// Sets default values
APawnFigure::APawnFigure()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DarkFigureAsset(TEXT("/Game/Chess/Models/Pawn/PawnDark_SM.PawnDark_SM"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WhiteFigureAsset(TEXT("/Game/Chess/Models/Pawn/PawnWhite_SM.PawnWhite_SM"));

	if (DarkFigureAsset.Succeeded())
	{
		darkInstance = DarkFigureAsset.Object;
	}

	if (WhiteFigureAsset.Succeeded())
	{
		whiteInstance = WhiteFigureAsset.Object;
	}

	mFigureValue = 1.f;
}

bool APawnFigure::GetPossibleMoves(TArray<TPair<int32, int32>>& moves)
{
	if (!ensure(mCurrentCell))
	{
		return false;
	}
	
	auto cellPosition = mCurrentCell->GetBoardPosition();
	const uint8 currentRow = cellPosition.Key;
	const uint8 currentColumn = cellPosition.Value;

	int8 teamDirection = 1;
	if (mCurrentTeam == ChessTeam::Dark)
	{
		teamDirection = -1;
	}
	else if(mCurrentTeam == ChessTeam::White)
	{
		teamDirection = 1;
	}
	else
	{
		return false;
	}

	const uint8 cellChecks = 2;
	for (int index = 1; index <= cellChecks; index++)
	{
		const int8 checkCellRow = currentRow + index * teamDirection;
		if(!CheckCellForFigure(TPair<uint8, uint8>(checkCellRow, currentColumn)))
		{
			moves.Add(TPair<int32, int32>(checkCellRow, currentColumn));
		}
		else
		{
			break;
		}

		if (!mFirstMove)
		{
			break;
		}
	}

	auto checkCellPosition = TPair<uint8, uint8>(currentRow + teamDirection, currentColumn + teamDirection);
	if (CheckCellForEnemy(checkCellPosition))
	{
		moves.Add(TPair<int32, int32>(checkCellPosition.Key, checkCellPosition.Value));
	}

	checkCellPosition.Value -= teamDirection * 2;
	if (CheckCellForEnemy(checkCellPosition))
	{
		moves.Add(TPair<int32, int32>(checkCellPosition.Key, checkCellPosition.Value));
	}

	return true;
}

bool APawnFigure::MoveTo(UBoardCell* newCell)
{
 	if (AFigureBase::MoveTo(newCell))
	{
		if (mFirstMove)
		{
			mFirstMove = false;
		}

		return true;
	}	
	
	return false;
}

bool APawnFigure::CheckCellForEnemy(TPair<uint8, uint8> cellPosition) const
{
	if (!mChessBoard)
	{
		return false;
	}

	UBoardCell* checkCell = mChessBoard->GetCell(cellPosition);
	if (checkCell)
	{
		AFigureBase* checkFigure = checkCell->GetFigure();
		if (checkFigure)
		{
			if (checkFigure->GetTeam() != mCurrentTeam)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}