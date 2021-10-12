#pragma once

#include "Containers/Array.h"


namespace util
{
    /// <summary>
    /// �ŏ��Ƀq�b�g����Component���P�擾
    /// </summary>
    template<typename T>
    T* GetComponent(AActor* Actor) {
        return  Cast<T>(Actor->GetComponentByClass(T::StaticClass()));
    }

    /// <summary>
    /// �w�肵���R���|�[�l���g���ׂĎ擾
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
    /// �w�肵���q���I�u�W�F�N1�g�擾
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
    /// �w�肵���q���I�u�W�F�N�g���ׂĎ擾
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