#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[]) {
    char *sql;

    try{
        connection C("dbname = ieee80211radio user = xlx password = 123456 \
                    hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open()) {
            cout << "Opened database successfully: " << C.dbname() << endl;
        } else {
            cout << "Can't open database" << endl;
            return 1;
        }

        /* Create a transactional object. */
        work W(C);

        if(0) {
            /* Create SQL statement */
            sql = "CREATE TABLE Pharmacy_Counting(" \
              "id                       TEXT                NOT NULL," \
              "prescriber_last_name     TEXT                NOT NULL," \
              "prescriber_first_name    TEXT                NOT NULL," \
              "drug_name                TEXT                NOT NULL," \
              "drug_cost                NUMERIC(10, 2)      NOT NULL );";
            /* Execute SQL query*/
            W.exec(sql);
            W.commit();
        }

        /* Import data into table from csv file */
        sql = "COPY Pharmacy_Counting(id, prescriber_last_name, prescriber_first_name, drug_name, drug_cost)" \
                  "FROM '/home/nislab-server-002/CLionProjects/PostgreSQL/data/itcont.txt' DELIMITER ',' CSV HEADER;";
        W.exec(sql);
        W.commit();
        cout << "Import data successfully" << endl;

        C.disconnect ();

    } catch (const std::exception &e) {
        cerr << e.what() << std::endl;
        return 1;
    }

}