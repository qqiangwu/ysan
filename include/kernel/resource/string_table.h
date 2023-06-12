#ifndef YRUI_CORE_COMMON_STRING_TABLE_H_
#define YRUI_CORE_COMMON_STRING_TABLE_H_

#include "kernel/component_fwd.h"
#include "kernel/config.h"
#include <boost/property_tree/ptree.hpp>

namespace yrui {
namespace resource {
    class String_table {
    public:
        String_table(const boost::property_tree::ptree& config);
        ~String_table();

        template <class T> std::string str(id_t id) const;

    private:
        std::string str_(const std::string& key, id_t id) const;

    private:
        YRUI_DEFINE_IMP();
    };
} // of namespace resource
} // of namespace yrui

namespace yrui {
namespace resource {
    namespace key_imp {
        template <class T> inline constexpr const char* key();
    }

    template <class T> inline std::string String_table::str(id_t id) const
    {
        //! try to get type name at runtime with all qualifiers striped.
        return str_(key_imp::key<T>(), id);
    }
}
} // of namespace yrui

#define STRING_TABLE_SUPPORT(class_name, category_name)                                                                \
    namespace yrui {                                                                                                   \
        namespace resource {                                                                                           \
            namespace key_imp {                                                                                        \
                template <> inline constexpr const char* key<class_name>() { return category_name; }                   \
            }                                                                                                          \
        }                                                                                                              \
    }

#endif // !YRUI_CORE_COMMON_STRING_TABLE_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
