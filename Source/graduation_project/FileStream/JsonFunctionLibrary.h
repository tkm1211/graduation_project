// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "JsonUtilities/Public/JsonUtilities.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Dom/JsonObject.h"
#include "JsonFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATION_PROJECT_API UJsonFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static TSharedPtr<FJsonObject> LoadJsonObject(const FString& Path);

	template<typename OutStructType>
	static TSharedPtr<OutStructType> LoadJsonStruct(const FString& Path)
	{
		const FString JsonFullPath = FPaths::ProjectDir().Append(Path).Append(TEXT(".json"));

		auto LoadError = [&JsonFullPath]()
		{
			UE_LOG(LogTemp, Error, TEXT("Failed LoadJson : %s"), *JsonFullPath);
			return nullptr;
		};

		FString loadFileString;
		auto temp = FFileHelper::LoadFileToString(loadFileString, *JsonFullPath);
		if (!temp)
		{
			LoadError();
		}

		TSharedPtr<OutStructType> woutStruct;
		OutStructType outStruct;
		if (!FJsonObjectConverter::JsonObjectStringToUStruct(loadFileString, &outStruct, 0, 0))
		{
			//UE_ERROR_LOG(fullPath + "failed deserialize");
			return nullptr;
		}

		return woutStruct;
	}
};
