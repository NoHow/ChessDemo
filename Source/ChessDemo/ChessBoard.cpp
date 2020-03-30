// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessBoard.h"
#include "PawnFigure.h"
#include "RookFigure.h"
#include "KnightFigure.h"
#include "BishopFigure.h"
#include "KingFigure.h"
#include "QueenFigure.h"
#include "FigureBase.h"
#include "Engine/StaticMesh.h"
#include "ChessDemoGameModeBase.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "BoardCell.h"
#include "PlayerChessController.h"

// Sets default values
AChessBoard::AChessBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoardAsset(TEXT("/Game/Chess/Models/Board/Board_SM.Board_SM"));
	if (BoardAsset.Succeeded())
	{
		m_MeshComponent->SetStaticMesh(BoardAsset.Object);
	}

	check(m_MeshComponent);

	size_t counter = 0;
	for (auto& row : mCells)
	{
		for (auto& cell : row)
		{
			FString name = "cell" + FString::FromInt(counter);
			cell = CreateDefaultSubobject<UBoardCell>(*name);
			counter++;
		}
	}

	SetRootComponent(Cast<USceneComponent>(m_MeshComponent));
}

// Called when the game s	tarts or when spawned
void AChessBoard::BeginPlay()
{
	Super::BeginPlay();

	InitBoard();

	FillTheBoard();

	UWorld* world = GetWorld();
	if (world)
	{
		APlayerChessController* controller = Cast<APlayerChessController>(world->GetFirstPlayerController());
		if (ensure(controller))
		{
			controller->SetBoard(this);
		}
	}
}

// Called every frame
void AChessBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChessBoard::FillTheBoard()
{
	SpawnFigure<APawnFigure>(2, 1);
	SpawnFigure<APawnFigure>(2, 2);
	SpawnFigure<APawnFigure>(2, 3);
	SpawnFigure<APawnFigure>(2, 4);

	SpawnFigure<ARookFigure>(1, 1);
	SpawnFigure<AKnightFigure>(1, 2, 90.f);
	SpawnFigure<ABishopFigure>(1, 3);
	SpawnFigure<AQueenFigure>(1, 4, 0, false);
	SpawnFigure<AKingFigure>(1, 5, 0, false);
}


void AChessBoard::InitBoard()
{
	const FVector meshBoardSize = m_MeshComponent->GetStaticMesh()->GetBounds().GetBox().GetSize();

	const int32 cellAmount = mCells.Num();
	const FVector cellSize = FVector{ meshBoardSize.X / cellAmount, meshBoardSize.Y / cellAmount, meshBoardSize.Z };

	const FVector offSet = cellSize / 2.f;
	const FVector startingPoint = GetActorLocation() - FVector{ meshBoardSize.X / 2.f, meshBoardSize.Y / 2.f, meshBoardSize.Z } -offSet;
	uint8 i = 1;
	for (auto& row : mCells)
	{
		uint8 j = 1;
		for (auto& cell : row)
		{
			cell->SetBoardPosition(TPair<uint8, uint8>(i, j));
			cell->InitBoxExtent(FVector{ cellSize.X / 2.f, cellSize.Y / 2.f, 1.f });

			const FVector cellOffset = FVector{ cellSize.X * i, cellSize.Y * j, meshBoardSize.Z * 2.f };
			cell->SetWorldLocation(startingPoint + cellOffset);
			++j;
		}
		++i;
	}

}

void AChessBoard::KillFigure(AFigureBase* figure)
{
	if (!figure)
	{
		return;
	}

	UBoardCell* cell = figure->GetCurrentCell();
	if (cell)
	{
		cell->SetFigure(nullptr);
	}

	if (figure->StaticClass() == AKingFigure::StaticClass())
	{
		UWorld* world = GetWorld();
		APlayerChessController* controller = nullptr;
		if (ensure(world))
		{
			controller = Cast<APlayerChessController>(world->GetFirstPlayerController());
		}
		
		if (controller)
		{
			if (figure->GetTeam() == ChessTeam::Dark)
			{
				controller->FinishGame(ChessTeam::White);
			}
			else
			{
				controller->FinishGame(ChessTeam::Dark);
			}
		}
	}
	figure->Destroy();
}

UBoardCell* AChessBoard::GetCell(const TPair<uint8, uint8>& position)
{
	const uint8 row = position.Key - 1;
	const uint8 column = position.Value - 1;
	if (row >= mCells.Num() || column >= mCells[row].Num())
	{
		return nullptr;
	}

	return mCells[row][column];
}