// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TFS_GameMode.generated.h"

// �Q�[���X�e�[�g
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

	// �^�C�����~�b�g
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config", meta = (BlueprintProtected = "true"))
	int32 TimeLimitBasis;

	// �^�C�����~�b�g�܂ł̎���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 CountdownTime;

	// �ō����B�_
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float heightRecord;

	// �Q�[���X�R�A
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 GameScore;

	// ���b�Z�[�W
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FString MessageStr;

	// Timer���Ǘ�����ϐ�
	FTimerHandle CountdownTimerHandle;

	// �^�C�g��Widget�N���X
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> TitleHUDWidgetClass;

	// MainHUDWidget�N���X
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> MainHUDWidgetClass;

	// HUD�̃C���X�^���X
	UPROPERTY()
	class UUserWidget* CurrentWidget;

	// �X�^�[�g����
	UFUNCTION(BlueprintCallable)
	void PlayStart();

	// �Q�[���I�[�o�[����
	UFUNCTION(BlueprintNativeEvent)
	void CountdownHasFinished();
	virtual void CountdownHasFinished_Implementation();

	// �S�[������
	UFUNCTION(BlueprintCallable)
	void OnGoalProcess();

	// �X�R�A���v�Z���ĕ\������
	void DispScore();

	// �^�C�}�[�̍X�V
	void AdvanceTimer();

private:
	// ���݃Q�[���̃X�e�[�g
	EGameState CurrentState;

	// ���݂̍���
	float CurrentHeight;

	// �X�e�[�g�ɉ����ď�����ς��邽�߂̃n���h��
	void HandleNewState(EGameState NewState);
};