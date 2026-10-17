#include <cassert>
#include <iostream>
#include "../../src/custom/fourth_job_trial_rules.hpp"

using namespace offline_fourth_job_trial;

int main() {
	assert(!owns(64999) && !owns(65019));
	for (int id = 65000; id <= 65018; ++id)
		assert(owns(id));
	for (int boss : {20520,2319,3074,20601,20610,1734,1885,2476,1917,20381,
		2996,20611,1768,1874,1871,3741,3254,2202,2322}) {
		int count[2] = {0, 0};
		auto kill = [&](int mob, bool mvp = true, bool cleanup = false) {
			const int index = objective_for_kill(mob, boss, mvp, count[0], count[1], cleanup);
			if (index >= 0) ++count[index];
		};
		kill(1002, false);
		kill(boss, true, true);
		assert(count[0] == 0 && count[1] == 0);
		kill(boss);
		assert(count[0] == 1 && count[1] == 0); // Assigned boss: no double credit.
		for (int i = 0; i < 10; ++i) kill(boss == 2322 ? 1039 : boss);
		assert(count[0] == 1 && count[1] == 10); // Other MVPs fill Bakonawa's generic objective.
		kill(boss);
		kill(1039);
		assert(count[0] == 1 && count[1] == 10); // Both caps respected.
		count[0] = count[1] = 0;
		for (int i = 0; i < 10; ++i) kill(1039);
		assert(count[0] == 0 && count[1] == 10); // Other MVPs can come first.
		kill(boss);
		assert(count[0] == 1 && count[1] == 10); // Boss still counted at generic cap.
		assert(objective_for_kill(boss, boss, false, 0, 0, false) == (boss == 2322 ? 0 : -1));
	}
	// Bakonawa's real DB mode is not MVP. Only the final form grants credit.
	assert(objective_for_kill(2320, 2322, false, 0, 0, false) == -1);
	assert(objective_for_kill(2321, 2322, false, 0, 0, false) == -1);
	assert(objective_for_kill(2322, 2322, false, 0, 0, false) == 0);
	assert(objective_for_kill(2322, 2322, false, 1, 0, false) == -1);
	assert(objective_for_kill(2322, 2322, false, 1, 10, false) == -1);
	assert(objective_for_kill(2322, 2322, false, 0, 10, false) == 0);
	assert(objective_for_kill(2322, 2322, false, 0, 0, true) == -1);
	assert(objective_for_kill(2322, 20520, false, 0, 0, false) == -1);
	// Even a future MVP flag must not turn Bakonawa into generic trial credit.
	assert(objective_for_kill(2322, 2322, true, 0, 0, false) == 0);
	assert(objective_for_kill(2322, 2322, true, 1, 0, false) == -1);
	assert(objective_for_kill(2322, 20520, true, 0, 0, false) == -1);
	std::cout << "Fourth-job trial kill rules: 19 boss routes passed\n";
}
