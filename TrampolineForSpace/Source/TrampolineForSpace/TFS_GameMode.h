// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TFS_GameMode.generated.h"

// ゲームステート
UENUM(BlueprintType)
enum class EGameState : uint8
{
	ETitle,
	EPlaying,
	EGameOver,
	EGameClear,
	EUnknown
};


UCLASS(MinimalAPI)
class ATFS_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATFS_GameMode();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Config")
	EGameState GetCurrentState() const;

	void SetCurrentState(EGameState NewState);
	
protected:

	// タイムリミット
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config", meta = (BlueprintProtected = "true"))
	int32 TimeLimitBasis;

	// タイムリミットまでの時間
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 CountdownTime;

	// 最高到達点
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float heightRecord;

	// ゲームスコア
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 GameScore;

	// メッセージ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FString MessageStr;

	// Timerを管理する変数
	FTimerHandle CountdownTimerHandle;

	// タイトルWidgetクラス
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> TitleHUDWidgetClass;

	// MainHUDWidgetクラス
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> MainHUDWidgetClass;

	// HUDのインスタンス
	UPROPERTY()
	class UUserWidget* CurrentWidget;

	// スタート処理
	UFUNCTION(BlueprintCallable)
	void PlayStart();

	// ゲームオーバー処理
	UFUNCTION(BlueprintNativeEvent)
	void CountdownHasFinished();
	virtual void CountdownHasFinished_Implementation();

	// ゴール処理
	UFUNCTION(BlueprintCallable)
	void OnGoalProcess();

	// スコアを計算して表示する
	void DispScore();

	// タイマーの更新
	void AdvanceTimer();

private:
	// 現在ゲームのステート
	EGameState CurrentState;

	// 現在の高さ
	float CurrentHeight;

	// ステートに応じて処理を変えるためのハンドラ
	void HandleNewState(EGameState NewState);
};