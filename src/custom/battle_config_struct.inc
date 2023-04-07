// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom battle config structure
 * Place the definition of custom battle configuration structure members in this file.
 *
 * Make sure that the name you want is not already taken by looking into battle.hpp
 * Do not forget to add initilization logic to battle_config_init.inc
 *
 * Format:
 *	 <datatype> name;
 **/

int skill_unit_timer_interval;
int active_ai_range;
int idle_skill_interval;
int min_mob_think_time;
int min_random_walk_time;
int walk_skill_interval;

int mob_chase_refresh_pcbehavior;
int view_range_rate_pcbehavior;
int chase_range_rate_pcbehavior;
int mob_eye_range_bonus_pcbehavior;
int idle_skill_interval_pcbehavior;
int min_mob_think_time_pcbehavior;
int min_random_walk_time_pcbehavior;
int walk_skill_interval_pcbehavior;
