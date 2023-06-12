#include "kernel/impl/serialization/parser.h"

namespace P = yrui::serialization;

template P::attr_t<P::Person_parser> P::read_db<>(const std::string&, const P::Person_parser&);

template P::attr_t<P::Fraction_parser> P::read_db<>(const std::string&, const P::Fraction_parser&);

template P::attr_t<P::Architecture_parser> P::read_db<>(const std::string&, const P::Architecture_parser&);
