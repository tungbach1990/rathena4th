// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom battle config initializations
 * Place the definition of custom battle configuration initializations in this file.
 *
 * Do not forget to add a comma at the end of the line, if you want to add another config
 *
 * Format:
 *	 { "name", &battle_config.<variable name>, <default value>, <minimum value>, <maximum value> },
 **/

{ "skill_unit_timer_interval",         &battle_config.skill_unit_timer_interval,			 100,	     0,      INT_MAX,        },
{ "active_ai_range",				   &battle_config.active_ai_range,				    	 2,			 0,      INT_MAX,        },
{ "idle_skill_interval",			   &battle_config.idle_skill_interval,					 10,		 0,      INT_MAX,        },
{ "min_mob_think_time",				   &battle_config.min_mob_think_time,					 100,		 0,      INT_MAX,        },
{ "min_random_walk_time",			   &battle_config.min_random_walk_time,				     4000,		 0,      INT_MAX,        },
{ "walk_skill_interval",			   &battle_config.walk_skill_interval,					 5,			 0,      INT_MAX,        },
{ "monster_chase_refresh_pcbehavior",  &battle_config.mob_chase_refresh_pcbehavior,		     30,         0,      MAX_MINCHASE,   },
{ "view_range_rate_pcbehavior",        &battle_config.view_range_rate_pcbehavior,						 100,		 0,      INT_MAX,        },
{ "chase_range_rate_pcbehavior",       &battle_config.chase_range_rate_pcbehavior,				         100,		 0,      INT_MAX,        },
{ "monster_eye_range_bonus_pcbehavior",&battle_config.mob_eye_range_bonus_pcbehavior,				     0,		     0,      10,             },
{ "idle_skill_interval_pcbehavior",			   &battle_config.idle_skill_interval_pcbehavior,					 10,		 0,      INT_MAX,        },
{ "min_mob_think_time_pcbehavior",	   &battle_config.min_mob_think_time_pcbehavior,					 100,		 0,      INT_MAX,        },
{ "min_random_walk_time_pcbehavior",			   &battle_config.min_random_walk_time_pcbehavior,				     4000,		 0,      INT_MAX,        },
{ "walk_skill_interval_pcbehavior",			   &battle_config.walk_skill_interval_pcbehavior,					 5,			 0,      INT_MAX,        },
