#include <iostream>
#include <pqxx/pqxx>
#include <string>

using namespace std;
using namespace pqxx;


int main(int argc, char* argv[]) {
    char *sql_cmd;
    string table = "pharmacy_counting";

    /* Connect to database */
    connection C("dbname = ieee80211radio user = xlx password = 123456 \
                  hostaddr = 127.0.0.1 port = 5432");
    connection nt_C("dbname = ieee80211radio user = xlx password = 123456 \
                  hostaddr = 127.0.0.1 port = 5432");
    if (C.is_open() && nt_C.is_open()) {
        cout << "Opened database successfully: " << C.dbname() << endl;
    } else {
        cout << "Can't open database" << endl;
        return 1;
    }
    /* Create a transactional object. */
    work W(C);
    /* Create a non-transactional object. */
    nontransaction N(nt_C);

    /*Check if the table exists*/
    string cmd = "SELECT relname FROM pg_class WHERE relname = '" + table + "';";
    const char *ntquery = cmd.c_str();
    result ntq(N.exec(ntquery));


    if(ntq.empty()) {
        /* Create SQL statement */
        sql_cmd = "CREATE TABLE pharmacy_counting(" \
              "id                       TEXT                NOT NULL," \
              "prescriber_last_name     TEXT                NOT NULL," \
              "prescriber_first_name    TEXT                NOT NULL," \
              "drug_name                TEXT                NOT NULL," \
              "drug_cost                NUMERIC(10, 2)      NOT NULL );";
        /* Execute SQL query*/
        W.exec(sql_cmd);
        W.commit();
    }else{
        cout << "Table " << table << " already exists!" << endl;
    }

    /* Import data into table from csv file */
    sql_cmd = "COPY Pharmacy_Counting(id, prescriber_last_name, prescriber_first_name, drug_name, drug_cost)" \
        "FROM '/home/nislab-server-002/CLionProjects/PostgreSQL/data/itcont.txt' DELIMITER ',' CSV HEADER;";
    W.exec(sql_cmd);
    W.commit();
    cout << "Import data successfully" << endl;

    /* Read content in the table */
    sql_cmd = "SELECT * from pharmacy_counting";
    result read_sql(N.exec(sql_cmd));
    for (result::const_iterator c = read_sql.begin(); c != read_sql.end(); ++c) {
        cout << "id = " << c[0].as<string>() << endl;
        cout << "prescriber_last_name = " << c[1].as<string>() << endl;
        cout << "prescriber_first_name = " << c[2].as<string>() << endl;
        cout << "drug_name = " << c[3].as<string>() << endl;
        cout << "drug_cost = " << c[4].as<float>() << endl;
    }
    cout << "Read done successfully" << endl;


    C.disconnect ();
    nt_C.disconnect();


}