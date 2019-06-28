#ifndef src_misc_includes_hpp
#define src_misc_includes_hpp

// src/misc_includes.hpp


#include <stack>
#include <map>
#include <set>
#include <functional>
#include <memory>

//#include <list>
//#include <forward_list>

#include <variant>
#include <type_traits>


#include "liborangepower_src/misc_includes.hpp"
#include "liborangepower_src/misc_output_classes.hpp"
#include "liborangepower_src/misc_input_classes.hpp"
#include "liborangepower_src/misc_types.hpp"
#include "liborangepower_src/misc_utility_funcs.hpp"
#include "liborangepower_src/misc_bitwise_funcs.hpp"
#include "liborangepower_src/misc_defines.hpp"
#include "liborangepower_src/gen_class_innards_defines.hpp"
#include "liborangepower_src/json_stuff.hpp"
#include "liborangepower_src/prev_curr_pair_classes.hpp"
#include "liborangepower_src/gmp_stuff.hpp"
#include "liborangepower_src/time_stuff.hpp"
#include "liborangepower_src/with_defines.hpp"
#include "liborangepower_src/linked_list_classes.hpp"
#include "liborangepower_src/range_funcs.hpp"

using namespace liborangepower::printout_etc;
using namespace liborangepower::misc_input;
using namespace liborangepower::integer_types;
using namespace liborangepower::misc_util;
using namespace liborangepower::bitwise;
using namespace liborangepower::json;
using namespace liborangepower::range;

using liborangepower::bignum::BigNum;

using liborangepower::containers::MoveOnlyPrevCurrPair;
using liborangepower::containers::CircLinkedList;

//#include <boost/mp11.hpp>
//using namespace boost::mp11;
using string = std::string;
template<typename Type>
using unique_ptr = std::unique_ptr<Type>;


#endif		// src_misc_includes_hpp
