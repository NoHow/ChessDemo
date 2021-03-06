// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/StaticArray.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerBox.h"
#include "FigureBase.h"
#include "BoardCell.h"
#include "ChessBoard.generated.h"

class AKingFigure;

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UBoardCell* GetCell(const TPair<uint8, uint8>& position);
	void KillFigure(AFigureBase* figure);
	void RestoreFigure(AFigureBase* figure);

	bool CheckMateUpdate(ChessTeam team);

	const TArray<AFigureBase*>& GetAllFigures() const;
	bool GetCheckStatus(ChessTeam team);

	void FinishGame(ChessTeam winner) const;
	bool CheckForPossibleMoves(ChessTeam team);

	bool CreateFigure(FigureType figureType, UBoardCell* cell, ChessTeam team, float rotation = 0.f);

private:
	void InitBoard();
	void FillTheBoard();

	//Spawn function which intakes bottom left corner position of the board and spawn 2 figures for each team
	bool AddFigureToBoard(UClass* figureClass, uint8 row, uint8 column, float rotation = 0, bool twoCopies = true);
	bool CreateFigure(UClass* figureClass, UBoardCell* cell, ChessTeam team, float rotation = 0.f);

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshComponent;

	TStaticArray<TStaticArray<UBoardCell*, 8>, 8> mCells;
	TArray<AFigureBase*> mFigures;

	AKingFigure* mWhiteKing = nullptr;
	AKingFigure* mDarkKing = nullptr;

	FVector mHideVector = FVector{ 0.f, 0.f, 500.f };
};
