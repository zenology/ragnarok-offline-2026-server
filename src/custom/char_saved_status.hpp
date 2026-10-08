#ifndef OFFLINE_CHAR_SAVED_STATUS_HPP
#define OFFLINE_CHAR_SAVED_STATUS_HPP
#include <common/mmo.hpp>
#include <common/showmsg.hpp>
#include <common/sql.hpp>
#include "char_saved_status_rows.hpp"

namespace offline_saved_status {
inline bool read_field(Sql* sql, unsigned int column, std::int64_t& value) {
	char* text = nullptr;
	return Sql_GetData(sql, column, &text, nullptr) == SQL_SUCCESS && parse_value(text, value);
}
inline int32 row_limit(uint64 total, int32 aid, int32 cid) {
	if (total > capacity<status_change_data>())
		ShowError("Saved status response failed for %d:%d: capacity exceeded; sending count=0.\n", aid, cid);
	return bounded_count<status_change_data>(total);
}
inline bool validate_current_row(Sql* sql) {
	return validate_values([sql](unsigned int column, std::int64_t& value) { return read_field(sql, column, value); });
}
inline int32 response_count(int32 expected, int32 actual, int32 aid, int32 cid) {
	if (expected < 0 || expected != actual)
		ShowError("Saved status response failed for %d:%d: incomplete/invalid rows; sending count=0.\n", aid, cid);
	return complete_count(expected, actual);
}
} // namespace offline_saved_status
#endif
