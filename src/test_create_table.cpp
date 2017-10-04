#include "test_create_table.h"

Test_create_table::Test_create_table()
{
    DataBase db("test_db" , "login" , "pass");

    vector<pair<tuple<QString, vector<DataTypes>, vector<QString>> , bool>> tests = {
        {{ "NewTable" , {Int , String} , {"id" , "name"} } , true},
        {{ "NewTable2" , {Int , Range , Txtfile} , {"id" , "some_range" , "some_file"} } , true},
        {{ "NewTable" , {Int , Real} , {"id" , "sum"} } , false},//name already exists
        {{ "NewTable3" , {Int} , {"id" , "name"} } , false}, // dif. sizes of field types and names
        {{ "NewTable4" , {Int , String} , {"id"} } , false}  // dif. sizes of field types and names
    };

    for(auto test : tests){
        auto name = get<0>(test.first);
        auto types = get<1>(test.first);
        auto fields = get<2>(test.first);
        printTest(db.createTable(name , types , fields) == test.second , name , types , fields);
    }
}

void Test_create_table::printTest(bool verdict , QString name ,
                                  vector<DataTypes> types , vector<QString> fields){
    qDebug() << "------------------------------------------------------------" << endl;
    qDebug() << "Verdict " << (verdict ? "Passed" : "Failed") << endl;
    qDebug() << "Test input: \n";
    qDebug() << "       Table name: " << name << endl;
    qDebug() << "       Types: "; for(auto & e : types)qDebug() << "            " << to_string(e) << " "; qDebug() << endl;
    qDebug() << "       Names: "; for(auto & e : fields)qDebug() << "            " << e << " "; qDebug() << endl;

}
