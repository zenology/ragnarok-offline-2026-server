#ifndef OFFLINE_FOURTH_JOB_TRIAL_HPP
#define OFFLINE_FOURTH_JOB_TRIAL_HPP

#include <map/clif.hpp>
#include <map/mob.hpp>
#include <map/pc.hpp>
#include <map/quest.hpp>
#include <map/status.hpp>
#include "fourth_job_trial_rules.hpp"

namespace offline_fourth_job_trial {

inline bool valid_objectives(const s_quest_db& definition) {
	return owns(definition.id) && definition.objectives.size() == 2
		&& definition.objectives[0]->mob_id != 0 && definition.objectives[0]->count == 1
		&& definition.objectives[1]->mob_id == 0 && definition.objectives[1]->index == 1
		&& definition.objectives[1]->count == 10 && definition.dropitem.empty();
}

inline bool ready(int quest_id) {
	const auto definition = quest_search(quest_id);
	return definition != nullptr && valid_objectives(*definition);
}

// Called inside native quest credit, so instance death labels and the existing
// party/pet/homunculus attribution keep working. All other quests stay native.
inline bool update_objective(map_session_data& sd, const mob_data& mob,
	quest& progress, const s_quest_db& definition) {
	if (!owns(progress.quest_id))
		return false;
	if (progress.state == Q_COMPLETE || !valid_objectives(definition))
		return true;
	const int objective = objective_for_kill(mob.mob_id, definition.objectives[0]->mob_id,
		status_has_mode(&mob.status, MD_MVP), progress.count[0], progress.count[1],
		mob.state.npc_killmonster);
	if (objective >= 0) {
		++progress.count[objective];
		sd.save_quest = true;
		clif_quest_update_objective(&sd, &progress);
	}
	return true;
}

} // namespace offline_fourth_job_trial
#endif
