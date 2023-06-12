#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "kernel/resource/string_table.h"

namespace yrui {
namespace resource {
    class String_table::Imp {
    public:
        Imp(const boost::property_tree::ptree& config)
        {
            auto file = config.get<std::string>("config.core.string_table");
            std::ifstream ifs(file);
            boost::property_tree::read_xml(ifs, table_);
            table_ = table_.get_child("table");
        }

        std::string str(const std::string& key, id_t id) const
        {
            auto& subtable = table_.get_child(key);
            return subtable.get<std::string>(boost::lexical_cast<std::string>(id));
        }

    private:
        boost::property_tree::ptree table_;
    };

    String_table::String_table(const boost::property_tree::ptree& config)
        : imp_(new Imp(config))
    {
    }

    String_table::~String_table() { }

    std::string String_table::str_(const std::string& key, id_t id) const { return imp_->str(key, id); }
}
} // of namespace yrui::resource

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
