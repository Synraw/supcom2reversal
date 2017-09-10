/*
Supreme Commander 2 Reverse Engineering Project
This project contains the classes and structures I have discovered and explored so far
in the supreme commander 2 build currently available for steam.

By Synraw / Mike
*/
#pragma once

#include "Math.h"
#include "MString.h"
#include "MArray.h"

namespace Moho
{
	enum EFaction
	{
		FAC_UEF,
		FAC_Cybran,
		FAC_Aeon,
	};

	class ArmyResources;
	class RuleGameRules;
	class Sim;
	class AiBrain;
	class AiReconDB;
	class ArmyStats;
	class Platoon;
	class Unit;

	class SimArmy
	{
	public:
		virtual void Function0(void) = 0;
		virtual Sim* GetParentSim(void) = 0;
		virtual bool IsHumanPlayer(void) = 0;
		virtual const char* GetDifficulty(void) = 0;
		virtual void Function4(void) = 0;
		virtual const char* GetAIScriptPath(void) = 0;
		virtual void Function6(void) = 0;
		virtual AiBrain* GetAIBrain(void) = 0;
		virtual AiReconDB* GetAIReconDB(void) = 0;
		virtual ArmyResources* GetResources(void) = 0;
		virtual void Function10(void) = 0;
		virtual void Function11(void) = 0;
		virtual void Function12(void) = 0;
		virtual void Function13(void) = 0;
		virtual void Function14(void) = 0;
		virtual void Function15(void) = 0;
		virtual void Function16(void) = 0;
		virtual void Function17(void) = 0;
		virtual void Function18(void) = 0;
		virtual void Function19(void) = 0;
		virtual void Function20(void) = 0;
		virtual void Function21(void) = 0;
		virtual void Function22(void) = 0;
		virtual void Function23(void) = 0;
		virtual void Function24(void) = 0;
		virtual Platoon* GetPlatoonByName(const char* name) = 0;
		virtual Platoon* GetUnitPlatoon(Unit* unit, int* something) = 0;
		virtual Platoon* GetPlatoonInArea(Vector3* location, float distance, Platoon* ignore, int taskNumber) = 0;
		virtual void Function28(void) = 0;
		virtual void Function29(void) = 0;
		virtual void Function30(void) = 0;
		virtual void Function31(void) = 0;
		virtual void Function32(void) = 0;
		virtual void Function33(void) = 0;
		virtual void Function34(void) = 0;
		virtual void Function35(void) = 0;
		virtual void Function36(void) = 0;
		virtual void AddUnitToArmy(Unit* unit) = 0;
		virtual void Function38(void) = 0;
		virtual void Function39(void) = 0;
		virtual void Function40(void) = 0;
		virtual void Function41(void) = 0;
		virtual void Function42(void) = 0;
		virtual void Function43(void) = 0;
		virtual void Function44(void) = 0;
		virtual void Function45(void) = 0;
		virtual void Function46(void) = 0;
		virtual void Function47(void) = 0;
		virtual void Function48(void) = 0;
		virtual void Function49(void) = 0;
		virtual void Function50(void) = 0;
		virtual void Function51(void) = 0;
		virtual void Function52(void) = 0;
		virtual void Function53(void) = 0;
		virtual void Function54(void) = 0;
		virtual void Function55(void) = 0;
		virtual void Function56(void) = 0;
		virtual void Function57(void) = 0;
		virtual void Function58(void) = 0;
		virtual void Function59(void) = 0;
		virtual void Function60(void) = 0;
		virtual void Function61(void) = 0;
		virtual void Function62(void) = 0;
		virtual void Function63(void) = 0;
		virtual void Function64(void) = 0;
		virtual void Function65(void) = 0;
		virtual void Function66(void) = 0;
		virtual void Function67(void) = 0;
		virtual void Function68(void) = 0;
		virtual void Function69(void) = 0;
		virtual void Function70(void) = 0;
	private:
		char pad_0x0000[0xC]; //0x0000
	public:
		MString m_strArmyName;
	private:
		char pad_0x0028[0x4]; //0x0028
	public:
		MString m_strPlayerName;
	private:
		char pad_0x0044[0x12C]; //0x0044
	public:
		unsigned int m_argbTeamColour; //0x0170 
	private:
		char pad_0x0174[0x4]; //0x0174
	public:
		MString m_strType;
		int m_iFaction; //0x0190 
	private:
		unsigned char N000001A5; //0x0194 
		unsigned char N00000D70; //0x0195 
		unsigned char N00000D73; //0x0196 
		unsigned char N00000D71; //0x0197 
		char pad_0x0198[0x28]; //0x0198
	public:
		RuleGameRules* m_pGameRules; //0x01C0 
	private:
		char pad_0x01C4[0x28]; //0x01C4
	public:
		Vector2 m_vec2SpawnLocation; //0x01EC 
	private:
		char pad_0x01F4[0x1C]; //0x01F4
	public:
		float m_flUnitCount; //0x0210 
	private:
		char pad_0x0214[0x54]; //0x0214
	public:
		Sim* m_pParentSim; //0x0268 
		AiBrain* m_pAiBrain; //0x026C 
		AiReconDB* m_pAiReconDB; //0x0270 
		ArmyResources* m_flArmyResources; //0x0274 
	private:
		char pad_0x0278[0x3C]; //0x0278
	public:
		ArmyStats* m_pArmyStats; //0x02B4 
	private:
		char pad_0x02B8[0x8]; //0x02B8
	public:
		MArray<Platoon*> m_platoonList;

		std::string GetFactionName()
		{
			switch (m_iFaction)
			{
			case EFaction::FAC_UEF:
				return "UEF";
			case EFaction::FAC_Cybran:
				return "Cybran";
			case EFaction::FAC_Aeon:
				return "Aeon";
			default:
				return "Unknown";
			};
		}
	}; //Size=0x0580

	class ArmyResources
	{
	public:
	private:
		char pad_0x0000[0x38]; //0x0000
	public:
		float m_flEnergy; //0x0038 
		float m_flMass; //0x003C 
		float m_flResearch; //0x0040 
		float m_flEnergyRate; //0x0044 
		float m_flMassRate; //0x0048 
		float m_flResearchRate; //0x004C 
	private:
		char pad_0x0050[0x3F0]; //0x0050

	}; //Size=0x0440

	// ****** BELOW HERE AREN'T REVERSED ******

	class CArmyStats
	{
	public:
	private:
		char pad_0x0000[0xC4]; //0x0000

	}; //Size=0x00C4

	class CAiBrain
	{
	public:
	private:
		char pad_0x0000[0x104]; //0x0000

	}; //Size=0x0104

	class CAiReconDB
	{
	public:
	private:
		char pad_0x0000[0xC4]; //0x0000

	}; //Size=0x00C4

	class CRuleGameRules
	{
	public:
	private:
		char pad_0x0000[0xC]; //0x0000
	public:
		void* N00000C93; //0x000C 
	private:
		char pad_0x0010[0xB4]; //0x0010

	}; //Size=0x00C4

}