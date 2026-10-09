#ifndef OFFLINE_PREMI_AUTOBUFF_PERSISTENCE_HPP
#define OFFLINE_PREMI_AUTOBUFF_PERSISTENCE_HPP

#include <common/showmsg.hpp>
#include <common/sql.hpp>

namespace offline_premi_autobuff {
// Logout/map-server transfer saves the active service before clearing memory.
// Only a real end on an active character invalidates that saved snapshot.
inline bool clear_saved_status(Sql* sql, uint32 account_id, uint32 char_id, int status, bool active) {
	if (!active) return true;
	// This workspace's conf/inter_athena.conf sets scdata_db to sc_data.
	if (sql == nullptr || Sql_Query(sql,
		"DELETE FROM `sc_data` WHERE `account_id` = '%u' AND `char_id` = '%u' AND `type` = '%d'",
		account_id, char_id, status) != SQL_SUCCESS) {
		ShowError("[Premi Auto Buff] saved status cleanup failed account=%u char=%u status=%d\n",
			account_id, char_id, status);
		return false;
	}
	return true;
}
} // namespace offline_premi_autobuff
#endif
