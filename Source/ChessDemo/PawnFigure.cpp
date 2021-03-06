// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnFigure.h"
#include "Components/InputComponent.h"
#include "PlayerChessController.h"
#include "Engine/Engine.h" 
#include "BoardCell.h" 
#include "Containers/Array.h"
#include "ChessBoard.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

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

void APawnFigure::GetPossibleMoves(TArray<TPair<int32, int32>>& moves, bool checkUpdate)
{
	check(mCurrentCell);

	auto cellPosition = mCurrentCell->GetBoardPosition();
	const uint8 currentRow = cellPosition.Key;
	const uint8 currentColumn = cellPosition.Value;

	int8 teamDirection = 1;
	if (mCurrentTeam == ChessTeam::Dark)
	{
		teamDirection = -1;
	}
	else if (mCurrentTeam == ChessTeam::White)
	{
		teamDirection = 1;
	}

	uint8 forwardCell = 1 * teamDirection;
	int8 checkCellRow = currentRow + forwardCell;
	if (!CheckCellForFigure(TPair<uint8, uint8>(checkCellRow, currentColumn)))
	{
		moves.Add(TPair<int32, int32>(checkCellRow, currentColumn));

		if (mFirstMove)
		{
			forwardCell += teamDirection;
			checkCellRow = currentRow + forwardCell;
			if (!CheckCellForFigure(TPair<uint8, uint8>(checkCellRow, currentColumn)))
			{
				moves.Add(TPair<int32, int32>(checkCellRow, currentColumn));
			}
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

	if (checkUpdate)
	{
		TArray<TPair<int32, int32>> finalMoves;
		finalMoves.Reserve(3);

		for (const auto& move : moves)
		{
			UBoardCell* checkCell = mChessBoard->GetCell(TPair<uint8, uint8>(move.Key, move.Value));
			if (MakeMove(checkCell))
			{
				checkCell->SetFigure(this);
				if (!mChessBoard->GetCheckStatus(mCurrentTeam))
				{
					finalMoves.Add(move);
				}

				AFigureBase::CancelMove();
			}
		}

		moves = finalMoves;
	}
}

bool APawnFigure::MoveTo(UBoardCell* newCell)
{
 	if (AFigureBase::MoveTo(newCell))
	{
		if (mFirstMove)
		{
			mFirstMove = false;
		}

		const auto& newPosition = mCurrentCell->GetBoardPosition();
		if (newPosition.Key == 1 || newPosition.Key == 8)
		{
			auto* controller = Cast<APlayerChessController>(GetWorld()->GetFirstPlayerController());
			controller->GetPawnPromotion(this);
		}
		return true;
	}	
	
	return false;
}

FigureType APawnFigure::GetFigureType() const
{
	return FigureType::Pawn;
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

bool APawnFigure::CheckCellForFigure(TPair<uint8, uint8> cellPosition) const
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
			return true;
		}
	}

	return false;
}

void APawnFigure::OnReplacementSelect(FigureType type)
{
	mChessBoard->KillFigure(this);

	if (mCurrentTeam == ChessTeam::Dark)
	{
		mChessBoard->CreateFigure(type, mCurrentCell, mCurrentTeam, 90.f);
	}
	else
	{
		mChessBoard->CreateFigure(type, mCurrentCell, mCurrentTeam, -90.f);
	}
}