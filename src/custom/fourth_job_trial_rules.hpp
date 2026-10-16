#ifndef OFFLINE_FOURTH_JOB_TRIAL_RULES_HPP
#define OFFLINE_FOURTH_JOB_TRIAL_RULES_HPP

namespace offline_fourth_job_trial {

constexpr int first_quest = 65000;
constexpr int last_quest = 65018;

constexpr bool owns(int quest_id) {
	return quest_id >= first_quest && quest_id <= last_quest;
}

// One death can advance only one objective. The assigned boss has priority.
// Bakonawa's final instance form has MVP drops but no MD_MVP in the base DB.
// Recognize that exact form only for its assigned objective, never generic credit.
// Scripted cleanup and other non-MVP monsters must never grant trial credit.
constexpr int objective_for_kill(int mob_id, int assigned_boss, bool is_mvp,
	int boss_count, int mvp_count, bool scripted_removal) {
	if (scripted_removal)
		return -1;
	if (mob_id == 2322)
		return (assigned_boss == 2322 && boss_count < 1) ? 0 : -1;
	if (!is_mvp)
		return -1;
	if (mob_id == assigned_boss && boss_count < 1)
		return 0;
	if (mvp_count < 10)
		return 1;
	return -1;
}

} // namespace offline_fourth_job_trial
#endif
