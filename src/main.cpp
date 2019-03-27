#include "parse_tree_visitor_class.hpp"
#include "expr_num_class.hpp"
#include "expression_classes.hpp"
#include "testing.hpp"


int main(int argc, char** argv)
{
	// We need at least *one* source file.
	if (argc < 2)
	{
		printerr("Usage:  ", argv[0], " <source_files>\n");
		exit(1);
	}

	frost_hdl::ParseTreeVisitor::ListParsedSrcCode s_list_parsed_src_code;

	//printout("argc:  ", argc, "\n");

	for (int i=1; i<argc; ++i)
	{
		s_list_parsed_src_code.push_back(std::unique_ptr<frost_hdl
			::ParsedSrcCode>(new frost_hdl::ParsedSrcCode(argv[i])));
	}

	frost_hdl::ParseTreeVisitor visitor(std::move(s_list_parsed_src_code));
	return visitor.run();

}
