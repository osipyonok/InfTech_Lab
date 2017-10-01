#include "test_datatypes.h"

Test_datatypes::Test_datatypes()
{
    vector<pair<pair<QString , DataTypes> , bool>> tests{
        {{"fsdfdsfds" , String} , true},
        {{"12313" , Int} , true},
        {{"-123.4032" , Real} , true},
        {{"e" , Char} , true},
        {{"[4;7]" , Range} , true},
        {{"[4;12;2]" , Range} , true},
        {{"[file.txt;content]" , Txtfile} , true},
        {{"5.0505" , Real} , true},

        {{"4trefse" , Int} , false},
        {{"[3;aaa;-6]" , Range} , false},
        {{"ee" , Char} , false},
    };
    for(auto test : tests){
        auto str = test.first.first;
        auto type = test.first.second;
        auto ans = test.second;
        printTest(ans == is_parsable_as_type(type , str) , str , type);
    }
}

void Test_datatypes::printTest(bool verdict, QString inputString, DataTypes inputType){
    qDebug() << "------------------------------------------------------------" << endl;
    qDebug() << "Verdict " << (verdict ? "Passed" : "Failed") << endl;
    qDebug() << "Test input: \n";
    qDebug() << "       String: " << inputString << endl;
    qDebug() << "       DataType: " << to_string(inputType) << endl;
}
