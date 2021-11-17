// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonFunctionLibrary.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"


TSharedPtr<FJsonObject> UJsonFunctionLibrary::LoadJsonObject(const FString& Path)
{
	const FString JsonFullPath = FPaths::ProjectDir().Append(Path).Append(TEXT(".json"));

	auto LoadError = [&JsonFullPath]()
	{
		UE_LOG(LogTemp, Error, TEXT("Failed LoadJson : %s"), *JsonFullPath);
		return nullptr;
	};

	FString loadFileString;
	if (FFileHelper::LoadFileToString(loadFileString, *JsonFullPath) == false)
	{
		LoadError();
	}

	const auto JsonReader = TJsonReaderFactory<TCHAR>::Create(loadFileString);
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(JsonReader, jsonObject) && jsonObject.IsValid())
	{
		return jsonObject;
	}
	return LoadError();
}