#include "test_projection.h"

Test_projection::Test_projection()
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
 //   db.addRow({{0 , "3"} , {1 , "5.5"} , {2 , "D"} , {3 , "[file3.txt;cbcb]"}} , "NewTable3");
    db.addRow({{0 , "3"} , {1 , "5.0505"} , {2 , "D"} , {3 , "[file3.txt;cbcb]"}} , "NewTable3");

    db.createTable("NewTable4" , {Int , Real , Char , Txtfile} , {"id" , "value" , "letter" , "txtfile"});
    db.addRow({{0 , "4"} , {1 , "3.1415"} , {2 , "D"} , {3 , "[file1.txt;abaa]"}} , "NewTable4");
    db.addRow({{0 , "1"} , {1 , "3.1415"} , {2 , "B"} , {3 , "[file1.txt;aaaa]"}} , "NewTable4");
    db.addRow({{0 , "2"} , {1 , "2.81"} , {2 , "C"} , {3 , "[file2.txt;bbbb]"}} , "NewTable4");
//    db.addRow({{0 , "3"} , {1 , "5.5"} , {2 , "D"} , {3 , "[file3.txt;cbcb]"}} , "NewTable4");
    db.addRow({{0 , "3"} , {1 , "5.0505"} , {2 , "D"} , {3 , "[file3.txt;cbcb]"}} , "NewTable4");
    db.addRow({{0 , "5"} , {1 , "6.01"} , {2 , "B"} , {3 , "[file1.txt;aaaa]"}} , "NewTable4");

    db.createTable("NewTable5" , {Int , Real , Char , Txtfile} , {"id" , "value" , "letter" , "file"});
    db.addRow({{0 , "2"} , {1 , "2.81"} , {2 , "C"} , {3 , "[file2.txt;bbbb]"}} , "NewTable5");
    db.addRow({{0 , "3"} , {1 , "5.0505"} , {2 , "D"} , {3 , "[file3.txt;cbcb]"}} , "NewTable5");

    vector<pair<pair<QString , vector<QString>> , vector<QString>>> tests = {
        {{"NewTable1" , {"ID"}} , {"ID"}},
        {{"NewTable4" , {"id" , "value" , "txtfile"}} , {"id" , "value" , "txtfile"}},
        {{"NewTable2" , {"NAME" , "ID"}} , {"ID" , "NAME"}},
        {{"NewTable5" , {"ide"}} , {}},
        {{"NewTable1" , {"ID"}} , {"ID"}}
    };
    for(auto & test : tests){
        DataTable tbl = db.getDataTable(test.first.first);
        auto fields = test.first.second;
        auto res = test.second;
        DataTable projected = DataTable::Project(tbl , fields);
        bool verdict = res.size() == projected.getDescription().size();
        set<QString> st;for(auto & e : res)st.insert(e);
        for(auto & e : projected.getFieldNames())verdict &= st.find(e.second) != st.end();
        printTest(verdict , projected , res);
    }
}

void Test_projection::printTest(bool verdict , DataTable tbl , vector<QString> res){
    qDebug() << "------------------------------------------------------------" << endl;
    qDebug() << "Verdict " << (verdict ? "Passed" : "Failed") << endl;
    if(verdict){
        qDebug() << "Got " << res.size() << " cols." << endl;
    }else{
        QString got = "";
        for(auto & e : tbl.getFieldNames())got += e.second + " ";
        qDebug() << "Got " << got << endl;
        QString exp = "";
        for(auto & e : res)exp += e + " ";
        qDebug() << "Expected " << exp << endl;
    }
}
