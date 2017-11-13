#include "test_crossjoin.h"

Test_crossjoin::Test_crossjoin()
{
    DataBase db("test_db" , "login" , "pass");

    db.createTable("NewTable1" , {String , Int} , {"ID" , "NAME"});
    db.addRow({{0 , "name1"} , {1 , "32"}} , "NewTable1");
    db.addRow({{0 , "name2"} , {1 , "-32"}} , "NewTable1");

    db.createTable("NewTable2" , {String , Int} , {"ID" , "NAME"});
    db.addRow({{0 , "name3"} , {1 , "17"}} , "NewTable2");
    db.addRow({{0 , "name1"} , {1 , "32"}} , "NewTable2");

    db.createTable("NewTable3" , {Int , Real , Char , Txtfile} , {"id" , "value" , "letter" , "file"});
    db.addRow({{0 , "1"} , {1 , "3.1415"} , {2 , "B"} , {3 , "[file1.txt;aaaa]"}} , "NewTable3");
    db.addRow({{0 , "2"} , {1 , "2.81"} , {2 , "C"} , {3 , "[file2.txt;bbbb]"}} , "NewTable3");
    db.addRow({{0 , "3"} , {1 , "5.5"} , {2 , "D"} , {3 , "[file3.txt;cbcb]"}} , "NewTable3");

    db.createTable("NewTable4" , {Int , Real , Char , Txtfile} , {"id" , "value" , "letter" , "txtfile"});
    db.addRow({{0 , "4"} , {1 , "3.1415"} , {2 , "D"} , {3 , "[file1.txt;abaa]"}} , "NewTable4");
    db.addRow({{0 , "1"} , {1 , "3.1415"} , {2 , "B"} , {3 , "[file1.txt;aaaa]"}} , "NewTable4");
    db.addRow({{0 , "2"} , {1 , "2.81"} , {2 , "C"} , {3 , "[file2.txt;bbbb]"}} , "NewTable4");
    db.addRow({{0 , "3"} , {1 , "5.5"} , {2 , "D"} , {3 , "[file3.txt;cbcb]"}} , "NewTable4");
    db.addRow({{0 , "5"} , {1 , "6.01"} , {2 , "B"} , {3 , "[file1.txt;aaaa]"}} , "NewTable4");

    db.createTable("NewTable5" , {Int , Real , Char , Txtfile} , {"id" , "value" , "letter" , "file"});
    db.addRow({{0 , "2"} , {1 , "2.81"} , {2 , "C"} , {3 , "[file2.txt;bbbb]"}} , "NewTable5");
    db.addRow({{0 , "3"} , {1 , "5.5"} , {2 , "D"} , {3 , "[file3.txt;cbcb]"}} , "NewTable5");

    vector<pair<vector<QString> , int>> tests = {
        {{"NewTable1" , "NewTable2"} , 4},
        {{"NewTable1" , "NewTable3"} , 6},
        {{"NewTable1" , "NewTable4"} , 10},
        {{"NewTable3" , "NewTable4"} , 15},
        {{"NewTable3" , "NewTable4" , "NewTable5" , "NewTable2"} , 60},
        {{"NewTable3" , "NewTable5"} , 6}
    };

    for(auto test : tests){
        auto tbls = test.first;
        auto cnt = test.second;
        auto crossJoin = db.CrossJoin(tbls);
        printTest(crossJoin.getRows().size() == cnt , tbls , crossJoin);
    }
}

void Test_crossjoin::printTest(bool verdict , vector<QString> tbls, DataTable result){
    qDebug() << "------------------------------------------------------------" << endl;
    qDebug() << "Verdict " << (verdict ? "Passed" : "Failed") << endl;
    qDebug() << "Tables: " << endl;
    for(auto & e : tbls)qDebug() << e << " ";qDebug() << endl;
    qDebug() << "Got " << result.getRows().size() << " rows" << endl;
    if(result.getRows().size()){
        qDebug() << "CrossJoin is: " << endl;
        vector<DataRow> rows = result.getRows();
        for(DataRow row : rows){
            QString cur = "";
            for(auto e : row.getData()){
                cur += to_string(e.second.type) + ": " +  e.second.value + "    ";
            }
            qDebug() << "           " << cur << endl;
        }
    }
}
