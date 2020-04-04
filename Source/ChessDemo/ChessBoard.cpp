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
#include "Containers/Array.h"

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

	for(uint8 rowIndex = 0; rowIndex < mCells.Num(); rowIndex++)
	{
		for(uint8 columnIndex = 0; columnIndex < mCells[rowIndex].Num(); columnIndex++)
		{
			FString name = "cell" + FString::FromInt(rowIndex) + FString::FromInt(columnIndex);
			mCells[rowIndex][columnIndex] = CreateDefaultSubobject<UBoardCell>(*name);
		}
	}

	SetRootComponent(Cast<USceneComponent>(m_MeshComponent));
}

// Called when the game starts or when spawned
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
	AddFigureToBoard(APawnFigure::StaticClass(), 2, 1);
	AddFigureToBoard(APawnFigure::StaticClass(), 2, 2);
	AddFigureToBoard(APawnFigure::StaticClass(), 2, 3);
	AddFigureToBoard(APawnFigure::StaticClass(), 2, 4);

	AddFigureToBoard(ARookFigure::StaticClass(), 1, 1);
	AddFigureToBoard(AKnightFigure::StaticClass(), 1, 2, 90.f);
	AddFigureToBoard(ABishopFigure::StaticClass(), 1, 3);
	AddFigureToBoard(AQueenFigure::StaticClass(), 1, 4, 0, false);
	AddFigureToBoard(AKingFigure::StaticClass(), 1, 5, 0, false);
}


void AChessBoard::InitBoard()
{
	const FVector meshBoardSize = m_MeshComponent->GetStaticMesh()->GetBounds().GetBox().GetSize();

	const int32 cellAmount = mCells.Num();
	const FVector cellSize = FVector{ meshBoardSize.X / cellAmount, meshBoardSize.Y / cellAmount, meshBoardSize.Z };

	const FVector offSet = cellSize / 2.f;
	const FVector startingPoint = GetActorLocation() - FVector{ meshBoardSize.X / 2.f, meshBoardSize.Y / 2.f, meshBoardSize.Z } - offSet;

	for(uint8 rowIndex = 0; rowIndex < mCells.Num(); ++rowIndex)
	{
		for(uint8 columnIndex = 0; columnIndex < mCells[rowIndex].Num(); ++columnIndex)
		{
			const uint8 offsetRow = rowIndex + 1u;
			const uint8 offsetColumn = columnIndex + 1u;

			mCells[rowIndex][columnIndex]->SetBoardPosition(TPair<uint8, uint8>(offsetRow, offsetColumn));
			mCells[rowIndex][columnIndex]->InitBoxExtent(FVector{ cellSize.X / 2.f, cellSize.Y / 2.f, 1.f });

			const FVector cellOffset = FVector{ cellSize.X * offsetRow, cellSize.Y * offsetColumn, meshBoardSize.Z * 2.f };
			mCells[rowIndex][columnIndex]->SetWorldLocation(startingPoint + cellOffset);
		}
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

	if (Cast<AKingFigure>(figure))
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
				controller->FinishChessGame(true);
			}
			else
			{
				controller->FinishChessGame(false);
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

bool AChessBoard::AddFigureToBoard(UClass* figureClass, uint8 row, uint8 column, float rotation , bool twoCopies)
{
	const uint8 boardBounds = 9;
	TArray<TPair<uint8, uint8>> spawnPositions;
	spawnPositions.Reserve(4);

	spawnPositions.Add(TPair<uint8, uint8>(row, column));
	spawnPositions.Add(TPair<uint8, uint8>(boardBounds - row, column));
	if (twoCopies)
	{
		spawnPositions.Add(TPair<uint8, uint8>(row, boardBounds - column));
		spawnPositions.Add(TPair<uint8, uint8>(boardBounds - row, boardBounds - column));
	}

	for (const auto& spawn : spawnPositions)
	{
		UBoardCell* spawnCell = GetCell(spawn);
		ChessTeam spawnTeam = ChessTeam::Dark;
		float spawnRotation = rotation;
		if (spawn.Key <= 4)
		{
			spawnTeam = ChessTeam::White;
			spawnRotation *= -1;
		}
		
		ensureAlways(CreateFigure(figureClass, spawnCell, spawnTeam, spawnRotation));
	}

	return true;
}

bool AChessBoard::CreateFigure(UClass* figureClass, UBoardCell* spawnCell, ChessTeam team, float rotation)
{
	UWorld* world = GetWorld();
	if (!world || !figureClass->IsChildOf(AFigureBase::StaticClass()))
	{
		return false;
	}

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	const FVector spawnLocation = spawnCell->GetComponentLocation();
	AFigureBase* figure = Cast<AFigureBase>(world->SpawnActor(figureClass, &spawnLocation, &FRotator::ZeroRotator, spawnParams));
	if (!figure)
	{
		return false;
	}

	figure->Init(team, spawnCell, this);
	figure->SetActorRotation(FRotator{ 0, rotation, 0 }, ETeleportType::TeleportPhysics);

	spawnCell->SetFigure(figure);
	
	return true;
}