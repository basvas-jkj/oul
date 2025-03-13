module;

#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#undef ERROR

export module zip:base;

import tmp;
import config;
import common;
import dir_iterator;

using namespace std;
using namespace boost::process;
using namespace boost::filesystem;

namespace oul
{
	export class MissingZipTool: public CommonException
	{
	public:
		MissingZipTool(ERROR name): CommonException(name)
		{}
	};
	export class ZippingError: public CommonException
	{
	public:
		ZippingError(ERROR name): CommonException(name)
		{}
	};

	template <class ...T>
	concept CmdArgs = ((same_as<string, T> || same_as<path, T>) && ...);

	export class ABSTRACT_ZIP_TOOL
	{
		TMP_FILE create_zip(cr<ITEM> component)
		{
			path json_path = "oul.component.json";
			TMP_FILE json(json_path.string(), true);
			ofstream out = json.write();
			save_json(component, out);
			out.close();

			path json_dir = json.get_directory();
			TMP_FILE zip_file(json_dir / "komponent.zip");
			zip(json_dir, zip_file, json.get_path());

			return zip_file;
		}

	protected:
		path zip_path;
		path unzip_path;

		ABSTRACT_ZIP_TOOL(cr<path> zip_path, cr<path> unzip_path):
			zip_path(zip_path), unzip_path(unzip_path)
		{}

		template <CmdArgs ...T>
		void call_tool(cr<path> tool_path, cr<path> working_directory, T... args)
		{
			child ch(tool_path, args..., start_dir(working_directory), std_out > null, std_err > null);
			ch.wait();
			if (ch.exit_code() != 0)
			{
				throw ZippingError(zipping_error);
			}
		}
		template <CmdArgs ...T>
		void call_tool_here(cr<path> tool_path, cr<path> working_directory, T... args)
		{
			call_tool(tool_path, current_path(), args...);
		}

		virtual void zip(cr<path> working_dir, cr<TMP_FILE> zip_file, cr<path> entry) = 0;
		virtual void unzip(cr<path> working_dir, cr<TMP_FILE> zip_file) = 0;
	public:
		void zip(cr<path> base, cr<ITEM> component)
		{
			path component_dir = base / component.location;
			TMP_FILE zip_file = create_zip(component);
			FILE_ITERATOR it = FILE_ITERATOR::init(component_dir, component.include, component.exclude);

			for (cr<path> entry : it)
			{
				zip(component_dir, zip_file, entry);
			}
		}
		//virtual optional<ITEM> unzip(const TMP_FILE& zip_file, const string& where) = 0;
		virtual ~ABSTRACT_ZIP_TOOL() = default;
	};
}