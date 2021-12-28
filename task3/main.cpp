#include "bigint.h"

int main(){
    // 129 should trigger assertion
    Storage<129> s("16");
    Storage<129> n(23);
    return 0;
}