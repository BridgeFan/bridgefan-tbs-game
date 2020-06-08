#pragma once
#ifndef ABILITY_HPP_INCLUDED
#define ABILITY_HPP_INCLUDED
#include <vector>
#include <optional>
#include <string>
constexpr int MAX_ABILITY_PARAMETERS_NUM = 5;
constexpr int ABILITY_COUNT = 5;
enum AbilityIndex ///8 first bites - n. of params, 24 last bites - index
{
	///ID + paramsNum * 2^24
	FirstStrike=0x0 + 0x0*0x1000000,
	UnlimitedCounterstrike=0x1 + 0x0*0x1000000,
	DoubleAttack=0x2 + 0x0*0x1000000,
	//NotFound=0xfffff
};

struct Ability
{
	static std::string name[ABILITY_COUNT];
	AbilityIndex abilityKind;
	int paramV[MAX_ABILITY_PARAMETERS_NUM];
	Ability(AbilityIndex id, int params[]);
	Ability(AbilityIndex id): abilityKind(id) {}
};

class AbilityList
{
	friend class Unit;
private:
	std::vector<Ability> aList;
public:
	void addAbility(Ability ab) {aList.push_back(ab);}
	void deleteAbility(AbilityIndex ab);
	std::optional<Ability> findAbility(AbilityIndex ab);
	std::vector<std::string> getNames();
};

#endif // ABILITY_HPP_INCLUDED
