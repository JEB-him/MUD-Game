#pragma once
#include"backpack.h"

Backpack::Backpack():item_creator("Item.json"), backpack_items(10){ }

vector<unique_ptr<Item>>& Backpack::getBackpack‌Items() {
    return backpack_items;
}

void Backpack::addItem(string item_name) {
    backpack_items.push_back(item_creator.createItem(item_name));
}
 
void Backpack::useFunctionOfItem(int order, Protagonist& protagonist) {
    if (order > backpack_items.size()) {
        /* TODO 抛出异常*/
    }
    else{
        int index = order - 1;
            if (backpack_items[index].get()->getIsConsumable()) {
                backpack_items[index].get()->use(protagonist);
                backpack_items.erase(backpack_items.begin() + index);
            }
            else {
                backpack_items[index].get()->equipAndUnequip(protagonist);
            }
    }
}