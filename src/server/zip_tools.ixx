export module zip:tools;
import :base;

namespace oul
{
	class TOOL_ZIP: public ABSTRACT_ZIP_TOOL
	{
	public:
		TOOL_ZIP(cr<path> zip_path, cr<path> unzip_path): ABSTRACT_ZIP_TOOL(zip_path, unzip_path)
		{}
		void zip(cr<path> working_dir, cr<TMP_FILE> zip_file, cr<path> entry) override
		{
			call_tool(zip_path, working_dir, zip_file.get_path(), entry);
		}
		void unzip(cr<path> working_dir, cr<TMP_FILE> zip_file) override
		{
			call_tool(unzip_path, working_dir, zip_file.get_path());
		}
	};
	class TOOL_7Z: public ABSTRACT_ZIP_TOOL
	{
	public:
		TOOL_7Z(cr<path> p): ABSTRACT_ZIP_TOOL(p, p)
		{}

		void zip(cr<path> working_dir, cr<TMP_FILE> zip_file, cr<path> entry) override
		{
			call_tool("a", zip_path, working_dir, zip_file.get_path(), entry);
		}
		void unzip(cr<path> working_dir, cr<TMP_FILE> zip_file) override
		{
			call_tool("x", zip_path, working_dir, zip_file.get_path());
		}
	};
}