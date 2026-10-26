#include <cassert>
#include <cstdio>

#include <custom/costume_equipment_limits.hpp>

int main() {
	const t_itemid limited_items[] = {20513, 31917};
	const int32 blocked_jobs[] = {
		JOB_SHINKIRO, JOB_NIGHT_WATCH, JOB_HYPER_NOVICE, JOB_SPIRIT_HANDLER
	};
	const int32 allowed_relatives[] = {
		JOB_SHIRANUI, JOB_KAGEROU, JOB_OBORO, JOB_REBELLION,
		JOB_SUPER_NOVICE, JOB_SUMMONER, JOB_ARCH_BISHOP
	};

	for (t_itemid item_id : limited_items) {
		int blocked_count = 0;
		for (int32 job_id = 0; job_id < JOB_MAX; ++job_id) {
			bool expected = false;
			for (int32 blocked_job : blocked_jobs)
				expected |= job_id == blocked_job;
			assert(offline_costume_limits::is_blocked(item_id, job_id) == expected);
			blocked_count += expected;
		}
		assert(blocked_count == 4);
		for (int32 job_id : allowed_relatives)
			assert(!offline_costume_limits::is_blocked(item_id, job_id));
	}

	for (t_itemid item_id : {t_itemid(0), t_itemid(20512), t_itemid(31918)})
		for (int32 job_id : blocked_jobs)
			assert(!offline_costume_limits::is_blocked(item_id, job_id));

	std::puts("PASS: two limited costumes, four blocked jobs, all other jobs and items allowed");
}
