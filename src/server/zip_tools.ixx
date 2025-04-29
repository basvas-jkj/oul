module;

#include <boost/filesystem/path.hpp>

export module zip:tools;
import :base;

namespace oul
{
    /// @brief Reprezentuje nástroje zip a unzip.
    /// (https://infozip.sourceforge.net/)
    class TOOL_ZIP: public ABSTRACT_ZIP_TOOL
    {
    public:
        TOOL_ZIP(cr<path> zip_path, cr<path> unzip_path): ABSTRACT_ZIP_TOOL(zip_path, unzip_path)
        {}
        void zip(cr<path> working_dir, cr<TMP_FILE> zip_file, cr<path> entry) override
        {
            bool success = call_tool(zip_path, working_dir, zip_file.get_path(), entry);
            if (!success)
            {
                throw new ZippingError(ERROR::zipping_error);
            }
        }
        void unzip(cr<path> working_dir, cr<TMP_FILE> zip_file) override
        {
            bool success = call_tool(unzip_path, working_dir, zip_file.get_path());
            if (!success)
            {
                throw new ZippingError(ERROR::zipping_error);
            }
        }
    };
    /// @brief Reprezentuje nástroje 7z
    /// (https://www.7-zip.org/)
    class TOOL_7Z: public ABSTRACT_ZIP_TOOL
    {
    public:
        TOOL_7Z(cr<path> p): ABSTRACT_ZIP_TOOL(p, p)
        {}

        void zip(cr<path> working_dir, cr<TMP_FILE> zip_file, cr<path> entry) override
        {
            bool success = call_tool("a", zip_path, working_dir, zip_file.get_path(), entry);
            if (!success)
            {
                throw new ZippingError(ERROR::zipping_error);
            }
        }
        void unzip(cr<path> working_dir, cr<TMP_FILE> zip_file) override
        {
            bool success = call_tool("x", zip_path, working_dir, zip_file.get_path());
            if (!success)
            {
                throw new ZippingError(ERROR::zipping_error);
            }
        }
    };
}
