#ifndef src_expression_classes_hpp
#define src_expression_classes_hpp

// src/expression_classes.hpp

#include "misc_includes.hpp"
#include "expr_num_class.hpp"
#include "general_allocator_class.hpp"

namespace frost_hdl
{

//inline BigNum max(const BigNum& a, const BigNum& b)
//{
//	return (a > b) ? a : b;
//}
//
//typedef BigNum* ExprNum;

class Expression
{
public:		// types
	//typedef SavedString OpStr;
	typedef SavedString Ident;

	//enum class Category
	//{
	//	UnOp,
	//	BinOp,
	//	Ternary,

	//	IdentSliced,
	//	HardCodedConstant,
	//};

	//enum class Type
	//{
	//	BinOpLogAnd,
	//	BinOpLogOr,

	//	BinOpCmpEq,
	//	BinOpCmpNe,
	//	BinOpCmpLt,
	//	BinOpCmpGt,
	//	BinOpCmpLe,
	//	BinOpCmpGe,

	//	BinOpPlus,
	//	BinOpMinus,

	//	UnOpPlus,
	//	UnOpMinus,
	//	UnOpBitInvert,

	//	BinOpMul,
	//	BinOpDiv,
	//	BinOpMod,

	//	BinOpBitAnd,
	//	BinOpBitOr,
	//	BinOpBitXor,

	//	BinOpBitLsl,
	//	BinOpBitLsr,
	//	BinOpBitAsr,

	//	Ternary,

	//	Ident,
	//	IdentSliced,
	//	HardCodedConstant,

	//	Lim,
	//};


protected:		// variables
	std::vector<Expression*> _children;

	Ident _ident;
	ExprNum _value;
	bool _is_self_determined;

public:		// functions
	Expression();


	// Don't want copies of raw Expression's
	Expression(const Expression& to_copy) = delete;

	// Moves are okay, though
	Expression(Expression&& to_move) = default;


	virtual ~Expression() = default;


	// Don't want copies of raw Expression's
	Expression& operator = (const Expression& to_copy) = delete;

	// Moves are okay, though
	Expression& operator = (Expression&& to_move) = default;




	// Don't call "evaluate()" until after the size of the expression has
	// been determined and the children have been modified.
	virtual void evaluate();


	inline size_t num_children() const
	{
		return children().size();
	}
	inline bool is_leaf() const
	{
		return (num_children() == 0);
	}


	inline const auto& ident() const
	{
		return (*_ident);
	}


	inline void set_ident(const std::string& n_ident)
	{
		_ident = dup_str(n_ident);
	}

	inline void set_value(const ExprNum& n_value)
	{
		_value = n_value;
	}
	//inline void set_value(const BigNum& n_value_data,
	//	size_t n_value_data_size, bool n_value_is_signed)
	//{
	//	set_value(ExprNum(n_value_data, n_value_data_size,
	//		n_value_is_signed));
	//}

	inline bool is_constant() const
	{
		return (_has_only_constant_children() || _is_always_constant());
	}

	GEN_GETTER_BY_CON_REF(children)
	GEN_GETTER_BY_CON_REF(value)

	GEN_GETTER_AND_SETTER_BY_VAL(is_self_determined)


protected:		// functions
	virtual bool _children_affect_length() const;

	// The length of the expression before being possibly casted to a
	// larger type
	virtual size_t _starting_length() const;

	virtual bool _is_always_constant() const;

	bool _has_any_unsigned_non_self_determined_children() const;

	bool _has_only_constant_children() const;

	//template<typename FirstArgType, typename... RemArgTypes>
	//inline bool _has_only_constant_children(FirstArgType&& first_child,
	//	RemArgTypes&&... rem_children)
	//{
	//	_set_children(first_child, rem_children...);
	//	return _has_only_constant_children();
	//}

	// Require at LEAST one argument.
	template<typename FirstArgType, typename... RemArgTypes>
	inline void _set_children(FirstArgType&& first_child,
		RemArgTypes&&... rem_children)
	{
		_children.push_back(first_child);

		// Oh hey, an actual use for "if constexpr" that actually CAN'T be
		// written as a plain old "if"!
		if constexpr (sizeof...(rem_children) > 0)
		{
			_set_children(rem_children...);
		}
		else
		{
			_value.set_size(_starting_length());
		}
	}
};

class ExprBaseUnOp : public Expression
{
public:		// functions
	ExprBaseUnOp(Expression* only_child);


protected:		// functions
	Expression* _only_child() const
	{
		return children().at(0);
	}
};

class ExprBaseBinOp : public Expression
{
public:		// functions
	ExprBaseBinOp(Expression* left_child, Expression* right_child);


protected:		// functions
	inline Expression* _left_child() const
	{
		return children().at(0);
	}

	inline Expression* _right_child() const
	{
		return children().at(1);
	}

	inline const ExprNum& _left_child_value() const
	{
		return _left_child()->value();
	}
	inline const ExprNum& _right_child_value() const
	{
		return _right_child()->value();
	}
};

// "&&", "||", "==", "!=", "<", ">", "<=", ">="
class ExprBaseLogCmpBinOp : public ExprBaseBinOp
{
public:		// functions
	ExprBaseLogCmpBinOp(Expression* left_child, Expression* right_child)
		: ExprBaseBinOp(left_child, right_child)
	{
	}

protected:		// functions
	inline void _get_resized_child_expr_nums(ExprNum& left_ret,
		ExprNum& right_ret) const
	{
		left_ret = _left_child_value();
		right_ret = _right_child_value();

		if (left_ret.size() < right_ret.size())
		{
			left_ret.set_size(right_ret.size());
		}
		else // if (left_ret.size() >= right_ret.size())
		{
			right_ret.set_size(left_ret.size());
		}
	}

	size_t _starting_length() const final
	{
		return 1;
	}

	bool _children_affect_length() const final
	{
		return false;
	}

};

// "+", "-", "*", "/", "%"
class ExprBaseArithBinOp : public ExprBaseBinOp
{
public:		// functions
	ExprBaseArithBinOp(Expression* left_child, Expression* right_child)
		: ExprBaseBinOp(left_child, right_child)
	{
	}

protected:		// functions
	size_t _starting_length() const final
	{
		// max(sizeof(left_child), sizeof(right_child))
		return max_va(_left_child()->value().size(),
			_right_child()->value().size());
	}

};

// "&", "|", "^"
class ExprBaseBitNonShiftBinOp : public ExprBaseBinOp
{
public:		// functions
	ExprBaseBitNonShiftBinOp(Expression* left_child,
		Expression* right_child)
		: ExprBaseBinOp(left_child, right_child)
	{
	}

protected:		// functions
	size_t _starting_length() const final
	{
		// max(sizeof(left_child), sizeof(right_child))
		return max_va(_left_child()->value().size(),
			_right_child()->value().size());
	}
};

// "<<", ">>", ">>>"
class ExprBaseBitShiftBinOp : public ExprBaseBinOp
{
public:		// functions
	ExprBaseBitShiftBinOp(Expression* left_child, Expression* right_child)
		: ExprBaseBinOp(left_child, right_child)
	{
		_right_child()->set_is_self_determined(true);
	}

protected:		// functions
	size_t _starting_length() const final
	{
		return _left_child()->value().size();
	}
};


// Finally, here are the "most derived" "Expression" classes, the ones at
// the bottom of the class hierarchy.


// "Expression" classes derived from "ExprBaseLogCmpBinOp"

// "&&"
class ExprBinOpLogAnd : public ExprBaseLogCmpBinOp
{
public:		// functions
	ExprBinOpLogAnd(Expression* left_child, Expression* right_child)
		: ExprBaseLogCmpBinOp(left_child, right_child)
	{
	}

	void evaluate() final
	{
		ExprNum left_expr_num, right_expr_num;
		_get_resized_child_expr_nums(left_expr_num, right_expr_num);

		_value.copy_from_bignum(static_cast<BigNum>(left_expr_num)
			&& static_cast<BigNum>(right_expr_num));
	}
};

// "||"
class ExprBinOpLogOr : public ExprBaseLogCmpBinOp
{
public:		// functions
	ExprBinOpLogOr(Expression* left_child, Expression* right_child)
		: ExprBaseLogCmpBinOp(left_child, right_child)
	{
	}

	void evaluate() final
	{
		ExprNum left_expr_num, right_expr_num;
		_get_resized_child_expr_nums(left_expr_num, right_expr_num);

		_value.copy_from_bignum(static_cast<BigNum>(left_expr_num)
			|| static_cast<BigNum>(right_expr_num));
	}
};

// "=="
class ExprBinOpCmpEq : public ExprBaseLogCmpBinOp
{
public:		// functions
	ExprBinOpCmpEq(Expression* left_child, Expression* right_child)
		: ExprBaseLogCmpBinOp(left_child, right_child)
	{
	}

	void evaluate() final
	{
		ExprNum left_expr_num, right_expr_num;
		_get_resized_child_expr_nums(left_expr_num, right_expr_num);

		_value.copy_from_bignum(static_cast<BigNum>(left_expr_num)
			== static_cast<BigNum>(right_expr_num));

		// Comparison results are unsigned
		_value.set_is_signed(false);
	}
};

// "!="
class ExprBinOpCmpNe : public ExprBaseLogCmpBinOp
{
public:		// functions
	ExprBinOpCmpNe(Expression* left_child, Expression* right_child)
		: ExprBaseLogCmpBinOp(left_child, right_child)
	{
	}

	void evaluate() final
	{
		ExprNum left_expr_num, right_expr_num;
		_get_resized_child_expr_nums(left_expr_num, right_expr_num);

		_value.copy_from_bignum(static_cast<BigNum>(left_expr_num)
			!= static_cast<BigNum>(right_expr_num));

		// Comparison results are unsigned
		_value.set_is_signed(false);
	}
};

// "<"
class ExprBinOpCmpLt : public ExprBaseLogCmpBinOp
{
public:		// functions
	ExprBinOpCmpLt(Expression* left_child, Expression* right_child)
		: ExprBaseLogCmpBinOp(left_child, right_child)
	{
	}

	void evaluate() final
	{
		ExprNum left_expr_num, right_expr_num;
		_get_resized_child_expr_nums(left_expr_num, right_expr_num);

		_value.copy_from_bignum(static_cast<BigNum>(left_expr_num)
			< static_cast<BigNum>(right_expr_num));

		// Comparison results are unsigned
		_value.set_is_signed(false);
	}
};

// ">"
class ExprBinOpCmpGt : public ExprBaseLogCmpBinOp
{
public:		// functions
	ExprBinOpCmpGt(Expression* left_child, Expression* right_child)
		: ExprBaseLogCmpBinOp(left_child, right_child)
	{
	}

	void evaluate() final
	{
		ExprNum left_expr_num, right_expr_num;
		_get_resized_child_expr_nums(left_expr_num, right_expr_num);

		_value.copy_from_bignum(static_cast<BigNum>(left_expr_num)
			> static_cast<BigNum>(right_expr_num));

		// Comparison results are unsigned
		_value.set_is_signed(false);
	}
};

// "<="
class ExprBinOpCmpLe : public ExprBaseLogCmpBinOp
{
public:		// functions
	ExprBinOpCmpLe(Expression* left_child, Expression* right_child)
		: ExprBaseLogCmpBinOp(left_child, right_child)
	{
	}

	void evaluate() final
	{
		ExprNum left_expr_num, right_expr_num;
		_get_resized_child_expr_nums(left_expr_num, right_expr_num);

		_value.copy_from_bignum(static_cast<BigNum>(left_expr_num)
			<= static_cast<BigNum>(right_expr_num));

		// Comparison results are unsigned
		_value.set_is_signed(false);
	}
};

// ">="
class ExprBinOpCmpGe : public ExprBaseLogCmpBinOp
{
public:		// functions
	ExprBinOpCmpGe(Expression* left_child, Expression* right_child)
		: ExprBaseLogCmpBinOp(left_child, right_child)
	{
	}

	void evaluate() final
	{
		ExprNum left_expr_num, right_expr_num;
		_get_resized_child_expr_nums(left_expr_num, right_expr_num);

		_value.copy_from_bignum(static_cast<BigNum>(left_expr_num)
			>= static_cast<BigNum>(right_expr_num));

		// Comparison results are unsigned
		_value.set_is_signed(false);
	}
};

// "Expression" classes derived from "ExprBaseArithBinOp"

// Binop "+"
class ExprBinOpPlus : public ExprBaseArithBinOp
{
public:		// functions
	ExprBinOpPlus(Expression* left_child, Expression* right_child)
		: ExprBaseArithBinOp(left_child, right_child)
	{
	}

	void evaluate() final
	{
		_value.copy_from_bignum(static_cast<BigNum>(_left_child_value())
			+ static_cast<BigNum>(_right_child_value()));
	}
};

// Binop "-"
class ExprBinOpMinus : public ExprBaseArithBinOp
{
public:		// functions
	ExprBinOpMinus(Expression* left_child, Expression* right_child)
		: ExprBaseArithBinOp(left_child, right_child)
	{
	}

	void evaluate() final
	{
		_value.copy_from_bignum(static_cast<BigNum>(_left_child_value())
			- static_cast<BigNum>(_right_child_value()));
	}
};

// "*"
class ExprBinOpMul : public ExprBaseArithBinOp
{
public:		// functions
	ExprBinOpMul(Expression* left_child, Expression* right_child)
		: ExprBaseArithBinOp(left_child, right_child)
	{
	}

	void evaluate() final
	{
		_value.copy_from_bignum(static_cast<BigNum>(_left_child_value())
			* static_cast<BigNum>(_right_child_value()));
	}
};

// "/"
class ExprBinOpDiv : public ExprBaseArithBinOp
{
public:		// functions
	ExprBinOpDiv(Expression* left_child, Expression* right_child)
		: ExprBaseArithBinOp(left_child, right_child)
	{
	}

	void evaluate() final
	{
		_value.copy_from_bignum(static_cast<BigNum>(_left_child_value())
			/ static_cast<BigNum>(_right_child_value()));
	}
};

// "%"
class ExprBinOpMod : public ExprBaseArithBinOp
{
public:		// functions
	ExprBinOpMod(Expression* left_child, Expression* right_child)
		: ExprBaseArithBinOp(left_child, right_child)
	{
	}

	void evaluate() final
	{
		_value.copy_from_bignum(static_cast<BigNum>(_left_child_value())
			% static_cast<BigNum>(_right_child_value()));
	}
};

// "Expression" classes derived from "ExprBaseBitNonShiftBinOp"

// "&"
class ExprBinOpBitAnd : public ExprBaseBitNonShiftBinOp
{
public:		// functions
	ExprBinOpBitAnd(Expression* left_child, Expression* right_child)
		: ExprBaseBitNonShiftBinOp(left_child, right_child)
	{
	}

	void evaluate() final
	{
		_value.copy_from_bignum(static_cast<BigNum>(_left_child_value())
			& static_cast<BigNum>(_right_child_value()));
	}
};

// "|"
class ExprBinOpBitOr : public ExprBaseBitNonShiftBinOp
{
public:		// functions
	ExprBinOpBitOr(Expression* left_child, Expression* right_child)
		: ExprBaseBitNonShiftBinOp(left_child, right_child)
	{
	}

	void evaluate() final
	{
		_value.copy_from_bignum(static_cast<BigNum>(_left_child_value())
			| static_cast<BigNum>(_right_child_value()));
	}
};

// "^"
class ExprBinOpBitXor : public ExprBaseBitNonShiftBinOp
{
public:		// functions
	ExprBinOpBitXor(Expression* left_child, Expression* right_child)
		: ExprBaseBitNonShiftBinOp(left_child, right_child)
	{
	}

	void evaluate() final
	{
		_value.copy_from_bignum(static_cast<BigNum>(_left_child_value())
			^ static_cast<BigNum>(_right_child_value()));
	}
};

// "Expression" classes derived from "ExprBaseBitShiftBinOp"

// "<<"
class ExprBinOpBitLsl : public ExprBaseBitShiftBinOp
{
public:		// functions
	ExprBinOpBitLsl(Expression* left_child, Expression* right_child)
		: ExprBaseBitShiftBinOp(left_child, right_child)
	{
	}

	void evaluate() final;
};

// ">>"
class ExprBinOpBitLsr : public ExprBaseBitShiftBinOp
{
public:		// functions
	ExprBinOpBitLsr(Expression* left_child, Expression* right_child)
		: ExprBaseBitShiftBinOp(left_child, right_child)
	{
	}

	void evaluate() final;
};

// ">>>"
class ExprBinOpBitAsr : public ExprBaseBitShiftBinOp
{
public:		// functions
	ExprBinOpBitAsr(Expression* left_child, Expression* right_child)
		: ExprBaseBitShiftBinOp(left_child, right_child)
	{
	}

	void evaluate() final;
};

} // namespace frost_hdl


#endif		// src_expression_classes_hpp
