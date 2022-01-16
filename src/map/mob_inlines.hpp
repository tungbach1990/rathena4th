// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef MOB_INLINES_HPP
#define MOB_INLINES_HPP
#include "mob.hpp"

namespace expanded_ai{
namespace predicates {
struct default_predicate {
	e_mob_skill_target target_type_id;
	default_predicate(e_mob_skill_target target_type_id) : target_type_id{ target_type_id } {}
	virtual inline bool operator()(const std::map<e_mob_skill_target, block_list*>& targets) = 0;
};
template <class TComparator>
struct percent_health: public default_predicate {
	TComparator comparator;
	int threshold;
	percent_health(e_mob_skill_target target_type_id,TComparator comparator,int threshold): default_predicate(target_type_id),comparator{comparator},threshold{threshold}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		block_list* target = targets.at(target_type_id);
		return this->comparator(get_percentage(status_get_hp(target),status_get_max_hp(target)),this->threshold);
	}
};


template <class TComparator>
struct health: public default_predicate {
	TComparator comparator;
	int threshold;
	health(e_mob_skill_target target_type_id,TComparator comparator,int threshold): default_predicate(target_type_id),comparator{comparator},threshold{threshold}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		return this->comparator(status_get_hp(targets.at(target_type_id)),this->threshold);
	}
};

template <class TComparator>
struct element_armor: public default_predicate {
	TComparator comparator;
	short element;
	short level;
	element_armor(e_mob_skill_target target_type_id,TComparator comparator,short element,short level): default_predicate(target_type_id),comparator{comparator},element{element},level{level}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		block_list* target = targets.at(target_type_id);
		return status_get_element(target) == element
			&& this->comparator(status_get_element_level(target),level);
	}
};
struct element_attack: public default_predicate {
	short element;
	element_attack(e_mob_skill_target target_type_id,short element): default_predicate(target_type_id),element{element}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		return status_get_attack_element(targets.at(target_type_id)) == this->element;
	}
};
template <class TComparator>
struct element_resist: public default_predicate {
	TComparator comparator;
	short element;
	int threshold;
	element_resist(e_mob_skill_target target_type_id,TComparator comparator,short element,int threshold): default_predicate(target_type_id),comparator{comparator},element{element},threshold{threshold}{
	}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		map_session_data* sd;
		if(sd = BL_CAST(BL_PC,targets.at(target_type_id)))
			return comparator(sd->indexed_bonus.subele_script[element],threshold);
		else
			return comparator(0,threshold); // 0 until mob elemental resistance feature is added
	}
};
struct cell: public default_predicate {
	cell_chk mCell;
	cell(e_mob_skill_target target_type_id,cell_chk mCell): default_predicate(target_type_id),mCell{mCell}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		block_list* target = targets.at(target_type_id);
		return map_getcell(target->m,target->x,target->y,mCell);
	}
};
struct status: public default_predicate {
	int mStatus;
	status(e_mob_skill_target target_type_id,int mStatus): default_predicate(target_type_id),mStatus{mStatus}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		return status_get_sc(targets.at(target_type_id))->data[mStatus] != NULL;
	}
};
template <class TComparator>
struct job: public default_predicate {
	TComparator comparator;
	int job;
	job(e_mob_skill_target target_type_id,TComparator comparator,int job): default_predicate(target_type_id),comparator{comparator},job{job}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		return status_get_class(targets.at(target_type_id)) == job;
	}
};
template <class TComparator>
struct attack_range: public default_predicate {
	TComparator comparator;
	int range;
	attack_range(e_mob_skill_target target_type_id,TComparator comparator,int range): default_predicate(target_type_id),comparator{comparator},range{range}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		return this->comparator(status_get_range(targets.at(target_type_id)),this->range);
	}
};
template <class TComparator>
struct target_distance: public default_predicate {
	TComparator comparator;
	int mDistance; // Conflict with distance_bl macro if named distance 
	target_distance(e_mob_skill_target target_type_id,TComparator comparator,int mDistance): default_predicate(target_type_id),comparator{comparator},mDistance{mDistance}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		return comparator(distance_bl(targets.at(MST_SELF),targets.at(target_type_id)),this->mDistance);
	}
};
template <class TComparator>
struct target_distance_from_ally: public default_predicate {
	TComparator comparator;
	int mDistance;
	target_distance_from_ally(e_mob_skill_target target_type_id,TComparator comparator,int mDistance): default_predicate(target_type_id),comparator{comparator},mDistance{mDistance}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		return comparator(distance_bl(targets.at(MST_FRIEND),targets.at(target_type_id)),this->mDistance);
	}
};
template <class TComparator>
struct target_distance_from_master: public default_predicate {
	TComparator comparator;
	int mDistance;
	target_distance_from_master(e_mob_skill_target target_type_id,TComparator comparator,int mDistance): default_predicate(target_type_id),comparator{comparator},mDistance{mDistance}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
		return comparator(distance_bl(targets.at(MST_MASTER),targets.at(target_type_id)),this->mDistance);
	}
};
template <class TComparator>
struct physical_reflect: public default_predicate {
	TComparator comparator;
	int threshold;
	physical_reflect(e_mob_skill_target target_type_id,TComparator comparator,int threshold): default_predicate(target_type_id),comparator{comparator},threshold{threshold}{}
	inline bool operator()(const std::map<e_mob_skill_target,block_list*>& targets) {
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
} //namespace predicates


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

}
#endif MOB_INLINE_HPP
