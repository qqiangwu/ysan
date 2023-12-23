#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "kernel/component_fwd.h"
#include <QObject>

namespace Bygone {
namespace Frontend {
    class Byapp;
    class MapView;
    class SelectDialog;

    //! 所有操作都会最终转发到这个类来
    class GameLogic : public QObject {
        Q_OBJECT

    public:
        explicit GameLogic(Byapp* app);

    public slots:
        //! affairs
        //! @{
        void onFarm();
        void onTrade();
        void onSteady();
        void onSecurity();
        void onTransport();
        //! @}

        //! personel
        //! @{
        void onSearch();
        void onMayor();
        void onMove();
        void onPromote();
        //! @}

        //! military
        //! @{
        void onCenteralize();
        void onConscript();
        void onTrain();
        void onReplenish();
        void onWar();
        void onDismiss();

        //! @}

        //! info
        //! @{
        void onJob();
        void onPerson();
        void onCity();
        void onFraction();
        void onAllPerson();
        void onAllCity();
        //! @}

        //! cheat
        //! @{
        void onCheatUnOfficial();
        void onCheatAllPerson();
        void onCheatAllArch();
        //! @}

        void onMoveCapital();

    private:
        SelectDialog* dialog_();
        yrui::Architecture* currentCity_();

    private:
        //! 行动力检查
        class Action_power_checker {
        public:
            explicit Action_power_checker(Byapp* app)
                : app_(app)
            {
            }

            ~Action_power_checker();

            bool is_ok() const { return app_ != nullptr; }
            bool is_commited() const { return commited_; }

            void commit()
            {
                BOOST_ASSERT(this->is_ok());
                commited_ = true;
            }

        private:
            Byapp* app_;
            bool commited_ = false;
        };

        Action_power_checker check_ap_();

    private:
        Byapp* app_;
    };
}
}

#endif // GAMELOGIC_H
