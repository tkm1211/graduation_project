#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"


template <typename WidgetT = UUserWidget, typename OwnerT = UObject>
WidgetT* CreateWidget(OwnerT* OwningObject, TSubclassOf<UUserWidget> UserWidgetClass = WidgetT::StaticClass(), FName WidgetName = NAME_None)
{
    static_assert(TIsDerivedFrom<WidgetT, UUserWidget>::IsDerived, "CreateWidget can only be used to create UserWidget instances. If creating a UWidget, use WidgetTree::ConstructWidget.");

    static_assert(TIsDerivedFrom<OwnerT, UWidget>::IsDerived
        || TIsDerivedFrom<OwnerT, UWidgetTree>::IsDerived
        || TIsDerivedFrom<OwnerT, APlayerController>::IsDerived
        || TIsDerivedFrom<OwnerT, UGameInstance>::IsDerived
        || TIsDerivedFrom<OwnerT, UWorld>::IsDerived, "The given OwningObject is not of a supported type for use with CreateWidget.");

    SCOPE_CYCLE_COUNTER(STAT_CreateWidget);

    if (OwningObject)
    {
        return Cast<WidgetT>(UUserWidget::CreateWidgetInstance(*OwningObject, UserWidgetClass, WidgetName));
    }
    return nullptr;
}