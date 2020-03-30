// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/StaticArray.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerBox.h"
#include "FigureBase.h"
#include "BoardCell.h"
#include "ChessBoard.generated.h"


UENUM(BlueprintType)
enum class EBoardColumn : uint8
{
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H
};


UCLASS(Blueprintable)
class CHESSDEMO_API AChessBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessBoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UBoardCell* GetCell(const TPair<uint8, uint8>& position);
	void KillFigure(AFigureBase* figure);

private:
	void InitBoard();
	void FillTheBoard();

	//Spawn function which intakes bottom left corner position of the board and spawn 2 figures for each team
	template <class Figure>
	bool SpawnFigure(uint8 row, uint8 column, float rotation = 0, bool twoCopies = true)
	{
		UWorld* world = GetWorld();
		if (!world || !Figure::StaticClass()->IsChildOf(AFigureBase::StaticClass()))
		{
			return false;
		}

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
			if (spawnCell)
			{
				FActorSpawnParameters spawnParams;
				spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				Figure* figure = world->SpawnActor<Figure>(Figure::StaticClass(), spawnCell->GetComponentLocation(), FRotator::ZeroRotator, spawnParams);
				if (figure)
				{
					if (spawn.Key <= 4)
					{
						figure->Init(ChessTeam::White, spawnCell, this);
						figure->SetActorRotation(FRotator{ 0, -rotation, 0 }, ETeleportType::TeleportPhysics);
					}
					else
					{
						figure->Init(ChessTeam::Dark, spawnCell, this);
						figure->SetActorRotation(FRotator{ 0, rotation, 0 }, ETeleportType::TeleportPhysics);
					}

					spawnCell->SetFigure(figure);
				}
			}
		}

		return true;
	}
private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshComponent;

	TStaticArray<TStaticArray<UBoardCell*, 8>, 8> mCells;
};
