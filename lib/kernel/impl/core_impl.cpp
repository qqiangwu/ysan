#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/count.hpp>
#include <boost/range/algorithm/transform.hpp>

#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "kernel/aggr_ai/aggressive_maker.h"
#include "kernel/aggr_ai/aggressiveai.h"
#include "kernel/arch/architecture.h"
#include "kernel/arch/architecture_mgr.h"
#include "kernel/core/timeline.h"
#include "kernel/diplomacy/diplomacy_mgr.h"
#include "kernel/effect/effect_mgr.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_mgr.h"
#include "kernel/military/military_mgr.h"
#include "kernel/person/person.h"
#include "kernel/person/person_mgr.h"
#include "kernel/report/report_mgr.h"
#include "kernel/resource/map.h"

#include "kernel/impl/archai/ai_architecture_aggressive.h"
#include "kernel/impl/archai/ai_architecture_defensive.h"
#include "kernel/impl/archai/ai_architecture_development.h"
#include "kernel/impl/core_impl.h"
#include "kernel/impl/serialization/parser.h"

namespace yrui {
using namespace person;
using namespace arch;
using namespace fraction;
using namespace diplomacy;
using namespace military;
using namespace effect;
using namespace report;
using namespace boost::adaptors;

namespace {
    // parsers
    serialization::Architecture_parser arch_parser_;
    serialization::Fraction_parser frac_parser_;
    serialization::Person_parser person_parser_;
    serialization::Map_parser map_parser_;
}

Core::Imp::Imp(const std::string& config)
{
    //! init resources
    boost::property_tree::read_xml(config, config_);
    //! read xml
    auto& core = config_.get_child("config.core");

    BOOST_LOG_NAMED_SCOPE("Init");

    //! load string table
    //! \todo   move string table to some other places
    //  stable_.reset(new resource::String_table(config_));

    //! init subsystem
    {
        BOOST_LOG_NAMED_SCOPE("CreateSubsystems");

        YLOG() << "Create subsystems";
        auto start_year = core.get<int>("timeline.year");
        auto start_month = core.get<int>("timeline.month");

        YLOG() << "Create Person Mgr";
        install_subsys_(new Person_mgr);

        YLOG() << "Create Architecture Mgr";
        install_subsys_(new Architecture_mgr);

        YLOG() << "Create Fraction Mgr";
        install_subsys_(new Fraction_mgr);

        YLOG() << "Create Military Mgr";
        install_subsys_(new Military_mgr);

        YLOG() << "Create Diplomacy Mgr";
        install_subsys_(new Diplomacy_mgr);

        YLOG() << "Create Report Mgr";
        install_subsys_(new Report_mgr);

        YLOG() << "Create Effect Mgr";
        install_subsys_(new Effect_mgr);

        YLOG() << format("Create Timeline %1% - %2%") % start_year % start_month;
        install_subsys_(new Timeline(start_year, start_month));
    }

    //! load all game resource from specified files
    load_scenario_(core);
    load_ai_("");
}

void Core::Imp::initialize_()
{
    {
        BOOST_LOG_NAMED_SCOPE("InitSubSys");

        YLOG() << "Initialize all subsystems";
        for (auto& sys : sys_ | map_values) {
            sys->init();
        }
    }

    {
        BOOST_LOG_NAMED_SCOPE("PostInit");
        YLOG() << "Post initialization";
        for (auto& func : post_initializer_) {
            func();
        }
        post_initializer_ = {};
    }

    {
        BOOST_LOG_NAMED_SCOPE("InstallSig");
        auto& tl = use_subsys_<Timeline>();
        auto& person_mgr = use_subsys_<Person_mgr>();
        auto& arch_mgr = use_subsys_<Architecture_mgr>();
        auto& frac_mgr = use_subsys_<Fraction_mgr>();
        auto& military_mgr = use_subsys_<Military_mgr>();

        //! setup signals
        YLOG() << "Install signals";
        tl.month_begin.connect([&person_mgr](Timeline& t) { person_mgr.on_month_begin(t); });
        tl.month_begin.connect([&arch_mgr](Timeline& t) { arch_mgr.on_month_begin(t); });
        tl.month_begin.connect([&frac_mgr](Timeline& t) { frac_mgr.on_month_begin(t); });

        tl.month_end.connect([&military_mgr](Timeline& t) { military_mgr.on_month_end(t); });
        tl.month_end.connect([&frac_mgr](Timeline& t) { frac_mgr.on_month_end(t); });
        tl.month_end.connect([&arch_mgr](Timeline& t) { arch_mgr.on_month_end(t); });
        tl.month_end.connect([&person_mgr](Timeline& t) { person_mgr.on_month_end(t); });

        tl.season_begin.connect([&arch_mgr](Timeline& t) { arch_mgr.on_season_begin(t); });
        tl.season_end.connect([&arch_mgr](Timeline& t) { arch_mgr.on_season_end(t); });

        tl.year_begin.connect([&person_mgr](Timeline& t) { person_mgr.on_year_begin(t); });
        tl.year_begin.connect([&arch_mgr](Timeline& t) { arch_mgr.on_year_begin(t); });
        tl.year_begin.connect([&frac_mgr](Timeline& t) { frac_mgr.on_year_begin(t); });

        tl.year_end.connect([&frac_mgr](Timeline& t) { frac_mgr.on_year_end(t); });
        tl.year_end.connect([&arch_mgr](Timeline& t) { arch_mgr.on_year_end(t); });
        tl.year_end.connect([&person_mgr](Timeline& t) { person_mgr.on_year_end(t); });

        frac_mgr.nation_unified.connect(sig_level::observer, [this](Fraction* f) {
            YLOG_SEV(trace) << format("%1% unifies the nation") % f->name();
            stop_();
        });

        person_mgr.person_die.connect(sig_level::observer, [&person_mgr, this](Person* p) {
            if (person_mgr.appeared().size() == 0) {
                YLOG_SEV(trace) << format("All persons have died");
                stop_();
            }
        });
    }
}

void Core::Imp::load_scenario_(const boost::property_tree::ptree& core)
{
    BOOST_LOG_NAMED_SCOPE("LoadScenario");

    load_architecture_(core.get<std::string>("architecture"));
    load_fraction_(core.get<std::string>("fraction"));
    load_person_(core.get<std::string>("person"));
    load_map_(core.get<std::string>("map"));
}

void Core::Imp::run_()
{
    BOOST_ASSERT(!is_run_);

    is_run_ = true;
    auto& timeline = use_subsys_<Timeline>();

    YLOG_SEV(trace) << "Start Game";
    // pass a month at a time
    while (is_run_) {
        YLOG_SEV(trace) << format("Date %1%/%2%") % timeline.year() % timeline.month();

        for (auto& sys : sys_ | map_values) {
            sys->update();
        }
    }

    YLOG_SEV(trace) << "Game end";
}

///////////////////////////////////////////////////////////////////////////
void Core::Imp::load_ai_(const std::string& dll)
{
    frac_ai_maker_ = yrui::make_fraction_ai;

    arch_ai_.emplace("core::attack", [] { return make_uptr<ai::Arch_aggressive_ai>(); });
    arch_ai_.emplace("core::defense", [] { return make_uptr<ai::Arch_defensive_ai>(); });
    arch_ai_.emplace("core::develop", [] { return make_uptr<ai::Arch_development_ai>(); });
}

//////////////////////////////////////////////////////////////////////////
namespace {
    std::string load_db(const std::string& db)
    {
        namespace R = yrui::serialization;

        std::ifstream ifs(db);
        BOOST_ASSERT_MSG(ifs, "database must exist and have corrent format");

        std::string src { std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>() };

        return src;
    }

    template <class RawT, class RealT> void transfrom(const std::vector<RawT>& info, std::vector<uptr<RealT>>& ret)
    {
        BOOST_ASSERT(info.size() == ret.size());

        boost::range::transform(info, ret.begin(), [](const RawT& x) { return make_uptr<RealT>(x); });

        BOOST_ASSERT(boost::range::count(ret, nullptr) == 0);
    }
} // of namespace unnamed

void Core::Imp::load_map_(const std::string& db)
{
    YLOG_SEV(trace) << "Load map";

    auto src = load_db(db);
    auto& mgr = use_subsys_<Military_mgr>();

    mgr.load(serialization::read_db(src, map_parser_));
}

void Core::Imp::load_person_(const std::string& db)
{
    YLOG_SEV(trace) << "Load person";

    using namespace serialization;

    auto src = load_db(db);
    auto raw_items = read_db(src, person_parser_);

    auto& mgr = use_subsys_<Person_mgr>();

    std::vector<uptr<Person>> items(raw_items.size());
    transfrom(raw_items, items);

    mgr.load(std::move(items));
}

void Core::Imp::load_architecture_(const std::string& db)
{
    using namespace serialization;

    YLOG_SEV(trace) << "Load architecture";

    auto src = load_db(db);
    auto raw_archs = read_db(src, arch_parser_);

    auto& arch_mgr = use_subsys_<Architecture_mgr>();

    std::vector<uptr<Architecture>> items(raw_archs.size());
    transfrom(raw_archs, items);

    arch_mgr.load(std::move(items));
}

void Core::Imp::load_fraction_(const std::string& db)
{
    using namespace serialization;

    YLOG_SEV(trace) << "Load fraction";

    auto src = load_db(db);
    auto raw_info = read_db(src, frac_parser_);

    auto& fmgr = use_subsys_<Fraction_mgr>();
    auto& dmgr = use_subsys_<Diplomacy_mgr>();

    //[ load fractions
    std::vector<uptr<Fraction>> items(raw_info.raw_fraction.size());
    transfrom(raw_info.raw_fraction, items);

    fmgr.load(std::move(items));
    //]

    //[ load diplomacy
    dmgr.load(std::move(raw_info.rel));
    //]
}
} // of namespace yrui
