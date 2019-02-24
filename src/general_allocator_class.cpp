#include "general_allocator_class.hpp"
#include "expr_num_class.hpp"
#include "expression_classes.hpp"

namespace frost_hdl
{

std::map<RawSavedString, std::unique_ptr<const RawSavedString>>
	GeneralAllocator::_str_pool;
std::map<RawExprNumData, std::unique_ptr<const RawExprNumData>>
	GeneralAllocator::_expr_num_data_pool;
std::vector<std::unique_ptr<Expression>> GeneralAllocator::_expr_pool;


SavedString GeneralAllocator::dup_str(const RawSavedString& to_dup)
{
	auto& pool = _str_pool;

	if (pool.count(to_dup) == 0)
	{
		std::unique_ptr<const RawSavedString> to_insert;
		to_insert.reset(new RawSavedString(to_dup));
		pool[to_dup] = std::move(to_insert);
	}

	return pool.at(to_dup).get();
}

ExprNumData GeneralAllocator::dup_expr_num_data
	(const RawExprNumData& to_dup)
{
	auto& pool = _expr_num_data_pool;

	if (pool.count(to_dup) == 0)
	{
		std::unique_ptr<const RawExprNumData> to_insert;
		to_insert.reset(new RawExprNumData(to_dup));
		pool[to_dup] = std::move(to_insert);
	}

	return pool.at(to_dup).get();
}


} // namespace frost_hdl
