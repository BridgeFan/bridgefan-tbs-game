#include "ability.hpp"
#include <algorithm>
std::string Ability::name[ABILITY_COUNT] = {"First strike","Unl. counters","Double attack","",""};
Ability::Ability(AbilityIndex id, int params[]): abilityKind(id)
{
    for(int i=0;i<std::min(id>>24,MAX_ABILITY_PARAMETERS_NUM);i++)
    {
		paramV[i]=params[i];
    }
    id=static_cast<AbilityIndex>((int)id&0xffffff);
}
std::optional<Ability> AbilityList::findAbility(AbilityIndex ab)
{
	auto it = std::find_if(aList.begin(),aList.end(),[ab](const Ability& a){return (int)(a.abilityKind)==(int)ab;});
	if(it!=aList.end())
		return *it;
	return std::nullopt;
	///operator bool has_value
}
void AbilityList::deleteAbility(AbilityIndex ab)
{
	auto it = std::find_if(aList.begin(),aList.end(),[ab](const Ability& a){return (int)(a.abilityKind)==(int)ab;});
	if(it!=aList.end())
		aList.erase(it);
}
std::vector<std::string> AbilityList::getNames()
{
	std::vector<std::string> ret;
	for(std::size_t i=0u;i<aList.size();i++)
		ret.push_back(Ability::name[aList[i].abilityKind]);
	return ret;
}
