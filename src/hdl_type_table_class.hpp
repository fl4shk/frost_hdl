#ifndef src_hdl_type_table_class_hpp
#define src_hdl_type_table_class_hpp

// src/hdl_type_table_class.hpp

#include "misc_includes.hpp"
#include "general_allocator_class.hpp"
#include "ident_table_classes.hpp"

namespace frost_hdl
{

class Expression;
class Symbol;
class HdlFunction;

// The type of a "Symbol"
// How can I implement parameterized HDL types?  "parameter"s will likely
// be stored as "Symbol"s.
class HdlType
{
public:		// types
	typedef Expression* Dimension;


	typedef std::vector<Symbol*> ParameterVars;
	typedef std::vector<Symbol*> CompositeVars;
	typedef CompositeVars EnumVals;
	typedef std::vector<HdlFunction*> CompositeFuncs;

	class ComponentData
	{
	private:		// variables
		bool _is_packed = false;
		bool _is_enum = false;
		ParameterVars _parameter_vars;

		// Both enum values and composite variables are stored here.
		CompositeVars _composite_vars;

		CompositeFuncs _composite_funcs;

	public:		// functions
		ComponentData() = default;

		ComponentData(bool s_is_packed,
			ParameterVars&& s_parameter_vars,
			CompositeVars&& s_composite_vars,
			CompositeFuncs&& s_composite_funcs);

		inline ComponentData(const ComponentData& to_copy) = delete;
		inline ComponentData(ComponentData&& to_move) = default;

		virtual ~ComponentData() = default;


		inline ComponentData& operator = (const ComponentData& to_copy)
			= delete;
		inline ComponentData& operator = (ComponentData&& to_move)
			= default;

		void init_as_enum(EnumVals&& s_vals_of_enum);

		inline bool can_be_parameterized() const
		{
			return (parameter_vars().size() > 0);
		}
		inline bool is_composite() const
		{
			//return ((composite_vars().size() > 0)
			//	|| (composite_funcs().size() > 0));
			return ((parameter_vars().size() > 0)
				|| (composite_vars().size() > 0)
				|| (composite_funcs().size() > 0));
		}

		inline const EnumVals& vals_of_enum() const
		{
			return _composite_vars;
		}

		GEN_GETTER_BY_VAL(is_packed)
		GEN_GETTER_BY_VAL(is_enum)
		GEN_GETTER_BY_CON_REF(parameter_vars)
		GEN_GETTER_BY_CON_REF(composite_vars)
		GEN_GETTER_BY_CON_REF(composite_funcs)
	};


protected:		// variables
	SavedString _ident = nullptr;

	bool _is_signed = false;

	// "_actually_exists" needs to be set to true before this
	// "HdlType" is permitted to be used.
	bool _actually_exists = false;

	ComponentData _component_data;

	// Only one set of dimensions left of the symbol's identifer.
	Dimension _left_dim_expr = nullptr;


public:		// functions
	HdlType() = default;
	HdlType(SavedString s_ident, bool s_is_signed);
	HdlType(SavedString s_ident, bool s_is_signed,
		Dimension s_left_dim_expr);
	HdlType(SavedString s_ident, ComponentData&& s_component_data);

	// We really don't want copies of "HdlType"s.
	inline HdlType(const HdlType& to_copy) = delete;
	inline HdlType(HdlType&& to_move) = default;

	virtual ~HdlType() = default;

	// We really don't want copies of "HdlType"s.
	inline HdlType& operator = (const HdlType& to_copy) = delete;
	inline HdlType& operator = (HdlType&& to_move) = default;


	inline bool can_be_parameterized() const
	{
		return component_data().can_be_parameterized();
	}

	inline bool is_enum() const
	{
		return component_data().is_enum();
	}

	// Whether or not this "HdlType" represents "struct" or "class".
	// If "is_composite()" is true, then "_left_dim_expr" and "_is_signed"
	// are not used.
	inline bool is_composite() const
	{
		return component_data().is_composite();
	}

	inline bool is_packed_composite() const
	{
		return (component_data().is_composite()
			&& component_data().is_packed());
	}
	inline bool is_unpacked_composite() const
	{
		return (component_data().is_composite()
			&& (!component_data().is_packed()));
	}

	inline bool is_builtin() const
	{
		return ((!is_composite()) && (!is_enum()));
	}

	size_t left_dim() const;


	GEN_GETTER_BY_VAL(ident)
	GEN_GETTER_BY_VAL(is_signed)

	GEN_GETTER_AND_SETTER_BY_VAL(actually_exists)


	GEN_GETTER_BY_CON_REF(component_data)

	GEN_SETTER_BY_VAL(left_dim_expr)
};


// "HdlTypeTable" isn't scoped because scoping information is stored
// in the "StatementTable" class.
class HdlTypeTable : public IdentToPointerTable<HdlType>
{
public:		// functions
	HdlTypeTable() = default;
	virtual ~HdlTypeTable() = default;
};

} // namespace frost_hdl

#endif		// src_hdl_type_table_class_hpp
