#ifndef OFFLINE_PREMI_AUTOBUFF_TRANSACTION_HPP
#define OFFLINE_PREMI_AUTOBUFF_TRANSACTION_HPP
#include <vector>

namespace offline_premi_autobuff {
enum class Debit { Success, Failed, RefundFailed };
// Reader returns 0 for an unrelated slot, -1 for an invalid currency slot,
// or its available amount, and preserves the original item in the entry.
template<class Entry, class Read>
bool plan_payment(int cost, int slots, Read read, std::vector<Entry>& entries) {
	entries.clear();
	if (cost <= 0) return false;
	for (int i = 0; i < slots && cost > 0; ++i) {
		Entry entry{};
		const int available = read(i, entry);
		if (available < 0) { entries.clear(); return false; }
		if (available == 0) continue;
		entry.index = i;
		entry.amount = cost < available ? cost : available;
		cost -= entry.amount;
		entries.push_back(entry);
	}
	if (cost != 0) { entries.clear(); return false; }
	return true;
}
// Entries already contain validated index, amount and original item snapshot.
// Native item operations are injected so partial failures can be tested safely.
template<class Entry, class Remove, class Restore, class Report>
Debit debit_entries(const std::vector<Entry>& entries, Remove remove, Restore restore, Report report) {
	std::size_t paid = 0;
	for (; paid < entries.size(); ++paid) {
		if (!remove(entries[paid])) {
			bool refunded = true;
			for (std::size_t i = 0; i < paid; ++i) {
				if (!restore(entries[i])) { report(entries[i]); refunded = false; }
			}
			return refunded ? Debit::Failed : Debit::RefundFailed;
		}
	}
	return Debit::Success;
}
} // namespace offline_premi_autobuff
#endif
