/*
Supreme Commander 2 Reverse Engineering Project
This project contains the classes and structures I have discovered and explored so far
in the supreme commander 2 build currently available for steam.

By Synraw / Mike
*/
#pragma once

// All the Moho engine includes
#include "MString.h"
#include "SimDriver.h"
#include "Sim.h"
#include "SimArmy.h"
#include "Platoon.h"
#include "Unit.h"

namespace Moho
{
	// Metadata about the build of the game
	constexpr char* GAME_NAME		= "Supreme Commander 2";
	constexpr char* GAME_VERSION	= "1.260";
}