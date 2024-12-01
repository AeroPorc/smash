#include "Ball.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "CPPClasses/SmashCharacter.h" 

ABall::ABall()
{
    PrimaryActorTick.bCanEverTick = true;

    USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    RootComponent = SphereComponent;
    SphereComponent->OnComponentHit.AddDynamic(this, &ABall::OnHit);
    SphereComponent->SetSimulatePhysics(true);
    SphereComponent->SetNotifyRigidBodyCollision(true);
    SphereComponent->BodyInstance.SetCollisionProfileName("BlockAllDynamic");

    SphereComponent->BodyInstance.bLockXRotation = true;
    SphereComponent->BodyInstance.bLockYRotation = true;
    SphereComponent->BodyInstance.bLockZRotation = true;
    
    Amplitude = 100.0f;
    Frequency = 1.0f;
    ThrowStrength = 500.0f;
    Gravity = -980.0f;
    LifeTime = 5.0f;
    BounceDamping = 0.7f;
    Speed = 300.0f;
}

void ABall::BeginPlay()
{
    Super::BeginPlay();

    InitialLocation = GetActorLocation();
    

    CurrentVelocity = FVector(Speed, 0.0f, -ThrowStrength);

    GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ABall::DestroyBall, LifeTime, false);

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Ball Spawned and Ready!"));
}

void ABall::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector CurrentLocation = GetActorLocation();

    CurrentVelocity.Z += Gravity * DeltaTime;
    CurrentLocation += CurrentVelocity * DeltaTime;

    FHitResult HitResult;
    FVector Start = CurrentLocation;
    FVector End = CurrentLocation + FVector::DownVector * 50.f;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);
    CollisionParams.AddIgnoredActor(OwnerActor);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_MAX, CollisionParams);
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 1);

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            FString HitActorTag = HitActor->Tags.Num() > 0 ? HitActor->Tags[0].ToString() : TEXT("None");
            GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Blue, FString::Printf(TEXT("Raycast hit: %s"), *HitActorTag));

            if (HitActor->ActorHasTag(TEXT("Ground")))
            {
                GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Green, TEXT("Raycast hit the ground"));
                if (CurrentLocation.Z <= InitialLocation.Z)
                {
                    CurrentLocation.Z = InitialLocation.Z;
                    CurrentVelocity.Z = FMath::Abs(CurrentVelocity.Z) * BounceDamping;
                    if (CurrentVelocity.Z < 100.f)
                    {
                        CurrentVelocity.Z = 100.f;
                    }
                }
            }
            else if(HitActor->IsA(ASmashCharacter::StaticClass()))
            {
                DestroyBall();
            }
        }
    }

    CurrentLocation.Y = InitialLocation.Y;

    SetActorLocation(CurrentLocation);

    GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow,
        FString::Printf(TEXT("Position: X=%.2f, Y=%.2f, Z=%.2f | Velocity: X=%.2f, Z=%.2f"),
                        CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z,
                        CurrentVelocity.X, CurrentVelocity.Z));
}
void ABall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != OwnerActor)
    {
        if (OtherActor->IsA(ASmashCharacter::StaticClass()))
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
                FString::Printf(TEXT("Ball Hit Player: %s"), *OtherActor->GetName()));
            DestroyBall();
        }
        else if (OtherComponent->ComponentHasTag(TEXT("Ground")))
        {
            CurrentVelocity.Z = FMath::Abs(CurrentVelocity.Z) * BounceDamping;
            if (CurrentVelocity.Z < 100.f)
            {
                CurrentVelocity.Z = 100.f;
            }
            if (CurrentVelocity.X == 0.f)
            {
                CurrentVelocity.X = Speed;
            }
        }
        else
        {
            CurrentVelocity.X *= 0.8f;
        }

        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
            FString::Printf(TEXT("Ball Hit: %s"), *OtherActor->GetName()));
    }
}

void ABall::DestroyBall()
{
    GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
    Destroy();
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ball Destroyed"));
}