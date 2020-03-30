// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessAI.h"
#include "ChessBoard.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Kismet/KismetMathLibrary.h" 

// Sets default values for this component's properties
UChessAI::UChessAI()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UChessAI::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UChessAI::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//Scan board for ai figures and select best move for this turn which is selected by simply selecting
//move with higher figure value (not taking into account figure position)
void UChessAI::ScanBoard(AChessBoard* board)
{
	if (!ensure(board))
	{
		return;
	}

	//Map used to select random move for random figure if didn't found best move
	TMap<AFigureBase*, TPair<int32, int32>> allFigures;

	float maxValue = FLT_MIN;
	UBoardCell* bestMove = nullptr;
	AFigureBase* bestFigure = nullptr;

	const uint8 boardLimit = 9;
	TArray<TPair<int32, int32>> moves;
	const size_t expectedMaxMoves = 64;
	moves.Reserve(expectedMaxMoves);
	for (uint8 row = 1; row < boardLimit; row++)
	{
		for (uint8 column = 1; column < boardLimit; column++)
		{
			UBoardCell* boardCell = board->GetCell(TPair<uint8, uint8>(row, column));
			if (!ensure(boardCell))
			{
				continue;
			}

			AFigureBase* figure = boardCell->GetFigure();
			if (!figure)
			{
				continue;
			}

			if (figure->GetTeam() == mAITeam)
			{
				if (figure->GetPossibleMoves(moves))
				{
					if (moves.Num() == 0)
					{
						continue;
					}
					auto& randomMove = moves[UKismetMathLibrary::RandomInteger(moves.Num())];
					allFigures.Add(figure, randomMove);

					for (const auto& move : moves)
					{
						UBoardCell* cell = board->GetCell(TPair<uint8, uint8>(move.Key, move.Value));
						if (!cell)
						{
							continue;
						}

						AFigureBase* valueFrom = cell->GetFigure();
						if (!valueFrom)
						{
							continue;
						}

						const float figureValue = valueFrom->GetFigureValue();
						if (figureValue > maxValue)
						{
							bestMove = cell;
							bestFigure = figure;
							maxValue = figureValue;
						}
					}
				}

			}

			moves.Reset(expectedMaxMoves);
		}
	}

	if (bestMove)
	{
		bestFigure->MoveTo(bestMove);
		bestMove->SetFigure(bestFigure);
	}
	else
	{
		size_t counter = 0;
		size_t randomIndex = UKismetMathLibrary::RandomInteger(allFigures.Num());
		for (const auto& it : allFigures)
		{
			if (counter == randomIndex)
			{
				AFigureBase* moveFigure = it.Key;
				if (moveFigure)
				{
					TPair<uint8, uint8> cellLocation{ it.Value.Key, it.Value.Value };
					UBoardCell* cell = board->GetCell(cellLocation);
					if (ensure(cell))
					{
						moveFigure->MoveTo(cell);
						cell->SetFigure(moveFigure);
					}
				}

				break;
			}
			counter++;
		}

	}
}