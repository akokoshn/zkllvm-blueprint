//---------------------------------------------------------------------------//
// Copyright (c) 2021 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2021 Nikita Kaskov <nbering@nil.foundation>
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//---------------------------------------------------------------------------//
// @file Declaration of interfaces for auxiliary components for the SHA256 component.
//---------------------------------------------------------------------------//

#ifndef CRYPTO3_ZK_BLUEPRINT_PLONK_CURVE_ELEMENT_TRIPLING_COMPONENT_HPP
#define CRYPTO3_ZK_BLUEPRINT_PLONK_CURVE_ELEMENT_TRIPLING_COMPONENT_HPP

#include <nil/crypto3/zk/components/blueprint.hpp>
#include <nil/crypto3/zk/components/algebra/curves/plonk/doubling.hpp>
#include <nil/crypto3/zk/components/algebra/curves/plonk/addition.hpp>

namespace nil {
    namespace crypto3 {
        namespace zk {
            namespace components {

                template<typename TBlueprintField, typename CurveType, 
                    std::size_t W0 = 0, std::size_t W1 = 1, std::size_t W2 = 2, std::size_t W3 = 3, 
                    std::size_t W4 = 4, std::size_t W5 = 5, std::size_t W6 = 6, std::size_t W7 = 7>
                class element_g1_tripling_plonk : public component<TBlueprintField> {
                    typedef snark::plonk_constraint_system<TBlueprintField> arithmetization_type;

                    typedef blueprint<arithmetization_type, TBlueprintField> blueprint_type;

                    element_g1_doubling_plonk<TBlueprintField, CurveType, W0, W1, W2, W3, W6> doubling_component;
                    element_g1_addition_plonk<TBlueprintField, CurveType, W0, W1, W2, W3, W4, W5, W7> addition_component;
                public:

                    element_g1_tripling_plonk(blueprint_type &bp) :
                        component<FieldType>(bp), doubling_component(bp), 
                        addition_component(bp) {
                    }

                    void generate_gates() {
                        doubling_component.generate_gates();
                        addition_component.generate_gates();
                    }

                    void generate_assignments(typename CurveType::value_type &P1) {
                        generate_assignments(P1, P1.doubled() + P1);
                    }

                    void generate_assignments(typename CurveType::value_type &P1, 
                                               typename CurveType::value_type &P2) {
                        doubling_component.generate_assignments(P1, P1.doubled());
                        addition_component.generate_assignments(P1.doubled(), P1, P2);
                    }
                };

            }    // namespace components
        }        // namespace zk
    }            // namespace crypto3
}    // namespace nil

#endif    // CRYPTO3_ZK_BLUEPRINT_PLONK_CURVE_ELEMENT_TRIPLING_COMPONENT_HPP
