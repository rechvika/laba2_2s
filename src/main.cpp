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

std::unique_ptr<MutableArraySequence<size_t>> g_array_seq;
std::unique_ptr<MutableListSequence<size_t>> g_list_seq;
std::unique_ptr<BitSequence> g_bit_seq;

size_t g_selected_type = 0;
std::string g_value_str = "0";
std::string g_index_str = "1";
std::string g_status = "Готов";
size_t g_selected_button = 0;

size_t GetValue() {
    if (g_value_str.empty()) return 0;
    return std::stoi(g_value_str);
}

size_t GetIndex() {
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

size_t GetLength() {
    switch (g_selected_type) {
        case 0: return g_array_seq->GetLength();
        case 1: return g_list_seq->GetLength();
        default: return g_bit_seq->GetLength();
    }
}

std::string GetContents() {
    std::string result;
    size_t len = GetLength();
    size_t max_show = 20;
    
    if (len == 0) return "пусто";
    
    switch (g_selected_type) {
        case 0:
            for (size_t i = 0; i < len && i < max_show; ++i) {
                if (i > 0) result += ", ";
                result += std::to_string(g_array_seq->Get(i));
            }
            break;
        case 1:
            for (size_t i = 0; i < len && i < max_show; ++i) {
                if (i > 0) result += ", ";
                result += std::to_string(g_list_seq->Get(i));
            }
            break;
        case 2:
            for (size_t i = 0; i < len && i < max_show; ++i) {
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
        size_t val = GetValue();
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
        size_t val = GetValue();
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
        size_t val = GetValue();
        size_t idx = GetIndex();
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
                std::function<size_t(const size_t&)> mapper = [](const size_t& x) { return x * x; };
                Sequence<size_t>* mapped = Map(*g_array_seq, mapper);
                g_array_seq.reset(dynamic_cast<MutableArraySequence<size_t>*>(mapped));
                UpdateStatus("Применено отображение: x -> x²");
                break;
            }
            case 1: {
                std::function<size_t(const size_t&)> mapper = [](const size_t& x) { return x * x; };
                Sequence<size_t>* mapped = Map(*g_list_seq, mapper);
                g_list_seq.reset(dynamic_cast<MutableListSequence<size_t>*>(mapped));
                UpdateStatus("Применено отображение: x -> x²");
                break;
            }
            case 2: {
                auto new_seq = std::make_unique<BitSequence>();
                for (size_t i = 0; i < g_bit_seq->GetLength(); ++i) {
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
                Sequence<size_t>* filtered = g_array_seq->Where([](const size_t& x) { return x % 2 == 0; });
                g_array_seq.reset(dynamic_cast<MutableArraySequence<size_t>*>(filtered));
                UpdateStatus("Отфильтрованы четные числа");
                break;
            }
            case 1: {
                Sequence<size_t>* filtered = g_list_seq->Where([](const size_t& x) { return x % 2 == 0; });
                g_list_seq.reset(dynamic_cast<MutableListSequence<size_t>*>(filtered));
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

void OnReduce() {
    std::stringstream ss;
    try {
        switch (g_selected_type) {
            case 0: {
                std::function<size_t(const size_t&, const size_t&)> reducer = 
                    [](const size_t& acc, const size_t& x) { return acc + x; };
                size_t sum = g_array_seq->Reduce(0, reducer);
                ss << "Сумма = " << sum;
                break;
            }
            case 1: {
                std::function<size_t(const size_t&, const size_t&)> reducer = 
                    [](const size_t& acc, const size_t& x) { return acc + x; };
                size_t sum = g_list_seq->Reduce(0, reducer);
                ss << "Сумма = " << sum;
                break;
            }
            case 2: {
                size_t ones = 0;
                for (size_t i = 0; i < g_bit_seq->GetLength(); ++i) {
                    if (g_bit_seq->Get(i).Value()) ones++;
                }
                ss << "Единиц = " << ones;
                break;
            }
        }
        UpdateStatus(ss.str());
    } catch (const std::exception& e) {
        UpdateStatus("Ошибка: " + std::string(e.what()));
    }
}

void OnClear() {
    switch (g_selected_type) {
        case 0:
            g_array_seq = std::make_unique<MutableArraySequence<size_t>>();
            for (size_t i = 1; i <= 5; ++i) g_array_seq->Append(i);
            break;
        case 1:
            g_list_seq = std::make_unique<MutableListSequence<size_t>>();
            for (size_t i = 1; i <= 5; ++i) g_list_seq->Append(i * 10);
            break;
        case 2:
            g_bit_seq = std::make_unique<BitSequence>();
            g_bit_seq->Append(Bit(1));
            g_bit_seq->Append(Bit(0));
            g_bit_seq->Append(Bit(1));
            g_bit_seq->Append(Bit(1));
            g_bit_seq->Append(Bit(0));
            break;
    }
    UpdateStatus("Последовательность сброшена в начальное состояние");
}

void InitData() {
    g_array_seq = std::make_unique<MutableArraySequence<size_t>>();
    g_list_seq = std::make_unique<MutableListSequence<size_t>>();
    g_bit_seq = std::make_unique<BitSequence>();
    
    for (size_t i = 1; i <= 5; ++i) {
        g_array_seq->Append(i);
        g_list_seq->Append(i * 10);
    }
    g_bit_seq->Append(Bit(1));
    g_bit_seq->Append(Bit(0));
    g_bit_seq->Append(Bit(1));
    g_bit_seq->Append(Bit(1));
    g_bit_seq->Append(Bit(0));
}
int main() {
    InitData();
    
    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> type_labels = {"ArraySequence", "ListSequence", "BitSequence"};
    size_t type_selected = 0;
    
    auto type_selector = Radiobox(&type_labels, &type_selected);

    auto input_value = Input(&g_value_str, "Значение");
    auto input_index = Input(&g_index_str, "Индекс (1..N)");

    auto info_panel = Renderer([&] {
        g_selected_type = type_selected;
        return window(
            text("Информация") | bold | center,
            vbox({
                text("Тип: " + GetTypeName()) | color(Color::Cyan),
                text("Размер: " + std::to_string(GetLength())) | color(Color::Cyan),
                text("Содержимое: " + GetContents()) | color(Color::Cyan),
                separator(),
                text("Статус: " + g_status) | color(g_status.find("Ошибка") != std::string::npos ? Color::Red : Color::Yellow),
            })
        );
    });

    auto btn_append = Button("Append", OnAppend);
    auto btn_prepend = Button("Prepend", OnPrepend);
    auto btn_insert = Button("InsertAt", OnInsert);
    auto btn_map = Button("Map (x²/NOT)", OnMap);
    auto btn_where = Button("Where (even/1)", OnWhere);
    auto btn_reduce = Button("Reduce (sum)", OnReduce);
    auto btn_clear = Button("Reset", OnClear);
    auto btn_exit = Button("Exit", screen.ExitLoopClosure());
    
    auto buttons_row1 = Container::Horizontal({btn_append, btn_prepend, btn_insert});
    auto buttons_row2 = Container::Horizontal({btn_map, btn_where, btn_reduce});
    auto buttons_row3 = Container::Horizontal({btn_clear, btn_exit});
    auto buttons = Container::Vertical({buttons_row1, buttons_row2, buttons_row3});

    auto layout = Container::Vertical({
        type_selector,
        input_value,
        input_index,
        info_panel,
        buttons,
    });

    auto component = Renderer(layout, [&] {
        return vbox({
            text("=== УПРАВЛЕНИЕ ПОСЛЕДОВАТЕЛЬНОСТЯМИ ===") | bold | center | color(Color::Green),
            separator(),
            text("Тип последовательности:") | bold,
            layout->Render(),
        }) | border;
    });
    
    screen.Loop(component);
    
    return 0;
}