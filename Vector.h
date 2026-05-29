#pragma once
/**
 * @file Vector.h
 * @brief Nuosavas dinaminis masyvas Vector<T> — std::vector atitikmuo.
 *
 * Realizuoja >80% std::vector sąsajos: konstruktorius, Rule of Five,
 * elementų prieigą, iteratorius, talpos valdymą ir modifikatorius.
 * Atmintis valdoma rankiniu būdu (::operator new / placement new),
 * kad teisingai veiktų ir su netrivialiais tipais (pvz. Studentas).
 */