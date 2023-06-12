#ifndef YRUI_CORE_AI_ARCHITECTURE_COMMON_H_
#define YRUI_CORE_AI_ARCHITECTURE_COMMON_H_

#include "kernel/arch/architecture.h"
#include "kernel/core/ai_architecture.h"
#include "kernel/person/person.h"

namespace yrui {
namespace ai {
    namespace detail {
        struct Job_base {
            Job_base(Architecture* arch)
                : arch_(arch)
            {
            }

        protected:
            ~Job_base() = default;

            Architecture* arch_;
        };

        struct Do_farm : Job_base {
            using Job_base::Job_base;

            void operator()(Person* p)
            {
                BOOST_ASSERT(!arch_->has_done_farm());
                arch_->develop_farming(p);
            }
        };

        struct Do_trade : Job_base {
            using Job_base::Job_base;

            void operator()(Person* p)
            {
                BOOST_ASSERT(!arch_->has_done_trade());
                arch_->develop_trade(p);
            }
        };

        struct Do_security : Job_base {
            using Job_base::Job_base;

            void operator()(Person* p)
            {
                BOOST_ASSERT(!arch_->has_done_security());
                arch_->develop_security(p);
            }
        };

        struct Do_steady : Job_base {
            using Job_base::Job_base;

            void operator()(Person* p)
            {
                BOOST_ASSERT(!arch_->has_done_steady());
                arch_->develop_steady(p);
            }
        };
    } // of namespace detail

    namespace detail {
        class Arch_ai_common : public Iarchitecture_ai {
        protected:
            Architecture* arch_;
            Person* mayor_;
            std::vector<Person*> wen_;
            std::vector<Person*> wu_;

        public:
            virtual void think(const Timeline& tl) final override;
            virtual void take_control_of(Architecture* arch) final override;

        protected:
            virtual void work_imp(const Timeline& tl) = 0;

        protected:
            virtual void work_wen() = 0;
            virtual void work_wu() = 0;
        };
    } // of namespace detail
} // of namespace ai
} // of namespace yrui

#endif // YRUI_CORE_AI_ARCHITECTURE_COMMON_H_
