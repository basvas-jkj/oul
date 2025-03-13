module;

#include <helpers/Helpers.h>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#undef ERROR

export module zip;

import std;
import tmp;
import config;
import common;
import message;
import dir_iterator;

using namespace std;
using namespace boost::process;
namespace fs = boost::filesystem;

namespace oul
{
	class TOOL_7Z;
	class TOOL_ZIP;
	export class ABSTRACT_ZIP_TOOL
	{
		TMP_FILE create_zip(cr<ITEM> component)
		{
			fs::path json_path = "oul.component.json";
			TMP_FILE json(json_path.string(), true);
			ofstream out = json.write();
			save_json(component, out);
			out.close();

			fs::path dir = json.get_directory();
			TMP_FILE zip_file(dir / "komponent.zip");
			call_tool(zip_file.get_path(), json.get_path(), dir);

			return zip_file;
		}

	protected:
		fs::path path;
		ABSTRACT_ZIP_TOOL(cr<fs::path> p): path(p)
		{}
		void call_tool(cr<fs::path> zip_file, cr<fs::path> entry)
		{
			call_tool(zip_file, entry, fs::current_path());
		}
		virtual void call_tool(cr<fs::path> zip_file, cr<fs::path> entry, cr<fs::path> working_directory) = 0;
	public:
		void zip(cr<fs::path> base, cr<ITEM> component)
		{
			fs::path component_dir = base / component.location;
			TMP_FILE zip_file = create_zip(component);
			FILE_ITERATOR it = FILE_ITERATOR::init(component_dir, component.include, component.exclude);

			for (cr<fs::path> entry : it)
			{
				println("{}", fs::relative(entry, component_dir).generic_string());
				call_tool(zip_file.get_path(), entry, component_dir);
			}
		}
		//virtual optional<ITEM> unzip(const TMP_FILE& zip_file, const string& where) = 0;
		virtual ~ABSTRACT_ZIP_TOOL() = default;
	};
	export using zip_tool_ptr = unique_ptr<ABSTRACT_ZIP_TOOL>;

	class TOOL_ZIP: public ABSTRACT_ZIP_TOOL
	{
	public:
		TOOL_ZIP(cr<fs::path> p): ABSTRACT_ZIP_TOOL(p)
		{}
		void call_tool(cr<fs::path> zip_file, cr<fs::path> entry, cr<fs::path> working_directory) override
		{
			child ch(path, zip_file, entry, start_dir(working_directory), std_out > null, std_err > null);
			ch.wait();
			if (ch.exit_code() == 0)
			{
				cerr << "ZIP was successful." << endl;
			}
			else
			{
				cerr << endl << "ZIP was not successful." << endl;
			}
		}
	};
	class TOOL_7Z: public ABSTRACT_ZIP_TOOL
	{
	public:
		TOOL_7Z(cr<fs::path> p): ABSTRACT_ZIP_TOOL(p)
		{}

		void call_tool(cr<fs::path> zip_file, cr<fs::path> entry, cr<fs::path> working_directory) override
		{
			fs::path rel = fs::relative(entry, working_directory);
			child ch(path, "a", zip_file, rel, start_dir(working_directory), std_out > null, std_err > null);
			ch.wait();
			if (ch.exit_code() == 0)
			{
				cerr << "ZIP was successful." << endl;
			}
			else
			{
				cerr << endl << "ZIP was not successful." << endl;
			}
		}
	};

	export class MissingZipTool: public CommonException
	{
	public:
		MissingZipTool(ERROR name): CommonException(name)
		{}
	};
	export zip_tool_ptr select_zip_tool()
	{
		fs::path tool_path = search_path("zip");
		if (tool_path != "")
		{
			//return make_unique<TOOL_ZIP>(tool_path);
		}
		tool_path = search_path("7z");
		if (tool_path != "")
		{
			return make_unique<TOOL_7Z>(tool_path);
		}
		throw MissingZipTool(missing_zip_tool);
	}
	/*static optional<ITEM> unzip(const TMP_FILE& zip_file, const string& where)
	{
		bool success;
		ITEM component;
		string zip_name = zip_file.get_name().string();

		string config = Zip::ExtractTextFromZip(zip_name, "oul.component.json", success);
		if (!success)
		{
			config = Zip::ExtractTextFromZip(zip_name, "oul.component.yaml", success);
			if (!success)
			{
				return {false, component};
			}
		}

		component = read_component(config).second;

		size_t count;
		TMP_FOLDER extracted_zip(zip_file.get_name().replace_extension());
		if (Zip::ExtractAllFilesFromZip(extracted_zip.get_name().string(), zip_name, count))
		{
			if (count < component.source_files.size() + component.test_files.size() + component.documentation.size())
			{
				return {false, component};
			}

			vector content{component.source_files, component.test_files, component.documentation};
			for (string& entry_name : content | views::join)
			{
				move(extracted_zip.get_name(), where, entry_name);
			}
		}

		return {true, component};
	}*/
}