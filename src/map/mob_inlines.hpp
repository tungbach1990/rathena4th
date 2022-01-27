// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MOB_INLINES_HPP
#define MOB_INLINES_HPP
#include "mob.hpp"
#include "unit.hpp"

namespace expanded_ai{
namespace predicates {

/**
 * @brief Function object that stores the data to be tested when called
*/
class condition_predicate {
protected:
	e_mob_skill_target target_type_id;
public:
	condition_predicate(e_mob_skill_target target_type_id) : target_type_id{ target_type_id } {}
	virtual bool operator()(const std::map<e_mob_skill_target, block_list*>& targets) = 0;
	inline bool condition_predicate::checkTarget(const std::map<e_mob_skill_target, block_list*>& targets) {
		return targets.count(target_type_id);
	}
};

struct exists : public condition_predicate {
	exists(e_mob_skill_target target_type_id) : condition_predicate(target_type_id){}
	inline bool operator()(const std::map<e_mob_skill_target, block_list*>& targets) {
		return checkTarget(targets);
	}
};

template <class TComparator>
struct percent_health: public condition_predicate {
	TComparator comparator;
	int threshold;
	percent_health(e_mob_skill_target target_type_id,TComparator comparator,int threshold): condition_predicate(target_type_id),comparator{comparator},threshold{threshold}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		if (!checkTarget(targets))
			return false;
		block_list* target = targets.at(target_type_id);
		return this->comparator(get_percentage(status_get_hp(target),status_get_max_hp(target)),this->threshold);
	}
};

template <class TComparator>
struct health: public condition_predicate {
	TComparator comparator;
	int threshold;
	health(e_mob_skill_target target_type_id,TComparator comparator,int threshold): condition_predicate(target_type_id),comparator{comparator},threshold{threshold}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		return checkTarget(targets) && this->comparator(status_get_hp(targets.at(target_type_id)),this->threshold);
	}
};

template <class TComparator>
struct element_armor : public condition_predicate {
	TComparator comparator;
	short element;
	short level;
	element_armor(e_mob_skill_target target_type_id, TComparator comparator, short element, short level) : condition_predicate(target_type_id), comparator{ comparator }, element{ element }, level{ level }{}
	inline bool operator()(const std::map<e_mob_skill_target, block_list*>& targets) {
		if (!checkTarget(targets))
			return false;
		block_list* target = targets.at(target_type_id);
		return status_get_element(target) == element && this->comparator(status_get_element_level(target), level);
	}
};
struct element_attack: public condition_predicate {
	short element;
	element_attack(e_mob_skill_target target_type_id,short element): condition_predicate(target_type_id),element{element}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		return checkTarget(targets) && status_get_attack_element(targets.at(target_type_id)) == this->element;
	}
};
template <class TComparator>
struct element_resist: public condition_predicate {
	TComparator comparator;
	short element;
	int threshold;
	element_resist(e_mob_skill_target target_type_id,TComparator comparator,short element,int threshold): condition_predicate(target_type_id),comparator{comparator},element{element},threshold{threshold}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		if (!checkTarget(targets))
			return false;
		map_session_data* sd;
		if(sd = BL_CAST(BL_PC,targets.at(target_type_id)))
			return comparator(sd->indexed_bonus.subele_script[element],threshold);
		else
			return comparator(0,threshold); // 0 until mob elemental resistance feature is added
	}
};
struct cell: public condition_predicate {
	cell_chk mCell;
	cell(e_mob_skill_target target_type_id,cell_chk mCell): condition_predicate(target_type_id),mCell{mCell}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		if (!checkTarget(targets))
			return false;
		block_list* target = targets.at(target_type_id);
		return map_getcell(target->m,target->x,target->y,mCell);
	}
};
struct status: public condition_predicate {
	int mStatus;
	status(e_mob_skill_target target_type_id,int mStatus): condition_predicate(target_type_id),mStatus{mStatus}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		return checkTarget(targets) && status_get_sc(targets.at(target_type_id))->data[mStatus] != NULL;
	}
};
template <class TComparator>
struct status_resist : public condition_predicate {
	TComparator comparator;
	sc_type mStatus;
	int threshold;
	status_resist(e_mob_skill_target target_type_id, TComparator comparator, sc_type mStatus, int threshold) : condition_predicate(target_type_id), comparator{ comparator }, mStatus{ mStatus }, threshold{ threshold }{}
	inline bool operator()(const std::map<e_mob_skill_target, block_list*>& targets) {
		return checkTarget(targets) && comparator(status_get_sc_total_resist(targets.at(MST_SELF), targets.at(this->target_type_id), this->mStatus), 100 * this->threshold);
	}
};


template <class TComparator>
struct job: public condition_predicate {
	TComparator comparator;
	int mjob;
	job(e_mob_skill_target target_type_id,TComparator comparator,int pjob): condition_predicate(target_type_id),comparator{comparator},mjob{pjob}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		return checkTarget(targets) && status_get_class(targets.at(target_type_id)) == mjob;
	}
};

template <class TComparator>
struct attack_range: public condition_predicate {
	TComparator comparator;
	int range;
	attack_range(e_mob_skill_target target_type_id,TComparator comparator,int range): condition_predicate(target_type_id),comparator{comparator},range{range}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {	
		return checkTarget(targets) && this->comparator(status_get_range(targets.at(target_type_id)),this->range);
	}
};
template <class TComparator>
struct distance_from_self: public condition_predicate {
	TComparator comparator;
	int mDistance; // Conflict with distance_bl macro if named distance 
	distance_from_self(e_mob_skill_target target_type_id,TComparator comparator,int mDistance): condition_predicate(target_type_id),comparator{comparator},mDistance{mDistance}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		return checkTarget(targets) && comparator(distance_bl(targets.at(MST_SELF),targets.at(target_type_id)),this->mDistance);
	}
};
template <class TComparator>
struct distance_from_friend: public condition_predicate {
	TComparator comparator;
	int mDistance;
	distance_from_friend(e_mob_skill_target target_type_id,TComparator comparator,int mDistance): condition_predicate(target_type_id),comparator{comparator},mDistance{mDistance}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		return checkTarget(targets) && comparator(distance_bl(targets.at(MST_FRIEND),targets.at(target_type_id)),this->mDistance);
	}
};
template <class TComparator>
struct distance_from_master: public condition_predicate {
	TComparator comparator;
	int mDistance;
	distance_from_master(e_mob_skill_target target_type_id,TComparator comparator,int mDistance): condition_predicate(target_type_id),comparator{comparator},mDistance{mDistance}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		return checkTarget(targets) && comparator(distance_bl(targets.at(MST_MASTER),targets.at(target_type_id)),this->mDistance);
	}
};
template <class TComparator>
struct physical_reflect: public condition_predicate {
	TComparator comparator;
	int threshold;
	physical_reflect(e_mob_skill_target target_type_id,TComparator comparator,int threshold): condition_predicate(target_type_id),comparator{comparator},threshold{threshold}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		if (!checkTarget(targets))
			return false;
		int totalReflect=0;
		block_list* target = targets.at(target_type_id);
		status_change *sc = status_get_sc(target);
		if(sc->data[SC_REFLECTSHIELD])
			totalReflect+=sc->data[SC_REFLECTSHIELD]->val2;
		map_session_data* ssd = BL_CAST(BL_PC,target);
		if(ssd && ssd->bonus.short_weapon_damage_return != 0)
			totalReflect+=ssd->bonus.short_weapon_damage_return;
		return comparator(totalReflect,this->threshold);
	}
};

template <class TComparator>
struct attack : public condition_predicate {
	TComparator comparator;
	int mAttack;
	attack(e_mob_skill_target target_type_id, TComparator comparator, int mAttack) : condition_predicate(target_type_id), comparator{ comparator }, mAttack{ mAttack }{}
	inline bool operator()(const std::map<e_mob_skill_target, block_list*>& targets) {
		return checkTarget(targets) && comparator(status_get_watk(targets.at(this->target_type_id)), this->mAttack);
	}
};

struct mode : public condition_predicate {
	e_mode mmode;
	mode(e_mob_skill_target target_type_id, e_mode mmode) : condition_predicate(target_type_id), mmode{ mmode }{}
	inline bool operator()(const std::map<e_mob_skill_target, block_list*>& targets) {
		return checkTarget(targets) && status_get_status_data(targets.at(this->target_type_id))->mode & mmode;
	}
};

struct knockback : public condition_predicate {
	knockback(e_mob_skill_target target_type_id) : condition_predicate(target_type_id){}
	inline bool operator()(const std::map<e_mob_skill_target, block_list*>& targets) {
		return checkTarget(targets) && unit_blown_immune(targets.at(this->target_type_id), 0x1);
	}
};


} //namespace predicates

namespace logic_gates {

//XOR logic gate
template <class _InIt, class _Pr>
inline bool one_only(_InIt _First, _InIt _Last, _Pr _Pred) {
	// test if all elements satisfy _Pred
	std::_Adl_verify_range(_First, _Last);
	auto _UFirst = std::_Get_unwrapped(_First);
	const auto _ULast = std::_Get_unwrapped(_Last);
	bool isOneTrue = (false);
	for (; _UFirst != _ULast; ++_UFirst) {
		if (_Pred(*_UFirst)) {
			if (!isOneTrue)
				isOneTrue = (true);
			else
				return (false);
		}
	}
	return (isOneTrue);
}

//NAND logic gate
template<class _InIt, class _Pr>
_NODISCARD inline bool any_false_of(const _InIt _First, const _InIt _Last, _Pr _Pred)
{	// test if any element satisfies _Pred
	_Adl_verify_range(_First, _Last);
	auto _UFirst = _Get_unwrapped(_First);
	const auto _ULast = _Get_unwrapped(_Last);
	for (; _UFirst != _ULast; ++_UFirst)
	{
		if (!_Pred(*_UFirst))
			return (true);
	}
	return (false);
}

//NXOR logic gate
template <class _InIt, class _Pr>
inline bool all_or_none(_InIt _First, _InIt _Last, _Pr _Pred) {
	// test if all elements satisfy _Pred
	std::_Adl_verify_range(_First, _Last);
	auto _UFirst = std::_Get_unwrapped(_First);
	const auto _ULast = std::_Get_unwrapped(_Last);
	bool first = _Pred(*_UFirst);
	++_UFirst;
	for (; _UFirst != _ULast; ++_UFirst) {
		if (_Pred(*_UFirst) != (first))
			return (false);
	}
	return (true);
}



class LogicalAND : public LogicGate {
public:
	LogicalAND(){}
	inline bool operator()(std::vector<std::shared_ptr<ExpandedCondition>> nodes, const std::map<e_mob_skill_target, block_list*>& targets)const override  {
		return std::all_of(nodes.cbegin(), nodes.cend(), [&](const auto& it) {return (*it)(targets); });
	}
};

class LogicalOR : public LogicGate {
public:
	LogicalOR(){}
	inline bool operator()(std::vector<std::shared_ptr<ExpandedCondition>> nodes, const std::map<e_mob_skill_target, block_list*>& targets)const override  {
		return std::any_of(nodes.cbegin(), nodes.cend(), [&](const auto& it) {return (*it)(targets); });
	}
};
class LogicalNAND : public LogicGate {
public:
	LogicalNAND() {}
	inline bool operator()(std::vector<std::shared_ptr<ExpandedCondition>> nodes, const std::map<e_mob_skill_target, block_list*>& targets)const override {
		return any_false_of(nodes.cbegin(), nodes.cend(), [&](const auto& it) {return (*it)(targets); });
	}
};
class LogicalNOR : public LogicGate {
public:
	LogicalNOR() {}
	inline bool operator()(std::vector<std::shared_ptr<ExpandedCondition>> nodes, const std::map<e_mob_skill_target, block_list*>& targets)const override {
		return std::none_of(nodes.cbegin(), nodes.cend(), [&](const auto& it) {return (*it)(targets); });
	}
};
class LogicalXOR : public LogicGate {
public:
	LogicalXOR() {}
	inline bool operator()(std::vector<std::shared_ptr<ExpandedCondition>> nodes, const std::map<e_mob_skill_target, block_list*>& targets)const override {
		return one_only(nodes.cbegin(), nodes.cend(), [&](const auto& it) {return (*it)(targets); });
	}
};
class LogicalNXOR : public LogicGate {
public:
	LogicalNXOR() {}
	inline bool operator()(std::vector<std::shared_ptr<ExpandedCondition>> nodes, const std::map<e_mob_skill_target, block_list*>& targets)const override {
		return all_or_none(nodes.cbegin(), nodes.cend(), [&](const auto& it) {return (*it)(targets); });
	}
};
}//logic_gates
}//expanded_ai
#endif MOB_INLINE_HPP
