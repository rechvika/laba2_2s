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

class Application {
public:
    void Run() {
        InitData();
        
        auto screen = ScreenInteractive::TerminalOutput();

        std::vector<std::string> type_labels = {"ArraySequence", "ListSequence", "BitSequence"};
        int selected_int = 0;
        auto type_selector = Radiobox(&type_labels, &selected_int);

        type_selector |= CatchEvent([&](Event event) {
            selected_type_ = selected_int;
            return false;
        });

        auto input_value = Input(&value_str_, "Значение");
        auto input_index = Input(&index_str_, "Индекс (1..N)");

        auto info_panel = Renderer([&]() {
            return window(
                text("Информация") | bold | center,
                vbox({
                    text("Тип: " + GetTypeName()) | color(Color::Cyan),
                    text("Размер: " + std::to_string(GetLength())) | color(Color::Cyan),
                    text("Содержимое: " + GetContents()) | color(Color::Cyan),
                    separator(),
                    text("Статус: " + status_) | color(status_.find("Ошибка") != std::string::npos ? Color::Red : Color::Yellow),
                })
            );
        });

        auto btn_append = Button("Append", [this]() { OnAppend(); });
        auto btn_prepend = Button("Prepend", [this]() { OnPrepend(); });
        auto btn_insert = Button("InsertAt", [this]() { OnInsert(); });
        auto btn_map = Button("Map (x^2/NOT)", [this]() { OnMap(); });
        auto btn_where = Button("Where (even/1)", [this]() { OnWhere(); });
        auto btn_reduce = Button("Reduce (sum)", [this]() { OnReduce(); });
        auto btn_clear = Button("Reset", [this]() { OnClear(); });
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
    }

private:
    std::unique_ptr<MutableArraySequence<size_t>> array_seq_;
    std::unique_ptr<MutableListSequence<size_t>> list_seq_;
    std::unique_ptr<BitSequence> bit_seq_;
    
    size_t selected_type_ = 0;
    std::string value_str_ = "0";
    std::string index_str_ = "1";
    std::string status_ = "Готов";

    size_t GetValue() {
        if (value_str_.empty()) {
            throw InvalidArgument("Ошибка, строка значения пуста");
        }
        try {
            size_t value = std::stoi(value_str_);
            return value;
        } catch (const std::invalid_argument& e) {
            throw InvalidArgument("Ошибка, значение должно быть числом: " + value_str_);
        } catch (const std::out_of_range& e) {
            throw InvalidArgument("Ошибка, значение выходит за пределы допустимого диапазона: " + value_str_);
        }
    }

    size_t GetIndex() {
        if (index_str_.empty()) {
            throw InvalidArgument("Ошибка, строка индекса пуста");
        }
        try {
            size_t index = std::stoi(index_str_);
            if (index == 0) {
                throw InvalidArgument("Ошибка, индекс должен быть больше 0");
            }
            return index - 1;
        } catch (const std::invalid_argument& e) {
            throw InvalidArgument("Ошибка, индекс должен быть числом: " + index_str_);
        } catch (const std::out_of_range& e) {
            throw InvalidArgument("Ошибка, индекс выходит за пределы допустимого диапазона: " + index_str_);
        }
    }

    std::string GetTypeName() {
        switch (selected_type_) {
            case 0: return "ArraySequence";
            case 1: return "ListSequence";
            default: return "BitSequence";
        }
    }

    size_t GetLength() {
        switch (selected_type_) {
            case 0: return array_seq_->GetLength();
            case 1: return list_seq_->GetLength();
            default: return bit_seq_->GetLength();
        }
    }

    std::string GetContents() {
        std::string result;
        size_t len = GetLength();
        size_t max_show = 20;
        
        if (len == 0) return "пусто";
        
        size_t count = 0;
        
        switch (selected_type_) {
            case 0: {
                auto enumerator = array_seq_->GetEnumerator();
                while (enumerator->MoveNext() && count < max_show) {
                    if (count > 0) result += ", ";
                    result += std::to_string(enumerator->Current());
                    ++count;
                }
                break;
            }
            case 1: {
                auto enumerator = list_seq_->GetEnumerator();
                while (enumerator->MoveNext() && count < max_show) {
                    if (count > 0) result += ", ";
                    result += std::to_string(enumerator->Current());
                    ++count;
                }
                break;
            }
            case 2: {
                auto enumerator = bit_seq_->GetEnumerator();
                while (enumerator->MoveNext() && count < max_show) {
                    if (count > 0) result += ", ";
                    result += enumerator->Current().ToChar();
                    ++count;
                }
                break;
            }
        }
        if (len > max_show) result += "...";
        return result;
    }

    void UpdateStatus(const std::string& msg) {
        status_ = msg;
    }

    void OnAppend() {
        try {
            size_t val = GetValue();
            switch (selected_type_) {
                case 0: array_seq_->Append(val); break;
                case 1: list_seq_->Append(val); break;
                case 2: bit_seq_->Append(Bit(val != 0)); break;
            }
            UpdateStatus("Добавлен элемент: " + std::to_string(val));
        } catch (const std::exception& e) {
            UpdateStatus("Ошибка: " + std::string(e.what()));
        }
    }

    void OnPrepend() {
        try {
            size_t val = GetValue();
            switch (selected_type_) {
                case 0: array_seq_->Prepend(val); break;
                case 1: list_seq_->Prepend(val); break;
                case 2: bit_seq_->Prepend(Bit(val != 0)); break;
            }
            UpdateStatus("Добавлен в начало: " + std::to_string(val));
        } catch (const std::exception& e) {
            UpdateStatus("Ошибка: " + std::string(e.what()));
        }
    }

    void OnInsert() {
        try {
            size_t val = GetValue();
            size_t index = GetIndex();
            size_t len = GetLength();
            if (index > len) {
                UpdateStatus("Ошибка, индекс " + std::to_string(index + 1) + 
                            " больше допустимого максимального значения " + std::to_string(len));
                return;
            }
            
            switch (selected_type_) {
                case 0: array_seq_->InsertAt(val, index); break;
                case 1: list_seq_->InsertAt(val, index); break;
                case 2: bit_seq_->InsertAt(Bit(val != 0), index); break;
            }
            UpdateStatus("Вставлен на позицию " + std::to_string(index + 1) + ": " + std::to_string(val));
        } catch (const std::exception& e) {
            UpdateStatus("Ошибка: " + std::string(e.what()));
        }
    }

    void OnMap() {
        try {
            switch (selected_type_) {
                case 0: {
                    std::function<size_t(const size_t&)> mapper = [](const size_t& x) { return x * x; };
                    Sequence<size_t>* mapped = Map(*array_seq_, mapper);
                    array_seq_.reset(dynamic_cast<MutableArraySequence<size_t>*>(mapped));
                    UpdateStatus("Применено отображение: x -> x^2");
                    break;
                }
                case 1: {
                    std::function<size_t(const size_t&)> mapper = [](const size_t& x) { return x * x; };
                    Sequence<size_t>* mapped = Map(*list_seq_, mapper);
                    list_seq_.reset(dynamic_cast<MutableListSequence<size_t>*>(mapped));
                    UpdateStatus("Применено отображение: x -> x^2");
                    break;
                }
                case 2: {
                    auto new_seq = std::make_unique<BitSequence>();
                    auto enumerator = bit_seq_->GetEnumerator();
                    while (enumerator->MoveNext()) {
                        new_seq->Append(~enumerator->Current());
                    }
                    bit_seq_ = std::move(new_seq);
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
            switch (selected_type_) {
                case 0: {
                    Sequence<size_t>* filtered = array_seq_->Where([](const size_t& x) { return x % 2 == 0; });
                    array_seq_.reset(dynamic_cast<MutableArraySequence<size_t>*>(filtered));
                    UpdateStatus("Отфильтрованы четные числа");
                    break;
                }
                case 1: {
                    Sequence<size_t>* filtered = list_seq_->Where([](const size_t& x) { return x % 2 == 0; });
                    list_seq_.reset(dynamic_cast<MutableListSequence<size_t>*>(filtered));
                    UpdateStatus("Отфильтрованы четные числа");
                    break;
                }
                case 2: {
                    Sequence<Bit>* filtered = bit_seq_->Where([](const Bit& b) { return b.Value() == 1; });
                    bit_seq_.reset(dynamic_cast<BitSequence*>(filtered));
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
            switch (selected_type_) {
                case 0: {
                    std::function<size_t(const size_t&, const size_t&)> reducer = 
                        [](const size_t& acc, const size_t& x) { return acc + x; };
                    size_t sum = array_seq_->Reduce<size_t>(0ULL, reducer);
                    ss << "Сумма = " << sum;
                    break;
                }
                case 1: {
                    std::function<size_t(const size_t&, const size_t&)> reducer = 
                        [](const size_t& acc, const size_t& x) { return acc + x; };
                    size_t sum = list_seq_->Reduce<size_t>(0ULL, reducer);
                    ss << "Сумма = " << sum;
                    break;
                }
                case 2: {
                    size_t ones = 0;
                    auto enumerator = bit_seq_->GetEnumerator();
                    while (enumerator->MoveNext()) {
                        if (enumerator->Current().Value()) ones++;
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
        switch (selected_type_) {
            case 0:
                array_seq_ = std::make_unique<MutableArraySequence<size_t>>();
                for (size_t i = 1; i <= 5; ++i) array_seq_->Append(i);
                break;
            case 1:
                list_seq_ = std::make_unique<MutableListSequence<size_t>>();
                for (size_t i = 1; i <= 5; ++i) list_seq_->Append(i * 10);
                break;
            case 2:
                bit_seq_ = std::make_unique<BitSequence>();
                bit_seq_->Append(Bit(1));
                bit_seq_->Append(Bit(0));
                bit_seq_->Append(Bit(1));
                bit_seq_->Append(Bit(1));
                bit_seq_->Append(Bit(0));
                break;
        }
        UpdateStatus("Последовательность сброшена в начальное состояние");
    }

    void InitData() {
        array_seq_ = std::make_unique<MutableArraySequence<size_t>>();
        list_seq_ = std::make_unique<MutableListSequence<size_t>>();
        bit_seq_ = std::make_unique<BitSequence>();
        
        for (size_t i = 1; i <= 5; ++i) {
            array_seq_->Append(i);
            list_seq_->Append(i * 10);
        }
        bit_seq_->Append(Bit(1));
        bit_seq_->Append(Bit(0));
        bit_seq_->Append(Bit(1));
        bit_seq_->Append(Bit(1));
        bit_seq_->Append(Bit(0));
    }
};

int main() {
    Application app;
    app.Run();
    return 0;
}