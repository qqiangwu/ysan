#include "Main/Byapp.h"
#include "Adaptor/QtAIPlayer.h"
#include "Adaptor/QtPlayer.h"
#include "Debugger.h"
#include "Form/About_form.h"
#include "Form/ByScreen.h"
#include "Form/Intro_form.h"
#include "Form/Select_form.h"
#include "Main/GameLogic.h"
#include "ScreenView/BattleView.h"
#include "ScreenView/Mapview.h"

#include <QDialog>
#include <QLabel>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "kernel/arch/architecture.h"
#include "kernel/common/randutil.h"
#include "kernel/core.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_mgr.h"
#include "kernel/military/intruded_arch.h"
#include "kernel/military/military_mgr.h"
#include "kernel/person/person.h"
#include "kernel/person/person_mgr.h"
#include <Detail/Shim.h>

#include <fstream>
#include <thread>

Q_DECLARE_METATYPE(yrui::Legion*);
Q_DECLARE_METATYPE(yrui::Architecture*);
Q_DECLARE_METATYPE(yrui::Person*);
Q_DECLARE_METATYPE(yrui::Fraction*);
Q_DECLARE_METATYPE(yrui::Intruded_arch*);

namespace Bygone {
namespace Frontend {
    namespace {
        std::map<yrui::id_t, QColor> load_color_map(const std::string& file)
        {
            qDebug() << "Load color map";
            boost::property_tree::ptree table;
            boost::property_tree::xml_parser::read_xml(file, table);

            std::map<yrui::id_t, QColor> ret;

            for (auto& ch : table.get_child("color")) {
                auto id = std::stoi(ch.first);
                auto& c = ch.second;
                auto r = c.get<int>("r", 0);
                auto g = c.get<int>("g", 0);
                auto b = c.get<int>("b", 0);
                ret.emplace(id, QColor(r, g, b));
            }

            return ret;
        }
    }

    Byapp::Byapp(int argc, char* argv[])
        : QApplication(argc, argv)
        , provider_("Resource/Image/", { 300, 300 })
    {
        intro_.reset(new Intro_form);
        about_.reset(new About_form);
        select_.reset(new Select_form);

        //! setup connections
        connect(intro_.get(), SIGNAL(accepted()), select_.get(), SLOT(show()));
        connect(intro_.get(), SIGNAL(rejected()), this, SLOT(quit()));
        connect(intro_.get(), SIGNAL(aboutClicked()), about_.get(), SLOT(open()));

        connect(select_.get(), SIGNAL(rejected()), intro_.get(), SLOT(show()));

        connect(select_.get(), SIGNAL(selectDone(std::string, int)), this, SLOT(runGame(std::string, int)));

        intro_->open();
    }

    Byapp::~Byapp() { }

    yrui::Fraction* Byapp::playerFraction() const
    {
        using yrui::fraction::Fraction_mgr;
        if (!fraction_) {
            auto& mgr = yrui::core()->use_subsys<Fraction_mgr>();
            fraction_ = mgr.get(this->player());
        }

        return fraction_;
    }

    yrui::Fraction_controller* Byapp::controller() const { return player_; }

    void Byapp::runGame(const std::string& config, int fraction)
    {
        BOOST_ASSERT_MSG(player_ == nullptr, "This can only be called once.");

        qDebug() << "Create core";
        yrui::Core::create(config);

        qDebug() << "Load resource";
        color_ = load_color_map(yrui::core()->config("ui").get<std::string>("color"));

        player_fraction_ = fraction;

        logic_.reset(new GameLogic(this));
        screen_.reset(new ByScreen(this));
        screen_->onStart();

        //! setup controllers and bind to screen
        using yrui::fraction::Fraction_mgr;
        for (auto f : yrui::core()->use_subsys<Fraction_mgr>().as_range()) {
            if (static_cast<int>(f->id()) == fraction) {
                BOOST_ASSERT_MSG(screen_->battleView(), "BattleView not created");

                player_ = new QtPlayer(f);

                connect(
                    player_, SIGNAL(playerThinking()), screen_.get(), SLOT(onPlayerThinking()), Qt::QueuedConnection);
                connect(player_, SIGNAL(actionRequired()), screen_->battleView(), SLOT(askForAction()),
                    Qt::BlockingQueuedConnection);
                connect(player_, SIGNAL(yesOrNo(QString)), screen_.get(), SLOT(askForChoice(QString)),
                    Qt::BlockingQueuedConnection);

                //TODO
                f->set_controller(yrui::uptr<yrui::Fraction_controller>(player_));
            } else {
                using namespace yrui;

                auto ai_ctrl = new QtAIPlayer(f);

                connect(ai_ctrl, SIGNAL(aiThinking(QString)), screen_.get(), SLOT(onAIThinking(QString)),
                    Qt::QueuedConnection);

                ai_ctrl->install_ai(yrui::core()->make_fraction_ai());

                f->set_controller(yrui::uptr<yrui::Fraction_controller>(ai_ctrl));
            }
        }

        register_signals_();

        qDebug() << "Start running";
        std::thread([] { yrui::core()->run(); }).detach();
    }

    void Byapp::endTurn() { player_->notify_self(); }

    void Byapp::saveGame() { }

    void Byapp::loadGame() { }

    void Byapp::register_signals_()
    {
        using yrui::Legion;
        using yrui::Person;
        using yrui::Architecture;
        using yrui::Fraction;
        using yrui::Intruded_arch;

        qRegisterMetaType<yrui::Legion*>();
        qRegisterMetaType<yrui::Person*>();
        qRegisterMetaType<yrui::Architecture*>();
        qRegisterMetaType<yrui::Fraction*>();
        qRegisterMetaType<yrui::Intruded_arch*>();

        auto core = yrui::core();

        auto& person_mgr = core->use_subsys<yrui::person::Person_mgr>();
        auto& frac_mgr = core->use_subsys<yrui::fraction::Fraction_mgr>();
        auto& mili_mgr = core->use_subsys<yrui::military::Military_mgr>();

        frac_mgr.arch_obtained.connect(yrui::sig_level::external, [this](yrui::Fraction* f, yrui::Architecture* arch) {
            QMetaObject::invokeMethod(
                screen_->main(), "changeCityColor", Qt::BlockingQueuedConnection, Q_ARG(yrui::Architecture*, arch));
        });

        frac_mgr.fraction_died.connect(yrui::sig_level::external, [this](yrui::Fraction* f) {
            QMetaObject::invokeMethod(this, "reportEvent", Qt::BlockingQueuedConnection, Q_ARG(QString, "die"),
                Q_ARG(QString, QString("%1 灭亡了").arg(to_name(f))));

            for (auto a : f->archs()) {
                QMetaObject::invokeMethod(
                    screen_->main(), "changeCityColor", Qt::BlockingQueuedConnection, Q_ARG(yrui::Architecture*, a));
            }

            if (f == this->fraction_) {
                yrui::core()->stop();
            }
        });

        frac_mgr.nation_unified.connect(yrui::sig_level::observer, [this](yrui::Fraction* f) {
            QMetaObject::invokeMethod(this, "reportEvent", Qt::BlockingQueuedConnection, Q_ARG(QString, "unify"),
                Q_ARG(QString, QString("%1 统一了全国！").arg(to_name(f))));
        });

        mili_mgr.arch_intruded.connect(yrui::sig_level::external, [this](yrui::Architecture* a, yrui::Legion* l) {
            if (a->fraction() == this->fraction_) {
                QMetaObject::invokeMethod(this, "reportEvent", Qt::BlockingQueuedConnection,
                    Q_ARG(QString, "enemy_detected"),
                    Q_ARG(QString, QString("%1 城外发现来自%2 的敌军").arg(to_name(a)).arg(to_name(l->fraction()))));
            }
        });

        mili_mgr.become_strategy_troop.connect(yrui::sig_level::external, [this](yrui::Person* p) {
            if (p->fraction() != screen_->battleView()->attacker()
                && p->fraction() != screen_->battleView()->defensor()) {
                return;
            }

            QMetaObject::invokeMethod(
                screen_->battleView(), "becomeStrategyTroop", Qt::BlockingQueuedConnection, Q_ARG(yrui::Person*, p));
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });

        mili_mgr.strategy_prepare.connect(yrui::sig_level::external, [this](yrui::Person* p) {
            if (p->fraction() != screen_->battleView()->attacker()
                && p->fraction() != screen_->battleView()->defensor()) {
                return;
            }

            QMetaObject::invokeMethod(
                screen_->battleView(), "tryStrategy", Qt::BlockingQueuedConnection, Q_ARG(yrui::Person*, p));
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });

        mili_mgr.strategy_attack.connect(
            yrui::sig_level::external, [this](yrui::Person* attacker, yrui::Strategy* s) {
                if (attacker->fraction() != screen_->battleView()->attacker()
                    && attacker->fraction() != screen_->battleView()->defensor()) {
                    return;
                }

                QMetaObject::invokeMethod(screen_->battleView(), "strategyAttack", Qt::BlockingQueuedConnection,
                    Q_ARG(yrui::Person*, attacker), Q_ARG(QString, s->name().c_str()));
                std::this_thread::sleep_for(std::chrono::seconds(1));
            });

        mili_mgr.strategy_detected.connect(
            yrui::sig_level::external, [this](yrui::Person* attacker, yrui::Person* defensor) {
                if (attacker->fraction() != this->fraction_ && defensor->fraction() != this->fraction_) {
                    return;
                }

                QMetaObject::invokeMethod(screen_->battleView(), "strategyFailed", Qt::BlockingQueuedConnection,
                    Q_ARG(yrui::Person*, attacker), Q_ARG(yrui::Person*, defensor));
                std::this_thread::sleep_for(std::chrono::seconds(1));
            });

        mili_mgr.strategy_conquer.connect(
            yrui::sig_level::external, [this](yrui::Person* attacker, yrui::Person* defensor) {
                if (attacker->fraction() != this->fraction_ && defensor->fraction() != this->fraction_) {
                    return;
                }

                QMetaObject::invokeMethod(screen_->battleView(), "strategyDefeat", Qt::BlockingQueuedConnection,
                    Q_ARG(yrui::Person*, attacker), Q_ARG(yrui::Person*, defensor));
                std::this_thread::sleep_for(std::chrono::seconds(1));
            });

        mili_mgr.battle_start.connect(
            yrui::sig_level::external, [this](yrui::Legion* attacker, yrui::Legion* defensor) {
                if (attacker->fraction() != this->fraction_ && defensor->fraction() != this->fraction_) {
                    return;
                }

                QMetaObject::invokeMethod(screen_->battleView(), "startWar", Qt::BlockingQueuedConnection,
                    Q_ARG(yrui::Legion*, attacker), Q_ARG(yrui::Legion*, defensor));
            });

        mili_mgr.battle_end.connect(yrui::sig_level::external, [this](yrui::Legion* winner, yrui::Legion* loser) {
            if (winner->fraction() != this->fraction_ && loser->fraction() != this->fraction_) {
                return;
            }

            QMetaObject::invokeMethod(screen_->battleView(), "endWar", Qt::BlockingQueuedConnection,
                Q_ARG(yrui::Legion*, winner), Q_ARG(yrui::Legion*, loser));
        });

        mili_mgr.combat.connect(
            yrui::sig_level::external, [this](yrui::Person* attacker, yrui::Person* defensor) {
                if (attacker->fraction() != this->fraction_ && defensor->fraction() != this->fraction_) {
                    return;
                }

                QMetaObject::invokeMethod(screen_->battleView(), "combat", Qt::BlockingQueuedConnection,
                    Q_ARG(yrui::Person*, attacker), Q_ARG(yrui::Person*, defensor));
                std::this_thread::sleep_for(std::chrono::seconds(1));
            });

        mili_mgr.defeat.connect(yrui::sig_level::external, [this](yrui::Person* winner, yrui::Person* loser) {
            if (winner->fraction() != this->fraction_ && loser->fraction() != this->fraction_) {
                return;
            }

            QMetaObject::invokeMethod(screen_->battleView(), "defeat", Qt::BlockingQueuedConnection,
                Q_ARG(yrui::Person*, winner), Q_ARG(yrui::Person*, loser));
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });

        mili_mgr.siege_start.connect(
            yrui::sig_level::external, [this](Legion* attacker, Intruded_arch* defensor) {
                if (attacker->fraction() != this->fraction_ && defensor->fraction() != this->fraction_) {
                    return;
                }

                QMetaObject::invokeMethod(screen_->battleView(), "seigeStart", Qt::BlockingQueuedConnection,
                    Q_ARG(yrui::Legion*, attacker), Q_ARG(yrui::Intruded_arch*, defensor));
                std::this_thread::sleep_for(std::chrono::seconds(1));
            });

        mili_mgr.siege_end.connect(
            yrui::sig_level::external, [this](Legion* attacker, Intruded_arch* defensor) {
                if (attacker->fraction() != this->fraction_ && defensor->fraction() != this->fraction_) {
                    return;
                }

                QMetaObject::invokeMethod(screen_->battleView(), "seigeEnd", Qt::BlockingQueuedConnection,
                    Q_ARG(yrui::Legion*, attacker), Q_ARG(yrui::Intruded_arch*, defensor));
            });

        mili_mgr.siege_success.connect(
            yrui::sig_level::external, [this](Legion* attacker, Intruded_arch* defensor) {
                auto arch = defensor->arch();
                if (arch->steady() == 0) {
                    QMetaObject::invokeMethod(this, "reportEvent", Qt::BlockingQueuedConnection,
                        Q_ARG(QString, "seige"),
                        Q_ARG(QString,
                            QString("%1 被 %2 军攻入城池而陷落")
                                .arg(defensor->name().c_str())
                                .arg(attacker->name().c_str())));
                } else if (arch->food() == 0) {
                    QMetaObject::invokeMethod(this, "reportEvent", Qt::BlockingQueuedConnection,
                        Q_ARG(QString, "seige"),
                        Q_ARG(QString,
                            QString("%1 粮草耗尽而被 %2 军攻陷了")
                                .arg(defensor->name().c_str())
                                .arg(attacker->name().c_str())));
                } else {
                    QMetaObject::invokeMethod(this, "reportEvent", Qt::BlockingQueuedConnection,
                        Q_ARG(QString, "seige"),
                        Q_ARG(QString,
                            QString("%1 被 %2 军攻陷了").arg(defensor->name().c_str()).arg(attacker->name().c_str())));
                }
            });

        mili_mgr.siege_failed.connect(
            yrui::sig_level::external, [this](Legion* attacker, Intruded_arch* defensor) {
                if (attacker->fraction() != this->fraction_ && defensor->fraction() != this->fraction_) {
                    return;
                }

                QMetaObject::invokeMethod(screen_->battleView(), "seigeFailed", Qt::BlockingQueuedConnection,
                    Q_ARG(yrui::Legion*, attacker), Q_ARG(yrui::Intruded_arch*, defensor));

                QMetaObject::invokeMethod(this, "reportEvent", Qt::BlockingQueuedConnection,
                    Q_ARG(QString, "seigeFail"),
                    Q_ARG(QString,
                        QString("%1 从 %2 城下败退了").arg(attacker->name().c_str()).arg(defensor->name().c_str())));
            });

        mili_mgr.siege_wait.connect(
            yrui::sig_level::external, [this](Legion* attacker, Intruded_arch* defensor) {
                if (attacker->fraction() != this->fraction_ && defensor->fraction() != this->fraction_) {
                    return;
                }

                QMetaObject::invokeMethod(screen_->battleView(), "seigeWait", Qt::BlockingQueuedConnection,
                    Q_ARG(yrui::Legion*, attacker), Q_ARG(yrui::Intruded_arch*, defensor));
                std::this_thread::sleep_for(std::chrono::seconds(1));
            });

        mili_mgr.siege_attack.connect(
            yrui::sig_level::external, [this](Person* attacker, Intruded_arch* defensor) {
                if (attacker->fraction() != this->fraction_ && defensor->fraction() != this->fraction_) {
                    return;
                }

                QMetaObject::invokeMethod(screen_->battleView(), "seigeAttack", Qt::BlockingQueuedConnection,
                    Q_ARG(yrui::Person*, attacker), Q_ARG(yrui::Intruded_arch*, defensor));
                std::this_thread::sleep_for(std::chrono::seconds(1));
            });

        mili_mgr.siege_attack_success.connect(
            yrui::sig_level::external, [this](Person* attacker, Intruded_arch* defensor) {
                if (attacker->fraction() != this->fraction_ && defensor->fraction() != this->fraction_) {
                    return;
                }

                QMetaObject::invokeMethod(screen_->battleView(), "seigeAttackSuccess", Qt::BlockingQueuedConnection,
                    Q_ARG(yrui::Person*, attacker), Q_ARG(yrui::Intruded_arch*, defensor));

                QMetaObject::invokeMethod(this, "reportEvent", Qt::BlockingQueuedConnection,
                    Q_ARG(QString, "seigeAttackSuccess"),
                    Q_ARG(QString,
                        QString("%1 攻陷了 %2 城").arg(attacker->name().c_str()).arg(defensor->name().c_str())));
            });

        mili_mgr.siege_attack_failed.connect(
            yrui::sig_level::external, [this](Person* attacker, Intruded_arch* defensor) {
                if (attacker->fraction() != this->fraction_ && defensor->fraction() != this->fraction_) {
                    return;
                }

                QMetaObject::invokeMethod(screen_->battleView(), "seigeAttackFailed", Qt::BlockingQueuedConnection,
                    Q_ARG(yrui::Person*, attacker), Q_ARG(yrui::Intruded_arch*, defensor));
                std::this_thread::sleep_for(std::chrono::seconds(1));
            });

        ///////////////////////////////////////////////////////////////////////
        person_mgr.person_join_fraction.connect(
            yrui::sig_level::external, [this](yrui::Person* p, yrui::Fraction* f) {
                if (f == this->playerFraction()) {
                    QMetaObject::invokeMethod(this, "reportPerson", Qt::BlockingQueuedConnection,
                        Q_ARG(yrui::Person*, p),
                        Q_ARG(QString,
                            QString("%1: 我将在%2为君立下不世之功").arg(to_name(p)).arg(to_name(p->stayed_city()))));
                }
            });

        person_mgr.person_captured.connect(yrui::sig_level::external, [this](yrui::Person* p, yrui::Fraction* f) {
            if (f == this->playerFraction()) {
                QMetaObject::invokeMethod(this, "reportPerson", Qt::BlockingQueuedConnection,
                    Q_ARG(yrui::Person*, p), Q_ARG(QString, QString("%1被俘虏了").arg(to_name(p))));
            }
        });
        person_mgr.person_capitulated.connect(yrui::sig_level::external, [this](yrui::Person* p, yrui::Fraction* f) {
            if (f == this->playerFraction()) {
                QMetaObject::invokeMethod(this, "reportPerson", Qt::BlockingQueuedConnection,
                    Q_ARG(yrui::Person*, p), Q_ARG(QString, QString("%1投降我军了").arg(to_name(p))));
            }
        });
        person_mgr.person_killed.connect(yrui::sig_level::external, [this](yrui::Person* p, yrui::Fraction* f) {
            QMetaObject::invokeMethod(this, "reportPerson", Qt::BlockingQueuedConnection, Q_ARG(yrui::Person*, p),
                Q_ARG(QString, QString("%1被%2处斩了").arg(to_name(p)).arg(to_name(f))));
        });
        person_mgr.person_released.connect(yrui::sig_level::external, [this](yrui::Person* p, yrui::Fraction* f) {
            if (f == this->playerFraction()) {
                QMetaObject::invokeMethod(this, "reportPerson", Qt::BlockingQueuedConnection,
                    Q_ARG(yrui::Person*, p), Q_ARG(QString, QString("%1被%2释放").arg(to_name(p)).arg(to_name(f))));
            }
        });
    }

    void Byapp::reportInfo(const QString& info) { screen_->reportInfo(info); }

    void Byapp::reportAll(const QString& title, const QString& info) { screen_->reportAll(title, info); }

    void Byapp::reportEvent(const QString& event, const QString& info) { screen_->reportEvent(event, info); }

    void Byapp::reportPerson(yrui::Person* p, const QString& info) { screen_->reportPerson(p, info); }
}
}

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
