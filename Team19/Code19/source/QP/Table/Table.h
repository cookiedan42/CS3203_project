#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include "../../TypeAlias/TypeAlias.h"

class Table {
private:
    std::vector<std::string> header;
    std::vector<std::vector<std::string>> items;

public:
    Table() = default;
    Table(std::vector<std::string> header, std::vector<std::vector<std::string>> items) {
        this->header = header;
        this->items = items;
    }

    int tableSize();
    std::vector<std::string> getTableHeader();
    std::vector<std::vector<std::string>> getTableRows();
    std::pair<int, int> getFirstCommonKeys();

    /*
    Given 2 index of the COMMON header, drop the row if val is different.
    Required: index1 < this->tableSize() && index2 < this->tableSize()
    After which, drop 1 of the column.
    */
    Table filterTable(int index1, int index2);

};