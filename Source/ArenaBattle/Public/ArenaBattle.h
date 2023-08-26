// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINIT,
	LOADING,
	READY,
	DEAD
};


DECLARE_LOG_CATEGORY_EXTERN(ArenaBattle, Log, All);

// 함수명, 해당라인위치
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

// UE_LOG로 길게 쓰는 대신 아래처럼 간략히 사용할 수 있다. 
// Verbosity는 Error, Warning, Log같은 로그 위험 레벨 수준
#define ABLOG_S(Verbosity) UE_LOG(ArenaBattle, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)

// 추가로 더 출력하고 싶은 값이 있으면 아래처럼 출력(가변 인자)
#define ABLOG(Verbosity, Format, ...) UE_LOG(ArenaBattle, Verbosity, TEXT("%s%s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

// Expr 조건이 참이 아니면 Error 로그가 발생해 붉게 뜨게 된다.
#define ABCHECK(Expr, ...)	{ if(!(Expr)) {ABLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; }}

