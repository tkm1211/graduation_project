#pragma once

#include "Containers/Array.h"


namespace util
{
    /// <summary>
    /// 最初にヒットしたComponentを１つ取得
    /// </summary>
    template<typename T>
    T* GetComponent(AActor* Actor) {
        return  Cast<T>(Actor->GetComponentByClass(T::StaticClass()));
    }

    /// <summary>
    /// 指定したコンポーネントすべて取得
    /// </summary>
    template<typename T>
    TArray<T*> GetComponents(AActor* Actor)
    {
        auto Components = Actor->GetComponentsByClass(T::StaticClass());

        TArray<T*> Result;

        for (auto Cmp : Components)
        {
            Result.Add(Cast<T>(Cmp));
        }

        return Result;
    }

    /// <summary>
    /// 指定した子供オブジェク1つト取得
    /// </summary>
    template<typename T>
    T* GetChildByClass(AActor* Actor)
    {
        TArray<AActor*> Childs;
        Actor->GetAllChildActors(Childs);

        T* FindAcotr;

        Childs.FindItemByClass<T>(&FindAcotr);

        return  FindAcotr;
    }

    /// <summary>
    /// 指定した子供オブジェクトすべて取得
    /// </summary>
    template<typename T>
    TArray<T*> GetChildByClassArray(AActor* Actor) {
        TArray<AActor*> Childs;
        Actor->GetAllChildActors(Childs);

        TArray<T*> Result;

        for (int32 i = 0; i < Childs.Num(); i++)
        {
            auto Child = Cast<T>(Childs[i]);
            if (Child != nullptr)
            {
                Result.Add(Child);
            }
        }

        return Result;
    }

}