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

	bool CreateFigure(UClass* figureClass, UBoardCell* cell, ChessTeam team, float rotation = 0.f);

	//Spawn function which intakes bottom left corner position of the board and spawn 2 figures for each team
	bool AddFigureToBoard(UClass* figureClass, uint8 row, uint8 column, float rotation = 0, bool twoCopies = true);

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_MeshComponent;

	TStaticArray<TStaticArray<UBoardCell*, 8>, 8> mCells;
};
