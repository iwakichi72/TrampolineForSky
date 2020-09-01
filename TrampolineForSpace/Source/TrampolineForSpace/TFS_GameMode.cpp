// Fill out your copyright notice in the Description page of Project Settings.


#include "TFS_GameMode.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/GameFramework/PawnMovementComponent.h"

ATFS_GameMode::ATFS_GameMode()
{
	// Tick関数を有効にする
	PrimaryActorTick.bCanEverTick = true;
}

void ATFS_GameMode::BeginPlay()
{
	Super::BeginPlay();

	// ステートをTitleに
	SetCurrentState(EGameState::ETitle);
	
}

void ATFS_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	CurrentHeight = (MyCharacter->GetActorLocation().Z - 205) / 100;
	// 最大到達点を更新し続ける
	if (heightRecord < CurrentHeight)
		heightRecord = CurrentHeight;

}

void ATFS_GameMode::AdvanceTimer()
{
	--CountdownTime;
	if (CountdownTime < 1)
	{
		// カウントダウンが完了、タイマーの実行を停止。
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);

		CountdownHasFinished();
	}
}

// MainBlueprintのBeginPlayから呼ばれる
void ATFS_GameMode::PlayStart()
{
	SetCurrentState(EGameState::EPlaying);
	// カウントダウンする時間をセットする
	CountdownTime = TimeLimitBasis;

	heightRecord = 0;

	// カウントダウンを始める
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ATFS_GameMode::AdvanceTimer, 1.0f, true);
}

void ATFS_GameMode::CountdownHasFinished_Implementation()
{
	SetCurrentState(EGameState::EGameOver);
}

void ATFS_GameMode::OnGoalProcess()
{
	SetCurrentState(EGameState::EGameClear);
}

EGameState ATFS_GameMode::GetCurrentState() const
{
	return CurrentState;
}

void ATFS_GameMode::SetCurrentState(EGameState NewState)
{
	CurrentState = NewState;
	HandleNewState(NewState);
}

void ATFS_GameMode::HandleNewState(EGameState NewState)
{

	switch (NewState)
	{
	case EGameState::ETitle:
	{
		if (TitleHUDWidgetClass != nullptr)
		{
			// Titleウィジェットの追加
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), TitleHUDWidgetClass);
			if (CurrentWidget != nullptr)
				CurrentWidget->AddToViewport();
		}
	}
	case EGameState::EPlaying:
	{
		//MessageStr = "Playing...";
		//FPlatformMisc::MessageBoxExt(EAppMsgType::Ok, TEXT("Message"), TEXT("Error"));// debuglog
		
		if (MainHUDWidgetClass != nullptr)
		{
			// ウィジェットの初期化
			if (CurrentWidget != nullptr)
				CurrentWidget->RemoveFromViewport();
			// Mainウィジェットの追加
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MainHUDWidgetClass);
			if (CurrentWidget != nullptr)
				CurrentWidget->AddToViewport();
		}
	}
	break;
	case EGameState::EGameClear:
	{
		MessageStr = "GOAL!";

		// カウントダウンが完了、タイマーの実行を停止。
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);

		DispScore();
	}
	break;
	case EGameState::EGameOver:
	{
		// プレイヤーの入力をオフに
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
			PlayerController->SetCinematicMode(true, false, false, true, true);

		MessageStr = "TIMEUP!";

		DispScore();
	}
	break;
	case EGameState::EUnknown:
	{}
	break;
	}
}

void ATFS_GameMode::DispScore() 
{
	// 最大到達点を表示
	MessageStr = MessageStr + "\nYour MaxJump : " + FString::SanitizeFloat((int)heightRecord) + "M";

	// スコア計算
	GameScore = (int)heightRecord * 100 + CountdownTime * 10;
	// ゴールしていたら1000pt加算
	if (GetCurrentState() == EGameState::EGameClear)
		GameScore = GameScore + 1000;

	// スコア表示
	MessageStr = MessageStr + "\nGame Score : " + FString::FromInt(GameScore) + "pt";
}