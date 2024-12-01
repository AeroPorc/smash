#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UCLASS()
class SMASHUE_API ABall : public AActor
{
	GENERATED_BODY()

public:
	ABall();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void DestroyBall();

	FVector CurrentVelocity;

	UPROPERTY(EditAnywhere, Category = "Ball")
	float Speed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball", meta = (AllowPrivateAccess = "true"))
	AActor* OwnerActor;


private:
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "Ball")
	float Amplitude;

	UPROPERTY(EditAnywhere, Category = "Ball")
	float Frequency;

	UPROPERTY(EditAnywhere, Category = "Ball")
	float ThrowStrength;

	UPROPERTY(EditAnywhere, Category = "Ball")
	float LifeTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball", meta = (AllowPrivateAccess = "true"))
	float Gravity; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball", meta = (AllowPrivateAccess = "true"))
	float BounceDamping;




	FVector InitialLocation;
	FTimerHandle DestroyTimerHandle;
	 
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};