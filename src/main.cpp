#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <string>
#include <sstream>
#include <memory>
#include <functional>

#include "array_sequence.hpp"
#include "list_sequence.hpp"
#include "bit_sequence.hpp"
#include "map.hpp"

using namespace lab2;
using namespace ftxui;

std::unique_ptr<MutableArraySequence<int>> g_array_seq;
std::unique_ptr<MutableListSequence<int>> g_list_seq;
std::unique_ptr<BitSequence> g_bit_seq;

int g_selected_type = 0;
std::string g_value_str = "0";
std::string g_index_str = "1";
std::string g_status = "Готов";
int g_selected_button = 0;

int GetValue() {
    if (g_value_str.empty()) return 0;
    return std::stoi(g_value_str);
}

int GetIndex() {
    if (g_index_str.empty()) return 0;
    return std::stoi(g_index_str) - 1;
}

std::string GetTypeName() {
    switch (g_selected_type) {
        case 0: return "ArraySequence";
        case 1: return "ListSequence";
        default: return "BitSequence";
    }
}

int GetLength() {
    switch (g_selected_type) {
        case 0: return g_array_seq->GetLength();
        case 1: return g_list_seq->GetLength();
        default: return g_bit_seq->GetLength();
    }
}

std::string GetContents() {
    std::string result;
    int len = GetLength();
    int max_show = 20;
    
    if (len == 0) return "пусто";
    
    switch (g_selected_type) {
        case 0:
            for (int i = 0; i < len && i < max_show; ++i) {
                if (i > 0) result += ", ";
                result += std::to_string(g_array_seq->Get(i));
            }
            break;
        case 1:
            for (int i = 0; i < len && i < max_show; ++i) {
                if (i > 0) result += ", ";
                result += std::to_string(g_list_seq->Get(i));
            }
            break;
        case 2:
            for (int i = 0; i < len && i < max_show; ++i) {
                if (i > 0) result += ", ";
                result += g_bit_seq->Get(i).ToChar();
            }
            break;
    }
    
    if (len > max_show) result += "...";
    return result;
}

void UpdateStatus(const std::string& msg) {
    g_status = msg;
}

void OnAppend() {
    try {
        int val = GetValue();
        switch (g_selected_type) {
            case 0: g_array_seq->Append(val); break;
            case 1: g_list_seq->Append(val); break;
            case 2: g_bit_seq->Append(Bit(val != 0)); break;
        }
        UpdateStatus("Добавлен элемент: " + std::to_string(val));
    } catch (const std::exception& e) {
        UpdateStatus("Ошибка: " + std::string(e.what()));
    }
}

void OnPrepend() {
    try {
        int val = GetValue();
        switch (g_selected_type) {
            case 0: g_array_seq->Prepend(val); break;
            case 1: g_list_seq->Prepend(val); break;
            case 2: g_bit_seq->Prepend(Bit(val != 0)); break;
        }
        UpdateStatus("Добавлен в начало: " + std::to_string(val));
    } catch (const std::exception& e) {
        UpdateStatus("Ошибка: " + std::string(e.what()));
    }
}

void OnInsert() {
    try {
        int val = GetValue();
        int idx = GetIndex();
        if (idx < 0 || idx > GetLength()) {
            UpdateStatus("Ошибка: индекс вне диапазона");
            return;
        }
        
        switch (g_selected_type) {
            case 0: g_array_seq->InsertAt(val, idx); break;
            case 1: g_list_seq->InsertAt(val, idx); break;
            case 2: g_bit_seq->InsertAt(Bit(val != 0), idx); break;
        }
        UpdateStatus("Вставлен на позицию " + std::to_string(idx + 1) + ": " + std::to_string(val));
    } catch (const std::exception& e) {
        UpdateStatus("Ошибка: " + std::string(e.what()));
    }
}

void OnMap() {
    try {
        switch (g_selected_type) {
            case 0: {
                std::function<int(const int&)> mapper = [](const int& x) { return x * x; };
                Sequence<int>* mapped = Map(*g_array_seq, mapper);
                g_array_seq.reset(dynamic_cast<MutableArraySequence<int>*>(mapped));
                UpdateStatus("Применено отображение: x -> x²");
                break;
            }
            case 1: {
                std::function<int(const int&)> mapper = [](const int& x) { return x * x; };
                Sequence<int>* mapped = Map(*g_list_seq, mapper);
                g_list_seq.reset(dynamic_cast<MutableListSequence<int>*>(mapped));
                UpdateStatus("Применено отображение: x -> x²");
                break;
            }
            case 2: {
                auto new_seq = std::make_unique<BitSequence>();
                for (int i = 0; i < g_bit_seq->GetLength(); ++i) {
                    new_seq->Append(~g_bit_seq->Get(i));
                }
                g_bit_seq = std::move(new_seq);
                UpdateStatus("Применено отображение: NOT");
                break;
            }
        }
    } catch (const std::exception& e) {
        UpdateStatus("Ошибка: " + std::string(e.what()));
    }
}

void OnWhere() {
    try {
        switch (g_selected_type) {
            case 0: {
                Sequence<int>* filtered = g_array_seq->Where([](const int& x) { return x % 2 == 0; });
                g_array_seq.reset(dynamic_cast<MutableArraySequence<int>*>(filtered));
                UpdateStatus("Отфильтрованы четные числа");
                break;
            }
            case 1: {
                Sequence<int>* filtered = g_list_seq->Where([](const int& x) { return x % 2 == 0; });
                g_list_seq.reset(dynamic_cast<MutableListSequence<int>*>(filtered));
                UpdateStatus("Отфильтрованы четные числа");
                break;
            }
            case 2: {
                Sequence<Bit>* filtered = g_bit_seq->Where([](const Bit& b) { return b.Value() == 1; });
                g_bit_seq.reset(dynamic_cast<BitSequence*>(filtered));
                UpdateStatus("Оставлены только единицы");
                break;
            }
        }
    } catch (const std::exception& e) {
        UpdateStatus("Ошибка: " + std::string(e.what()));
    }
}