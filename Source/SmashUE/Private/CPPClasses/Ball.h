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

	UFUNCTION(BlueprintCallable, Category = "Ball")
	void StartCharging();

	UFUNCTION(BlueprintCallable, Category = "Ball")
	void StopCharging();

private:
	float CurrentSize;
	float GrowthRate;
	bool bIsCharging;

	FTimerHandle DestroyTimerHandle;

	UFUNCTION()
	void DestroyBall();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};