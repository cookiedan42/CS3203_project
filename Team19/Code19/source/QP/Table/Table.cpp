#include "./Table.h"

int Table::tableSize() {
    return static_cast<int>(header.size());
}

std::vector<std::string> Table::getTableHeader() {
    return header;
}

std::vector<std::vector<std::string>> Table::getTableRows() {
    return items;
}

std::pair<int, int> Table::getFirstCommonKeys() {
    //Even though its a n^2 loop but it should be fast because #of variables are little
    for (int i = 0; i < static_cast<int>(this->header.size()); i++) {
        for (int j = i + 1; j < static_cast<int>(this->header.size()); j++) {
            if (header[i] == header[j]) {
                return std::pair<int, int>(i, j);
            }
        }
    }
    return std::pair<int, int>();
}


/*
Given 2 index of the COMMON header, drop the row if val is different.
Required: index1 < this->tableSize() && index2 < this->tableSize()
After which, drop 1 of the column.
*/
Table Table::filterTable(int index1, int index2) {
    //Remove col at index2
    std::vector<std::string> newHeader(this->header);
    newHeader.erase(newHeader.begin() + index2);

    std::vector<std::vector<std::string>> newItems;

    for (auto item : this->items) {
        if (item[index1] == item[index2]) {
            item.erase(item.begin() + index2);
            newItems.push_back(item);
        }
    }

    return Table(newHeader, newItems);
}