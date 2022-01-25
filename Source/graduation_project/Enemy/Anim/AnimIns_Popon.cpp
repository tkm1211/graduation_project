// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimIns_Popon.h"

void UAnimIns_Popon::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (enm)
    {
        if (IsWander)
        {
            WanderRate += 400 * DeltaTime;

            if (WanderRate >= 100.f)
            {
                WanderRate = 100.f;
                wandertime += DeltaTime;
                if (wandertime >= 10.f)
                {
                    IsWander = false;
                    wandertime = 0.f;
                }
            }
        }
        else
        {
            WanderRate -= 400 * DeltaTime;

            if (WanderRate <= 0.f)
            {
                WanderRate = 0.f;
                if (Speed >= 100.f && FMath::RandRange(0, 99) == 31)
                {
                    IsWander = true;
                }
            }
        }
    }
}

void UAnimIns_Popon::NativePostEvaluateAnimation()
{
    Super::NativePostEvaluateAnimation();

    if (enm)
    {
        float DeltaTime = GetWorld()->GetDeltaSeconds();
        if (IsWander)
        {
            WanderRate += 400 * DeltaTime;

            if (WanderRate >= 100.f)
            {
                WanderRate = 100.f;
                wandertime += DeltaTime;
                if (wandertime >= 10.f)
                {
                    IsWander = false;
                    wandertime = 0.f;
                }
            }
        }
        else
        {
            WanderRate -= 400 * DeltaTime;

            if (WanderRate <= 0.f)
            {
                WanderRate = 0.f;
                if (Speed >= 100.f && FMath::RandRange(0, 99) == 31)
                {
                    IsWander = true;
                }
            }
        }
    }

}
