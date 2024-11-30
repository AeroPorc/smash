// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraFollowTarget.h"
#include "SmashCharacterSettings.h"
#include "GameFramework/Character.h"
#include "SmashCharacter.generated.h"

class USmashCharacterStateMachine;
class ABall;

UCLASS()
class SMASHUE_API ASmashCharacter : public ACharacter, public ICameraFollowTarget
{
	GENERATED_BODY()
#pragma  region Unreal Default
public:
	ASmashCharacter();



protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma endregion
#pragma region Orient
	
public:
	float GetOrientX() const;
	const USmashCharacterSettings* GetSettings() const;
	void SetOrientX(float NewOrientX);

protected:
	UPROPERTY(BlueprintReadOnly)
	float OrientX = 0.f;
	float LastOrientX=1.f;
	void  RotateMeshUsingOrientX() const;
	
#pragma endregion
#pragma region State Machine
public:
	void CreateStateMachine();

	void InitStateMachine();

	void TickStateMachine(float DeltaTime) const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USmashCharacterStateMachine> StateMachine;
#pragma endregion

#pragma region Input Data / Mapping Context
public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<USmashCharacterInputData> InputData;

protected:
	void SetupMappingContextIntoCrontroller() const;

#pragma endregion

#pragma region Input Move X
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveXEvent, float, InputMoveX);
public:
	
	float GetInputMoveX() const;

	float GetInputThresholdX() const;
	float GetInputThresholdY() const;
	float GetInputSpecialActionThreshold() const;

	UPROPERTY()
	FInputMoveXEvent InputMoveXFastEvent;


	

	
	
protected:
	UPROPERTY()
	float InputMoveX = 0.f;

	UPROPERTY()
	float InputActionMoveThresholdX = 0.1f;

private:
	void OnInputMoveXFast(const FInputActionValue& InputActionValue);
	void BindInputMoveXAxisAndActions(UEnhancedInputComponent* InputComponent);
	
	void OnInputMoveX(const FInputActionValue& Value);

#pragma endregion

#pragma region Input Y
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveYEvent, float, InputMoveY);
public:
	float GetInputMoveY() const;
	FInputMoveYEvent InputMoveYEvent;
protected:
	UPROPERTY()
	float InputMoveY = 0.f;
private:
	void BindInputMoveYAxisAndActions(UEnhancedInputComponent* InputComponent);
	void OnInputMoveY(const FInputActionValue& Value);

#pragma region Special Action
public:
	float GetInputSpecialAction() const;
protected:
	UPROPERTY()
	float InputSpecialAction =  0.f;
private:
	void BindInputSpecialActionAndActions(UEnhancedInputComponent* InputComponent);
	void OnInputSpecialAction(const FInputActionValue& Value);
#pragma endregion
private:
#pragma endregion 
#pragma region Camera

public:
	virtual FVector GetFollowPosition() override;
	virtual bool IsFollowable() override;
	virtual bool IsFollowing();
#pragma endregion

#pragma region Ball

public:
	UFUNCTION(BlueprintCallable, Category = "Ball")
	void LaunchBall();
private:
	UPROPERTY(EditDefaultsOnly, Category = "Ball")
	TSubclassOf<ABall> BallClass;
#pragma endregion 
};


