// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPClasses/SmashCharacter.h"
#include "CPPClasses/SmashCharacterStateMachine.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "CPPClasses/SmashCharacterSettings.h"
#include "InputAction.h"
#include "CPPClasses/CameraWorldSubsystem.h"
#include "CPPClasses/Ball.h"

// Sets default values
ASmashCharacter::ASmashCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASmashCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateStateMachine();
	InitStateMachine();

	GetWorld()->GetSubsystem<UCameraWorldSubsystem>()->AddFollowTarget(this);
	
}

// Called every frame
void ASmashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickStateMachine(DeltaTime);
	if(FMath::Abs(OrientX)>=1)
	{
		LastOrientX=OrientX;
	}
	RotateMeshUsingOrientX();
}

// Called to bind functionality to input
void ASmashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// SetupMappingContextIntoCrontroller();

	UEnhancedInputComponent* LocalInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(LocalInputComponent == nullptr) return;

	BindInputMoveXAxisAndActions(LocalInputComponent);
	BindInputMoveYAxisAndActions(LocalInputComponent);
	BindInputSpecialActionAndActions(LocalInputComponent);
}

float ASmashCharacter::GetOrientX() const
{
	return OrientX;
}

void ASmashCharacter::SetOrientX(float NewOrientX)
{
	OrientX = NewOrientX;
}

void ASmashCharacter::RotateMeshUsingOrientX() const
{
	FRotator Rotation=GetMesh()->GetRelativeRotation();
	int X=LastOrientX;

	Rotation.Yaw = -90.f*X;
	GetMesh()->SetRelativeRotation(Rotation);
}

void ASmashCharacter::CreateStateMachine()
{
	StateMachine = NewObject<USmashCharacterStateMachine>(this);
}
void ASmashCharacter::InitStateMachine()
{
	if(StateMachine == nullptr) return;
	StateMachine->Init(this);
}

void ASmashCharacter::TickStateMachine(float DeltaTime) const
{
	if(StateMachine == nullptr) return;
	StateMachine->Tick(DeltaTime);
}

void ASmashCharacter::SetupMappingContextIntoCrontroller() const
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if(PlayerController == nullptr) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if(Player == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if(InputSystem == nullptr) return;

	InputSystem->AddMappingContext(InputMappingContext,0);
}

float ASmashCharacter::GetInputMoveX() const
{
	return InputMoveX;
}

void ASmashCharacter::BindInputMoveXAxisAndActions(UEnhancedInputComponent* LocalInputComponent)
{
    if(InputData == nullptr) return;

    if(InputData->InputActionMoveX)
    {
        LocalInputComponent->BindAction
        (
            InputData->InputActionMoveX,
            ETriggerEvent::Started,
            this,
            &ASmashCharacter::OnInputMoveX
        );
    	LocalInputComponent->BindAction
		(
			InputData->InputActionMoveX,
			ETriggerEvent::Completed,
			this,
			&ASmashCharacter::OnInputMoveX
		);
    	LocalInputComponent->BindAction
		(
			InputData->InputActionMoveX,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveX
		);
    }
    if (InputData->InputActionMoveXFast)
    {
    	LocalInputComponent->BindAction(
    		InputData->InputActionMoveXFast,
    		ETriggerEvent::Triggered,
    		this,
    		&ASmashCharacter::OnInputMoveXFast
    		);
    }
}
void ASmashCharacter::OnInputMoveX(const FInputActionValue& Value)
{
	InputMoveX = Value.Get<float>();
}

float ASmashCharacter::GetInputThresholdX() const
{
	return GetDefault<USmashCharacterSettings>()->InputActionMoveThresholdX;
}

void ASmashCharacter::OnInputMoveXFast(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
	InputMoveXFastEvent.Broadcast(InputMoveX);
}

float ASmashCharacter::GetInputMoveY() const
{
	return InputMoveY;
}
void ASmashCharacter::BindInputMoveYAxisAndActions(UEnhancedInputComponent* LocalInputComponent)
{
	if(InputData == nullptr) return;

	if(InputData->InputActionMoveY)
	{
		LocalInputComponent->BindAction(
			InputData->InputActionMoveY,
			ETriggerEvent::Started,
			this,
			&ASmashCharacter::OnInputMoveY
		);
		LocalInputComponent->BindAction(
			InputData->InputActionMoveY,
			ETriggerEvent::Completed,
			this,
			&ASmashCharacter::OnInputMoveY
		);
		LocalInputComponent->BindAction(
			InputData->InputActionMoveY,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveY
		);
	}
}
void ASmashCharacter::OnInputMoveY(const FInputActionValue& Value)
{
	InputMoveY = Value.Get<float>();
}

float ASmashCharacter::GetInputThresholdY() const
{
	return GetDefault<USmashCharacterSettings>()->InputActionMoveThresholdY;
}

float ASmashCharacter::GetInputSpecialActionThreshold() const
{
	return GetDefault<USmashCharacterSettings>()->InputSpecialActionThreshold;
}

const USmashCharacterSettings* ASmashCharacter::GetSettings() const
{
	return GetDefault<USmashCharacterSettings>();
}

bool ASmashCharacter::IsFollowable()
{
	return true;
}

FVector ASmashCharacter::GetFollowPosition()
{
	return GetActorLocation();
}

bool ASmashCharacter::IsFollowing() 
{
	return true;
}


void ASmashCharacter::LaunchBall()
{
	if (BallClass)
	{
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f * LastOrientX;
		FRotator SpawnRotation = GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		ABall* Ball = GetWorld()->SpawnActor<ABall>(BallClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (Ball)
		{
			Ball->CurrentVelocity.X = Ball->Speed * LastOrientX;
			Ball->OwnerActor = this;
			Ball->SetOwner(nullptr);

			Ball->Tags.Add(FName("IgnoreCamera"));
			// Ball->SetActorHiddenInGame(true);
			// Ball->SetActorEnableCollision(false);
			// Ball->SetActorTickEnabled(false);
		}
	}
}

void ASmashCharacter::BindInputSpecialActionAndActions(UEnhancedInputComponent* LocalInputComponent)
{
	if(InputData == nullptr) return;

	if(InputData->InputSpecialAction)
	{
		LocalInputComponent->BindAction(
			InputData->InputSpecialAction,
			ETriggerEvent::Started,
			this,
			&ASmashCharacter::OnInputSpecialAction
		);
		LocalInputComponent->BindAction(
			InputData->InputSpecialAction,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputSpecialAction
		);
		LocalInputComponent->BindAction(
			InputData->InputSpecialAction,
			ETriggerEvent::Completed,
			this,
			&ASmashCharacter::OnInputSpecialAction
		);
	}
}

void ASmashCharacter::OnInputSpecialAction(const FInputActionValue& Value)
{
	InputSpecialAction = Value.Get<float>();
}

float ASmashCharacter::GetInputSpecialAction() const
{
	return InputSpecialAction;
}







