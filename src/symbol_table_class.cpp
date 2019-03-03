#include "symbol_table_class.hpp"
//#include "expression_classes.hpp"
#include "general_allocator_class.hpp"
#include "expression_builder_class.hpp"

namespace frost_hdl
{

// Unknown type symbol where all we have is its name, due to it being
// used earlier than it was defined (needs stuff filled in later!)
Symbol::Symbol(SavedString s_ident)
{
	_ident = s_ident;
	_hdl_type = nullptr;
	_is_constant = false;
	_right_dim_expr = nullptr;
}

// Non-constant scalar constructor
Symbol::Symbol(SavedString s_ident, HdlType* s_hdl_type)
{
	_ident = s_ident;
	_hdl_type = s_hdl_type;
	_is_constant = false;
	_right_dim_expr = nullptr;
}

// Constant scalar constructor, or non-constant array constructor
Symbol::Symbol(SavedString s_ident, HdlType* s_hdl_type,
	bool s_is_array, Expression* scalar_val_or_arr_size)
{
	if (!s_is_array)
	{
		_ident = s_ident;
		_hdl_type = s_hdl_type;
		_is_constant = true;
		_right_dim_expr = nullptr;
		_value_exprs.push_back(scalar_val_or_arr_size);
	}
	else // if (s_is_array)
	{
		_ident = s_ident;
		_is_constant = false;
		_hdl_type = s_hdl_type;
		_right_dim_expr = scalar_val_or_arr_size;
	}
}

// Constant array constructor
Symbol::Symbol(SavedString s_ident, HdlType* s_hdl_type,
	ValueExprs&& s_value_exprs)
{
	_ident = s_ident;
	_is_constant = true;
	_hdl_type = s_hdl_type;
	_value_exprs = std::move(s_value_exprs);
	_right_dim_expr = ExpressionBuilder::make_expr_hc_num
		(_value_exprs.size());
}

bool Symbol::has_default_value() const
{
	if (_value_exprs.size() == 0)
	{
		return false;
	}

	for (const auto& iter : _value_exprs)
	{
		if (iter == nullptr)
		{
			return false;
		}
	}

	return true;
}

} // namespace frost_hdl
