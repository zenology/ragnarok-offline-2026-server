// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef CONFIG_CUSTOM_DEFINES_PRE_HPP
#define CONFIG_CUSTOM_DEFINES_PRE_HPP

/**
 * rAthena configuration file (http://rathena.org)
 * For detailed guidance on these check http://rathena.org/wiki/SRC/config/
 **/

#define PACKETVER 20250716

/// Timed Premium Buff / iRO Premium Subscription. VIP_SCRIPT stays 0 in core.hpp.
#define VIP_ENABLE
/// Offline already uses MAX_STORAGE (600). Do not drop inactive accounts to 300.
#define MIN_STORAGE 600

#endif /* CONFIG_CUSTOM_DEFINES_PRE_HPP */
