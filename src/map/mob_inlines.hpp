// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MOB_INLINES_HPP
#define MOB_INLINES_HPP
#include "mob.hpp"
#include "unit.hpp"
#include "pc.hpp"
#include "../common/utils.hpp"

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
	inline bool checkTarget(const std::map<e_mob_skill_target, block_list*>& targets) {
		return targets.count(target_type_id);
	}
};

struct exists : public condition_predicate {
	exists(e_mob_skill_target target_type_id) : condition_predicate(target_type_id){}
	inline bool operator()(const std::map<e_mob_skill_target, block_list*>& targets) {
		return checkTarget(targets);
	}
};

struct isnew : public condition_predicate {
	isnew(e_mob_skill_target target_type_id) : condition_predicate(target_type_id){}
	inline bool operator()(const std::map<e_mob_skill_target, block_list*>& targets) {
		return checkTarget(targets) && targets.at(target_type_id)->id!=((mob_data*)targets.at(MST_SELF))->target_id;
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
		return checkTarget(targets) && targets.count(MST_FRIEND) && comparator(distance_bl(targets.at(MST_FRIEND),targets.at(target_type_id)),this->mDistance);
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
		block_list* target = targets.at(this->target_type_id);
		return checkTarget(targets) && comparator(status_get_batk(target)+status_get_watk(target), this->mAttack);
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


template <class TComparator>
struct spiritball : public condition_predicate {
	TComparator comparator;
	int balls;
	spiritball(e_mob_skill_target target_type_id, TComparator comparator, int balls) : condition_predicate(target_type_id), comparator{ comparator }, balls{ balls }{}
	inline bool operator()(const std::map<e_mob_skill_target, block_list*>& targets) {
		return checkTarget(targets) && comparator(status_get_spiritball(targets.at(this->target_type_id)), this->balls);
	}
};

struct type : public condition_predicate {
	bl_type mtype;
	type(e_mob_skill_target target_type_id, bl_type type) : condition_predicate(target_type_id), mtype{ type }{}
	inline bool operator()(const std::map<e_mob_skill_target, block_list*>& targets) {
		return checkTarget(targets) && (targets.at(this->target_type_id)->type==mtype);
	}
};


struct isaligned : public condition_predicate {

	isaligned(e_mob_skill_target target_type_id) : condition_predicate(target_type_id){}
	inline bool operator()(const std::map<e_mob_skill_target, block_list*>& targets) {
		block_list* target = targets.at(this->target_type_id);
		return checkTarget(targets) && ((target->x%2 == targets.at(MST_SELF)->x%2)||(target->y%2 == targets.at(MST_SELF)->y%2));
	}
};



} //namespace predicates

namespace logic_gates {

//XOR logic gate
template<class InputIterator, class UnaryPredicate>
inline bool one_only(InputIterator _First,InputIterator _Last, UnaryPredicate _Pred){
	// test if all elements satisfy _Pred
	bool isOneTrue = (false);
	for (; _First != _Last; ++_First) {
		if (_Pred(*_First)) {
			if (!isOneTrue)
				isOneTrue = (true);
			else
				return (false);
		}
	}
	return (isOneTrue);
}

//NAND logic gate
template<class InputIterator, class UnaryPredicate>
inline bool any_false_of(InputIterator _First, InputIterator _Last, UnaryPredicate _Pred){
	// test if any element satisfies _Pred
	for (; _First != _Last; ++_First){
		if (!_Pred(*_First))
			return (true);
	}
	return (false);
}

//NXOR logic gate
template<class InputIterator, class UnaryPredicate>
inline bool all_or_none(InputIterator _First, InputIterator _Last, UnaryPredicate _Pred) {
	bool first = _Pred(*_First);
	++_First;
	for (; _First != _Last; ++_First) {
		if (_Pred(*_First) != (first))
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
#endif
