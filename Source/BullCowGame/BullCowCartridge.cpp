// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    
    Isograms = GetValidWords(Words);
    SetUpGame();

    WelcomeMessage();
 
    PrintLine(TEXT("\nThe Hidden Word is: %s.\nIt is %i characters long"), *HiddenWord, HiddenWord.Len());
    PrintLine(TEXT("The number of possible words is %i"), Words.Num());

}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if(bGameOver)
    {
        ClearScreen();
        SetUpGame();
        WelcomeMessage(); 
    }
    else
    {
        CheckPlayerGuess(Input);
    }
}

void UBullCowCartridge::SetUpGame()
{
    HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num()-1)];
    Lives = HiddenWord.Len();
    bGameOver = false;
}

void UBullCowCartridge::WelcomeMessage()
{
    PrintLine(TEXT("Hello and welcome to Bulls and Cows"));
    PrintLine(TEXT("Enter the %i Letter Word"), HiddenWord.Len());
    PrintLine(TEXT("You have %i Guesses"), Lives); 
    PrintLine(TEXT("\nType in your Guess and\nPress Enter to Continue"));
}

void UBullCowCartridge::CheckPlayerGuess(const FString& PlayerWord)
{
    if (PlayerWord == HiddenWord)
    {
        ClearScreen();
        PrintLine(TEXT("You Win!"));
        EndGame();
        return;
    }

    if (PlayerWord.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Incorrect Length. \nThe Word is %i characters long. \nTry Again."), HiddenWord.Len());
        return;
    }

    if (!IsIsogram(PlayerWord))
    {
        PrintLine(TEXT("No repeating letters, guess again"));
    }

    PrintLine(TEXT("You lost a life!"));
    PrintLine(TEXT("You have %i lives remaining."), Lives);
    --Lives;

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
        return;
    }

    int32 Bulls, Cows;
    GetBullCows(PlayerWord, Bulls, Cows);

    PrintLine(TEXT("Have have %i Bulls and %i Cows"), Bulls, Cows);
    PrintLine(TEXT("Guess again. You have %i lives left."), Lives);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress Enter to Play Again"));
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> Isograms;
    for (FString Word : WordList)
    {
        if(4 <= Word.Len() && Word.Len() <= 8 && IsIsogram(Word))
        {
            Isograms.Emplace(Word);
        }
    }
    return Isograms;
}

void UBullCowCartridge::GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const
{
    BullCount = 0;
    CowCount = 0;

    //for each line in the guess. if it equals the index of hiddenword, its a bull otherwise its a cow

    for (int32 g = 0; g < Guess.Len(); g++)
    {
        if(Guess[g] == HiddenWord[g])
        {
            BullCount++;
            continue;
        }
        
        for (int32 h = 0; h < HiddenWord.Len(); h++)
        {
            if (Guess[g] == HiddenWord[h])
            {
                CowCount++;
                break;
            }
        } 
    }
}