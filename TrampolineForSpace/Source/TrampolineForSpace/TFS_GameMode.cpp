// Fill out your copyright notice in the Description page of Project Settings.


#include "TFS_GameMode.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/GameFramework/PawnMovementComponent.h"

ATFS_GameMode::ATFS_GameMode()
{
	// Tick�֐���L���ɂ���
	PrimaryActorTick.bCanEverTick = true;
}

void ATFS_GameMode::BeginPlay()
{
	Super::BeginPlay();

	// �X�e�[�g��Title��
	SetCurrentState(EGameState::ETitle);
	
}

void ATFS_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	CurrentHeight = (MyCharacter->GetActorLocation().Z - 205) / 100;
	// �ő哞�B�_���X�V��������
	if (heightRecord < CurrentHeight)
		heightRecord = CurrentHeight;

}

void ATFS_GameMode::AdvanceTimer()
{
	--CountdownTime;
	if (CountdownTime < 1)
	{
		// �J�E���g�_�E���������A�^�C�}�[�̎��s���~�B
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);

		CountdownHasFinished();
	}
}

// MainBlueprint��BeginPlay����Ă΂��
void ATFS_GameMode::PlayStart()
{
	SetCurrentState(EGameState::EPlaying);
	// �J�E���g�_�E�����鎞�Ԃ��Z�b�g����
	CountdownTime = TimeLimitBasis;

	heightRecord = 0;

	// �J�E���g�_�E�����n�߂�
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
			// Title�E�B�W�F�b�g�̒ǉ�
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
			// �E�B�W�F�b�g�̏�����
			if (CurrentWidget != nullptr)
				CurrentWidget->RemoveFromViewport();
			// Main�E�B�W�F�b�g�̒ǉ�
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MainHUDWidgetClass);
			if (CurrentWidget != nullptr)
				CurrentWidget->AddToViewport();
		}
	}
	break;
	case EGameState::EGameClear:
	{
		MessageStr = "GOAL!";

		// �J�E���g�_�E���������A�^�C�}�[�̎��s���~�B
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);

		DispScore();
	}
	break;
	case EGameState::EGameOver:
	{
		// �v���C���[�̓��͂��I�t��
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
	// �ő哞�B�_��\��
	MessageStr = MessageStr + "\nYour MaxJump : " + FString::SanitizeFloat((int)heightRecord) + "M";

	// �X�R�A�v�Z
	GameScore = (int)heightRecord * 100 + CountdownTime * 10;
	// �S�[�����Ă�����1000pt���Z
	if (GetCurrentState() == EGameState::EGameClear)
		GameScore = GameScore + 1000;

	// �X�R�A�\��
	MessageStr = MessageStr + "\nGame Score : " + FString::FromInt(GameScore) + "pt";
}